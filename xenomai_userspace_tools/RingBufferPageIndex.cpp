/*
 * RingBufferIndex.cpp
 *
 *  Created on: 07.02.2013
 *      Author: ihl
 */

#include "RingBufferPageIndex.h"
#include "DataRecorderExceptions.h"

/**
 * Constructor for the RingBufferPageIndex calculator class.
 * @param ringBufferConsumer
 *
 *
 */
RingBufferPageIndex::RingBufferPageIndex(RingBufferConsumer &ringBufferConsumer) :
		index(0), ringBufferConsumer(ringBufferConsumer), count_pages(0), page(
				0), page_width(DEFAULT_PAGE_WIDTH), max_index(0)
{
	setPage(0, DEFAULT_PAGE_WIDTH);
}

/**
 * Set a new Index for viewing a Memory Page in the RingBuffer.
 * It throws an exception if the page_width = 0 or
 * if the page index exceeds the maximum possible value.
 *
 * Example:
 * Page width is 5 Page is 1
 * => start index is 5 * 1
 * => max_index is 9 = start index + page width -1
 * because 5, 6, 7, 8, 9 are possible values
 * Other Example:
 * Page width is 1 Page is 0
 * => max_index = start_index + page_width -1 = 0
 * If Page width is 1 Pager is 5
 * => start_index = 5*1 = 5
 * => max_index = 5
 *  bigger than ringbuffer / pagewidth.
 * @param page			The page index Nr. begins with 0 ends
 * 						with size of ringbuffer / page_width
 * @param page_width	The maximum Page width for a RingBuffer Page.
 * 						If the page size is bigger than the ringbuffer size it will be
 * 						shrinked to the ringbuffer size.
 */
void RingBufferPageIndex::setPage(unsigned long long page,
		unsigned long long page_width)
{
	unsigned long long count_p;
	if (page_width > 0)
	{
		if (page_width >= ringBufferConsumer.getSize())
			page_width = ringBufferConsumer.getSize();

		count_p = ringBufferConsumer.getSize() / page_width;
		/**
		 * If we have size/page_width = 10/5 = 5 than we
		 * have to pages:
		 * 0 1 2 3 4 | 5 6 7 8 9
		 * If we have 10/3 than we have 4 pages:
		 * 0 1 2 | 3 4 5 | 6 7 8 | 9 10
		 */
		if((ringBufferConsumer.getSize() % page_width)>0)
			count_p++;
		if (page >= count_p)
		{
			throw RingBufferPageOutOfRange();
		}
		else
		{
			this->page = page;
			this->page_width = page_width;
			this->count_pages = count_p;

			this->index = page_width * page;

			this->max_index = this->index + page_width - 1;
			if (this->max_index > ringBufferConsumer.getSize())
			{
				this->max_index = ringBufferConsumer.getSize() - 1;
			}
		}
	}
	else
	{
		throw RingBufferPageOutOfRange();
	}
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

/**
 * Increments the index in the Page to the next possible value.
 * For example if the page_width is 5 and the index is 0
 * possible values are 0, 1, 2, 3, 4.
 * @return It returns false if the Page Index is already at the end,
 * so that it can't be incremented.
 */
bool RingBufferPageIndex::incrementIndex()
{
	bool result = false;
	if (index < max_index)
	{
		this->index++;
		result = true;
	}
	return result;
}

unsigned long long RingBufferPageIndex::getPossibleIncrementations()
{
	return this->max_index - this->index;
}

unsigned long long RingBufferPageIndex::getCountPages()
{
	return this->count_pages;
}

RingBufferPageIndex::~RingBufferPageIndex()
{

}

