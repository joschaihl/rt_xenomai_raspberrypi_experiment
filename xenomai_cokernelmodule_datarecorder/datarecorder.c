/******************************************************************************/
/* Real Time Data Recorder Xenomai Linux Co-Kernel Module                     */
/* Samples Data from GPIO Port                                                */
/* Saves Data to a shared memory ringbuffer                                   */
/* Can be controlled via Realtime Pipe /dev/rtp7                              */
/* 2013 by Joscha Ihl <joscha@grundfarm.de>                                   */
/******************************************************************************/
#include "print.h"
#include "control_pipe.h"
#include "rec_state.h"
#include "rec_ringbuffer.h"

#include <linux/init.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/kernel.h>

/* GPIO Pin configuration */
#include <mach/gpio.h>

#include <time.h>

#define GPIO_PIN_1 17
#define GPIO_PIN_SAMPLE_STATUS_LED 16

/* Task Control */
#include <native/task.h>

/* Xenomai Modules */
#include <rtdm/rtdm_driver.h>

/* Datastructures for sample timer */
int frequency = 1 * 1000 * 1000;
rtdm_task_t timer_task;
rtdm_timer_t datarecorder_timer;

uint64_t previous, now;
int blink = 0;

/**
 * Sampling Data from GPIO Pins via Timer
 */
static void timer_proc(rtdm_timer_t *timer)
{
    static int firstrun = 0;
    SensorData currentSample;

    // For latency measuring
    now = rtdm_clock_read_monotonic();
    
    // For latency measuring
    if(firstrun==0)
    {
        previous = now;
    }
    
    if(get_recorder_state()==rec_state_running)
    {
				#ifdef DEBUG_DATARECORDER
        rtdm_printk(KERN_INFO DPRINT_PREFIX "Jiffies: %llu\n", now);
        #endif
        currentSample.sensorID = 0;
        currentSample.sampleTimeCode = now;
        if(gpio_get_value(GPIO_PIN_1))
        {
            currentSample.sensorValue = 1;
            #ifdef DEBUG_DATARECORDER
            rtdm_printk(KERN_INFO DPRINT_PREFIX "HIGH %llu\n",
                        /*1000000000 - */(now - previous));
            #endif
        }
        else
        {
            currentSample.sensorValue = 0;
            #ifdef DEBUG_DATARECORDER
            rtdm_printk(KERN_INFO DPRINT_PREFIX "LOW %llu\n",
                        /*1000000000 - */(now - previous));
          	#endif
        }
        insertSampleToRingBuffer(currentSample);
        gpio_set_value(GPIO_PIN_SAMPLE_STATUS_LED, blink);
        if(blink==0)
        {
            blink = 1;
        }
        else
        {
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

    DPRINT("Datarecorder Kernel Module removed");
}

module_init(datarecorder_init);
module_exit(datarecorder_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Joscha Ihl <joscha@grundfarm.de>");
MODULE_DESCRIPTION("Real Time Data Recorder Linux Co-Kernel Module");
