/******************************************************************************/
/* Real Time Data Recorder Xenomai Linux Co-Kernel Module                     */
/******************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/kernel.h>
#include <mach/gpio.h>
/* Xenomai Modules */
#include <rtdm/rtdm_driver.h>

int frequency = 10 * 1000 * 1000;

rtdm_task_t timer_task;
rtdm_timer_t timer;

uint64_t previous, now;
int blink = 0;

static void timer_proc(rtdm_timer_t *timer)
{
	now = rtdm_clock_read_monotonic();

	if(gpio_get_value(17))
	{
		blink = 0;
		rtdm_printk(KERN_INFO "HIGH %llu\n", /*1000000000 - */(now - previous));
	}
	else
	{
		blink = 1;
		rtdm_printk(KERN_INFO "LOW %llu\n", /*1000000000 - */(now - previous));
	}
	gpio_set_value(16, blink);

	previous = rtdm_clock_read_monotonic();
}

char *ringbuffer = NULL;

/**
 * Datarecorder Entry-Point
 */
static int datarecorder_init(void)
{
	int i;
	i = 32000*PAGE_SIZE;
	printk(KERN_INFO "Reserving %d MB\n", i/1024/1024);
	ringbuffer = vmalloc(i);
	if(!ringbuffer)
    {
		printk(KERN_ALERT "Error with vmalloc");
        return -1;
    }
    else
    {
        rtdm_timer_init(&timer, timer_proc, "test");
        rtdm_timer_start(&timer, rtdm_clock_read_monotonic(), frequency, RTDM_TIMERMODE_REALTIME);
        return 0;
    }
}

/*
 * Datarecorder destruction
 */
static void datarecorder_exit(void)
{
	rtdm_timer_stop(&timer);
	rtdm_timer_destroy(&timer);

	if(ringbuffer)
	{
		vfree(ringbuffer);
	}
}

module_init(datarecorder_init);
module_exit(datarecorder_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Joscha Ihl <joscha@grundfarm.de>");
MODULE_DESCRIPTION("Real Time Data Recorder Linux Co-Kernel Module");
