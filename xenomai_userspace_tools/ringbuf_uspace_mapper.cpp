#include "xen_ringbuf_model.h"
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
//#include <sys/mmap.h>
#include <pthread.h>
#include <mqueue.h>

int main(int argc, char **argv)
{
    int err;
    int fd = -1;
    unsigned long long i;


   /* puts("Reading shared memory from Kernel Module...");
    fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0777);
    if (fd == -1)
		{
        xnprintf("shm_open: %d\n", errno);
        return -errno;
		}

    read(fd, &buffer, BUF_SIZE); */
    
    /*consumer_shm = mmap(NULL, sizeof(consumer_shm), PROT_READ, MAP_SHARED, fd, 0);
    if (consumer_shm == MAP_FAILED)
	{
        xnprintf("mmap(consumer_shm): %d\n", errno);
        return -1;
	}*/
	return 0;
}

