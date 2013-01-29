#!/usr/bin/python
# Communicate with the Kernel-Module via Pipe /dev/rtp7

import os

pipe_name = "/dev/rtp7"

pipeout = os.open(pipe_name, os.O_WRONLY)
os.write(pipeout, "hello");




