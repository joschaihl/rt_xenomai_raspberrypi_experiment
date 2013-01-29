/******************************************************************************/
/* Real Time Data Recorder Xenomai Linux Co-Kernel Module                     */
/* Samples Data from GPIO Port                                                */
/* Saves Data to a ringbuffer                                                 */
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
RT_HEAP datarecorder_heap;
void *shmem;

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
/* Datastructes for sample timer */
int frequency = 1000 * 1000 * 1000;
rtdm_task_t timer_task;
rtdm_timer_t datarecorder_timer;

uint64_t previous, now;
int blink = 0;

#define DPRINT(s) rtdm_printk(KERN_INFO "DataRecorder: %s\n", s) 

void recorder_control_task(void *cookie)
{
	RT_PIPE_MSG *msgin;
	int err, len, n;
	for(;;)
	{
		/* Then wait for the reply string "World": */
		n = rt_pipe_receive(&pipe_desc, &msgin, TM_INFINITE);

		if (n < 0) {
			rtdm_printk(KERN_INFO "receive error> errno=%d\n", n);
		    continue;
		}
		if (n == 0) {
		        if (msgin == NULL) {
		        	rtdm_printk(KERN_INFO "pipe closed by peer");
		        	continue;
		        }
		        rtdm_printk(KERN_INFO "Empty message received");
		} else
			rtdm_printk(KERN_INFO "received msg> %s, size=%d\n", P_MSGPTR(msgin),
		       P_MSGSIZE(msgin));
		/* Free the received message buffer. */
		rt_pipe_free(&pipe_desc, msgin);
	}
}

static void timer_proc(rtdm_timer_t *timer)
{
	//RT_PIPE_MSG *msgout;
	//int len;
	now = rtdm_clock_read_monotonic();

	//len = sizeof("Hello");
	                /* Get a message block of the right size in order to
	                   initiate the message-oriented dialog with the
	                   user-space process. Sending a continuous stream of
	                   bytes is also possible using rt_pipe_stream(), in
	                   which case no message buffer needs to be
	                   preallocated. */
	//msgout = rt_pipe_alloc(&pipe_desc, len);
	//if (!msgout)
	//	fail();
	/* Send prompt message "Hello" (the output buffer will be freed
	                   automatically)... */
	//strcpy((char *) msgout, "Hello");
	//rt_pipe_send(&pipe_desc, msgout, len, P_NORMAL);
	//rt_pipe_flush(&pipe_desc, XNPIPE_OFLUSH);

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



/**
 * Datarecorder Entry-Point
 */
static int datarecorder_init(void)
{
	int err;
    DPRINT("Initializing Datarecorder...");

	/* Create the heap in kernel space */
    rtdm_printk(KERN_INFO "Reserving %d MB for shared Memory RecorderRingbufferHeap...\n", ringbuffer_size_mb);
	err = rt_heap_create(&datarecorder_heap,"RecorderRingbufferHeap",ringbuffer_size_mb * 1024 * 1024,H_SHARED);

	/* Get the shared memory address */
	err = rt_heap_alloc(&datarecorder_heap,0,TM_NONBLOCK,&shmem);

	/* Connect the kernel-side of the message pipe to the
	         special device file /dev/rtp7. */
    DPRINT("Creating Real Time Recorder Control Pipe /dev/rtp7...");
	err = rt_pipe_create(&pipe_desc,"RecorderControlPipe",7,0);
    if(err!=0)
    {
        DPRINT("Can't create pipe :(");
        return -1;
    }
    
    DPRINT("Creating and starting Real Time Recorder Control Task...");
    err = rt_task_create(&def_recorder_control_task, "RecorderControlTask",
                         4096, 99, T_FPU);
    
    rt_task_start(&def_recorder_control_task, &recorder_control_task, NULL);
    
    if (!err)
        rt_task_start(&def_recorder_control_task,&recorder_control_task,NULL);
    
    DPRINT("Starting Real Time Recorder Sample Timer Task...");
    rtdm_timer_init(&datarecorder_timer, timer_proc, "datarecorder_sample_timer");
    rtdm_timer_start(&datarecorder_timer, rtdm_clock_read_monotonic(), frequency, RTDM_TIMERMODE_REALTIME);

    DPRINT("Datarecorder initialized and ready :)");
    return 0;
}

/*
 * Datarecorder destruction
 */
static void datarecorder_exit(void)
{
    DPRINT("Exiting Datarecorder...");
	rtdm_timer_stop(&datarecorder_timer);
	rtdm_timer_destroy(&datarecorder_timer);
	rt_heap_delete(&datarecorder_heap);
    rt_task_delete(&def_recorder_control_task);
    DPRINT("Datarecorder Kernel Module removed");
}

module_init(datarecorder_init);
module_exit(datarecorder_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Joscha Ihl <joscha@grundfarm.de>");
MODULE_DESCRIPTION("Real Time Data Recorder Linux Co-Kernel Module");
