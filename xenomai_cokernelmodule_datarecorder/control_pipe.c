#include "control_pipe.h"
#include "rec_state.h"
#include "datarecorder.h"
#include "print.h"
#include "configuration.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/kernel.h>


void parse_recorder_control_cmd(char *control_string,
		unsigned int string_length)
{
	/**
	 * If the grammar will be more complex, it would be better
	 * to implement an LL(k), LR Parser or something like this.
	 * At the moment the grammar is very simple, so that we
	 * don't need this.
	 */
#ifdef DEBUG_DATARECORDER
	unsigned int i;
	for(i=0; i< string_length;i++)
	{
		rtdm_printk(KERN_INFO DPRINT_PREFIX "string[%d] = %c = %d", i, control_string[i], control_string[i]);
	}
#endif
	if (strncmp(control_string, "start\n", string_length) == 0)
	{
		DPRINT("START");
		set_recorder_state(rec_state_running);
	}
	else if (strncmp(control_string, "pause\n", string_length) == 0)
	{
		DPRINT("PAUSE");
		set_recorder_state(rec_state_pause);
	}
	else if (string_length >= 8)
	{
		if(strncmp("speed=", control_string, 6)==0)
		{
			int err;

			/* terminate string with 0 instead of newline */
			control_string[string_length-1] = '\0';
			//sscanf(control_string, "speed=%d", &speed);
			char *int_str = control_string+6;
			int value = 0;
			err = kstrtoint(int_str, 10, &value);
			if(err || value<=0)
			{
				DPRINT("Can't read speed value");
			}
			else
			{
				DPRINT("Set new speed value");
				set_speed(value);
				reset_timer_frequency();
				rtdm_printk(KERN_INFO DPRINT_PREFIX "SPEED=%d\n", get_speed());
			}
		}
	}
}

/**
 * Controls the recorder via RealTime Pipe
 */
void recorder_control_task(void *cookie)
{
	RT_PIPE_MSG *msgin;
	int n;
	for (;;)
	{
		/* Then wait for the reply string "World": */
		n = rt_pipe_receive(&pipe_desc, &msgin, TM_INFINITE);

		if (n < 0)
		{
			DPRINT("recorder_control_task() Pipe receive error");
			continue;
		}
		else if (n == 0)
		{
			if (msgin == NULL )
			{
#ifdef DEBUG_DATARECORDER
				DPRINT("recorder_control_task() pipe closed by peer");
#endif
				continue;
			}
			DPRINT("recorder_control_task() Empty message received");
		}
		else
		{
			parse_recorder_control_cmd(P_MSGPTR(msgin), P_MSGSIZE(msgin));
		}
		/* Free the received message buffer. */
		rt_pipe_free(&pipe_desc, msgin);
	}
}

static int control_pipe_init(void)
{
	/* Connect the kernel-side of the message pipe to the
	 special device file /dev/rtp7. */
	int err;
	DPRINT("Creating Real Time Recorder Control Pipe " DEV_RTP_STR "...");
	err = rt_pipe_create(&pipe_desc, "RecorderControlPipe", DEV_RTP_NR, 0);

	switch (err)
	{
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
	err = rt_task_create(&def_recorder_control_task, "RecorderControlTask", 4096,
			99, T_FPU);
	switch (err)
	{
	case 0:
		break;
	default:
		DPRINT("Can't create task RecorderControlTask");
		return -1;
	}
	err = rt_task_start(&def_recorder_control_task, &recorder_control_task, NULL );
	switch (err)
	{
	case 0:
		break;
	default:
		DPRINT("Can't start task RecorderControlTask");
		return -1;
	}

	return 0;
}

static void control_pipe_exit(void)
{
	DPRINT("Deleting Control Pipe...");
	rt_pipe_delete(&pipe_desc);

	DPRINT("Stopping Control Task...");
	rt_task_delete(&def_recorder_control_task);
}

module_init(control_pipe_init);
module_exit(control_pipe_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Joscha Ihl <joscha@grundfarm.de>");
MODULE_DESCRIPTION(
	"Control Pipe Linux Co-Kernel Module for Real Time Data Recorder");
