/*
 * RingBufferLastDataIndex.cpp
 *
 *  Created on: 09.02.2013
 *      Author: ihl
 */

#include "RingBufferLastDataIndex.h"

void RingBufferLastDataIndex::setLastData(unsigned long long index, unsigned long long datasets)
{

}

unsigned long long RingBufferLastDataIndex::getIndex()
{
	return 0;
}

bool RingBufferLastDataIndex::incrementIndex()
{
	return false;
}


RingBufferLastDataIndex::RingBufferLastDataIndex(RingBufferConsumer &ringBufferConsumer)
: ringBufferConsumer(ringBufferConsumer)
{

}

RingBufferLastDataIndex::~RingBufferLastDataIndex()
{

}

