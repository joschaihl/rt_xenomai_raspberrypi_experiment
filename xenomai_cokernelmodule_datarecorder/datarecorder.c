/******************************************************************************/
/* Real Time Data Recorder Xenomai Linux Co-Kernel Module                     */
/* Samples Data from GPIO Port                                                */
/* Saves Data to a shared memory ringbuffer                                   */
/* Can be controlled via Realtime Pipe /dev/rtp7                              */
/* 2013 by Joscha Ihl <joscha@grundfarm.de>                                   */
/******************************************************************************/
#include "print.h"
#include "ringbuffer_model.h"
#include "control_pipe.h"
#include "rec_state.h"

#include <linux/init.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/kernel.h>

/* GPIO Pin configuration */
#include <mach/gpio.h>

#define GPIO_PIN_1 17
#define GPIO_PIN_SAMPLE_STATUS_LED 16

/* Shared Memory */
#include <native/heap.h>

typedef struct {
    uint8_t sensor_id;
    uint64_t sample_time;
    uint8_t sensor_value;
} SensorDataValueModel;

/* Datastructes for shared memory */
unsigned int ringbuffer_size_mb = 16;
unsigned int ringbuffer_index = 0;
unsigned int ringbuffer_max_index = 1000;
#define SHM_NAME "RecorderRingbufferHeap"
RT_HEAP datarecorder_heap;
void *shmem;

/* Task Control */
#include <native/task.h>


/* Xenomai Modules */
#include <rtdm/rtdm_driver.h>

//int frequency = 10 * 1000 * 1000;

/* Datastructures for DataRecorder */


/* Datastructures for sample timer */
int frequency = 1000 * 1000 * 1000;
rtdm_task_t timer_task;
rtdm_timer_t datarecorder_timer;

uint64_t previous, now;
int blink = 0;

void insertSampleToRingBuffer(SensorDataValueModel sample)
{
    SensorDataValueModel *ringbuffer;
    ringbuffer = shmem;
    ringbuffer[ringbuffer_index] = sample;
    ringbuffer_index++;
}

/**
 * Sampling Data from GPIO Pins via Timer
 */
static void timer_proc(rtdm_timer_t *timer)
{
	//RT_PIPE_MSG *msgout;
	//int len;
    static int firstrun = 0;
    SensorDataValueModel currentSample;

    // For latency measuring
    now = rtdm_clock_read_monotonic();
    
    // For latency measuring
    if(firstrun==0)
    {
        previous = now;
    }
    
    if(get_recorder_state()==rec_state_running)
    {
        char *ch_shmem;
        ch_shmem = shmem;
        /* FIXME: Don't use a Linux kernel-function here, 
           because of possible real time violation through 
           context-switching */
        currentSample.sensor_id = 0;
        currentSample.sample_time = now;
        
        if(gpio_get_value(GPIO_PIN_1))
        {
            currentSample.sensor_value = 1;
            rtdm_printk(KERN_INFO DPRINT_PREFIX "HIGH %llu\n",
                        /*1000000000 - */(now - previous));
        }
        else
        {
            currentSample.sensor_value = 0;
            rtdm_printk(KERN_INFO DPRINT_PREFIX "LOW %llu\n",
                        /*1000000000 - */(now - previous));
        }
        
        gpio_set_value(GPIO_PIN_SAMPLE_STATUS_LED, blink);
        
        
        if(blink==0)
        {
            blink = 1;
            ch_shmem[0] = 0;
            //datarecorder_heap[0] = 1;
        }
        else
        {
            ch_shmem[0] = 1;
            blink = 0;
        }
        
    }
    
    if(firstrun==0)
    {
        firstrun = 1;
    }
    
    // For latency measuring
    previous = rtdm_clock_read_monotonic();
}

/**
 * Datarecorder Entry-Point
 */
static int datarecorder_init(void)
{
	int err;
    DPRINT("Initializing Datarecorder...");
    
	/* Create the heap in kernel space */
    rtdm_printk(KERN_INFO DPRINT_PREFIX
                "Reserving %d MB for shared memory RecorderRingbufferHeap... ",
                ringbuffer_size_mb);
    err = rt_heap_create(&datarecorder_heap, SHM_NAME,
                         ringbuffer_size_mb * 1024 * 1024,H_SHARED);
    
    switch(err) {
        case 0:
            break;
        case EEXIST:
            DPRINT("rt_heap_create(): Shared Memory name is already in use by some registered object :(");
            return -1;
        case EINVAL:
            DPRINT("rt_heap_create(): Heapsize is null, greater than the system limit, or name is null or empty for a mappable heap. :(");
            return -1;
        case ENOMEM:
            DPRINT("rt_heap_create(): Not enough system memory is available to create or register the heap. :(");
            return -1;
        case EPERM:
            DPRINT("rt_heap_create(): This service was called from an invalid context. :(");
            return -1;
        case ENOSYS:
            DPRINT("Mode specifies H_MAPPABLE, but the real-time support in user-space is unavailable. :(");
            return -1;
        default:
            rtdm_printk(KERN_INFO DPRINT_PREFIX "Can't create heap :( - rt_heap_create returned value %d\n", err);
            return -1;
    }

    
	/* Get the shared memory address */
    DPRINT("Get a shared memory address...");
	err = rt_heap_alloc(&datarecorder_heap,0,TM_NONBLOCK,&shmem);
    switch(err) {
        case 0:
            break;
        default:
            rtdm_printk(KERN_INFO DPRINT_PREFIX "rt_heap_alloc returned %d\n", err);
            return -1;
    }
    
    DPRINT("Starting Real Time Recorder Sample Timer Task...");
    err = rtdm_timer_init(&datarecorder_timer, timer_proc,
                    "datarecorder_sample_timer");
    switch(err) {
        case 0:
            break;
        default:
            DPRINT("Can't init sample timer task");
            return -1;
    }
    err = rtdm_timer_start(&datarecorder_timer, rtdm_clock_read_monotonic(),
                     frequency, RTDM_TIMERMODE_REALTIME);
    switch(err) {
        case 0:
            break;
        default:
            DPRINT("Can't start sample timer task");
            return -1;
    }
    set_recorder_state(rec_state_running);
    DPRINT("Datarecorder initialized and running :)");
    
    return 0;
}

/*
 * Datarecorder destruction
 */
static void datarecorder_exit(void)
{
    DPRINT("Exiting Datarecorder...");
    
    DPRINT("Stopping Sample Timer...");
	rtdm_timer_stop(&datarecorder_timer);
	rtdm_timer_destroy(&datarecorder_timer);
    
    DPRINT("Removing Shared Memory");
	rt_heap_delete(&datarecorder_heap);

    DPRINT("Datarecorder Kernel Module removed");
}

module_init(datarecorder_init);
module_exit(datarecorder_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Joscha Ihl <joscha@grundfarm.de>");
MODULE_DESCRIPTION("Real Time Data Recorder Linux Co-Kernel Module");
