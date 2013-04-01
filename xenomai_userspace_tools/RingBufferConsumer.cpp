/*
 * RingBuffer.cpp
 *
 *  Created on: 05.02.2013
 *      Author: ihl
 */

#include "RingBufferConsumer.h"

#include "ringbuffer_model.h"
#include "xen_ringbuf_controller.h"
#include <sys/mman.h>
#include <rtdm/rtdm.h>
#include <stdlib.h>

RingBufferConsumer::RingBufferConsumer() : sharedMemoryIsReady(false),
shmem(NULL), ringBuffer(NULL), currentIndex(0), needs_refresh(true),
fromIndex(0), toIndex(0), toIndex2(0), holds_copy(false), holds_copy2(false),
localCopy(NULL),
localCopy2(NULL), currentDataSet(NULL)
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

	return true;
}

void RingBufferConsumer::copyData(unsigned long long index)
throw(IndexOutOfRangeException)
{
	bool out_of_range = false;
#ifdef BUFFERED_RINGBUFFER_ACCESS
	if (holds_copy && index != currentIndex &&
			index >= fromIndex && index <= toIndex)
	{
		// Use the local copy
		// Calculate the offset of the index
		// Eg. if we copied index 100-200
		// and need index 150
		// the calculated index is 50 = 150-100 = index - fromIndex
		unsigned long long copyIndex = index - fromIndex;
		currentDataSet = &localCopy[copyIndex];
	}
	else if (holds_copy2 && index != currentIndex &&
			index >= 0 && index <= toIndex2)
	{
		currentDataSet = &localCopy2[index];
	}
	else
#endif
	if (needs_refresh || index != currentIndex)
	{
		/**
		 * Critical section
		 */
		CRITICAL_RINGBUFFER_ACCESS(
				out_of_range = (index >= this->ringBuffer->size);
				if(!out_of_range)
				{
					localSingleCopy = this->ringBuffer->sensorData[index];
				}
		);
		currentDataSet = &localSingleCopy;
		needs_refresh = false;
	}

	if (out_of_range)
	{
		throw IndexOutOfRangeException();
	}
	currentIndex = index;
}

void RingBufferConsumer::checkRange(unsigned long long from,
		unsigned long long to)
	throw(IndexOutOfRangeException, SharedMemoryNotInitialized)
{
	bool isInRange;
	if(sharedMemoryIsReady==false)
	{
		throw SharedMemoryNotInitialized();
	}
	/**
	 * Critical section
	 */
	CRITICAL_RINGBUFFER_ACCESS(
			isInRange = (from < this->ringBuffer->size) && (to < this->ringBuffer->size);
	);

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
	result = currentDataSet->sensorID;
	return result;
}

unsigned long long RingBufferConsumer::getSampleTimeCode(unsigned long long index)
throw(IndexOutOfRangeException, SharedMemoryNotInitialized)
{
	unsigned long long result;
	checkAndCopy(index);
	result = currentDataSet->sampleTimeCode;
	return result;
}

unsigned char RingBufferConsumer::getSensorValue(unsigned long long index)
throw(IndexOutOfRangeException, SharedMemoryNotInitialized)
{
	unsigned char result;
	checkAndCopy(index);
	result = currentDataSet->sensorValue;
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
	bool result = true;
	unsigned long int sizeToReserve;

#ifdef BUFFERED_RINGBUFFER_ACCESS

	// Check if fromIndex or toIndex is valid
	checkRange(p_fromIndex, p_toIndex);

	if(p_fromIndex <= p_toIndex)
	{
		sizeToReserve = (p_toIndex - p_fromIndex +1)*sizeof(SensorData);
		if(localCopy==NULL)
			localCopy = (SensorData *) malloc(sizeToReserve);
		else
			localCopy = (SensorData *) realloc(localCopy, sizeToReserve);
		if(localCopy==NULL)
		{
			result = false;
		}
		else
		{
			void *fromPtr = NULL;
			fromPtr = &ringBuffer->sensorData[fromIndex];

			// Switch to Real-Time-Mode and fill the local copy with data
			CRITICAL_RINGBUFFER_ACCESS(memcpy(localCopy, fromPtr, sizeToReserve));

			holds_copy = true;
			holds_copy2 = false;
			fromIndex = p_fromIndex;
			toIndex = p_toIndex;
			toIndex2 = 0;

		}
	}
	else
	{
		unsigned long long rbufSize;

		rbufSize = getSize();

		sizeToReserve = (rbufSize-p_fromIndex)*sizeof(SensorData);
		if(localCopy==NULL)
			localCopy = (SensorData *) malloc(sizeToReserve);
		else
			localCopy = (SensorData *) realloc(localCopy, sizeToReserve);

		if(localCopy==NULL)
		{
			result = false;
		}
		else
		{
			unsigned long int sizeToReserve2;
			sizeToReserve2 = (toIndex)*sizeof(SensorData);
			if(localCopy2==NULL)
				localCopy2 = (SensorData *) malloc(sizeToReserve2);
			else
				localCopy2 = (SensorData *) realloc(localCopy, sizeToReserve2);
			if(localCopy2==NULL)
			{
				result = false;
			}
			else
			{
				void *fromPtr = NULL, *fromPtr2 = NULL;
				fromPtr = &ringBuffer->sensorData[fromIndex];
				fromPtr2 = &ringBuffer->sensorData[0];

				// Switch to Real-Time-Mode and fill the local copy with data
				CRITICAL_RINGBUFFER_ACCESS(
						memcpy(localCopy, fromPtr, sizeToReserve);
						memcpy(localCopy, fromPtr2, sizeToReserve2);
				);

				holds_copy = true;
				holds_copy2 = true;
				fromIndex = p_fromIndex;
				toIndex = p_toIndex;
			}
		}
	}

	if(!result)
	{
		holds_copy = false;
		holds_copy2 = false;
		//needs_refresh = true;
		fromIndex = 0;
		toIndex = 0;
		toIndex2 = 0;
	}
#endif
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
		if(holds_copy2)
		{
			free(localCopy2);
		}

		rt_heap_unbind(&datarecorder_heap);
	}
}

