/*
 * read_shm_html.c
 *
 *  Created on: 04.02.2013
 *      Author: ihl
 */
#include <ringbuffer_model.h>
#include <stdio.h>

void read_shm(unsigned long long from, unsigned long long length, RingBuffer *ringBuffer)
{
	unsigned long long i;
	puts("<table border=1>");
    for(i = from; i <length;i++)
    {
    	puts("<tr>");
    	printf("<td>%llu</td><td>%u</td><td>%llu</td><td>%u</td>\n", i,
    			ringBuffer->sensorData[i].sensorID,
        		ringBuffer->sensorData[i].sampleTimeCode,
        		ringBuffer->sensorData[i].sensorValue);
    	puts("</tr>");
    }
	puts("</table>");
}
