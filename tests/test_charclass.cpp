#include <gtest/gtest.h>

#include "char_class.h"

class CCBTest : public testing::Test
{
  protected:
    virtual void SetUp()
    {
    }
    virtual void TearDown()
    {
    }

    ustr::re::CharClassBuilder ccb;
};

TEST_F(CCBTest, Init)
{
  EXPECT_FALSE(ccb.contains('a'));
  auto cc = ccb.GetCharClass();
  EXPECT_FALSE(cc->Contains('a'));
  cc->Delete();
  ccb.AddRange('a','z');
  ccb.AddRange('A','Z');
  EXPECT_TRUE(ccb.contains('a'));
  EXPECT_TRUE(ccb.FoldsASCII());
  EXPECT_FALSE(ccb.contains('1'));
  ccb.RemoveAbove('a');
  EXPECT_FALSE(ccb.contains('m'));
  EXPECT_TRUE(ccb.contains('B'));
  EXPECT_TRUE(ccb.contains('A'));
}

TEST_F(CCBTest, Negate)
{
  ccb.AddRange('a', 'g');
  ccb.AddRange('o', 'z');
  ccb.Negate();
  for(char a = 'a'; a <= 'g'; ++a)
    ASSERT_FALSE(ccb.contains(a));
  for(char a = 'h'; a <= 'n'; ++a)
    ASSERT_TRUE(ccb.contains(a));
  for(char a = 'o'; a <= 'z'; ++a)
    ASSERT_FALSE(ccb.contains(a));

  ccb.Negate();
  ccb.AddRange('0','9');
  for(int i = 0; i <= 9; ++i)
    ASSERT_TRUE(ccb.contains(i + '0'));
  ccb.Negate();
  for(int i = 0; i <= 9; ++i)
    ASSERT_FALSE(ccb.contains(i + '0'));
  ASSERT_TRUE(ccb.contains('0' - 1));
  ASSERT_TRUE(ccb.contains('9' + 1));
}

TEST_F(CCBTest, AddCharClass)
{
  using ustr::re::CharClassBuilder;
  ccb.AddRange('a', 'z');
  CharClassBuilder *ccb2 = new CharClassBuilder;
  ccb2->AddRange('0', '9');

  for(char x = 'a'; x <= 'z'; ++x)
    ASSERT_TRUE(ccb.contains(x));
  for(int i = 0; i <= 9; ++i){
    ASSERT_FALSE(ccb.contains(i + '0'));
    ASSERT_TRUE(ccb2->contains(i + '0'));
  }
  ccb.AddCharClass(ccb2);
  for(int i = 0; i <= 9; ++i)
    ASSERT_TRUE(ccb.contains(i + '0'));
  for(char x = 'a'; x <= 'z'; ++x)
    ASSERT_TRUE(ccb.contains(x));
  delete ccb2;
}

int main(int argc, char* argv[])
{

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
