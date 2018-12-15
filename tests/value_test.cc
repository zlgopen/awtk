#include "tkc/value.h"
#include "gtest/gtest.h"

TEST(value, i8) {
  value_t v;
  ASSERT_EQ(&v, value_set_int8(&v, 10));
  ASSERT_EQ(value_int8(&v), 10);
}

TEST(value, u8) {
  value_t v;
  ASSERT_EQ(&v, value_set_uint8(&v, 10));
  ASSERT_EQ(value_uint8(&v), 10);
}

TEST(value, i16) {
  value_t v;
  ASSERT_EQ(&v, value_set_int16(&v, 10));
  ASSERT_EQ(value_int16(&v), 10);
}

TEST(value, u16) {
  value_t v;
  ASSERT_EQ(&v, value_set_uint16(&v, 10));
  ASSERT_EQ(value_uint16(&v), 10);
}

TEST(value, i32) {
  value_t v;
  ASSERT_EQ(&v, value_set_int32(&v, 10));
  ASSERT_EQ(value_int32(&v), 10);
}

TEST(value, u32) {
  value_t v;
  ASSERT_EQ(&v, value_set_uint32(&v, 10));
  ASSERT_EQ(value_uint32(&v), 10);
}

TEST(value, i64) {
  value_t v;
  ASSERT_EQ(&v, value_set_int64(&v, 10));
  ASSERT_EQ(value_int64(&v), 10);
}

TEST(value, u64) {
  value_t v;
  ASSERT_EQ(&v, value_set_uint64(&v, 10));
  ASSERT_EQ(value_uint64(&v), 10);
}

TEST(value, float) {
  value_t v;
  ASSERT_EQ(&v, value_set_float(&v, 10));
  ASSERT_EQ(value_float(&v), 10);
}

TEST(value, double) {
  value_t v;
  ASSERT_EQ(&v, value_set_double(&v, 10));
  ASSERT_EQ(value_double(&v), 10);
}

TEST(ValueTest, str) {
  value_t v;
  const char* str = "str";

  ASSERT_EQ(&v, value_set_str(&v, str));
  ASSERT_EQ(strcmp(value_str(&v), "str"), 0);
}

TEST(value, int) {
  value_t v;
  ASSERT_EQ(value_int(value_set_uint64(&v, 10)), 10);
}
