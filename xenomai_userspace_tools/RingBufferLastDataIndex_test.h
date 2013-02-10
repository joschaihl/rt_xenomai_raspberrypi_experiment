#ifndef RINGBUFFERLASTDATAINDEX_TEST_H_
#define RINGBUFFERLASTDATAINDEX_TEST_H_

#include <cxxtest/TestSuite.h>

#include "RingBufferLastDataIndex.h"
#include "RingBufferConsumer.h"

class RingBufferLastDataIndex_test : public CxxTest::TestSuite
{
public:
	RingBufferConsumer rb;
	void setUp()
	{
		rb.init();
	}

	void tearDown()
	{

	}

	void testRingBufferLastDataIndex()
	{
		RingBufferLastDataIndex rbl(rb);
		// Default Constructor:
		// Last 100 Datasets
		// until Index from RingBufferConsumer
	/*	TS_ASSERT_THROWS_NOTHING(rbl.setLastData());
		TS_ASSERT_THROWS_NOTHING(rbl.setLastData());

		// Last 50 Datasets
		// Index = 100
		TS_ASSERT_THROWS_NOTHING(rbl.setLastData(100, 50));
		TS_ASSERT_THROWS_EQUALS(rbl.getIndex(), 50);
		TS_ASSERT(rbl.incrementIndex());
		TS_ASSERT_THROWS_EQUALS(rbl.getIndex(), 51);*/

	}

};

#endif /*RINGBUFFERLASTDATAINDEX_TEST_H_*/
