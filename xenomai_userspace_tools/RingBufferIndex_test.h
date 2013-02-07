#ifndef RINGBUFFERINDEX_TEST_H_
#define RINGBUFFERINDEX_TEST_H_

#include <cxxtest/TestSuite.h>

#include "RingBufferIndex.h"

class RingBufferIndex_test : public CxxTest::TestSuite
{
public:
	void setUp()
	{
		// TODO: Implement setUp() function.
	}

	void tearDown()
	{
		// TODO: Implement tearDown() function.
	}

	void testRingBufferIndex()
	{
		RingBufferConsumer rb;
		rb.init();
		RingBufferIndex rbi(rb);

		TS_ASSERT_EQUALS(rbi.getIndex(), 0);
		TS_ASSERT_EQUALS(rbi.getPage(), 0);
		TS_ASSERT_EQUALS(rbi.getPageWidth(), RingBufferIndex::DEFAULT_PAGE_WIDTH);
	}

	void testGetPageWidth()
	{
		RingBufferConsumer rb;
		rb.init();
		RingBufferIndex rbi(rb, 0, 666);
		TS_ASSERT_EQUALS(rbi.getPageWidth(), 666);
	}

	void testGetPage()
	{
		RingBufferConsumer rb;
		rb.init();
		RingBufferIndex rbi(rb, 777, 1);
		TS_ASSERT_EQUALS(rbi.getPage(), 777);
	}

	void testGetIndex()
	{
		RingBufferConsumer rb;
		rb.init();
		RingBufferIndex rbi(rb, 1, rb.getSize()/10);
		//TS_ASSERT_EQUALS(rbi.getIndex(), 777);

	}

	void testIncrementIndex()
	{
		// TODO: Implement testIncrementIndex() function.
	}

};

#endif /*RINGBUFFERINDEX_TEST_H_*/
