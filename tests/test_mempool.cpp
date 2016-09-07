#include <gtest/gtest.h>
#include "utils/mem_pool.h"


class MockObject
{
  public:
  long long field1;
  long long field2;
  unsigned short field3;
  public:
  MockObject(long long ll1, long long ll2, unsigned short us3):
    field1(ll1), field2(ll2), field3(us3)
  {}
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

TEST(MEMPOOL_TEST, new_and_delete)
{
  ustr::MemoryPool<MockObject, 1024> mp;
  auto p1 = mp.new_element(1,2,3);
  EXPECT_EQ(p1->field1, 1);
  EXPECT_EQ(p1->field2, 2);
  EXPECT_EQ(p1->field3, 3);
  mp.delete_element(p1);
}

int main(int argc, char* argv[])
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


