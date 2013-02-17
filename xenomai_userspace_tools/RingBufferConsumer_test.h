#ifndef RINGBUFFERCONSUMER_TEST_H_
#define RINGBUFFERCONSUMER_TEST_H_

#include <cxxtest/TestSuite.h>

#include "RingBufferConsumer.h"

class RingBufferConsumer_test : public CxxTest::TestSuite
{
public:
	RingBufferConsumer rbuf;
	void setUp()
	{
	//	TS_ASSERT_THROWS_ANYTHING(throw SharedMemoryNotInitialized());
	}

	void tearDown()
	{
		// TODO: Implement tearDown() function.
	}

	void testRingBufferConsumer()
	{

		TS_ASSERT_THROWS_ANYTHING(rbuf.getSensorValue(0));
		TS_ASSERT_THROWS(rbuf.getSensorValue(0), SharedMemoryNotInitialized);
		TS_ASSERT_THROWS(rbuf.getSensorID(0), SharedMemoryNotInitialized);
		TS_ASSERT_THROWS(rbuf.getSampleTimeCode(0), SharedMemoryNotInitialized);
		TS_ASSERT_THROWS(rbuf.setSize(rbuf.getSize()-1), SharedMemoryNotInitialized);
		TS_ASSERT_THROWS(rbuf.getCurrentIndex(), SharedMemoryNotInitialized);
		TS_ASSERT(rbuf.init());
		unsigned long long size = rbuf.getSize();
		TS_ASSERT_THROWS(rbuf.getSensorValue(size), IndexOutOfRangeException);
		TS_ASSERT_THROWS(rbuf.getSampleTimeCode(size), IndexOutOfRangeException);
		TS_ASSERT_THROWS(rbuf.getSensorID(size), IndexOutOfRangeException);
		TS_ASSERT_THROWS_NOTHING(rbuf.getSensorValue(size-1));
		TS_ASSERT_THROWS_NOTHING(rbuf.getSampleTimeCode(size-1));
		TS_ASSERT_THROWS_NOTHING(rbuf.getSensorID(size-1));
		TS_ASSERT_THROWS_NOTHING(rbuf.getCurrentIndex());
	}

	void testRingBufferSetSize()
	{
		unsigned long long originalsize = rbuf.getSize();
		TS_ASSERT(rbuf.setSize(100));

		// Set the size to maximum value
		TS_ASSERT(rbuf.setSize(MAX_RINGBUFFER_SAMPLES));
		TS_ASSERT_EQUALS(rbuf.getSize(), MAX_RINGBUFFER_SAMPLES);

		TS_ASSERT(!rbuf.setSize(MAX_RINGBUFFER_SAMPLES+1));

		// Back to original value
		TS_ASSERT(rbuf.setSize(originalsize));
		//TS_ASSERT_EQUALS(rbuf.getSize(), originalsize);
		// Check that
		TS_ASSERT_EQUALS(rbuf.getSize(), originalsize);

	}

};

#endif /*RINGBUFFERCONSUMER_TEST_H_*/
