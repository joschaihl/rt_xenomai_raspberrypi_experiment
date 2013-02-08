/*
 * RingBufferIndex.cpp
 *
 *  Created on: 07.02.2013
 *      Author: ihl
 */

#include "RingBufferPageIndex.h"
#include "DataRecorderExceptions.h"

/**
 * @param ringBufferConsumer
 * @param page
 * @param page_width
 * Generates a Page Index
 *
 *
 */
RingBufferPageIndex::RingBufferPageIndex(RingBufferConsumer &ringBufferConsumer)
 : index(0), ringBufferConsumer(ringBufferConsumer),
   count_pages(0), page(0), page_width(DEFAULT_PAGE_WIDTH)
{

}

void RingBufferPageIndex::setPage(unsigned long long page,
		unsigned long long page_width)
{
	if(page_width > 0)
	{
		this->page_width = page_width;
		count_pages = ringBufferConsumer.getSize() / page_width;
		if(page > count_pages)
		{
			throw RingBufferPageOutOfRange();
		}
		else
		{
			this->page = page;
		}
	}
	else
	{
		throw RingBufferPageOutOfRange();
	}
	this->index = page_width * page;
}

unsigned long long RingBufferPageIndex::getPageWidth()
{
	return this->page_width;
}
unsigned long long RingBufferPageIndex::getPage()
{
	return this->page;
}

unsigned long long RingBufferPageIndex::getIndex()
{
	return this->index;
}

bool RingBufferPageIndex::incrementIndex()
{
	this->index++;
	return true;
}

RingBufferPageIndex::~RingBufferPageIndex()
{

}

