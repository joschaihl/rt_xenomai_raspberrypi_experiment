/*
 * RingBuffer.h
 *
 *  Created on: 05.02.2013
 *      Author: ihl
 */

#ifndef RINGBUFFERCONSUMER_H_
#define RINGBUFFERCONSUMER_H_

#include "DataRecorderExceptions.h"
#include "IDataContainer.h"
#include "ringbuffer_model.h"
#include "configuration.h"

/* Shared Memory */
#include <native/heap.h>
#include <native/mutex.h>

/**
 * Class for getting samples from the RealTime Datarecorder Ringbuffer
 */
class RingBufferConsumer
{
private:
	RT_HEAP datarecorder_heap;
#ifdef USE_MUTEX
	RT_MUTEX ringbuffer_mutex;
#endif
	void *shmem;
	RingBuffer *ringBuffer;
	bool sharedMemoryIsReady;
	void check(unsigned long long index)
		throw(IndexOutOfRangeException, SharedMemoryNotInitialized);
public:
	/**
	 *
	 */
	RingBufferConsumer();
	/**
	 *
	 * @return
	 */
	bool init();
	/**
	 *
	 * @param ringBufferSize
	 * @return
	 */
	bool setSize(unsigned long long ringBufferSize);
	/**
	 *
	 */
	unsigned long long getSize()
		throw(SharedMemoryNotInitialized);
	/**
	 *
	 */
	unsigned long long getCurrentIndex()
		throw(SharedMemoryNotInitialized);
	/**
	 *
	 * @param index
	 * @return
	 */
    unsigned char getSensorID(unsigned long long index)
    	throw(IndexOutOfRangeException, SharedMemoryNotInitialized);
    /**
     *
     * @param index
     */
    unsigned long long getSampleTimeCode(unsigned long long index)
    	throw(IndexOutOfRangeException, SharedMemoryNotInitialized);
    /**
     *
     * @param index
     * @return
     */
    unsigned char getSensorValue(unsigned long long index)
    	throw(IndexOutOfRangeException, SharedMemoryNotInitialized);

	virtual ~RingBufferConsumer();
};

#endif /* RINGBUFFER_H_ */
