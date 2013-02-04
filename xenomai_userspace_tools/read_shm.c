#include <xen_ringbuf_controller.h>


int main (int argc, char *argv[])
{
    unsigned long long i;
    if(init_shmem_consumer()!=0)
    {
    	printf("Error");
    	return -1;
    }

  	printf("Ringbuffer size = %llu, overflows = %llu\n", ringBuffer->size, ringBuffer->overflows);
    for(i = 0; i < ringBuffer->size;i++)
    {
        printf("Sample %llu: %u %llu %u\n", i, ringBuffer->sensorData[i].sensorID, ringBuffer->sensorData[i].sampleTimeCode, ringBuffer->sensorData[i].sensorValue);
    }
    
    cleanup();
}



