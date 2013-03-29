#ifndef __XEN_RINGBUF_MODEL_H__
#define __XEN_RINGBUF_MODEL_H__

#include "ringbuffer_model.h"

/* Shared Memory */
#include <native/heap.h>
#include <native/mutex.h>

#ifdef USE_MUTEX
#define RBUF_MUTEX_NAME ringbuffer_mutex
#define RBUF_MUTEX RT_MUTEX RBUF_MUTEX_NAME;
#else
#define RBUF_MUTEX
#endif

#define RBUF_MEMBERS \
	RT_HEAP datarecorder_heap; \
	void *shmem; \
	RingBuffer *ringBuffer; \
	RBUF_MUTEX

#endif
