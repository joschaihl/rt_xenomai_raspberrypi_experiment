import os

class RingBuffer(object):
	CMD_READ_SHM = "/opt/rtraspberrylinu-code/xenomai_cokernelmodule_datarecorder/read_shm"
	def get(self):
		ausgabe = os.popen(self.CMD_READ_SHM)
		samples = [zeile.strip() for zeile in ausgabe]
		return samples		
