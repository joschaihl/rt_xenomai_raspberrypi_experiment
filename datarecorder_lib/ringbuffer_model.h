#ifndef __RINGBUFFER_MODEL_H_
#define __RINGBUFFER_MODEL_H_

#define KB 1024
#define MB (KB * KB)

#define SHM_NAME "RecorderRingbufferHeap"
//#define MAX_RINGBUFFER_SAMPLES 4000000
#define MAX_RINGBUFFER_SAMPLES 5000000
#define SHM_SIZE sizeof(RingBuffer)

typedef struct __attribute__((packed)){
    unsigned char sensorID;
    unsigned long long sampleTimeCode;
    unsigned char sensorValue;
} SensorData;

typedef struct {
  /* The maximum index (should never be bigger than MAX_RINGBUFFER_SAMPLES) of the ringbuffer */
	unsigned long long size;
	/* The current index */
	unsigned long long index;
	/* Will be incremented after reaching the end of the ringbuffer */
	unsigned long long overflows;
	SensorData sensorData[MAX_RINGBUFFER_SAMPLES];
} RingBuffer;

#endif
