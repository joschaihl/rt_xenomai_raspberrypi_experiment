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

/**
 * Class for calculating amount of Indices of last Data to a specific index.
 * It can be used for calculating the indices of last written data.
 */

class RingBufferLastDataIndex: public IIncrementableIndex
{
private:
	RingBufferConsumer &ringBufferConsumer;
	unsigned long long incrementCounter;
	unsigned long long index;
	unsigned long long datasets;
public:
	static const long long int DEFAULT_DATASETS = 100ULL;
	/**
	 * The default last index is 0.
	 * @param ringBufferConsumer The default constructor needs a RingBuffer.
	 * If RingBufferConsumer.getSize() < DEFAULT_DATASETS.
	 * The value for getDataSets() is DEFAULT_DATASETS.
	 * Else getDataSets() = RingBufferConsumer.getSize().
	 *
	 */
	RingBufferLastDataIndex(RingBufferConsumer &ringBufferConsumer);
	/**
	 * Sets the range from from the last recorded Data Index.
	 * Beginning Index =  (end_index + (ringbuffersize - (datasets-1))) mod ringbuffersize.
	 * It throws not an IndexOutOfRangeException if:
	 * 1. The end_index must be smaller than the ringbuffersize.
	 * 2. The datasets value must be bigger than 0.
	 * @param end_index		The ending index, for example the current Index.
	 * @param datasets	The amount of datasets.
	 */
	void setLastData(unsigned long long end_index, unsigned long long datasets = DEFAULT_DATASETS);
	virtual unsigned long long getIndex();
	unsigned long long getDataSets();
	/**
	 * It increments the index.
	 * The index can not be incremented if the last index is reached.
	 * @result True if the index can be incremented.
	 */
	virtual bool incrementIndex();
	virtual ~RingBufferLastDataIndex();
};

#endif /* RINGBUFFERLASTDATAINDEX_H_ */
