/*
 * SensorDataPrinterTool.h
 *
 *  Created on: 12.02.2013
 *      Author: ihl
 */

#ifndef SENSORDATAPRINTERTOOL_H_
#define SENSORDATAPRINTERTOOL_H_

#include "RingBufferConsumer.h"
#include "ArgumentParser.h"
#include "IIncrementableIndex.h"

class SensorDataPrinterTool
{
public:
	int argc;
	char **argv;
	ArgumentParser aParser;
	RingBufferConsumer rbuf;

	SensorDataPrinterTool(int argc, char **argv);
	int run(void (&printer) (RingBufferConsumer &rbuf, IIncrementableIndex &indexer));
	virtual ~SensorDataPrinterTool();
};

#endif /* SENSORDATAPRINTERTOOL_H_ */
