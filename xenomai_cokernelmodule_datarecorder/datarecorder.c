/******************************************************************************/
/* Real Time Data Recorder Xenomai Linux Co-Kernel Module                     */
/* Samples Data from GPIO Port                                                */
/* Saves Data to a shared memory ringbuffer                                   */
/* Can be controlled via Realtime Pipe /dev/rtp7                              */
/* 2013 by Joscha Ihl <joscha@grundfarm.de>                                   */
/******************************************************************************/
#include "print.h"
#include "datarecorder.h"
#include "rec_state.h"
#include "rec_ringbuffer.h"
#include "configuration.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/kernel.h>

/* GPIO Pin configuration */
#include <mach/gpio.h>

#include <time.h>

/* Task Control */
#include <native/task.h>

/* Xenomai Modules */
#include <rtdm/rtdm_driver.h>

/* Datastructures for sample timer */
//int frequency = 100 * 1000;

#ifdef USE_RTDM_TIMER
rtdm_timer_t datarecorder_timer;
#else
rtdm_task_t timer_task;
#endif

#ifdef AVOID_LINUX_SYSTEM_CALLS
unsigned int *gplev0 = 0x7E200034;
#endif

uint64_t previous, now;
int blink = 0;

void sample_gpio(void)
{
    SensorData currentSample;

    // For latency measuring
#ifdef ENABLE_TIMESTAMP
    static int firstrun = 0;
    now = rtdm_clock_read_monotonic();
    // For latency measuring
    if(firstrun==0)
    {
        previous = now;
    }

#else
    now = 0;
#endif

	if (get_recorder_state() == rec_state_running)
	{
		currentSample.sensorID = 0;
		currentSample.sampleTimeCode = now;

#ifdef AVOID_LINUX_SYSTEM_CALLS
		if(*gplev0 & (1 << GPIO_PIN_1))
#else
		if (gpio_get_value(GPIO_PIN_1))
#endif
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
#ifdef USE_ATOMIC_EXECUTION
		RTDM_EXECUTE_ATOMICALLY(insertSampleToRingBuffer(currentSample));
#else
		insertSampleToRingBuffer(currentSample);
#endif

#ifdef AVOID_LINUX_SYSTEM_CALLS

#else
        gpio_set_value(GPIO_PIN_SAMPLE_STATUS_LED, blink);
	#ifdef USE_TASK_SWITCH_OUTPUT_PIN
        gpio_set_value(GPIO_PIN_TASK_SWITCH_OUT, blink);
	#endif
#endif
        if(blink==0)
        {
            blink = 1;
        }
        else
        {
            blink = 0;
        }

    }
#ifdef ENABLE_TIMESTAMP
    if(firstrun==0)
    {
        firstrun = 1;
    }
    // For latency measuring
    previous = rtdm_clock_read_monotonic();
#endif
}


/**
 * Sampling Data from GPIO Pins via Timer
 */
static void timer_proc(rtdm_timer_t *timer)
{
	sample_gpio();
}

void sample_timer(void *arg)
{
	while(1)
	{
		sample_gpio();
		rt_task_wait_period(NULL);
	}
}

int start_timer(void)
{
	int err;
#ifdef USE_RTDM_TIMER
	err = rtdm_timer_start(&datarecorder_timer, rtdm_clock_read_monotonic(),
	                     get_speed(), RTDM_TIMERMODE_REALTIME);
#else
	err = rtdm_task_init(&timer_task, "SampleTimerTask",
			sample_timer, NULL, RTDM_TASK_HIGHEST_PRIORITY, get_speed());
#endif


    switch(err) {
        case 0:
            break;
        default:
            DPRINT("Can't start sample timer task");
            return -1;
    }
    return 0;
}

void stop_timer(void)
{
    DPRINT("Stopping Sample Timer...");
#ifdef USE_RTDM_TIMER
	rtdm_timer_stop(&datarecorder_timer);
#else
	rtdm_task_destroy(&timer_task);
#endif
}

void reset_timer_frequency(void)
{
	if(get_recorder_state()==rec_state_pause||
			get_recorder_state()==rec_state_running)
	{
		stop_timer();
		start_timer();
	}
}

/**
 * Datarecorder Entry-Point
 */
static int datarecorder_init(void)
{
	int err;
    DPRINT("Initializing Datarecorder...");
    // Lock memory : avoid memory swapping for this program
#ifdef USE_TASK_SWITCH_OUTPUT_PIN
    gpio_direction_output(GPIO_PIN_TASK_SWITCH_OUT, 1);
#endif
    DPRINT("Starting Real Time Recorder Sample Timer Task...");
#ifdef USE_RTDM_TIMER
    err = rtdm_timer_init(&datarecorder_timer, timer_proc,
                    "datarecorder_sample_timer");
#endif
    switch(err) {
        case 0:
            break;
        default:
            DPRINT("Can't init sample timer task");
            return -1;
    }
    start_timer();
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
    
    stop_timer();
#ifdef USE_RTDM_TIMER
    rtdm_timer_destroy(&datarecorder_timer);
#endif
    DPRINT("Datarecorder Kernel Module removed");
}

module_init(datarecorder_init);
module_exit(datarecorder_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Joscha Ihl <joscha@grundfarm.de>");
MODULE_DESCRIPTION("Real Time Data Recorder Linux Co-Kernel Module");

EXPORT_SYMBOL(reset_timer_frequency);
