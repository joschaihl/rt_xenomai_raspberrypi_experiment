/*
 * IIncrementableIndex.h
 *
 *  Created on: 09.02.2013
 *      Author: ihl
 */

#ifndef IINCREMENTABLEINDEX_H_
#define IINCREMENTABLEINDEX_H_

class IIncrementableIndex
{
public:
	virtual unsigned long long getIndex() = 0;
	virtual unsigned long long getPossibleIncrementations() = 0;
	virtual bool incrementIndex() = 0;
	virtual ~IIncrementableIndex() {};
};

#endif /* IINCREMENTABLEINDEX_H_ */
