/*
 * RingBufferLastDataIndex.cpp
 *
 *  Created on: 09.02.2013
 *      Author: ihl
 */

#include "RingBufferLastDataIndex.h"
#include <cstdlib>


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


RingBufferLastDataIndex::RingBufferLastDataIndex(RingBufferConsumer &ringBufferConsumer)
: ringBufferConsumer(ringBufferConsumer), index(0), datasets(0), incrementCounter(0)
{
	setLastData(0, DEFAULT_DATASETS);
}

RingBufferLastDataIndex::~RingBufferLastDataIndex()
{

}

