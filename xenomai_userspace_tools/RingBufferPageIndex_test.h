#ifndef RINGBUFFERINDEX_TEST_H_
#define RINGBUFFERINDEX_TEST_H_

#include <cxxtest/TestSuite.h>

#include "RingBufferPageIndex.h"
#include <sys/mman.h>

class RingBufferPageIndex_test : public CxxTest::TestSuite
{
public:

	unsigned long long int originalsize;
	RingBufferPageIndex_test() : originalsize(0)
	{

	}
	void setUp()
	{
		/**
		 * Disable swapping and paging for Real-Time
		 */
		mlockall(MCL_CURRENT|MCL_FUTURE);
		RingBufferConsumer rb;
		rb.init();
		originalsize = rb.getSize();
	}

	void tearDown()
	{
		RingBufferConsumer rb;
		rb.init();
		rb.setSize(originalsize);
	}


	void testCountPages()
	{
		RingBufferConsumer rb;
		rb.init();
		TS_ASSERT(rb.setSize(10));
		RingBufferPageIndex rbi(rb);

		TS_ASSERT_THROWS_NOTHING(rbi.setPage(2,3));
		TS_ASSERT_EQUALS(rbi.getCountPages(), 4);

		TS_ASSERT_THROWS_NOTHING(rbi.setPage(1,5));
		TS_ASSERT_EQUALS(rbi.getCountPages(), 2);
	}

	void testRingBufferIndex()
	{
		RingBufferConsumer rb;
		rb.init();
		RingBufferPageIndex rbi(rb);
		TS_ASSERT(rb.setSize(1000));
		TS_ASSERT_EQUALS(rbi.getIndex(), 0);
		TS_ASSERT_EQUALS(rbi.getPage(), 0);
		TS_ASSERT_EQUALS(rbi.getPageWidth(), RingBufferPageIndex::DEFAULT_PAGE_WIDTH);
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getIndex(), 1);
	}

	void testGetPageWidth()
	{
		RingBufferConsumer rb;
		rb.init();
		RingBufferPageIndex rbi(rb);
		TS_ASSERT(rb.setSize(1000));
		rbi.setPage(0, 666);
		TS_ASSERT_EQUALS(rbi.getPageWidth(), 666);
	}

	void testGetPage()
	{
		RingBufferConsumer rb;
		rb.init();
		RingBufferPageIndex rbi(rb);
		TS_ASSERT(rb.setSize(1000));
		rbi.setPage(777, 1);
		TS_ASSERT_EQUALS(rbi.getPage(), 777);
	}

	void testIncrementIndex()
	{
		RingBufferConsumer rb;
		rb.init();
		TS_ASSERT(rb.setSize(10));
		RingBufferPageIndex rbi(rb);
		TS_ASSERT_THROWS_NOTHING(rbi.setPage(0,3));
		TS_ASSERT_EQUALS(rbi.getPossibleIncrementations(),2);
		TS_ASSERT_EQUALS(rbi.getIndex(), 0);
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getPossibleIncrementations(),1);
		TS_ASSERT_EQUALS(rbi.getIndex(), 1);
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getPossibleIncrementations(),0);
		TS_ASSERT_EQUALS(rbi.getIndex(), 2);
		TS_ASSERT(!rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getIndex(), 2);
		TS_ASSERT_THROWS_NOTHING(rbi.setPage(3,3));
		TS_ASSERT_EQUALS(rbi.getIndex(), 9);
		TS_ASSERT(!rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getPageWidth(), 3);
		TS_ASSERT_EQUALS(rbi.getPage(), 3);
	}

	void testSetPage()
	{
		RingBufferConsumer rb;
		rb.init();
		TS_ASSERT(rb.setSize(1000));
		RingBufferPageIndex rbi(rb);
		TS_ASSERT_THROWS_NOTHING(rbi.setPage(0, 100));
		TS_ASSERT_EQUALS(rbi.getIndex(), 0);

		TS_ASSERT_THROWS(rbi.setPage(666, 0), RingBufferPageOutOfRange);
		TS_ASSERT_EQUALS(rbi.getPage(), 0);

		TS_ASSERT_THROWS(rbi.setPage(1, 1000), RingBufferPageOutOfRange);
		TS_ASSERT_THROWS_NOTHING(rbi.setPage(5, 100));
		TS_ASSERT_EQUALS(rbi.getIndex(), 500);
		TS_ASSERT_THROWS_NOTHING(rbi.setPage(10, 66));
		TS_ASSERT(rbi.getIndex()==660);
		TS_ASSERT_THROWS_NOTHING(rbi.setPage(0, 1000));
		TS_ASSERT_THROWS_NOTHING(rbi.setPage(0, 10000));
		TS_ASSERT_EQUALS(rbi.getPageWidth(), 1000);
	}
};

#endif /*RINGBUFFERINDEX_TEST_H_*/
