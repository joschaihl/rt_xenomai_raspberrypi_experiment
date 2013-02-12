/*
 * read_shm_raw.c
 *
 *  Created on: 04.02.2013
 *      Author: ihl
 */
#include "SensorDataPrinterTool.h"
void printer(RingBufferConsumer &rbuf, IIncrementableIndex &indexer)
{
	do
    {
    	printf("%llu %u %llu %u\n",indexer.getIndex(),
    			rbuf.getSensorID(indexer.getIndex()),
    			rbuf.getSampleTimeCode(indexer.getIndex()),
    			rbuf.getSensorValue(indexer.getIndex()));
    } while(indexer.incrementIndex());
}

int main(int argc, char **argv)
{
	SensorDataPrinterTool sprinter(argc, argv);
	return sprinter.run(printer);
}
