#include "tkc/endian.h"
#include "gtest/gtest.h"

TEST(Endian, int16) {
  int16_t a = 0x1122;
  ASSERT_EQ(is_little_endian(), TRUE);
  ASSERT_EQ(int16_endian_invert(a), 0x2211);
  ASSERT_EQ(int16_to_big_endian(a), 0x2211);
  ASSERT_EQ(int16_from_big_endian(int16_to_big_endian(a)), a);
}

TEST(Endian, int32) {
  int32_t a = 0x11223344;
  ASSERT_EQ(is_little_endian(), TRUE);
  ASSERT_EQ(int32_endian_invert(a), 0x44332211);
  ASSERT_EQ(int32_to_big_endian(a), 0x44332211);
  ASSERT_EQ(int32_from_big_endian(int32_to_big_endian(a)), a);
}

TEST(Endian, int64) {
  int64_t a = 0x1122334455667788;
  ASSERT_EQ(is_little_endian(), TRUE);
  ASSERT_EQ(int64_endian_invert(a), 0x8877665544332211);
  ASSERT_EQ(int64_to_big_endian(a), 0x8877665544332211);
  ASSERT_EQ(int64_from_big_endian(int64_to_big_endian(a)), a);
}

TEST(Endian, float) {
  float_t a = 1122.3344;
  ASSERT_EQ(float_from_big_endian(float_to_big_endian(a)), a);
}

TEST(Endian, double) {
  double_t a = 1122.3344;
  ASSERT_EQ(double_from_big_endian(double_to_big_endian(a)), a);

  a = 12;
  ASSERT_EQ(double_from_big_endian(double_to_big_endian(a)), a);
}
