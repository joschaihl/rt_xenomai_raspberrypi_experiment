#ifndef __XEN_RINGBUF_CONTROLLER_H__
#define __XEN_RINGBUF_CONTROLLER_H__

#include "xen_ringbuf_model.h"

int init_shmem_consumer(void)
{
    int err;
    /* Bind to a shared heap which has been created elsewhere, either
       in kernel or user-space. Here we cannot wait and the heap must
       be available at once, since the caller is not a Xenomai-enabled
       thread. The heap should have been created with the H_SHARED
       mode set. */
    err = rt_heap_bind(&datarecorder_heap,SHM_NAME,TM_NONBLOCK);

    if (err)
    {
        printf("Can't bind to shared memory");
        return -1;
    }
    /* Get the address of the shared memory segment. The "size" and
       "timeout" arguments are unused here. */

    rt_heap_alloc(&datarecorder_heap,0,TM_NONBLOCK,&shmem);

    ringBuffer = (RingBuffer *) shmem;
    return 0;
}


void cleanup_shmem_consumer (void)
{
    /* We need to unbind explicitly from the heap in order to
       properly release the underlying memory mapping. Exiting the
       process unbinds all mappings automatically. */
    rt_heap_unbind(&datarecorder_heap);
}


#endif
