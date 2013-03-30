#ifndef __XEN_RINGBUF_CONTROLLER_H__
#define __XEN_RINGBUF_CONTROLLER_H__

#include "xen_ringbuf_model.h"

#if SYNC_METHOD == NO_SYNC
	#define CRITICAL_RINGBUFFER_ACCESS(macro) { \
		macro; \
	}
#elif SYNC_METHOD == SYNC_WITH_ATOMIC_EXECUTION
	#ifdef __KERNEL__
		#define CRITICAL_RINGBUFFER_ACCESS(macro) RTDM_EXECUTE_ATOMICALLY(macro)
	#else
		#define CRITICAL_RINGBUFFER_ACCESS(macro) { \
			macro; \
		}
	#endif
#elif SYNC_METHOD == SYNC_WITH_SCHEDULER_LOCK
	#define CRITICAL_RINGBUFFER_ACCESS(macro) { \
		macro; \
	}
#elif SYNC_METHOD == SYNC_WITH_MUTEX
	#define CRITICAL_RINGBUFFER_ACCESS(macro) { \
		rt_mutex_acquire(&ringbuffer_mutex, TM_INFINITE);\
		macro; \
		rt_mutex_release(&ringbuffer_mutex);\
	}
#elif SYNC_METHOD == SYNC_WITH_SPINLOCK

#endif

#endif
