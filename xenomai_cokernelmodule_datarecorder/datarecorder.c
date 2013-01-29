/******************************************************************************/
/* Real Time Data Recorder Xenomai Linux Co-Kernel Module                     */
/* Samples Data from GPIO Port                                                */
/* Saves Data to a shared memory ringbuffer                                   */
/* Can be controlled via Realtime Pipe /dev/rtp7                              */
/* 2013 by Joscha Ihl <joscha@grundfarm.de>                                   */
/******************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/kernel.h>
#include <mach/gpio.h>

/* Shared Memory */
#include <native/heap.h>

/* Datastructes for shared memory */
unsigned int ringbuffer_size_mb = 16;
char *ringbuffer = NULL;
#define SHM_NAME "RecorderRingbufferHeap"
RT_HEAP datarecorder_heap;
char *shmem;

/* Task Control */
#include <native/task.h>
RT_TASK def_recorder_control_task;


/* Command Pipe */
#include <native/pipe.h>
#include <fcntl.h>
#include <linux/string.h>
RT_PIPE pipe_desc;

/* Xenomai Modules */
#include <rtdm/rtdm_driver.h>

//int frequency = 10 * 1000 * 1000;

/* Datastructures for DataRecorder */
typedef enum e_SystemState {
    rec_state_pause = 0,
    rec_state_running = 1,
    rec_state_error = 2,
    rec_state_init = 3,
} SystemState;
SystemState recorderState = rec_state_init;

/* Datastructures for sample timer */
int frequency = 1000 * 1000 * 1000;
rtdm_task_t timer_task;
rtdm_timer_t datarecorder_timer;

uint64_t previous, now;
int blink = 0;

#define DPRINT_PREFIX "DataRecorder: "
#define DPRINT(s) rtdm_printk(KERN_INFO DPRINT_PREFIX "%s\n", s) 

void parse_control_cmd(char *control_string, unsigned int string_length)
{
    unsigned int i;
#ifdef DEBUG_DATARECORDER
    for(i=0; i< string_length;i++)
    {
        rtdm_printk(KERN_INFO DPRINT_PREFIX "string[%d] = %c = %d", i, control_string[i], control_string[i]);
    }
#endif
    if(strncmp(control_string, "start\n", string_length)==0)
    {
        DPRINT("START");
        recorderState = rec_state_running;
    }
    else if(strncmp(control_string, "pause\n", string_length)==0)
    {
        DPRINT("PAUSE");
        recorderState = rec_state_pause;
    }
}

/**
 * Controls the recorder via RealTime Pipe
 */
void recorder_control_task(void *cookie)
{
	RT_PIPE_MSG *msgin;
	int err, len, n;
	for(;;)
	{
		/* Then wait for the reply string "World": */
		n = rt_pipe_receive(&pipe_desc, &msgin, TM_INFINITE);
        
		if (n < 0) {
            DPRINT("recorder_control_task() Pipe receive error");
            continue;
		}
		if (n == 0) {
            if (msgin == NULL) {
#ifdef DEBUG_DATARECORDER
                DPRINT("recorder_control_task() pipe closed by peer");
#endif
                continue;
            }
            DPRINT("recorder_control_task() Empty message received");
		}
        else
        {
            parse_control_cmd(P_MSGPTR(msgin), P_MSGSIZE(msgin));
        }
		/* Free the received message buffer. */
		rt_pipe_free(&pipe_desc, msgin);
	}
}

/**
 * Sampling Data from GPIO Pins via Timer
 */
static void timer_proc(rtdm_timer_t *timer)
{
	//RT_PIPE_MSG *msgout;
	//int len;
    static int firstrun = 0;
    

    // For latency measuring
    now = rtdm_clock_read_monotonic();
    
    // For latency measuring
    if(firstrun==0)
    {
        previous = now;
    }
    
    if(recorderState==rec_state_running)
    {
        /* FIXME: Don't use a Linux kernel-function here, 
           because of possible real time violation through 
           context-switching */
        if(gpio_get_value(17))
        {
            rtdm_printk(KERN_INFO DPRINT_PREFIX "HIGH %llu\n",
                        /*1000000000 - */(now - previous));
        }
        else
        {
            rtdm_printk(KERN_INFO DPRINT_PREFIX "LOW %llu\n",
                        /*1000000000 - */(now - previous));
        }
        
        gpio_set_value(16, blink);
        if(blink==0)
        {
            blink = 1;
            shmem[0] = 1;
            //datarecorder_heap[0] = 1;
        }
        else
        {
            blink = 0;
            shmem[0] = 0;
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
            rtdm_printk(KERN_INFO DPRINT_PREFIX "Can't create pipe :( - rt_pipe_create returned value %d\n", err);
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
    
	/* Connect the kernel-side of the message pipe to the
     special device file /dev/rtp7. */
    DPRINT("Creating Real Time Recorder Control Pipe /dev/rtp7...");
    err = rt_pipe_create(&pipe_desc,"RecorderControlPipe",7,0);

    switch(err) {
        case 0:
            DPRINT("Pipe created :)");
            break;
        case ENOMEM:
            DPRINT("Can't create pipe :( - No more Memory");
            return -1;
            break;
        case EEXIST:
            DPRINT("Can't create pipe :( - Pipe already exists");
            return -2;
            break;
        case ENODEV:
            DPRINT("Can't create pipe :( - Pipe minor number is invalid");
            return -3;
            break;
        case EBUSY:
            DPRINT("Can't create pipe :( - The pipe is already opened");
            return -4;
            break;
        case EPERM:
            DPRINT("Can't create pipe :( - The service was called from an asynchronous context");
            return -5;
            break;
        default:
            rtdm_printk(KERN_INFO DPRINT_PREFIX "Can't create pipe :( - rt_pipe_create returned value %d\n", err);
            return -6;
    }
    
    DPRINT("Creating and starting Real Time Recorder Control Task...");
    err = rt_task_create(&def_recorder_control_task, "RecorderControlTask",
                         4096, 99, T_FPU);
    switch(err) {
        case 0:
            break;
        default:
            DPRINT("Can't create task RecorderControlTask");
            return -1;
    }
    err = rt_task_start(&def_recorder_control_task,&recorder_control_task,NULL);
    switch(err) {
        case 0:
            break;
        default:
            DPRINT("Can't start task RecorderControlTask");
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
    recorderState = rec_state_running;
    DPRINT("Datarecorder initialized and running :)");
    
    return 0;
}

/*
 * Datarecorder destruction
 */
static void datarecorder_exit(void)
{
    DPRINT("Exiting Datarecorder...");
    
    DPRINT("Deleting Control Pipe...");
    rt_pipe_delete(&pipe_desc);
    
    DPRINT("Stopping Control Task...");
    rt_task_delete(&def_recorder_control_task);
    
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
