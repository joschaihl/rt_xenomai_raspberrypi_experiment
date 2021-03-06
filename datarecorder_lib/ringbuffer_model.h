#ifndef __RINGBUFFER_MODEL_H_
#define __RINGBUFFER_MODEL_H_
#include "configuration.h"

#define KB 1024
#define MB (KB * KB)

#define SHM_NAME "RecorderRingbufferHeap"
#define SHM_MUTEX_NAME "RingbufferMutex"
#define SHM_SIZE sizeof(RingBuffer)

#ifdef USE_MUTEX
#include <native/mutex.h>
#endif

typedef struct __attribute__((packed)){
/*#ifdef USE_MUTEX
	RT_MUTEX dataset_mutex;
#endif*/
	unsigned char sensorID;
	unsigned long long sampleTimeCode;
	unsigned char sensorValue;
} SensorData;

typedef struct {
/* #ifdef USE_MUTEX
	RT_MUTEX index_mutex;
#endif */
  /* The maximum index (should never be bigger than MAX_RINGBUFFER_SAMPLES) of the ringbuffer */
	unsigned long long size;
	/* The current index */
	unsigned long long index;
	/* Will be incremented after reaching the end of the ringbuffer */
	unsigned long long overflows;
	SensorData sensorData[MAX_RINGBUFFER_SAMPLES];
} RingBuffer;

#endif
