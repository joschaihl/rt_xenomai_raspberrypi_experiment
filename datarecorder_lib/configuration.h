/*
 * configuration.h
 *
 *  Created on: 29.03.2013
 *      Author: ihl
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define NO_SYNC 0
#define SYNC_WITH_ATOMIC_EXECUTION 1
#define SYNC_WITH_SCHEDULER_LOCK 2
#define SYNC_WITH_MUTEX 3
#define SYNC_WITH_SPINLOCK 4

#define SYNC_METHOD SYNC_WITH_MUTEX
#define BUFFERED_RINGBUFFER_ACCESS


/*/dev/rtp7 */
#define DEV_RTP_NR 7
#define DEV_RTP_STR "/dev/rtp" STR(DEV_RTP_NR)

#define MAX_RINGBUFFER_SAMPLES 5000000
#define DEFAULT_SPEED 25 * 1000;
#define ENABLE_TIMESTAMP
// #define DEBUG_DATARECORDER
#define USE_RTDM_TIMER
// #define AVOID_LINUX_SYSTEM_CALLS

#define USE_TASK_SWITCH_OUTPUT_PIN

#define GPIO_PIN_1 17			 // GPIO_GEN6 on GPIO Header
#define GPIO_PIN_SAMPLE_STATUS_LED 16   // ACT LED on RPI
#define GPIO_PIN_TASK_SWITCH_OUT 24 // GPIO_GEN5 on GPIO Header


#if SYNC_METHOD == NO_SYNC
	#define USE_NO_SYNC
#elif SYNC_METHOD == SYNC_WITH_ATOMIC_EXECUTION
	#define USE_ATOMIC_EXECUTION
	#error Not fully implemented
#elif SYNC_METHOD == SYNC_WITH_SCHEDULER_LOCK
	#define USE_SCHEDULER_LOCK
	#error Not implemented
#elif SYNC_METHOD == SYNC_WITH_MUTEX
	#define USE_MUTEX
#elif SYNC_METHOD == SYNC_WITH_SPINLOCK
	#define USE_SPINLOCK
	#error Not implemented
#endif






#endif /* CONFIGURATION_H_ */
