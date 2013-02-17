#include "rec_state.h"
#include "print.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/kernel.h>

SystemState recorderState = rec_state_init;
#define DEFAULT_SPEED 100 * 1000;
int current_speed = DEFAULT_SPEED;

void set_recorder_state(SystemState systemState)
{
	recorderState = systemState;
}

SystemState get_recorder_state(void)
{
	return recorderState;
}

void set_speed(int timer_overflow_in_nanoseconds)
{
	current_speed = timer_overflow_in_nanoseconds;
}

int get_speed(void)
{
	return current_speed;
}

static int recorder_state_init(void)
{
	DPRINT("Creating Recoder State Machine...");
	return 0;
}

static void recorder_state_exit(void)
{
	DPRINT("Deleting Recoder State Machine...");
}

module_init(recorder_state_init);
module_exit(recorder_state_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Joscha Ihl <joscha@grundfarm.de>");
MODULE_DESCRIPTION("Recorder State Machine Linux Co-Kernel Module for Real Time Data Recorder");
EXPORT_SYMBOL(set_recorder_state);
EXPORT_SYMBOL(get_recorder_state);
EXPORT_SYMBOL(get_speed);
EXPORT_SYMBOL(set_speed);
