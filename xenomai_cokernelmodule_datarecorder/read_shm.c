#include <stdint.h>
#include "ringbuffer_model.h"
#include <native/heap.h>

RT_HEAP heap_desc;

void *shared_mem; /* Start address of the shared memory segment */

/* A shared memory segment with Xenomai is implemented as a mappable
   real-time heap object managed as a single memory block. In this
   mode, the allocation routine always returns the start address of
   the heap memory to all callers, and the free routine always leads
   to a no-op. */

void cleanup (void)
{
    /* We need to unbind explicitly from the heap in order to
       properly release the underlying memory mapping. Exiting the
       process unbinds all mappings automatically. */
    rt_heap_unbind(&heap_desc);
}

int main (int argc, char *argv[])

{
    int err, i;

    /* Bind to a shared heap which has been created elsewhere, either
       in kernel or user-space. Here we cannot wait and the heap must
       be available at once, since the caller is not a Xenomai-enabled
       thread. The heap should have been created with the H_SHARED
       mode set. */

    err = rt_heap_bind(&heap_desc,SHM_NAME,TM_NONBLOCK);

    if (err)
    {
        printf("Can't bind to shared memory");
        return -1;
    }
    /* Get the address of the shared memory segment. The "size" and
       "timeout" arguments are unused here. */
    rt_heap_alloc(&heap_desc,0,TM_NONBLOCK,&shared_mem);
    
    //char *ch_Ptr = (char *) shared_mem;
    RingBuffer *rb = shared_mem;
    
    //SensorData *ringbuffer = shared_mem;
    printf("Ringbuffer size = %llu, overflows = %llu\n", rb->size, rb->overflows);
    for(i = 0; i < rb->size;i++)
    {
        printf("%u %llu %u\n", rb->sensorData[i].sensorID, rb->sensorData[i].sampleTimeCode, rb->sensorData[i].sensorValue);
    }
    //printf("shared_mem[%d]=%d\n",0,ch_Ptr[0]);

    /* ... */
    cleanup();
}



