#include "print.h"
#include "rec_ringbuffer.h"

#include <linux/init.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/kernel.h>

/* Shared Memory */
#include <native/heap.h>

/* Datastructes for shared memory */
unsigned int ringbuffer_size_mb = 16;
unsigned int ringbuffer_index = 0;
unsigned int ringbuffer_max_index = 1000;

RT_HEAP datarecorder_heap;
void *shmem;

void insertSampleToRingBuffer(SensorDataValueModel sample)
{
    SensorDataValueModel *ringbuffer;
    ringbuffer = shmem;
    ringbuffer[ringbuffer_index] = sample;
    ringbuffer_index++;
    if(ringbuffer_index > ringbuffer_max_index)
    {
    	ringbuffer_index = 0;
    }
}

static int rec_ringuffer_init(void)
{
		int err;
		/* Create the heap in kernel space */
    rtdm_printk(KERN_INFO DPRINT_PREFIX
                "Reserving %d MB for shared memory RecorderRingbufferHeap... ",
                ringbuffer_size_mb);
    err = rt_heap_create(&datarecorder_heap, SHM_NAME,
                         ringbuffer_size_mb * 1024 * 1024,H_SHARED);
  
    switch(err) {
        case 0:
            break;
        case EEXIST:
            DPRINT("rt_heap_create(): Shared Memory name is already in use by some registered object :(");
            return -1;
        case EINVAL:
            DPRINT("rt_heap_create(): Heapsize is null, greater than the system limit, or name is null or empty for a mappable heap. :(");
            return -1;
        case ENOMEM:
            DPRINT("rt_heap_create(): Not enough system memory is available to create or register the heap. :(");
            return -1;
        case EPERM:
            DPRINT("rt_heap_create(): This service was called from an invalid context. :(");
            return -1;
        case ENOSYS:
            DPRINT("Mode specifies H_MAPPABLE, but the real-time support in user-space is unavailable. :(");
            return -1;
        default:
            rtdm_printk(KERN_INFO DPRINT_PREFIX "Can't create heap :( - rt_heap_create returned value %d\n", err);
            return -1;
    }

		/* Get the shared memory address */
    DPRINT("Get a shared memory address...");
		err = rt_heap_alloc(&datarecorder_heap,0,TM_NONBLOCK,&shmem);
    switch(err) {
        case 0:
            break;
        default:
            rtdm_printk(KERN_INFO DPRINT_PREFIX "rt_heap_alloc returned %d\n", err);
            return -1;
    }
    return 0;
}

static void rec_ringbuffer_exit(void)
{
    DPRINT("Removing Shared Memory");
		rt_heap_delete(&datarecorder_heap);
}

module_init(rec_ringuffer_init);
module_exit(rec_ringbuffer_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Joscha Ihl <joscha@grundfarm.de>");
MODULE_DESCRIPTION("Real Time Data Recorder Linux Co-Kernel Module");
EXPORT_SYMBOL(insertSampleToRingBuffer);

