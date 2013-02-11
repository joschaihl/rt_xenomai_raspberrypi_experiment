/*
 * RingBufferLastDataIndex.cpp
 *
 *  Created on: 09.02.2013
 *      Author: ihl
 */

#include "RingBufferLastDataIndex.h"
#include <cstdlib>

/**
 * Sets the range from from the last recorded Data Index.
 * Beginning Index =  (end_index + (ringbuffersize - (datasets-1))) mod ringbuffersize.
 * It throws not an IndexOutOfRangeException if:
 * 1. The end_index must be smaller than the ringbuffersize.
 * 2. The datasets value must be bigger than 0.
 * @param end_index		The ending index, for example the current Index.
 * @param datasets	The amount of datasets.
 */
void RingBufferLastDataIndex::setLastData(unsigned long long end_index,
		unsigned long long datasets)
{
	if(end_index < ringBufferConsumer.getSize() && datasets > 0)
	{
		if(datasets > ringBufferConsumer.getSize())
		{
			datasets = ringBufferConsumer.getSize();
		}
		unsigned long long size =  ringBufferConsumer.getSize();

		//this->index = ringBufferConsumer.getSize() - llabs(datasets-(end_index+1));
		this->index = (end_index + (size - (datasets-1))) % size;
		this->datasets = datasets;
		this->incrementCounter = 0;
	}
	else
	{
		throw IndexOutOfRangeException();
	}
}

unsigned long long RingBufferLastDataIndex::getDataSets()
{
	return this->datasets;
}

unsigned long long RingBufferLastDataIndex::getIndex()
{
	return this->index;
}

/**
 * It increments the index.
 * The index can not be incremented if the last index is reached.
 * @result True if the index can be incremented.
 */
bool RingBufferLastDataIndex::incrementIndex()
{
	bool result = false;
	if(incrementCounter+1 < datasets)
	{
		if((index+1)<ringBufferConsumer.getSize())
		{
			index++;
		}
		else
		{
			index = 0;
		}

		incrementCounter++;
		result = true;
	}
	return result;
}

/**
 * Default Constructor.
 * The default last index is 0.
 * If RingBufferConsumer.getSize() < DEFAULT_DATASETS.
 * The value for getDataSets() is DEFAULT_DATASETS.
 * Else getDataSets() = RingBufferConsumer.getSize().
 */
RingBufferLastDataIndex::RingBufferLastDataIndex(RingBufferConsumer &ringBufferConsumer)
: ringBufferConsumer(ringBufferConsumer), index(0), datasets(0), incrementCounter(0)
{
	setLastData(0, DEFAULT_DATASETS);
}

RingBufferLastDataIndex::~RingBufferLastDataIndex()
{

}

