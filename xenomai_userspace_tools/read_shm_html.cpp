/*
 * read_shm_html.c
 *
 *  Created on: 04.02.2013
 *      Author: ihl
 */

#include "SensorDataPrinterTool.h"

void printer(RingBufferConsumer &rbuf, IIncrementableIndex &indexer)
{
	puts("<table class=\"ringbufferdata\">");
	puts("<tr><td>Index</td><td>Sensor ID</td><td>Time Code (Jiffies)</td><td>Value</td></tr>");
	do
    {
    	puts("<tr>");
    	printf("<td>%llu</td><td>%u</td><td>%llu</td><td>%u</td>\n",indexer.getIndex(),
    			rbuf.getSensorID(indexer.getIndex()),
    			rbuf.getSampleTimeCode(indexer.getIndex()),
    			rbuf.getSensorValue(indexer.getIndex()));
    	puts("</tr>");
    } while(indexer.incrementIndex());
	puts("</table>");
}

int main(int argc, char **argv)
{
	SensorDataPrinterTool sprinter(argc, argv);
	return sprinter.run(printer);
}
