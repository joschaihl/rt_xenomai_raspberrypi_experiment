/*
 * RingBufferIndex.cpp
 *
 *  Created on: 07.02.2013
 *      Author: ihl
 */

#include "RingBufferIndex.h"

RingBufferIndex::RingBufferIndex(unsigned long long page, unsigned long long page_width)
 : index(0)
{
	this->page = page;
	this->page_width = page_width;
}
unsigned long long RingBufferIndex::getPageWidth()
{
	return this->page_width;
}
unsigned long long RingBufferIndex::getPage()
{
	return this->page;
}

unsigned long long RingBufferIndex::getIndex()
{
	return this->index;
}

bool RingBufferIndex::incrementIndex()
{
	this->index++;
	return true;
}

RingBufferIndex::~RingBufferIndex()
{

}

