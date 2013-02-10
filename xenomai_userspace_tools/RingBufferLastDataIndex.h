/*
 * RingBufferLastDataIndex.h
 *
 *  Created on: 09.02.2013
 *      Author: ihl
 */

#ifndef RINGBUFFERLASTDATAINDEX_H_
#define RINGBUFFERLASTDATAINDEX_H_

#include "IIncrementableIndex.h"
#include "RingBufferConsumer.h"

class RingBufferLastDataIndex: public IIncrementableIndex
{
private:
	RingBufferConsumer &ringBufferConsumer;
public:
	static const long long int DEFAULT_DATASETS = 100;
	RingBufferLastDataIndex(RingBufferConsumer &ringBufferConsumer);
	void setLastData(unsigned long long index, unsigned long long datasets = DEFAULT_DATASETS);
	virtual unsigned long long getIndex();
	virtual bool incrementIndex();
	virtual ~RingBufferLastDataIndex();
};

#endif /* RINGBUFFERLASTDATAINDEX_H_ */
