#ifndef __CONTROL_PIPE_H__
#define __CONTROL_PIPE_H__

/* Task Control */
#include <native/task.h>

/* Command Pipe */
#include <native/pipe.h>
#include <fcntl.h>
#include <linux/string.h>
RT_PIPE pipe_desc;
RT_TASK def_recorder_control_task;

void parse_recorder_control_cmd(char *control_string, unsigned int string_length);
void recorder_control_task(void *cookie);

#endif
