#include "charset/encoding.h"
#include "gtest/gtest.h"

TEST(Encoding, basic) {
  const char* str = "中文";
  char gbk[32];
  char utf8[32];
  ASSERT_EQ(encoding_utf8_to_gbk(str, strlen(str), gbk, sizeof(gbk)), RET_OK);
  ASSERT_EQ(encoding_gbk_to_utf8(gbk, strlen(gbk), utf8, sizeof(utf8)), RET_OK);
  ASSERT_STREQ(str, utf8);
}
