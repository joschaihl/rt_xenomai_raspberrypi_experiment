import subprocess

class RingBuffer(object):
	CMD_READ_SHM = "/sbin/read_shm"
	def get(self):
		output = subprocess.check_output(["/sbin/read_shm"])
		samples = eval(output)
		#ausgabe = os.popen(self.CMD_READ_SHM)
		#samples = [zeile.strip() for zeile in ausgabe]
		#for zeile in ausgabe
		
		return samples		
