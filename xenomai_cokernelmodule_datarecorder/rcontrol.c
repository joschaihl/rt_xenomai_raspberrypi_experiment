/*
 * rcontrol.c
 *
 *  Created on: 11.01.2013
 *      Author: ihl
 */

#define PIPE_MINOR 7

#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <native/pipe.h>

int pipe_fd;

int main(int argc, char **argv)
{
	char devname[32], buf[16];
	/* ... */
	sprintf(devname, "/dev/rtp%s", PIPE_MINOR);
	pipe_fd = open(devname, O_RDWR);
	if (pipe_fd < 0)
	        return -1;
	/* Wait for the prompt string "Hello"... */
	read(pipe_fd, buf, sizeof(buf));

	close(pipe_fd);

	return 0;
}
