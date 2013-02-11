/*
 * RingBufferLastDataIndex.h
 *
 *  Created on: 09.02.2013
 *      Author: ihl
 */

#ifndef RINGBUFFERLASTDATAINDEX_H_
#define RINGBUFFERLASTDATAINDEX_H_

#include "DataRecorderExceptions.h"
#include "IIncrementableIndex.h"
#include "RingBufferConsumer.h"

class RingBufferLastDataIndex: public IIncrementableIndex
{
private:
	RingBufferConsumer &ringBufferConsumer;
	unsigned long long incrementCounter;
	unsigned long long index;
	unsigned long long datasets;
public:
	static const long long int DEFAULT_DATASETS = 100ULL;
	RingBufferLastDataIndex(RingBufferConsumer &ringBufferConsumer);
	void setLastData(unsigned long long end_index, unsigned long long datasets = DEFAULT_DATASETS);
	virtual unsigned long long getIndex();
	unsigned long long getDataSets();
	virtual bool incrementIndex();
	virtual ~RingBufferLastDataIndex();
};

#endif /* RINGBUFFERLASTDATAINDEX_H_ */
