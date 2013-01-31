#ifndef __RINGBUFFER_MODEL_H_
#define __RINGBUFFER_MODEL_H_

#define SHM_NAME "RecorderRingbufferHeap"

typedef struct {
    uint8_t sensor_id;
    uint64_t sample_time;
    uint8_t sensor_value;
} SensorDataValueModel;

#endif
