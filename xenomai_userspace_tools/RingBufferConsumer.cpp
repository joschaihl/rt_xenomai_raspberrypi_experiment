/*
 * RingBuffer.cpp
 *
 *  Created on: 05.02.2013
 *      Author: ihl
 */

#include <stdlib.h>
#include "RingBufferConsumer.h"
#include <rtdm/rtdm.h>
#include "ringbuffer_model.h"
#include "xen_ringbuf_controller.h"

RingBufferConsumer::RingBufferConsumer() : sharedMemoryIsReady(false),
shmem(NULL), ringBuffer(NULL), currentIndex(MAX_RINGBUFFER_SAMPLES)
{

}

bool RingBufferConsumer::init()
{
	int err;
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

	//check(0);
	return true;
}

void RingBufferConsumer::copyData(unsigned long long index)
throw(IndexOutOfRangeException)
{
	bool out_of_range = false;
	if(index != currentIndex)
	{
		CRITICAL_RINGBUFFER_ACCESS(
			out_of_range = (index >= this->ringBuffer->size);
			if(!out_of_range)
			{
				currentData = this->ringBuffer->sensorData[index];
			}
		);
		if(out_of_range)
		{
			throw IndexOutOfRangeException();
		}
	}
}

void RingBufferConsumer::check(unsigned long long index)
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
	check(index);
	result = currentData.sensorID;
	return result;
}

unsigned long long RingBufferConsumer::getSampleTimeCode(unsigned long long index)
throw(IndexOutOfRangeException, SharedMemoryNotInitialized)
{
	unsigned long long result;
	check(index);
	result = currentData.sampleTimeCode;
	return result;
}

unsigned char RingBufferConsumer::getSensorValue(unsigned long long index)
throw(IndexOutOfRangeException, SharedMemoryNotInitialized)
{
	unsigned char result;
	check(index);
	result = currentData.sensorValue;
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

		rt_heap_unbind(&datarecorder_heap);
	}
}

