#include <xen_ringbuf_controller.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main (int argc, char *argv[])
{
    unsigned long long i;
    if(init_shmem_consumer()!=0)
    {
    	printf("Error");
    	return -1;
    }
/*       while ((c = getopt (argc, argv, "abc:")) != -1)
         switch (c)
           {
           case 'a':
             aflag = 1;
             break;
           case 'b':
             bflag = 1;
             break;
           case 'c':
             cvalue = optarg;
             break;
           case '?':
             if (optopt == 'c')
               fprintf (stderr, "Option -%c requires an argument.\n", optopt);
             else if (isprint (optopt))
               fprintf (stderr, "Unknown option `-%c'.\n", optopt);
             else
               fprintf (stderr,
                        "Unknown option character `\\x%x'.\n",
                        optopt);
             return 1;
           default:
             abort ();
           }

       printf ("aflag = %d, bflag = %d, cvalue = %s\n",
               aflag, bflag, cvalue);

      for (index = optind; index < argc; index++)
         printf ("Non-option argument %s\n", argv[index]);
 */

  	//printf("Ringbuffer size = %llu, overflows = %llu<br>\n", ringBuffer->size, ringBuffer->overflows);
    //puts("<table border=1>");
    //ringBuffer->size = 100;
    for(i = 0; i < /* ringBuffer->size*/ 1000;i++)
    {
    	if(i<(/*ringBuffer->size -1*/ 1000-1))
    	{
    		printf("[%llu,%u,%llu,%u],", i,
    			ringBuffer->sensorData[i].sensorID,
        		ringBuffer->sensorData[i].sampleTimeCode,
        		ringBuffer->sensorData[i].sensorValue);
    	}
    	else
    	{
    		printf("[%llu,%u,%llu,%u]", i,
    			ringBuffer->sensorData[i].sensorID,
        		ringBuffer->sensorData[i].sampleTimeCode,
        		ringBuffer->sensorData[i].sensorValue);
    	}
    }
    //puts("</table>");
    
    cleanup();
}



