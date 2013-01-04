#include <linux/init.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/kernel.h>
#include <mach/gpio.h>

#include <rtdm/rtdm_driver.h>


// Physical addresses for various peripheral regiser sets
/// Base Physical Address of the BCM 2835 peripheral registers
//#define BCM2835_PERI_BASE               0x20000000
/// Base Physical Address of the GPIO registers
//#define BCM2835_GPIO_BASE               (BCM2835_PERI_BASE + 0x200000)

//static volatile uint32_t *gpio = MAP_FAILED;

int frequency = 10 * 1000 * 1000;
//#define TASK_PRIO  99              /* Highest RT priority */
//#define TASK_MODE  T_FPU|T_CPU(0)  /* Uses FPU, bound to CPU #0 */
//#define TASK_STKSZ 4096            /* Stack size (in bytes) */

//RT_TASK timer_task;
int err;

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
	
/*	if(blink)
		blink = 0;
	else
		blink = 1;*/
	gpio_set_value(16, blink);

	previous = rtdm_clock_read_monotonic();
}
//void timer_task_handler (void *cookie)
//{
//	RTIME now, previous;
	        /*
	         * Arguments: &task (NULL=self),
	* start time,
	         *            period (here: 1 s)
	         */
//	        rt_task_set_periodic(NULL, TM_NOW, 1000000000);
//	        previous = rt_timer_read();
//    for (;;)
//    {
      /* Task processing loop in kernel space. */
//    	rt_task_wait_period(NULL);
//    	now = rt_timer_read();
    	/*
    	 * NOTE: printf may have unexpected impact on the timing of
    	 *       your program. It is used here in the critical loop
    	 *       only for demonstration purposes.
    	 */
    	// ioread32 um GPIO Ports auszulesen
   //	rtdm_printk(KERN_INFO "Time since last turn: %ld.%06ld ms\n",
    //	       (long)(now - previous) / 1000000,
   // 	       (long)(now - previous) % 1000000);
//    	previous = now;
//    }
//}

char *buf;

/**
 * hallo_init - der Entry-Point des Moduls
 */
static int hallo_init(void)
{
	int i;
	i = 32000*PAGE_SIZE;
	printk(KERN_INFO "Reserving %d MB\n", i/1024/1024);
	buf = vmalloc(i);
	if(!buf)
		printk(KERN_ALERT "Error with vmalloc");

	rtdm_timer_init(&timer, timer_proc, "test");
	rtdm_timer_start(&timer, rtdm_clock_read_monotonic(), frequency, RTDM_TIMERMODE_REALTIME);

	//err = rtdm_task_init(&timer_task, "timerbench", timer_task_proc, ctx,
	//		     config->priority, 0);

	/* Init Real Time printf, to avoid switching to the Linux-Context */
	/* Perform auto-init of rt_print buffers if the task doesn't do so */
	//rt_print_auto_init(1);

	/* Initialise the rt_print buffer for this task explicitly */
	//rt_print_init(4096, "Timer");

	//err = rt_task_create(&timer_task, "TimerTask", TASK_STKSZ, TASK_PRIO, TASK_MODE);
	//if (!err)
	 //     rt_task_start(&timer_task,&timer_task_handler,NULL);
  /*  int fd = -1;
    fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0777);
    if (fd == -1)
	{
    	xnprintf("shm_open: %d\n", errno);
    	return -errno;
	}

    shm = mmap(NULL, 65536, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shm == MAP_FAILED)
	{
    	xnprintf("mmap(producer_shm): %d\n", errno);
    	return -errno;
	}*/
	/*for(i = 0; i < 10; i++)
	{
		printk(KERN_INFO "%d\n", i);
	}*/
    return 0;
}

/*
 * hallo_exit - die exit-function
 */
static void hallo_exit(void)
{
	rtdm_timer_stop(&timer);
	rtdm_timer_destroy(&timer);

	if(buf)
	{
		vfree(buf);
	}
	if(!err)
	{
	//	rt_task_delete(&timer_task);
	}
    //printk(KERN_ALERT "You are screwed!\n");
}

module_init(hallo_init);
module_exit(hallo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Joscha Ihl <joscha@grundfarm.de>");
MODULE_DESCRIPTION("Real Time Data Recorder Linux Co-Kernel Module");
