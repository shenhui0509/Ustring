#include <gtest/gtest.h>
#include "mem_pool.h"


class MockObject
{
  long long field1;
  long long field2;
  unsigned short field3;
};

TEST(MEMPOOL_TEST, Allocate)
{
  ustr::MemoryPool<MockObject, 1024> mem_pool;
  auto pMO1 = mem_pool.allocate();
  auto M_arr = mem_pool.allocate(10);
  mem_pool.deallocate(M_arr, 10);
  auto pMo2 = mem_pool.allocate();
  auto pMo3 = mem_pool.allocate();
  EXPECT_EQ(pMo2 + 1, pMo3);
  EXPECT_EQ(pMO1, mem_pool.address(*pMO1));
  mem_pool.deallocate(pMO1);
}

int main(int argc, char* argv[])
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


