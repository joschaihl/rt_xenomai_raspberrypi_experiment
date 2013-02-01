#ifndef __RINGBUFFER_MODEL_H_
#define __RINGBUFFER_MODEL_H_

#define MB 1024 * 1024

#define SHM_NAME "RecorderRingbufferHeap"
#define SHM_SIZE (64 * MB)

typedef struct {
    uint8_t sensor_id;
    uint64_t sample_time;
    uint8_t sensor_value;
} SensorDataValueModel;

#endif
