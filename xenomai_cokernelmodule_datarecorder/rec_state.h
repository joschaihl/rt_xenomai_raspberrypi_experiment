#ifndef __REC_STATE_H__
#define __REC_STATE_H__

typedef enum e_SystemState {
    rec_state_pause = 0,
    rec_state_running = 1,
    rec_state_error = 2,
    rec_state_init = 3,
} SystemState;

void set_speed(int timer_overflow_in_nanoseconds);
int get_speed(void);
void set_recorder_state(SystemState systemState);
SystemState get_recorder_state(void);

#endif
