/*
 * RingBuffer.cpp
 *
 *  Created on: 05.02.2013
 *      Author: ihl
 */

#include "RingBufferConsumer.h"

#include "ringbuffer_model.h"
#include "xen_ringbuf_controller.h"

#include <rtdm/rtdm.h>
#include <sys/mman.h>
#include <stdlib.h>

RingBufferConsumer::RingBufferConsumer() : sharedMemoryIsReady(false),
shmem(NULL), ringBuffer(NULL), currentIndex(0), needs_refresh(true),
fromIndex(0), toIndex(0), holds_copy(false), localCopy(NULL) /*,
currentDataSet(NULL)*/
{

}

bool RingBufferConsumer::init()
{
	int err;
	/**
	 * Disable swapping and paging for Real-Time
	 */
	mlockall(MCL_CURRENT|MCL_FUTURE);
	/* Bind to a shared heap which has been created elsewhere, either
       in kernel or user-space. Here we cannot wait and the heap must
       be available at once, since the caller is not a Xenomai-enabled
       thread. The heap should have been created with the H_SHARED
       mode set. */
	err = rt_heap_bind(&datarecorder_heap,SHM_NAME,TM_NONBLOCK);

	if (err)
	{
		return false;
	}
	/* Get the address of the shared memory segment. The "size" and
       "timeout" arguments are unused here. */

	err = rt_heap_alloc(&datarecorder_heap,0,TM_NONBLOCK,&shmem);

	if (err)
	{
		return false;
	}

	ringBuffer = (RingBuffer *) shmem;
	sharedMemoryIsReady = true;

#ifdef USE_MUTEX
	/*
	 * Bind to existing mutex and don't wait if it not exist
	 */
	err = rt_mutex_bind(&ringbuffer_mutex, SHM_MUTEX_NAME, TM_NONBLOCK);
#endif

	return true;
}

void RingBufferConsumer::copyData(unsigned long long index)
throw(IndexOutOfRangeException)
{
	bool out_of_range = false;
	if(needs_refresh || index != currentIndex)
	{
		CRITICAL_RINGBUFFER_ACCESS(
			out_of_range = (index >= this->ringBuffer->size);
			if(!out_of_range)
			{
				localSingleCopy = this->ringBuffer->sensorData[index];
			}
		);

		if(out_of_range)
		{
			throw IndexOutOfRangeException();
		}
		// currentDataSet = &localSingleCopy;
	}
}

void RingBufferConsumer::checkRange(unsigned long long from, unsigned long long to)
	throw(IndexOutOfRangeException, SharedMemoryNotInitialized)
{
	bool isInRange;
	if(sharedMemoryIsReady==false)
	{
		throw SharedMemoryNotInitialized();
	}
	isInRange = (from <= to);
	if(isInRange)
	{
		CRITICAL_RINGBUFFER_ACCESS(
			isInRange = (from < this->ringBuffer->size) && (to < this->ringBuffer->size);
		);
	}

	if(!isInRange)
	{
		throw IndexOutOfRangeException();
	}
}

void RingBufferConsumer::checkAndCopy(unsigned long long index)
throw(IndexOutOfRangeException, SharedMemoryNotInitialized)

{
	if(sharedMemoryIsReady==false)
	{
		throw SharedMemoryNotInitialized();
	}
	/**
	 * Critical section
	 */

	copyData(index);
}

bool RingBufferConsumer::setSize(unsigned long long ringBufferSize)
{
	bool result = false;
	if(sharedMemoryIsReady==false)
	{
		throw SharedMemoryNotInitialized();
	}

	if(ringBufferSize <= MAX_RINGBUFFER_SAMPLES)
	{
		CRITICAL_RINGBUFFER_ACCESS(this->ringBuffer->size = ringBufferSize);
		result = true;
	}

	return result;
}

unsigned long long RingBufferConsumer::getSize()
throw(SharedMemoryNotInitialized)
{
	unsigned long long result;
	if(sharedMemoryIsReady==false)
	{
		throw SharedMemoryNotInitialized();
	}
	CRITICAL_RINGBUFFER_ACCESS(result = ringBuffer->size);
	return result;
}

unsigned char RingBufferConsumer::getSensorID(unsigned long long index)
throw(IndexOutOfRangeException, SharedMemoryNotInitialized)
{
	unsigned char result;
	checkAndCopy(index);
	result = localSingleCopy.sensorID;
	return result;
}

unsigned long long RingBufferConsumer::getSampleTimeCode(unsigned long long index)
throw(IndexOutOfRangeException, SharedMemoryNotInitialized)
{
	unsigned long long result;
	checkAndCopy(index);
	result = localSingleCopy.sampleTimeCode;
	return result;
}

unsigned char RingBufferConsumer::getSensorValue(unsigned long long index)
throw(IndexOutOfRangeException, SharedMemoryNotInitialized)
{
	unsigned char result;
	checkAndCopy(index);
	result = localSingleCopy.sensorValue;
	return result;
}

unsigned long long RingBufferConsumer::getCurrentIndex()
	throw(SharedMemoryNotInitialized)
{
	unsigned long long result;
	if(sharedMemoryIsReady==false)
	{
		throw SharedMemoryNotInitialized();
	}
	CRITICAL_RINGBUFFER_ACCESS(result = ringBuffer->index);
	return result;
}

bool RingBufferConsumer::getSharedMemoryCopy(int p_fromIndex, int p_toIndex)
throw(IndexOutOfRangeException, SharedMemoryNotInitialized)
{
	bool result = false;
	unsigned long int sizeToReserve = (p_toIndex - p_fromIndex +1)*sizeof(SensorData);

	// Check if fromIndex or toIndex is valid
	checkRange(p_fromIndex, p_toIndex);

	if(localCopy==NULL)
	{
		localCopy = (SensorData *) malloc(sizeToReserve);
	}
	else
	{
		localCopy = (SensorData *) realloc(localCopy, sizeToReserve);
	}
	if(localCopy==NULL)
	{
		holds_copy = false;
		needs_refresh = true;
		fromIndex = 0;
		toIndex = 0;
		result = false;
	}
	else
	{
		void *fromPtr = NULL;
		fromPtr = &ringBuffer->sensorData[fromIndex];

		// Switch to Real-Time-Mode and fill the local copy with data
		rt_mutex_acquire(&ringbuffer_mutex, TM_INFINITE);
		memcpy(localCopy, fromPtr, sizeToReserve);
		rt_mutex_release(&ringbuffer_mutex);

		holds_copy = true;
		fromIndex = p_fromIndex;
		toIndex = p_toIndex;


		result = true;
	}
	return result;
}

RingBufferConsumer::~RingBufferConsumer()
{
	/* We need to unbind explicitly from the heap in order to
       properly release the underlying memory mapping. Exiting the
       process unbinds all mappings automatically. */
	if(sharedMemoryIsReady)
	{

#ifdef USE_MUTEX
	/*
	 * Bind to existing mutex and don't wait if it not exist
	 */
		rt_mutex_unbind(&ringbuffer_mutex);
#endif
		if(holds_copy)
		{
			free(localCopy);
		}

		rt_heap_unbind(&datarecorder_heap);
	}
}

