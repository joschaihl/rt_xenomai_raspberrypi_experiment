/*
 * IndexOutOfRangeException.h
 *
 *  Created on: 07.02.2013
 *      Author: ihl
 */

#ifndef DATARECORDER_EXCEPTIONS_H_
#define DATARECORDER_EXCEPTIONS_H_

#include <exception>
using namespace std;

class IndexOutOfRangeException : public exception
{
  virtual const char* what() const throw()
  {
    return "RingBuffer Index out of Range";
  }
};

class RingBufferPageOutOfRange : public exception
{
	virtual const char* what() const throw()
	{
	   return "RingBuffer Page out of Range";
	}
};

class SharedMemoryNotInitialized : public exception
{
	virtual const char* what() const throw()
	{
		return "Not connected to shared Memory from Datarecorder";
	}
};

class SharedMemoryBindError: public exception
{
	virtual const char* what() const throw()
	{
		return "Can't bind to shared memory";
	}
};

#endif /* INDEXOUTOFRANGEEXCEPTION_H_ */
