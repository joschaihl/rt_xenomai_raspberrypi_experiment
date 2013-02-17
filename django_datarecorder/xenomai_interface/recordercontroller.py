'''
Created on 29.11.2012

@author: ihl
'''

import os

class RecorderController(object):
	RECORDER_CONTROL_PIPE_NAME = "/dev/rtp7"
	
	'''
	classdocs
	'''
	def __init__(self):
			'''
			Constructor
			'''
			
	def __open(self):
		self.recorder_control_pipe = os.open(self.RECORDER_CONTROL_PIPE_NAME, os.O_WRONLY)
		
	def __close(self):
		os.close(self.recorder_control_pipe)
					
	def pause_record(self):
		self.__open()
		os.write(self.recorder_control_pipe, "pause")
		self.__close()
		
	def start_record(self):
		self.__open()
		os.write(self.recorder_control_pipe, "start")
		self.__close()
	
	def speed(self, speed):
		self.__open()
		speedstr = "speed=%d" % (speed)
		os.write(self.recorder_control_pipe, speedstr)
		self.__close()	
		
		