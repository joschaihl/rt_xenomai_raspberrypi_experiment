#ifndef __XEN_RINGBUF_CONTROLLER_H__
#define __XEN_RINGBUF_CONTROLLER_H__

#include "xen_ringbuf_model.h"

#ifdef USE_MUTEX
#define CRITICAL_RINGBUFFER_ACCESS(macro) { \
	rt_mutex_acquire(&ringbuffer_mutex, TM_INFINITE);\
	macro; \
	rt_mutex_release(&ringbuffer_mutex);\
}
#else
#define CRITICAL_RINGBUFFER_ACCESS(macro) { \
	macro; \
}
#endif

#endif
