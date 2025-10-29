#include "tkc/revertible_str.h"
#include "gtest/gtest.h"
#include "common.h"

TEST(revertible_str, create) {
  revertible_str_t* s = revertible_str_create(10, 10);
  ASSERT_EQ(s != NULL, true);

  revertible_str_append(s, "abc");
  ASSERT_STREQ(revertible_str_get_str(s), "abc");

  revertible_str_append(s, "123");
  ASSERT_STREQ(revertible_str_get_str(s), "abc123");

  revertible_str_revert(s);
  ASSERT_STREQ(revertible_str_get_str(s), "abc");

  revertible_str_revert(s);
  ASSERT_STREQ(revertible_str_get_str(s), "");

  ASSERT_EQ(revertible_str_destroy(s), RET_OK);
}

TEST(revertible_str, append) {
  revertible_str_t s;
  revertible_str_init(&s, 10, 10);

  revertible_str_append(&s, "abc");
  ASSERT_STREQ(revertible_str_get_str(&s), "abc");

  revertible_str_append(&s, "123");
  ASSERT_STREQ(revertible_str_get_str(&s), "abc123");

  revertible_str_revert(&s);
  ASSERT_STREQ(revertible_str_get_str(&s), "abc");

  revertible_str_revert(&s);
  ASSERT_STREQ(revertible_str_get_str(&s), "");

  revertible_str_deinit(&s);
}

TEST(revertible_str, append_format) {
  revertible_str_t s;
  revertible_str_init(&s, 10, 10);

  revertible_str_append_format(&s, 10, "%s", "abc");
  ASSERT_STREQ(revertible_str_get_str(&s), "abc");

  revertible_str_append_format(&s, 10, "%s", "123");
  ASSERT_STREQ(revertible_str_get_str(&s), "abc123");

  revertible_str_revert(&s);
  ASSERT_STREQ(revertible_str_get_str(&s), "abc");

  revertible_str_revert(&s);
  ASSERT_STREQ(revertible_str_get_str(&s), "");

  revertible_str_deinit(&s);
}
