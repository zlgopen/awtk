#include "tkc/sha256.h"
#include "gtest/gtest.h"

TEST(SHA256, basic) {
  str_t str;
  str_init(&str, 100);

  tk_sha256((const uint8_t*)"foobar", 6, &str);
  //echo -n "foobar" | openssl dgst -sha256
  ASSERT_STREQ(str.str, "c3ab8ff13720e8ad9047dd39466b3c8974e592c2fa383d4a3960714caef0c4f2");

  str_set(&str, "");
  tk_sha256((const uint8_t*)"123\n", 4, &str);
  //echo "123" | openssl dgst -sha256
  ASSERT_STREQ(str.str, "181210f8f9c779c26da1d9b2075bde0127302ee0e3fca38c9a83f5b1dd8e5d3b");

  str_reset(&str);
}
