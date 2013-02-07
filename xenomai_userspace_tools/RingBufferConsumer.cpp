/*
 * RingBuffer.cpp
 *
 *  Created on: 05.02.2013
 *      Author: ihl
 */

#include <stdlib.h>
#include "RingBufferConsumer.h"

RingBufferConsumer::RingBufferConsumer() : sharedMemoryIsReady(false),
recorder_shmem(NULL), recorder_ringBuffer(NULL)
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
	err = rt_heap_bind(&recorder_heap,SHM_NAME,TM_NONBLOCK);

	if (err)
	{
		return false;
	}
	/* Get the address of the shared memory segment. The "size" and
       "timeout" arguments are unused here. */

	err = rt_heap_alloc(&recorder_heap,0,TM_NONBLOCK,&recorder_shmem);

	if (err)
	{
		return false;
	}

	recorder_ringBuffer = (RingBuffer *) recorder_shmem;
	sharedMemoryIsReady = true;
	return true;
}

void RingBufferConsumer::check(unsigned long long index)
throw(IndexOutOfRangeException, SharedMemoryNotInitialized)

{
	if(sharedMemoryIsReady==false)
	{
		throw SharedMemoryNotInitialized();
	}
	if(index >= this->recorder_ringBuffer->size)
	{
		throw IndexOutOfRangeException();
	}
}

unsigned long long RingBufferConsumer::getSize()
throw(SharedMemoryNotInitialized)
{
	if(sharedMemoryIsReady==false)
	{
		throw SharedMemoryNotInitialized();
	}
	return recorder_ringBuffer->size;
}

unsigned char RingBufferConsumer::getSensorID(unsigned long long index)
throw(IndexOutOfRangeException, SharedMemoryNotInitialized)
{
	check(index);
	return recorder_ringBuffer->sensorData[index].sensorID;
}

unsigned long long RingBufferConsumer::getSampleTimeCode(unsigned long long index)
throw(IndexOutOfRangeException, SharedMemoryNotInitialized)
{
	check(index);
	return recorder_ringBuffer->sensorData[index].sampleTimeCode;
}

unsigned char RingBufferConsumer::getSensorValue(unsigned long long index)
throw(IndexOutOfRangeException, SharedMemoryNotInitialized)
{
	check(index);
	return recorder_ringBuffer->sensorData[index].sensorValue;
}

RingBufferConsumer::~RingBufferConsumer()
{
	/* We need to unbind explicitly from the heap in order to
       properly release the underlying memory mapping. Exiting the
       process unbinds all mappings automatically. */
	if(sharedMemoryIsReady)
	{
		rt_heap_unbind(&recorder_heap);
	}
}

