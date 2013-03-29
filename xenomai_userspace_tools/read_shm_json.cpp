/*
 * read_shm_json.c
 *
 *  Created on: 04.02.2013
 *      Author: ihl
 */

#include "SensorDataPrinterTool.h"
/**
 *
 * @param rbuf
 * @param indexer
 */
void printer(RingBufferConsumer &rbuf, IIncrementableIndex &indexer)
{
	bool isNotLastIndex = true;
	printf("[");
	do
	{
		printf("[%llu,%u,%llu,%u]", indexer.getIndex(),
			rbuf.getSensorID(indexer.getIndex()),
			rbuf.getSampleTimeCode(indexer.getIndex()),
			rbuf.getSensorValue(indexer.getIndex()));
		isNotLastIndex = indexer.incrementIndex();
		if(isNotLastIndex)
		{
			printf(",");
		}
	} while(isNotLastIndex);
	printf("]");
}
/**
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char **argv)
{
	SensorDataPrinterTool sprinter(argc, argv);
	return sprinter.run(printer);
}
