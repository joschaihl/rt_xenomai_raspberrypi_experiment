/*
 * read_shm_json.c
 *
 *  Created on: 04.02.2013
 *      Author: ihl
 */

#include "ArgumentParser.h"
#include "RingBufferConsumer.h"

int main(int argc, char **argv)
{
	unsigned long long i;
	unsigned long long to = 100;
	ArgumentParser aParser;
	RingBufferConsumer rbuf;
	if(aParser.parse(argc, argv))
	{
		if(rbuf.init())
		{
			switch(aParser.getReaderState())
			{
			case READ_LAST_WRITTEN_DATA:
				break;
			case READ_SPECIFIC_PAGE:
				printf("[");
			    for(i = 0; i <to;i++)
				{
					printf("[%llu,%u,%llu,%u]", i,
						rbuf.getSensorID(i),
						rbuf.getSampleTimeCode(i),
						rbuf.getSensorValue(i));
					if(i<(to -1))
					{
						printf(",");
					}
				}
				printf("]");
				break;
			case READER_STATE_UNDEFINED:
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
