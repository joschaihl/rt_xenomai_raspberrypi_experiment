#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <pthread.h>
#include <mqueue.h>

#define xnarch_printf printf

static void *consumer_shm = MAP_FAILED;

#define SHM_NAME "RecorderRingbufferHeap"

#define BUF_SIZE 1024

int main(int argc, char **argv)
{
    int fd = -1;

    char buffer[BUF_SIZE];
    puts("Reading shared memory from Kernel Module...");
    fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0777);
    if (fd == -1)
	{
        xnprintf("shm_open: %d\n", errno);
        return -errno;
	}

    read(fd, &buffer, BUF_SIZE);
    
    printf("buffer[%d]=%d\n", 0, buffer[0]);
    
    /*consumer_shm = mmap(NULL, sizeof(consumer_shm), PROT_READ, MAP_SHARED, fd, 0);
    if (consumer_shm == MAP_FAILED)
	{
        xnprintf("mmap(consumer_shm): %d\n", errno);
        return -1;
	}*/
	return 0;
}

