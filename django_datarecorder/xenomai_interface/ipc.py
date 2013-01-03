'''
Created on 29.11.2012

@author: ihl
'''
import string

class SharedMemory:
    fileName = string("")

class NamedSemaphore:
    fileName = string("")
    
class Mutex(NamedSemaphore):
    fileName = string("")