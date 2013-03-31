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
#include "xen_ringbuf_model.h"
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
	RBUF_MEMBERS;
	bool needs_refresh;
	bool holds_copy;
	unsigned long long fromIndex;
	unsigned long long toIndex;
	SensorData *localCopy;
	SensorData localSingleCopy;
	//SensorData *currentDataSet;

	unsigned long long currentIndex;
	void copyData(unsigned long long index)
		throw(IndexOutOfRangeException);
	bool sharedMemoryIsReady;

	void checkAndCopy(unsigned long long index)
		throw(IndexOutOfRangeException, SharedMemoryNotInitialized);

	void checkRange(unsigned long long from, unsigned long long to)
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
	 * Gets a copy from the Shared Memory to
	 * allow faster access without heavy usage of
	 * mutex.
	 */
	bool getSharedMemoryCopy(int p_fromIndex, int p_toIndex)
		throw(IndexOutOfRangeException, SharedMemoryNotInitialized);
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
