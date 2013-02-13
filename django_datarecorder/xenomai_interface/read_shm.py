import subprocess, os, json

class ReadRingBuffer(object):
	# Constructor
	def __init__(self):
		self.__parameterString = ""
		
	# Check types to avoid execution of shell scripts via appending some bad strings
	def setLastDataMode(self, max_length=100):
		if (type(max_length) is int) or (type(max_length) is long):
			if max_length>=0 :
				self.__parameterString1 = "-l "
				self.__parameterString2 = "-m %d" % (max_length)
	
	def setPageMode(self, page_number, max_length=100):
		if ((type(page_number) is int) or (type(page_number) is long)) and \
		 ((type(max_length) is int) or (type(max_length) is long)):
			self.__parameterString1 = "-p %d" % (page_number)
			self.__parameterString2 = "-m %d" % (max_length)
		
	# 10 Seconds with 9 MB Data
	#def get_plain(self):
	#	ausgabe = os.popen("/sbin/read_shm_html")
	#	samples = [zeile.strip() for zeile in ausgabe]
	#	return samples
	
	# 9 Seconds with 9 MB Data
	def get_raw(self):
		output = subprocess.check_output(["/sbin/read_shm_raw",  self.__parameterString1, self.__parameterString2])
		return output
	
	def get_html_table(self):
		output = subprocess.check_output(["/sbin/read_shm_html", self.__parameterString1, self.__parameterString2])
		return output
	
	def get_json(self):
		output = subprocess.check_output(["/sbin/read_shm_json", self.__parameterString1, self.__parameterString2])
		return output

	# Crash with 9 MB Data - eval is also insecure and evil
	#def get(self):
	#	output = subprocess.check_output(["/sbin/read_shm"])
	#	samples = eval(output)		
	#	return samples
			
	# 40 Seconds with 9 MB Data
	#def get_json(self):
	#	output = subprocess.check_output(["/sbin/read_shm_json", "-p", "0", "-m", "1000"])
	#	samples = json.loads(output)		
	# return samples
	

