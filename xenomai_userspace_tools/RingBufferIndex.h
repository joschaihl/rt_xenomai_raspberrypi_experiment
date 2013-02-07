/*
 * RingBufferIndex.h
 *
 *  Created on: 07.02.2013
 *      Author: ihl
 */

#ifndef RINGBUFFERINDEX_H_
#define RINGBUFFERINDEX_H_
#include "IndexOutOfRangeException.h"

class RingBufferIndex
{
	unsigned long long index;
	unsigned long long page;
	unsigned long long page_width;
public:
	RingBufferIndex(unsigned long long page, unsigned long long page_width);
	unsigned long long getPageWidth();
	unsigned long long getPage();
	unsigned long long getIndex();
	bool incrementIndex();
	virtual ~RingBufferIndex();
};

#endif /* RINGBUFFERINDEX_H_ */
