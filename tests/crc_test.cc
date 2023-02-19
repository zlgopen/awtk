#include "tkc/crc.h"
#include "gtest/gtest.h"

TEST(crc, crc32_file) {
  ASSERT_EQ(tk_crc32_file("tests/testdata/test.png", 1024), 1567897941);
}
