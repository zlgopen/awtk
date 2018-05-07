#include "base/str.h"
#include "gtest/gtest.h"

TEST(Str, basic) {
  str_t str;
  str_t* s = NULL;
  s = str_init(&str, 0);

  ASSERT_EQ(s->size, 0);

  ASSERT_EQ(str_set(s, "hello"), RET_OK);
  ASSERT_EQ(str_eq(s, "hello"), TRUE);

  ASSERT_EQ(str_append(s, " world"), RET_OK);
  ASSERT_EQ(str_eq(s, "hello world"), TRUE);

  ASSERT_EQ(str_to_upper(s), RET_OK);
  ASSERT_EQ(str_eq(s, "HELLO WORLD"), TRUE);

  ASSERT_EQ(str_to_lower(s), RET_OK);
  ASSERT_EQ(str_eq(s, "hello world"), TRUE);

  ASSERT_EQ(str_replace(s, "o", "ooo"), RET_OK);
  ASSERT_EQ(str_eq(s, "hellooo wooorld"), TRUE);

  ASSERT_EQ(str_replace(s, "ooo", "xxx"), RET_OK);
  ASSERT_EQ(str_eq(s, "hellxxx wxxxrld"), TRUE);

  ASSERT_EQ(str_trim_left(s, "hel"), RET_OK);
  ASSERT_EQ(str_eq(s, "xxx wxxxrld"), TRUE);

  ASSERT_EQ(str_trim_right(s, "rld"), RET_OK);
  ASSERT_EQ(str_eq(s, "xxx wxxx"), TRUE);

  ASSERT_EQ(str_trim(s, "x "), RET_OK);
  ASSERT_EQ(str_eq(s, "w"), TRUE);

  ASSERT_EQ(str_trim_right(s, "w"), RET_OK);
  ASSERT_EQ(str_eq(s, ""), TRUE);

  ASSERT_EQ(str_set(s, "test.png"), RET_OK);
  ASSERT_EQ(str_start_with(s, "test"), TRUE);
  ASSERT_EQ(str_end_with(s, ".png"), TRUE);

  str_reset(s);
}

TEST(Str, set_with_len) {
  str_t str;
  str_t* s = NULL;
  s = str_init(&str, 0);

  ASSERT_EQ(str_set_with_len(s, "hello world", 0), RET_OK);
  ASSERT_EQ(str_eq(s, ""), TRUE);

  ASSERT_EQ(str_set_with_len(s, "hello world", 1), RET_OK);
  ASSERT_EQ(str_eq(s, "h"), TRUE);

  ASSERT_EQ(str_set_with_len(s, "hello world", 5), RET_OK);
  ASSERT_EQ(str_eq(s, "hello"), TRUE);

  str_reset(s);
}
