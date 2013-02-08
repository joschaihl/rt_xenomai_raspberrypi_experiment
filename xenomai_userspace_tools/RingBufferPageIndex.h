/*
 * RingBufferIndex.h
 *
 *  Created on: 07.02.2013
 *      Author: ihl
 */

#ifndef RINGBUFFERINDEX_H_
#define RINGBUFFERINDEX_H_
#include "DataRecorderExceptions.h"
#include "RingBufferConsumer.h"

class RingBufferPageIndex
{
	RingBufferConsumer &ringBufferConsumer;
	unsigned long long index;
	unsigned long long max_index;
	unsigned long long page;
	unsigned long long page_width;
	unsigned long long count_pages;
public:
	static const long long int DEFAULT_PAGE_WIDTH = 100;

	RingBufferPageIndex(RingBufferConsumer &ringBufferConsumer);

	void setPage(unsigned long long page = 0,
			unsigned long long page_width = DEFAULT_PAGE_WIDTH);

	unsigned long long getPageWidth();
	unsigned long long getPage();
	unsigned long long getIndex();
	unsigned long long getCountPages();
	bool incrementIndex();
	virtual ~RingBufferPageIndex();
};

#endif /* RINGBUFFERINDEX_H_ */
