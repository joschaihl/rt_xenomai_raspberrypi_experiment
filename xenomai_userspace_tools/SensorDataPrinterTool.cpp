/*
 * SensorDataPrinterTool.cpp
 *
 *  Created on: 12.02.2013
 *      Author: ihl
 */

#include "SensorDataPrinterTool.h"
#include "RingBufferLastDataIndex.h"
#include "RingBufferPageIndex.h"

SensorDataPrinterTool::SensorDataPrinterTool(int argc, char **argv) :
	argc(argc), argv(argv)
{

}

int SensorDataPrinterTool::run(void (&printer) (RingBufferConsumer &rbuf, IIncrementableIndex &indexer))
{
	if(aParser.parse(argc, argv))
	{
		if(rbuf.init())
		{
			if(aParser.getReaderState()==READ_LAST_WRITTEN_DATA)
			{
				RingBufferLastDataIndex rbld(rbuf);
				rbld.setLastData(rbuf.getCurrentIndex(), aParser.getMaxLength());
				printer(rbuf, rbld);
			}
			else if (aParser.getReaderState()==READ_SPECIFIC_PAGE)
			{
				RingBufferPageIndex rbpi(rbuf);
				rbpi.setPage(aParser.getPage(), aParser.getMaxLength());
				printer(rbuf, rbpi);
			}
			else
			{
				return -1;
			}

			return 0;
		}
		else
		{
			fprintf(stderr, "Can't bind to shared memory");
			return -2;
		}
	}
	else
		return -1;
}

SensorDataPrinterTool::~SensorDataPrinterTool()
{
	// TODO Auto-generated destructor stub
}

