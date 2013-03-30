/*
 * RingBufferImager.cpp
 *
 *  Created on: 30.03.2013
 *      Author: ihl
 */


#include "xen_ringbuf_controller.h"
#include <iostream>
#include <sys/mman.h>

using namespace std;

RBUF_MEMBERS;
RingBuffer rbufCopy;

int main(int argc, char **argv)
{
	FILE *imageFile;
	int err;
	mlockall(MCL_CURRENT|MCL_FUTURE);
	/* Bind to a shared heap which has been created elsewhere, either
       in kernel or user-space. Here we cannot wait and the heap must
       be available at once, since the caller is not a Xenomai-enabled
       thread. The heap should have been created with the H_SHARED
       mode set. */
	err = rt_heap_bind(&datarecorder_heap,SHM_NAME,TM_NONBLOCK);

	if (err)
	{
		return -1;
	}
	/* Get the address of the shared memory segment. The "size" and
       "timeout" arguments are unused here. */

	err = rt_heap_alloc(&datarecorder_heap,0,TM_NONBLOCK,&shmem);

	if (err)
	{
		return -1;
	}

	ringBuffer = (RingBuffer *) shmem;

#ifdef USE_MUTEX
	/*
	 * Bind to existing mutex and don't wait if it not exist
	 */
	err = rt_mutex_bind(&ringbuffer_mutex, SHM_MUTEX_NAME, TM_NONBLOCK);
#endif

	// Make the copy
	//CRITICAL_RINGBUFFER_ACCESS();
	cout << "Make copy of Ringbuffer" << endl;

	rt_mutex_acquire(&ringbuffer_mutex, TM_INFINITE);
	memcpy(&rbufCopy, shmem, sizeof(RingBuffer));
	rt_mutex_release(&ringbuffer_mutex);

	imageFile = fopen("ringbufferimage.img", "wb");
	fwrite(&rbufCopy, 1, sizeof(rbufCopy), imageFile);
	fclose(imageFile);

	/* cout << "Copy ready" << endl;
	cout << "Index = " << rbufCopy.index << endl;
	cout << "Size = " << rbufCopy.size << endl;
	cout << "Overflows = " << rbufCopy.overflows << endl;
	for(unsigned long long i = 0; i < rbufCopy.size; i++)
	{
		printf("%llu %u %llu %u\n", i,rbufCopy.sensorData[i].sensorID, rbufCopy.sensorData[i].sampleTimeCode,
				rbufCopy.sensorData[i].sensorValue);
	}*/


#ifdef USE_MUTEX
	/*
	 * Bind to existing mutex and don't wait if it not exist
	 */
		rt_mutex_unbind(&ringbuffer_mutex);
#endif

	rt_heap_unbind(&datarecorder_heap);

	return 0;
}
