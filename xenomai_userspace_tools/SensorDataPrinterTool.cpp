/*
 * SensorDataPrinterTool.cpp
 *
 *  Created on: 12.02.2013
 *      Author: ihl
 */

#include "SensorDataPrinterTool.h"
#include "RingBufferLastDataIndex.h"
#include "RingBufferPageIndex.h"
#include <sys/mman.h>
#include <native/task.h>

RT_TASK DataPrinterTask_desc;
int task_result = 0;

struct PrinterTaskParameters {
	void (&printer) (RingBufferConsumer &rbuf, IIncrementableIndex &indexer);
	SensorDataPrinterTool &tool;
	PrinterTaskParameters(void (&a_printer) (RingBufferConsumer &rbuf, IIncrementableIndex &indexer),
			SensorDataPrinterTool &a_tool) : printer(a_printer), tool(a_tool)
	{

	}
};


SensorDataPrinterTool::SensorDataPrinterTool(int argc, char **argv) :
	argc(argc), argv(argv)
{
	mlockall(MCL_CURRENT|MCL_FUTURE);
}

void data_printer_task(void *cookie)
{
	PrinterTaskParameters *p = (PrinterTaskParameters *) cookie;
	if(p->tool.aParser.parse(p->tool.argc, p->tool.argv))
	{
		if(p->tool.rbuf.init())
		{
			if(p->tool.aParser.getReaderState()==READ_LAST_WRITTEN_DATA)
			{
				RingBufferLastDataIndex rbld(p->tool.rbuf);
				rbld.setLastData(p->tool.rbuf.getCurrentIndex(), p->tool.aParser.getMaxLength());
				p->printer(p->tool.rbuf, rbld);
			}
			else if (p->tool.aParser.getReaderState()==READ_SPECIFIC_PAGE)
			{
				RingBufferPageIndex rbpi(p->tool.rbuf);
				rbpi.setPage(p->tool.aParser.getPage(), p->tool.aParser.getMaxLength());
				p->printer(p->tool.rbuf, rbpi);
			}
			else
			{
				task_result = -1;
			}
		}
		else
		{
			fprintf(stderr, "Can't bind to shared memory");
			task_result = -1;
		}
	}
	else
		task_result = -1;

}

int SensorDataPrinterTool::run(void (&printer) (RingBufferConsumer &rbuf, IIncrementableIndex &indexer))
{
	/* disable swapping and paging, because of determinism */
	int result;

	/*
	result = rt_task_create(&DataPrinterTask_desc,
                         "DataPrinterTask",
                         0,
                         99,
                         T_FPU);
    if (!result)
    {
    	PrinterTaskParameters printerTaskParameters(printer, *this);
        rt_task_start(&DataPrinterTask_desc,&data_printer_task,&printerTaskParameters);
        // wait for termination
        result = rt_task_join(&DataPrinterTask_desc);
        if(!result)
        	result = task_result;
    }

*/


	PrinterTaskParameters printerTaskParameters(printer, *this);
	data_printer_task((void *) &printerTaskParameters);

	return task_result;

}

SensorDataPrinterTool::~SensorDataPrinterTool()
{
	// TODO Auto-generated destructor stub
}

