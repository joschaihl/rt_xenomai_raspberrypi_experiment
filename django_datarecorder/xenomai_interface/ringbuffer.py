import subprocess, os, json

class RingBuffer(object):
	CMD_READ_SHM = "/sbin/read_shm"
	
	# 10 Seconds with 9 MB Data
	def get_plain(self):
		ausgabe = os.popen(self.CMD_READ_SHM)
		samples = [zeile.strip() for zeile in ausgabe]
		return samples
	
	# 9 Seconds with 9 MB Data
	def get_raw(self):
		output = subprocess.check_output(["/sbin/read_shm"])
		return output
	
	# Crash with 9 MB Data - eval is also insecure and evil
	def get(self):
		output = subprocess.check_output(["/sbin/read_shm"])
		samples = eval(output)		
		return samples
			
	# 40 Seconds with 9 MB Data
	def get_json(self):
		output = subprocess.check_output(["/sbin/read_shm_json", "-p", "0", "-m", "1000"])
		samples = json.loads(output)		
		return samples
	
	def get_html_table(self):
		output = subprocess.check_output(["/sbin/read_shm"])
		return output
	

def test_json():
	r = RingBuffer()
	a = r.get_json()
	return a

def test_raw():
	r = RingBuffer()
	a = r.get_raw()
	return a
