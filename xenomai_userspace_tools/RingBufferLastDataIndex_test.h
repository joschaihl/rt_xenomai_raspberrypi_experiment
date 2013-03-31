#ifndef RINGBUFFERLASTDATAINDEX_TEST_H_
#define RINGBUFFERLASTDATAINDEX_TEST_H_

#include <cxxtest/TestSuite.h>

#include "RingBufferLastDataIndex.h"
#include "RingBufferConsumer.h"
#include <sys/mman.h>

class RingBufferLastDataIndex_test : public CxxTest::TestSuite
{
public:
	RingBufferConsumer rb;
	unsigned long long int originalsize;
	RingBufferLastDataIndex_test() : originalsize(0)
	{

	}
	void setUp()
	{
		/**
		 * Disable swapping and paging for Real-Time
		 */
		mlockall(MCL_CURRENT|MCL_FUTURE);
		rb.init();
		originalsize = rb.getSize();
	}

	void tearDown()
	{
		rb.setSize(originalsize);
	}

	void testDefaultConstructor1()
	{
		// Test for Index without setLastData()
		TS_ASSERT(rb.setSize(RingBufferLastDataIndex::DEFAULT_DATASETS));
		TS_ASSERT_EQUALS(rb.getSize(), RingBufferLastDataIndex::DEFAULT_DATASETS);
		RingBufferLastDataIndex rbl(rb);
		// rb size = 100
		// end_index = 0
		// datasets = 100
		// 100 - abs(100-(0+1)) = 100-99 = 1
		TS_ASSERT_EQUALS(rbl.getIndex(), 1);
		TS_ASSERT(rbl.incrementIndex());
		TS_ASSERT_EQUALS(rbl.getIndex(), 2);
		TS_ASSERT_EQUALS(rbl.getDataSets(), RingBufferLastDataIndex::DEFAULT_DATASETS)
	}

	void testDefaultConstructor2()
	{
		TS_ASSERT(rb.setSize(RingBufferLastDataIndex::DEFAULT_DATASETS-1));
		RingBufferLastDataIndex rbl(rb);
		// Test for Index without setLastData()

		TS_ASSERT_EQUALS(rbl.getDataSets(), RingBufferLastDataIndex::DEFAULT_DATASETS-1)
		// rb size = 99
		// end_index = 0
		// datasets = 99
		// 99 - abs(99-(0+1) = 99-98=1
		TS_ASSERT_EQUALS(rbl.getIndex(), 1);
		TS_ASSERT(rbl.incrementIndex());
		TS_ASSERT_EQUALS(rbl.getIndex(), 2);

	}
	void testRingBufferLastDataIndex()
	{
		TS_ASSERT(rb.setSize(1000));
		RingBufferLastDataIndex rbl(rb);
		// Last 50 Datasets
		// rbsize = 100
		// Index = 100
		// 100 - abs(50-(100+1)) = 100 - 51 = 49
		TS_ASSERT_THROWS_NOTHING(rbl.setLastData(100, 50));
		TS_ASSERT_EQUALS(rbl.getIndex(), 51);
		TS_ASSERT(rbl.incrementIndex());
		TS_ASSERT_EQUALS(rbl.getIndex(), 52);

	}

