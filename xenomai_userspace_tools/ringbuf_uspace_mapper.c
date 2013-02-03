#include "xen_ringbuf_model.h"
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <pthread.h>
#include <mqueue.h>

int main(int argc, char **argv)
{
    int err;
    int fd = -1;
    unsigned long long i;
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

    puts("Reading shared memory from Kernel Module...");
    fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0777);
    if (fd == -1)
		{
        xnprintf("shm_open: %d\n", errno);
        return -errno;
		}

    read(fd, &buffer, BUF_SIZE);
    
    /*consumer_shm = mmap(NULL, sizeof(consumer_shm), PROT_READ, MAP_SHARED, fd, 0);
    if (consumer_shm == MAP_FAILED)
	{
        xnprintf("mmap(consumer_shm): %d\n", errno);
        return -1;
	}*/
	return 0;
}

