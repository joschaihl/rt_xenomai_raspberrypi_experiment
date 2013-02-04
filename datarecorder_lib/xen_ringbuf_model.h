#ifndef __XEN_RINGBUF_MODEL_H__
#define __XEN_RINGBUF_MODEL_H__

#include "ringbuffer_model.h"

/* Shared Memory */
#include <native/heap.h>

RT_HEAP datarecorder_heap;
void *shmem = NULL;
RingBuffer *ringBuffer = NULL;

#endif
