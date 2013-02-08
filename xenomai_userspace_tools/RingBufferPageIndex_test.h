#ifndef RINGBUFFERINDEX_TEST_H_
#define RINGBUFFERINDEX_TEST_H_

#include <cxxtest/TestSuite.h>

#include "RingBufferPageIndex.h"

class RingBufferPageIndex_test : public CxxTest::TestSuite
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
		RingBufferPageIndex rbi(rb);

		TS_ASSERT_EQUALS(rbi.getIndex(), 0);
		TS_ASSERT_EQUALS(rbi.getPage(), 0);
		TS_ASSERT_EQUALS(rbi.getPageWidth(), RingBufferPageIndex::DEFAULT_PAGE_WIDTH);
	}

	void testGetPageWidth()
	{
		RingBufferConsumer rb;
		rb.init();
		RingBufferPageIndex rbi(rb);
		rbi.setPage(0, 666);
		TS_ASSERT_EQUALS(rbi.getPageWidth(), 666);
	}

	void testGetPage()
	{
		RingBufferConsumer rb;
		rb.init();
		RingBufferPageIndex rbi(rb);
		rbi.setPage(777, 1);
		TS_ASSERT_EQUALS(rbi.getPage(), 777);
	}

	void testGetIndex()
	{
		RingBufferConsumer rb;
		rb.init();
		unsigned long long int originalSize = rb.getSize();
		TS_ASSERT(rb.setSize(1000));
		RingBufferPageIndex rbi(rb);
		rbi.setPage(0, 100);
		TS_ASSERT_EQUALS(rbi.getIndex(), 0);

		rbi.setPage(5, 100);

		TS_ASSERT_EQUALS(rbi.getIndex(), 500);

		TS_ASSERT(rb.setSize(originalSize));

	}

	void testIncrementIndex()
	{
		// TODO: Implement testIncrementIndex() function.
	}

};

#endif /*RINGBUFFERINDEX_TEST_H_*/