	void testRingBufferGetIndex()
	{

		RingBufferLastDataIndex rbi(rb);
		TS_ASSERT(rb.setSize(10));
		 /* Example 1:
		 * ringbuffer size = 10
		 * index = 9
		 * datasets = 5
		 * Last Data Index Range: 5, 6, 7, 8, 9
		 */
		TS_ASSERT_THROWS_NOTHING(rbi.setLastData(9, 5));
		TS_ASSERT_EQUALS(rbi.getIndex(),5);

		 /* Example 2:
		 * ringbuffer size = 10
		 * index = 3
		 * datasets = 5
		 * Last Data Index Range:  9,0,1,2,3
		 */
		TS_ASSERT_THROWS_NOTHING(rbi.setLastData(3, 5));
		TS_ASSERT_EQUALS(rbi.getPossibleIncrementations(), 4);
		TS_ASSERT_EQUALS(rbi.getIndex(),9); // 1
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getPossibleIncrementations(), 3);
		TS_ASSERT_EQUALS(rbi.getIndex(),0); // 2
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getPossibleIncrementations(), 2);
		TS_ASSERT_EQUALS(rbi.getIndex(),1); // 3
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getPossibleIncrementations(), 1);
		TS_ASSERT_EQUALS(rbi.getIndex(),2); // 4
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getPossibleIncrementations(), 0);
		TS_ASSERT_EQUALS(rbi.getIndex(),3); // 5 and end
		TS_ASSERT(!rbi.incrementIndex());

		TS_ASSERT(rb.setSize(3));
		 /* Example 3:
		 * ringbuffer size = 3 (0, 1, 2)
		 * index = 0
		 * datasets = 2
		 * Last Data Index Range: 2, 0
		 */
		TS_ASSERT_THROWS_NOTHING(rbi.setLastData(0, 2));
		TS_ASSERT_EQUALS(rbi.getPossibleIncrementations(), 1);
		TS_ASSERT_EQUALS(rbi.getIndex(),2); // 1
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getIndex(),0); // 2 and end
		TS_ASSERT(!rbi.incrementIndex());
		TS_ASSERT(rb.setSize(10));
		 /* Example 4:
		 * ringbuffer size = 10
		 * index = 5
		 * datasets = 10
		 * Last Data Index Range: 6,7,8,9,0,1,2,3,4,5
		 */
		TS_ASSERT_THROWS_NOTHING(rbi.setLastData(5, 10));
		TS_ASSERT_EQUALS(rbi.getIndex(),6); // 1
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getIndex(),7); // 2
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getIndex(),8); // 3
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getIndex(),9); // 4
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getIndex(),0); // 5
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getIndex(),1); // 6
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getIndex(),2); // 7
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getIndex(),3); // 8
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getIndex(),4); // 9
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getIndex(),5); // 10 and end
		TS_ASSERT(!rbi.incrementIndex());

		// rb size = 10
		// end_index = 0
		// datasets = 10
		// 1,2,3,4,5 ,6,7,8,9,0
		TS_ASSERT_THROWS_NOTHING(rbi.setLastData(0, 10));
		TS_ASSERT_EQUALS(rbi.getIndex(),1); // 1
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getIndex(),2); // 2
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getIndex(),3); // 3
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getIndex(),4); // 4
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getIndex(),5); // 5
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getIndex(),6); // 6
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getIndex(),7); // 7
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getIndex(),8); // 8
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getIndex(),9); // 9
		TS_ASSERT(rbi.incrementIndex());
		TS_ASSERT_EQUALS(rbi.getIndex(),0); // 10 and end
		TS_ASSERT(!rbi.incrementIndex());
	}
	void testExceptions()
	{
		TS_ASSERT(rb.setSize(10000));
		RingBufferLastDataIndex rbi(rb);
		TS_ASSERT_THROWS(rbi.setLastData(50,0), IndexOutOfRangeException);
		TS_ASSERT_THROWS(rbi.setLastData(10000,100), IndexOutOfRangeException);
	}
	void testSmallPageWidthCase()
	{
		TS_ASSERT(rb.setSize(10000));
		RingBufferLastDataIndex rbi(rb);
		TS_ASSERT_THROWS_NOTHING(rbi.setLastData(100,1));
		TS_ASSERT_EQUALS(rbi.getDataSets(), 1);
		TS_ASSERT_EQUALS(rbi.getIndex(), 100);
		TS_ASSERT_EQUALS(rbi.incrementIndex(), false);


	}

};

#endif /*RINGBUFFERLASTDATAINDEX_TEST_H_*/
