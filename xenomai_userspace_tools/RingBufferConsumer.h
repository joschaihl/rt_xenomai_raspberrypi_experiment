/*
 * RingBuffer.h
 *
 *  Created on: 05.02.2013
 *      Author: ihl
 */

#ifndef RINGBUFFERCONSUMER_H_
#define RINGBUFFERCONSUMER_H_

#include "DataRecorderExceptions.h"
#include "ringbuffer_model.h"

/* Shared Memory */
#include <native/heap.h>

/**
 * Class for getting samples from the RealTime Datarecorder Ringbuffer
 */
class RingBufferConsumer
{
private:
	RT_HEAP recorder_heap;
	void *recorder_shmem;
	RingBuffer *recorder_ringBuffer;

	bool sharedMemoryIsReady;
	void check(unsigned long long index)
		throw(IndexOutOfRangeException, SharedMemoryNotInitialized);
public:
	RingBufferConsumer();
	bool init();
	bool setSize(long long ringBufferSize);
	unsigned long long getSize()
		throw(SharedMemoryNotInitialized);
    unsigned char getSensorID(unsigned long long index)
    	throw(IndexOutOfRangeException, SharedMemoryNotInitialized);
    unsigned long long getSampleTimeCode(unsigned long long index)
    	throw(IndexOutOfRangeException, SharedMemoryNotInitialized);
    unsigned char getSensorValue(unsigned long long index)
    	throw(IndexOutOfRangeException, SharedMemoryNotInitialized);

	virtual ~RingBufferConsumer();
};

#endif /* RINGBUFFER_H_ */
