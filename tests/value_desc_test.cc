#include "tkc/value_desc.h"
#include "gtest/gtest.h"

TEST(ValueDesc, int8) {
  value_t v;
  value_desc_int8_t desc;

  memset(&desc, 0x00, sizeof(desc));
  desc.min = 10;
  desc.max = 30;
  desc.value_desc.type = VALUE_DESC_TYPE_INT8;

  value_set_int8(&v, 9);
  ASSERT_NE(value_desc_validate(&(desc.value_desc), &v), RET_OK);

  value_set_int8(&v, 19);
  ASSERT_EQ(value_desc_validate(&(desc.value_desc), &v), RET_OK);

  value_set_int8(&v, 10);
  ASSERT_EQ(value_desc_validate(&(desc.value_desc), &v), RET_OK);

  value_set_int8(&v, 30);
  ASSERT_EQ(value_desc_validate(&(desc.value_desc), &v), RET_OK);
}

TEST(ValueDesc, int16) {
  value_t v;
  value_desc_int16_t desc;

  memset(&desc, 0x00, sizeof(desc));
  desc.min = 10;
  desc.max = 30;
  desc.value_desc.type = VALUE_DESC_TYPE_INT16;

  value_set_int16(&v, 9);
  ASSERT_NE(value_desc_validate(&(desc.value_desc), &v), RET_OK);

  value_set_int16(&v, 19);
  ASSERT_EQ(value_desc_validate(&(desc.value_desc), &v), RET_OK);

  value_set_int16(&v, 10);
  ASSERT_EQ(value_desc_validate(&(desc.value_desc), &v), RET_OK);

  value_set_int16(&v, 30);
  ASSERT_EQ(value_desc_validate(&(desc.value_desc), &v), RET_OK);
}

TEST(ValueDesc, int32) {
  value_t v;
  value_desc_int32_t desc;

  memset(&desc, 0x00, sizeof(desc));
  desc.min = 10;
  desc.max = 30;
  desc.value_desc.type = VALUE_DESC_TYPE_INT32;

  value_set_int32(&v, 9);
  ASSERT_NE(value_desc_validate(&(desc.value_desc), &v), RET_OK);

  value_set_int32(&v, 19);
  ASSERT_EQ(value_desc_validate(&(desc.value_desc), &v), RET_OK);

  value_set_int32(&v, 10);
  ASSERT_EQ(value_desc_validate(&(desc.value_desc), &v), RET_OK);

  value_set_int32(&v, 30);
  ASSERT_EQ(value_desc_validate(&(desc.value_desc), &v), RET_OK);
}

TEST(ValueDesc, int64) {
  value_t v;
  value_desc_int64_t desc;

  memset(&desc, 0x00, sizeof(desc));
  desc.min = 10;
  desc.max = 30;
  desc.value_desc.type = VALUE_DESC_TYPE_INT64;

  value_set_int64(&v, 9);
  ASSERT_NE(value_desc_validate(&(desc.value_desc), &v), RET_OK);

  value_set_int64(&v, 19);
  ASSERT_EQ(value_desc_validate(&(desc.value_desc), &v), RET_OK);

  value_set_int64(&v, 10);
  ASSERT_EQ(value_desc_validate(&(desc.value_desc), &v), RET_OK);

  value_set_int64(&v, 30);
  ASSERT_EQ(value_desc_validate(&(desc.value_desc), &v), RET_OK);
}

TEST(ValueDesc, uint8) {
  value_t v;
  value_desc_uint8_t desc;

  memset(&desc, 0x00, sizeof(desc));
  desc.min = 10;
  desc.max = 30;
  desc.value_desc.type = VALUE_DESC_TYPE_INT8;

  value_set_uint8(&v, 9);
  ASSERT_NE(value_desc_validate(&(desc.value_desc), &v), RET_OK);

  value_set_uint8(&v, 19);
  ASSERT_EQ(value_desc_validate(&(desc.value_desc), &v), RET_OK);

  value_set_uint8(&v, 10);
  ASSERT_EQ(value_desc_validate(&(desc.value_desc), &v), RET_OK);

  value_set_uint8(&v, 30);
  ASSERT_EQ(value_desc_validate(&(desc.value_desc), &v), RET_OK);
}

TEST(ValueDesc, uint16) {
  value_t v;
  value_desc_uint16_t desc;

  memset(&desc, 0x00, sizeof(desc));
  desc.min = 10;
  desc.max = 30;
  desc.value_desc.type = VALUE_DESC_TYPE_INT16;

  value_set_uint16(&v, 9);
  ASSERT_NE(value_desc_validate(&(desc.value_desc), &v), RET_OK);

  value_set_uint16(&v, 19);
  ASSERT_EQ(value_desc_validate(&(desc.value_desc), &v), RET_OK);

  value_set_uint16(&v, 10);
  ASSERT_EQ(value_desc_validate(&(desc.value_desc), &v), RET_OK);

  value_set_uint16(&v, 30);
  ASSERT_EQ(value_desc_validate(&(desc.value_desc), &v), RET_OK);
}

TEST(ValueDesc, uint32) {
  value_t v;
  value_desc_uint32_t desc;

  memset(&desc, 0x00, sizeof(desc));
  desc.min = 10;
  desc.max = 30;
  desc.value_desc.type = VALUE_DESC_TYPE_INT32;

  value_set_uint32(&v, 9);
  ASSERT_NE(value_desc_validate(&(desc.value_desc), &v), RET_OK);

  value_set_uint32(&v, 19);
  ASSERT_EQ(value_desc_validate(&(desc.value_desc), &v), RET_OK);

  value_set_uint32(&v, 10);
  ASSERT_EQ(value_desc_validate(&(desc.value_desc), &v), RET_OK);

  value_set_uint32(&v, 30);
  ASSERT_EQ(value_desc_validate(&(desc.value_desc), &v), RET_OK);
}

TEST(ValueDesc, uint64) {
  value_t v;
  value_desc_uint64_t desc;

  memset(&desc, 0x00, sizeof(desc));
  desc.min = 10;
  desc.max = 30;
  desc.value_desc.type = VALUE_DESC_TYPE_INT64;

  value_set_uint64(&v, 9);
  ASSERT_NE(value_desc_validate(&(desc.value_desc), &v), RET_OK);

  value_set_uint64(&v, 19);
  ASSERT_EQ(value_desc_validate(&(desc.value_desc), &v), RET_OK);

  value_set_uint64(&v, 10);
  ASSERT_EQ(value_desc_validate(&(desc.value_desc), &v), RET_OK);

  value_set_uint64(&v, 30);
  ASSERT_EQ(value_desc_validate(&(desc.value_desc), &v), RET_OK);
}
