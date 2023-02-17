#include "tkc/value.h"
#include "gtest/gtest.h"
#include "tkc/object_default.h"

TEST(value, i8) {
  value_t v;
  ASSERT_EQ(&v, value_set_int8(&v, 10));
  ASSERT_EQ(value_int8(&v), 10);
}

TEST(value, stri8) {
  value_t v;
  ASSERT_EQ(&v, value_set_str(&v, "10"));
  ASSERT_EQ(value_int8(&v), 10);
}

TEST(value, wstri8) {
  value_t v;
  ASSERT_EQ(&v, value_set_wstr(&v, L"10"));
  ASSERT_EQ(value_int8(&v), 10);
}

TEST(value, u8) {
  value_t v;
  ASSERT_EQ(&v, value_set_uint8(&v, 10));
  ASSERT_EQ(value_uint8(&v), 10);
}

TEST(value, stru8) {
  value_t v;
  ASSERT_EQ(&v, value_set_str(&v, "10"));
  ASSERT_EQ(value_uint8(&v), 10);
}

TEST(value, wstru8) {
  value_t v;
  ASSERT_EQ(&v, value_set_wstr(&v, L"10"));
  ASSERT_EQ(value_uint8(&v), 10);
}

TEST(value, i16) {
  value_t v;
  ASSERT_EQ(&v, value_set_int16(&v, 10));
  ASSERT_EQ(value_int16(&v), 10);
}

TEST(value, stri16) {
  value_t v;
  ASSERT_EQ(&v, value_set_str(&v, "10"));
  ASSERT_EQ(value_int16(&v), 10);
}

TEST(value, wstri16) {
  value_t v;
  ASSERT_EQ(&v, value_set_wstr(&v, L"10"));
  ASSERT_EQ(value_int16(&v), 10);
}

TEST(value, u16) {
  value_t v;
  ASSERT_EQ(&v, value_set_uint16(&v, 10));
  ASSERT_EQ(value_uint16(&v), 10);
}

TEST(value, stru16) {
  value_t v;
  ASSERT_EQ(&v, value_set_str(&v, "10"));
  ASSERT_EQ(value_uint16(&v), 10);
}

TEST(value, wstru16) {
  value_t v;
  ASSERT_EQ(&v, value_set_wstr(&v, L"10"));
  ASSERT_EQ(value_uint16(&v), 10);
}

TEST(value, i32) {
  value_t v;
  ASSERT_EQ(&v, value_set_int32(&v, 10));
  ASSERT_EQ(value_int32(&v), 10);
}

TEST(value, stri32) {
  value_t v;
  ASSERT_EQ(&v, value_set_str(&v, "10"));
  ASSERT_EQ(value_int32(&v), 10);
}

TEST(value, wstri32) {
  value_t v;
  ASSERT_EQ(&v, value_set_wstr(&v, L"10"));
  ASSERT_EQ(value_int32(&v), 10);
}

TEST(value, u32) {
  value_t v;
  ASSERT_EQ(&v, value_set_uint32(&v, 10));
  ASSERT_EQ(value_uint32(&v), 10u);
}

TEST(value, stru32) {
  value_t v;
  ASSERT_EQ(&v, value_set_str(&v, "10"));
  ASSERT_EQ(value_uint32(&v), 10u);
}

TEST(value, wstru32) {
  value_t v;
  ASSERT_EQ(&v, value_set_wstr(&v, L"10"));
  ASSERT_EQ(value_uint32(&v), 10u);
}

TEST(value, i64) {
  value_t v;
  ASSERT_EQ(&v, value_set_int64(&v, 10));
  ASSERT_EQ(value_int64(&v), 10);
}

TEST(value, stri64) {
  value_t v;
  ASSERT_EQ(&v, value_set_str(&v, "10"));
  ASSERT_EQ(value_int64(&v), 10);
}

TEST(value, wstri64) {
  value_t v;
  ASSERT_EQ(&v, value_set_wstr(&v, L"10"));
  ASSERT_EQ(value_int64(&v), 10);
}

TEST(value, u64) {
  value_t v;
  ASSERT_EQ(&v, value_set_uint64(&v, 10));
  ASSERT_EQ(value_uint64(&v), 10u);
}

TEST(value, stru64) {
  value_t v;
  ASSERT_EQ(&v, value_set_str(&v, "10"));
  ASSERT_EQ(value_uint64(&v), 10u);
}

TEST(value, wstru64) {
  value_t v;
  ASSERT_EQ(&v, value_set_wstr(&v, L"10"));
  ASSERT_EQ(value_uint64(&v), 10u);
}

TEST(value, float) {
  value_t v;
  ASSERT_EQ(&v, value_set_float(&v, 10));
  ASSERT_EQ(value_float(&v), 10.0f);
}

TEST(value, strfloat) {
  value_t v;
  ASSERT_EQ(&v, value_set_str(&v, "10"));
  ASSERT_EQ(value_float(&v), 10.0f);
}

TEST(value, wstrfloat) {
  value_t v;
  ASSERT_EQ(&v, value_set_wstr(&v, L"10"));
  ASSERT_EQ(value_float(&v), 10.0f);
}

TEST(value, double) {
  value_t v;
  ASSERT_EQ(&v, value_set_double(&v, 10));
  ASSERT_EQ(value_double(&v), 10.0);
}

TEST(value, bool_to_double) {
  value_t v;
  ASSERT_EQ(&v, value_set_bool(&v, true));
  ASSERT_EQ(value_double(&v), 1.0);
}

TEST(value, strdouble) {
  value_t v;
  ASSERT_EQ(&v, value_set_str(&v, "10"));
  ASSERT_EQ(value_double(&v), 10.0);
}

TEST(value, wstrdouble) {
  value_t v;
  ASSERT_EQ(&v, value_set_wstr(&v, L"10"));
  ASSERT_EQ(value_double(&v), 10.0);
}

TEST(ValueTest, str) {
  value_t v;
  const char* str = "str";

  ASSERT_EQ(&v, value_set_str(&v, str));
  ASSERT_EQ(strcmp(value_str(&v), "str"), 0);
}

TEST(ValueTest, sized_str) {
  value_t v;
  const char* str = "str";
  sized_str_t* sized_str = NULL;
  ASSERT_EQ(&v, value_set_sized_str(&v, (char*)str, 2));
  sized_str = value_sized_str(&v);
  ASSERT_EQ(sized_str != NULL, true);
  ASSERT_EQ(sized_str->size, 2u);
}

TEST(ValueTest, binary_data) {
  value_t v;
  const char* str = "str";
  binary_data_t* binary_data = NULL;
  ASSERT_EQ(&v, value_set_binary_data(&v, (void*)str, 2));
  binary_data = value_binary_data(&v);
  ASSERT_EQ(binary_data != NULL, true);
  ASSERT_EQ(binary_data->size, 2u);
}

TEST(ValueTest, token) {
  value_t v;

  ASSERT_EQ(&v, value_set_token(&v, 123));
  ASSERT_EQ(value_token(&v), 123u);
}

TEST(ValueTest, wstr) {
  value_t v;
  const wchar_t* str = L"str";

  ASSERT_EQ(&v, value_set_wstr(&v, str));
  ASSERT_EQ(wcscmp(value_wstr(&v), L"str"), 0);
}

TEST(ValueTest, object) {
  value_t v;
  tk_object_t* o = object_default_create();

  ASSERT_EQ(&v, value_set_object(&v, o));
  ASSERT_EQ(o, value_object(&v));

  tk_object_unref(o);
}

TEST(value, int) {
  value_t v;
  ASSERT_EQ(value_int(value_set_uint64(&v, 10)), 10);
}

TEST(value, id) {
  value_t v;
  ASSERT_STREQ(value_id(value_set_id(&v, "abc", 3)), "abc");
  value_reset(&v);
}

TEST(value, func) {
  value_t v;
  ASSERT_STREQ((char*)value_func(value_set_func(&v, (void*)"abc")), "abc");
  value_reset(&v);
}

TEST(ValueTest, copy) {
  value_t v;
  value_t other;
  const char* str = "str";

  ASSERT_EQ(&other, value_set_str(&other, str));
  ASSERT_EQ(value_copy(&v, &other), RET_OK);

  ASSERT_EQ(strcmp(value_str(&v), "str"), 0);
  ASSERT_EQ(v.value.str, other.value.str);

  value_reset(&v);
  value_reset(&other);
}

TEST(ValueTest, deepcopy) {
  value_t v;
  value_t other;
  const char* str = "str";

  ASSERT_EQ(&other, value_set_str(&other, str));
  ASSERT_EQ(value_deep_copy(&v, &other), RET_OK);

  ASSERT_EQ(strcmp(value_str(&v), "str"), 0);
  ASSERT_NE(v.value.str, other.value.str);

  value_reset(&v);
  value_reset(&other);
}

TEST(ValueTest, deepcopy_binary) {
  value_t v;
  value_t other;
  const char* str = "str";
  binary_data_t* bin1 = NULL;
  binary_data_t* bin2 = NULL;

  ASSERT_EQ(&other, value_set_binary_data(&other, (void*)str, 4));
  ASSERT_EQ(value_deep_copy(&v, &other), RET_OK);

  bin1 = value_binary_data(&v);
  bin2 = value_binary_data(&other);

  ASSERT_STREQ((const char*)(bin1->data), str);
  ASSERT_STREQ((const char*)(bin2->data), str);
  ASSERT_EQ(bin1->data != bin2->data, true);

  value_reset(&v);
  value_reset(&other);
}

TEST(ValueTest, copy_str) {
  value_t v;
  value_t other;

  ASSERT_EQ(value_dup_str(&v, "hello"), &v);
  ASSERT_EQ(value_copy(&other, &v), RET_OK);

  value_reset(&v);
  value_reset(&other);
}

TEST(ValueTest, ubjson) {
  value_t v;
  const char* str = "str";
  binary_data_t* ubjson = NULL;
  ASSERT_EQ(&v, value_set_ubjson(&v, (void*)str, 2));
  ASSERT_EQ(v.type, (uint32_t)VALUE_TYPE_UBJSON);

  ubjson = value_ubjson(&v);
  ASSERT_EQ(ubjson != NULL, true);
  ASSERT_EQ(ubjson->size, 2u);
}

TEST(ValueTest, gradient) {
  value_t v;
  const char* str = "str";
  binary_data_t* gradient = NULL;
  ASSERT_EQ(&v, value_set_gradient(&v, (void*)str, 2));
  ASSERT_EQ(v.type, (uint32_t)VALUE_TYPE_GRADIENT);

  gradient = value_gradient(&v);
  ASSERT_EQ(gradient != NULL, true);
  ASSERT_EQ(gradient->size, 2u);
}

TEST(value, i64_from_str) {
  value_t v;
  ASSERT_EQ(&v, value_set_str(&v, "12345678912345"));
  ASSERT_EQ(value_int64(&v), 12345678912345);

  ASSERT_EQ(&v, value_set_str(&v, "-12345678912345"));
  ASSERT_EQ(value_int64(&v), -12345678912345);
}

TEST(value, ui64_from_str) {
  value_t v;
  ASSERT_EQ(&v, value_set_str(&v, "12345678912345"));
  ASSERT_EQ(value_uint64(&v), 12345678912345);
}

#include "tkc/utils.h"
#include "tkc/rect.h"
TEST(value, pointer) {
  value_t v;
  char* str = tk_strdup("abc");
  ASSERT_EQ(&v, value_set_pointer_ex(&v, str, default_destroy));
  ASSERT_STREQ((char*)value_pointer(&v), "abc");
  value_reset(&v);
}

TEST(value, bitmap) {
  value_t v;
  void* bitmap = &v;
  ASSERT_EQ(value_bitmap(value_set_bitmap(&v, bitmap)), bitmap);
}

TEST(value, rect) {
  value_t v;
  rect_t r1 = rect_init(10, 10, 20, 20);
  rect_t* r2 = value_rect(value_set_rect(&v, r1));
  ASSERT_EQ(r1.x, r2->x);
  ASSERT_EQ(r1.y, r2->y);
  ASSERT_EQ(r1.w, r2->w);
  ASSERT_EQ(r1.h, r2->h);
}

TEST(value, lshift) {
  value_t v;
  value_t r;
  uint32_t i = 0;

  for (i = 0; i < 100; i++) {
    int8_t i8 = 1;
    value_set_int8(&v, i8);
    ASSERT_EQ(value_lshift(&v, &r, i), RET_OK);
    ASSERT_EQ(value_int8(&r), (int8_t)(i8 << i));

    i8 = 0b10101010;
    value_set_int8(&v, i8);
    ASSERT_EQ(value_lshift(&v, &r, i), RET_OK);
    ASSERT_EQ(r.type, VALUE_TYPE_INT8);
    ASSERT_EQ(value_int8(&r), (int8_t)(i8 << i));
  }

  for (i = 0; i < 100; i++) {
    int16_t i16 = 1;
    value_set_int16(&v, i16);
    ASSERT_EQ(value_lshift(&v, &r, i), RET_OK);
    ASSERT_EQ(value_int16(&r), (int16_t)(i16 << i));

    i16 = 0b10101010;
    value_set_int16(&v, i16);
    ASSERT_EQ(value_lshift(&v, &r, i), RET_OK);
    ASSERT_EQ(r.type, VALUE_TYPE_INT16);
    ASSERT_EQ(value_int16(&r), (int16_t)(i16 << i));
  }

  for (i = 0; i < 100; i++) {
    int32_t i32 = 1;
    value_set_int32(&v, i32);
    ASSERT_EQ(value_lshift(&v, &r, i), RET_OK);
    ASSERT_EQ(value_int32(&r), (int32_t)(i32 << i));

    i32 = 0b10101010;
    value_set_int32(&v, i32);
    ASSERT_EQ(value_lshift(&v, &r, i), RET_OK);
    ASSERT_EQ(r.type, VALUE_TYPE_INT32);
    ASSERT_EQ(value_int32(&r), (int32_t)(i32 << i));
  }

  for (i = 0; i < 100; i++) {
    int64_t i64 = 1;
    value_set_int64(&v, i64);
    ASSERT_EQ(value_lshift(&v, &r, i), RET_OK);
    ASSERT_EQ(value_int64(&r), (int64_t)(i64 << i));

    i64 = 0b10101010;
    value_set_int64(&v, i64);
    ASSERT_EQ(value_lshift(&v, &r, i), RET_OK);
    ASSERT_EQ(r.type, VALUE_TYPE_INT64);
    ASSERT_EQ(value_int64(&r), (int64_t)(i64 << i));
  }

  for (i = 0; i < 100; i++) {
    uint8_t i8 = 1;
    value_set_uint8(&v, i8);
    ASSERT_EQ(value_lshift(&v, &r, i), RET_OK);
    ASSERT_EQ(value_uint8(&r), (uint8_t)(i8 << i));

    i8 = 0b10101010;
    value_set_uint8(&v, i8);
    ASSERT_EQ(value_lshift(&v, &r, i), RET_OK);
    ASSERT_EQ(r.type, VALUE_TYPE_UINT8);
    ASSERT_EQ(value_uint8(&r), (uint8_t)(i8 << i));
  }

  for (i = 0; i < 100; i++) {
    uint16_t i16 = 1;
    value_set_uint16(&v, i16);
    ASSERT_EQ(value_lshift(&v, &r, i), RET_OK);
    ASSERT_EQ(value_uint16(&r), (uint16_t)(i16 << i));

    i16 = 0b10101010;
    value_set_uint16(&v, i16);
    ASSERT_EQ(value_lshift(&v, &r, i), RET_OK);
    ASSERT_EQ(r.type, VALUE_TYPE_UINT16);
    ASSERT_EQ(value_uint16(&r), (uint16_t)(i16 << i));
  }

  for (i = 0; i < 100; i++) {
    uint32_t i32 = 1;
    value_set_uint32(&v, i32);
    ASSERT_EQ(value_lshift(&v, &r, i), RET_OK);
    ASSERT_EQ(value_uint32(&r), (uint32_t)(i32 << i));

    i32 = 0b10101010;
    value_set_uint32(&v, i32);
    ASSERT_EQ(value_lshift(&v, &r, i), RET_OK);
    ASSERT_EQ(r.type, VALUE_TYPE_UINT32);
    ASSERT_EQ(value_uint32(&r), (uint32_t)(i32 << i));
  }

  for (i = 0; i < 100; i++) {
    uint64_t i64 = 1;
    value_set_uint64(&v, i64);
    ASSERT_EQ(value_lshift(&v, &r, i), RET_OK);
    ASSERT_EQ(value_uint64(&r), (uint64_t)(i64 << i));

    i64 = 0b10101010;
    value_set_uint64(&v, i64);
    ASSERT_EQ(value_lshift(&v, &r, i), RET_OK);
    ASSERT_EQ(r.type, VALUE_TYPE_UINT64);
    ASSERT_EQ(value_uint64(&r), (uint64_t)(i64 << i));
  }
}

TEST(value, rshift) {
  value_t v;
  value_t r;
  uint32_t i = 0;

  for (i = 0; i < 100; i++) {
    int8_t i8 = 0xf0;
    value_set_int8(&v, i8);
    ASSERT_EQ(value_rshift(&v, &r, i), RET_OK);
    ASSERT_EQ(value_int8(&r), (int8_t)(i8 >> i));

    i8 = 0b10101010;
    value_set_int8(&v, i8);
    ASSERT_EQ(value_rshift(&v, &r, i), RET_OK);
    ASSERT_EQ(r.type, VALUE_TYPE_INT8);
    ASSERT_EQ(value_int8(&r), (int8_t)(i8 >> i));
  }

  for (i = 0; i < 100; i++) {
    int16_t i16 = 0xfe00;
    value_set_int16(&v, i16);
    ASSERT_EQ(value_rshift(&v, &r, i), RET_OK);
    ASSERT_EQ(value_int16(&r), (int16_t)(i16 >> i));

    i16 = 0b10101010;
    value_set_int16(&v, i16);
    ASSERT_EQ(value_rshift(&v, &r, i), RET_OK);
    ASSERT_EQ(r.type, VALUE_TYPE_INT16);
    ASSERT_EQ(value_int16(&r), (int16_t)(i16 >> i));
  }

  for (i = 0; i < 100; i++) {
    int32_t i32 = 0xf0f01000;
    value_set_int32(&v, i32);
    ASSERT_EQ(value_rshift(&v, &r, i), RET_OK);
    ASSERT_EQ(value_int32(&r), (int32_t)(i32 >> i));

    i32 = 0b10101010;
    value_set_int32(&v, i32);
    ASSERT_EQ(value_rshift(&v, &r, i), RET_OK);
    ASSERT_EQ(r.type, VALUE_TYPE_INT32);
    ASSERT_EQ(value_int32(&r), (int32_t)(i32 >> i));
  }

  for (i = 0; i < 100; i++) {
    int64_t i64 = 0xf000f000f0001111;
    value_set_int64(&v, i64);
    ASSERT_EQ(value_rshift(&v, &r, i), RET_OK);
    ASSERT_EQ(value_int64(&r), (int64_t)(i64 >> i));

    i64 = 0b10101010;
    value_set_int64(&v, i64);
    ASSERT_EQ(value_rshift(&v, &r, i), RET_OK);
    ASSERT_EQ(r.type, VALUE_TYPE_INT64);
    ASSERT_EQ(value_int64(&r), (int64_t)(i64 >> i));
  }

  for (i = 0; i < 100; i++) {
    uint8_t i8 = 1;
    value_set_uint8(&v, i8);
    ASSERT_EQ(value_rshift(&v, &r, i), RET_OK);
    ASSERT_EQ(value_uint8(&r), (uint8_t)(i8 >> i));

    i8 = 0b10101010;
    value_set_uint8(&v, i8);
    ASSERT_EQ(value_rshift(&v, &r, i), RET_OK);
    ASSERT_EQ(r.type, VALUE_TYPE_UINT8);
    ASSERT_EQ(value_uint8(&r), (uint8_t)(i8 >> i));
  }

  for (i = 0; i < 100; i++) {
    uint16_t i16 = 0xf123;
    value_set_uint16(&v, i16);
    ASSERT_EQ(value_rshift(&v, &r, i), RET_OK);
    ASSERT_EQ(value_uint16(&r), (uint16_t)(i16 >> i));

    i16 = 0b10101010;
    value_set_uint16(&v, i16);
    ASSERT_EQ(value_rshift(&v, &r, i), RET_OK);
    ASSERT_EQ(r.type, VALUE_TYPE_UINT16);
    ASSERT_EQ(value_uint16(&r), (uint16_t)(i16 >> i));
  }

  for (i = 0; i < 100; i++) {
    uint32_t i32 = 0xfa0fa;
    value_set_uint32(&v, i32);
    ASSERT_EQ(value_rshift(&v, &r, i), RET_OK);
    ASSERT_EQ(value_uint32(&r), (uint32_t)(i32 >> i));

    i32 = 0b10101010;
    value_set_uint32(&v, i32);
    ASSERT_EQ(value_rshift(&v, &r, i), RET_OK);
    ASSERT_EQ(r.type, VALUE_TYPE_UINT32);
    ASSERT_EQ(value_uint32(&r), (uint32_t)(i32 >> i));
  }

  for (i = 0; i < 100; i++) {
    uint64_t i64 = 0x1000200030004000;
    value_set_uint64(&v, i64);
    ASSERT_EQ(value_rshift(&v, &r, i), RET_OK);
    ASSERT_EQ(value_uint64(&r), (uint64_t)(i64 >> i));

    i64 = 0b10101010;
    value_set_uint64(&v, i64);
    ASSERT_EQ(value_rshift(&v, &r, i), RET_OK);
    ASSERT_EQ(r.type, VALUE_TYPE_UINT64);
    ASSERT_EQ(value_uint64(&r), (uint64_t)(i64 >> i));
  }
}

TEST(value, lshift_r) {
  value_t v;
  value_t r;

  uint8_t u8 = 0b11000011;
  value_set_uint8(&v, u8);
  ASSERT_EQ(value_lshift_r(&v, &r, 3), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT8);
  ASSERT_EQ(value_uint8(&r), (uint8_t)(0b00011110));

  u8 = 0b11000011;
  value_set_uint8(&v, u8);
  ASSERT_EQ(value_lshift_r(&v, &r, 11), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT8);
  ASSERT_EQ(value_uint8(&r), (uint8_t)(0b00011110));

  uint16_t u16 = 0xf1f2;
  value_set_uint16(&v, u16);
  ASSERT_EQ(value_lshift_r(&v, &r, 4), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT16);
  ASSERT_EQ(value_uint16(&r), 0x1f2f);

  u16 = 0xf1f2;
  value_set_uint16(&v, u16);
  ASSERT_EQ(value_lshift_r(&v, &r, 20), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT16);
  ASSERT_EQ(value_uint16(&r), 0x1f2f);

  uint32_t u32 = 0xf1f2abcd;
  value_set_uint32(&v, u32);
  ASSERT_EQ(value_lshift_r(&v, &r, 4), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT32);
  ASSERT_EQ(value_uint32(&r), 0x1f2abcdf);

  u32 = 0xf1f2abcd;
  value_set_uint32(&v, u32);
  ASSERT_EQ(value_lshift_r(&v, &r, 36), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT32);
  ASSERT_EQ(value_uint32(&r), 0x1f2abcdf);

  uint64_t u64 = 0xf1f2abcdf1f2abcd;
  value_set_uint64(&v, u64);
  ASSERT_EQ(value_lshift_r(&v, &r, 4), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT64);
  ASSERT_EQ(value_uint64(&r), 0x1f2abcdf1f2abcdf);

  u64 = 0xf1f2abcdf1f2abcd;
  value_set_uint64(&v, u64);
  ASSERT_EQ(value_lshift_r(&v, &r, 68), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT64);
  ASSERT_EQ(value_uint64(&r), 0x1f2abcdf1f2abcdf);
}

TEST(value, rshift_r) {
  value_t v;
  value_t r;

  uint8_t u8 = 0b11000011;
  value_set_uint8(&v, u8);
  ASSERT_EQ(value_rshift_r(&v, &r, 3), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT8);
  ASSERT_EQ(value_uint8(&r), (uint8_t)(0b01111000));

  u8 = 0b11000011;
  value_set_uint8(&v, u8);
  ASSERT_EQ(value_rshift_r(&v, &r, 11), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT8);
  ASSERT_EQ(value_uint8(&r), (uint8_t)(0b01111000));

  uint16_t u16 = 0xf1f2;
  value_set_uint16(&v, u16);
  ASSERT_EQ(value_rshift_r(&v, &r, 4), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT16);
  ASSERT_EQ(value_uint16(&r), 0x2f1f);

  u16 = 0xf1f2;
  value_set_uint16(&v, u16);
  ASSERT_EQ(value_rshift_r(&v, &r, 20), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT16);
  ASSERT_EQ(value_uint16(&r), 0x2f1f);

  uint32_t u32 = 0xf1f2abcd;
  value_set_uint32(&v, u32);
  ASSERT_EQ(value_rshift_r(&v, &r, 4), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT32);
  ASSERT_EQ(value_uint32(&r), 0xdf1f2abc);

  u32 = 0xf1f2abcd;
  value_set_uint32(&v, u32);
  ASSERT_EQ(value_rshift_r(&v, &r, 36), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT32);
  ASSERT_EQ(value_uint32(&r), 0xdf1f2abc);

  uint64_t u64 = 0xf1f2abcdf1f2abcd;
  value_set_uint64(&v, u64);
  ASSERT_EQ(value_rshift_r(&v, &r, 4), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT64);
  ASSERT_EQ(value_uint64(&r), 0xdf1f2abcdf1f2abc);

  u64 = 0xf1f2abcdf1f2abcd;
  value_set_uint64(&v, u64);
  ASSERT_EQ(value_rshift_r(&v, &r, 68), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT64);
  ASSERT_EQ(value_uint64(&r), 0xdf1f2abcdf1f2abc);
}

TEST(value, bits) {
  value_t v;
  value_t r;
  uint32_t i = 0;

  for (i = 0; i < sizeof(int8_t); i++) {
    value_set_int8(&v, 0);
    ASSERT_EQ(value_set_bit(&v, &r, i, TRUE), RET_OK);
    ASSERT_EQ(value_int8(&r), 1 << i);
    v = r;
    ASSERT_EQ(value_get_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_bool(&r), TRUE);

    ASSERT_EQ(value_toggle_bit(&v, &r, i), RET_OK);
    v = r;
    ASSERT_EQ(value_get_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_bool(&r), FALSE);

    value_set_int8(&v, 0);
    ASSERT_EQ(value_set_bit(&v, &r, i, TRUE), RET_OK);
    v = r;
    ASSERT_EQ(value_toggle_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_get_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_bool(&r), TRUE);
  }

  for (i = 0; i < sizeof(uint8_t); i++) {
    value_set_uint8(&v, 0);
    ASSERT_EQ(value_set_bit(&v, &r, i, TRUE), RET_OK);
    ASSERT_EQ(value_uint8(&r), 1 << i);
    v = r;
    ASSERT_EQ(value_get_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_bool(&r), TRUE);

    ASSERT_EQ(value_toggle_bit(&v, &r, i), RET_OK);
    v = r;
    ASSERT_EQ(value_get_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_bool(&r), FALSE);

    value_set_uint8(&v, 0);
    ASSERT_EQ(value_set_bit(&v, &r, i, TRUE), RET_OK);
    v = r;
    ASSERT_EQ(value_toggle_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_get_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_bool(&r), TRUE);
  }

  for (i = 0; i < sizeof(int16_t); i++) {
    value_set_int16(&v, 0);
    ASSERT_EQ(value_set_bit(&v, &r, i, TRUE), RET_OK);
    ASSERT_EQ(value_int16(&r), 1 << i);
    v = r;
    ASSERT_EQ(value_get_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_bool(&r), TRUE);

    ASSERT_EQ(value_toggle_bit(&v, &r, i), RET_OK);
    v = r;
    ASSERT_EQ(value_get_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_bool(&r), FALSE);

    value_set_int16(&v, 0);
    ASSERT_EQ(value_set_bit(&v, &r, i, TRUE), RET_OK);
    v = r;
    ASSERT_EQ(value_toggle_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_get_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_bool(&r), TRUE);
  }

  for (i = 0; i < sizeof(uint16_t); i++) {
    value_set_uint16(&v, 0);
    ASSERT_EQ(value_set_bit(&v, &r, i, TRUE), RET_OK);
    ASSERT_EQ(value_uint16(&r), 1 << i);
    v = r;
    ASSERT_EQ(value_get_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_bool(&r), TRUE);

    ASSERT_EQ(value_toggle_bit(&v, &r, i), RET_OK);
    v = r;
    ASSERT_EQ(value_get_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_bool(&r), FALSE);

    value_set_uint16(&v, 0);
    ASSERT_EQ(value_set_bit(&v, &r, i, TRUE), RET_OK);
    v = r;
    ASSERT_EQ(value_toggle_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_get_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_bool(&r), TRUE);
  }

  for (i = 0; i < sizeof(int32_t); i++) {
    value_set_int32(&v, 0);
    ASSERT_EQ(value_set_bit(&v, &r, i, TRUE), RET_OK);
    ASSERT_EQ(value_int32(&r), 1 << i);
    v = r;
    ASSERT_EQ(value_get_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_bool(&r), TRUE);

    ASSERT_EQ(value_toggle_bit(&v, &r, i), RET_OK);
    v = r;
    ASSERT_EQ(value_get_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_bool(&r), FALSE);

    value_set_int32(&v, 0);
    ASSERT_EQ(value_set_bit(&v, &r, i, TRUE), RET_OK);
    v = r;
    ASSERT_EQ(value_toggle_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_get_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_bool(&r), TRUE);
  }

  for (i = 0; i < sizeof(uint32_t); i++) {
    value_set_uint32(&v, 0);
    ASSERT_EQ(value_set_bit(&v, &r, i, TRUE), RET_OK);
    ASSERT_EQ(value_uint32(&r), 1 << i);
    v = r;
    ASSERT_EQ(value_get_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_bool(&r), TRUE);

    ASSERT_EQ(value_toggle_bit(&v, &r, i), RET_OK);
    v = r;
    ASSERT_EQ(value_get_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_bool(&r), FALSE);

    value_set_uint32(&v, 0);
    ASSERT_EQ(value_set_bit(&v, &r, i, TRUE), RET_OK);
    v = r;
    ASSERT_EQ(value_toggle_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_get_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_bool(&r), TRUE);
  }

  for (i = 0; i < sizeof(int64_t); i++) {
    value_set_int64(&v, 0);
    ASSERT_EQ(value_set_bit(&v, &r, i, TRUE), RET_OK);
    ASSERT_EQ(value_int64(&r), 1 << i);
    v = r;
    ASSERT_EQ(value_get_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_bool(&r), TRUE);

    ASSERT_EQ(value_toggle_bit(&v, &r, i), RET_OK);
    v = r;
    ASSERT_EQ(value_get_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_bool(&r), FALSE);

    value_set_int64(&v, 0);
    ASSERT_EQ(value_set_bit(&v, &r, i, TRUE), RET_OK);
    v = r;
    ASSERT_EQ(value_toggle_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_get_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_bool(&r), TRUE);
  }

  for (i = 0; i < sizeof(uint64_t); i++) {
    value_set_uint64(&v, 0);
    ASSERT_EQ(value_set_bit(&v, &r, i, TRUE), RET_OK);
    ASSERT_EQ(value_uint64(&r), 1 << i);
    v = r;
    ASSERT_EQ(value_get_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_bool(&r), TRUE);

    ASSERT_EQ(value_toggle_bit(&v, &r, i), RET_OK);
    v = r;
    ASSERT_EQ(value_get_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_bool(&r), FALSE);

    value_set_uint64(&v, 0);
    ASSERT_EQ(value_set_bit(&v, &r, i, TRUE), RET_OK);
    v = r;
    ASSERT_EQ(value_toggle_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_get_bit(&v, &r, i), RET_OK);
    ASSERT_EQ(value_bool(&r), TRUE);
  }
  value_set_int8(&v, 0x70);
  ASSERT_EQ(value_set_bit(&v, &r, 1, TRUE), RET_OK);
  ASSERT_EQ(value_int8(&r), 0x72);

  v = r;
  ASSERT_EQ(value_get_bit(&v, &r, 1), RET_OK);
  ASSERT_EQ(value_bool(&r), TRUE);

  ASSERT_EQ(value_toggle_bit(&v, &r, 1), RET_OK);
  ASSERT_EQ(value_int8(&r), 0x70);

  v = r;
  ASSERT_EQ(value_get_bit(&v, &r, 1), RET_OK);
  ASSERT_EQ(value_bool(&r), FALSE);

  ASSERT_EQ(value_toggle_bit(&v, &r, 1), RET_OK);
  ASSERT_EQ(value_int8(&r), 0x72);
  v = r;
  ASSERT_EQ(value_get_bit(&v, &r, 1), RET_OK);
  ASSERT_EQ(value_bool(&r), TRUE);

  value_set_int8(&v, 0xf1);
  ASSERT_EQ(value_bit_not(&v, &r), RET_OK);
  ASSERT_EQ(value_int8(&r), ~value_int8(&v));

  value_set_bool(&v, TRUE);
  ASSERT_EQ(value_bit_not(&v, &r), RET_OK);
  ASSERT_EQ(value_bool(&r), FALSE);

  value_set_bool(&v, FALSE);
  ASSERT_EQ(value_bit_not(&v, &r), RET_OK);
  ASSERT_EQ(value_bool(&r), TRUE);
}

TEST(value, bit_and_or_xor) {
  value_t v1;
  value_t v2;
  value_t r;

  value_set_int8(&v1, 0xf0);
  value_set_int8(&v2, 0x0f);
  ASSERT_EQ(value_bit_and(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_INT8);
  ASSERT_EQ(value_int8(&r), 0);

  ASSERT_EQ(value_bit_or(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_INT8);
  ASSERT_EQ(value_int8(&r), (int8_t)0xff);

  ASSERT_EQ(value_bit_xor(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_INT8);
  ASSERT_EQ(value_int8(&r), (int8_t)0xff);

  value_set_uint8(&v1, 0xf0);
  value_set_uint8(&v2, 0x0f);
  ASSERT_EQ(value_bit_and(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT8);
  ASSERT_EQ(value_uint8(&r), 0);

  ASSERT_EQ(value_bit_or(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT8);
  ASSERT_EQ(value_uint8(&r), (uint8_t)0xff);

  ASSERT_EQ(value_bit_xor(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT8);
  ASSERT_EQ(value_uint8(&r), (uint8_t)0xff);

  value_set_int16(&v1, 0xf0);
  value_set_int16(&v2, 0x0f);
  ASSERT_EQ(value_bit_and(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_INT16);
  ASSERT_EQ(value_int16(&r), 0);

  ASSERT_EQ(value_bit_or(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_INT16);
  ASSERT_EQ(value_int16(&r), (int16_t)0xff);

  ASSERT_EQ(value_bit_xor(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_INT16);
  ASSERT_EQ(value_int16(&r), (int16_t)0xff);

  value_set_uint16(&v1, 0xf0);
  value_set_uint16(&v2, 0x0f);
  ASSERT_EQ(value_bit_and(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT16);
  ASSERT_EQ(value_uint16(&r), 0);

  ASSERT_EQ(value_bit_or(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT16);
  ASSERT_EQ(value_uint16(&r), (uint16_t)0xff);

  ASSERT_EQ(value_bit_xor(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT16);
  ASSERT_EQ(value_uint16(&r), (uint16_t)0xff);

  value_set_int32(&v1, 0xf0);
  value_set_int32(&v2, 0x0f);
  ASSERT_EQ(value_bit_and(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_INT32);
  ASSERT_EQ(value_int32(&r), 0);

  ASSERT_EQ(value_bit_or(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_INT32);
  ASSERT_EQ(value_int32(&r), (int32_t)0xff);

  ASSERT_EQ(value_bit_xor(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_INT32);
  ASSERT_EQ(value_int32(&r), (int32_t)0xff);

  value_set_uint32(&v1, 0xf0);
  value_set_uint32(&v2, 0x0f);
  ASSERT_EQ(value_bit_and(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT32);
  ASSERT_EQ(value_uint32(&r), 0);

  ASSERT_EQ(value_bit_or(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT32);
  ASSERT_EQ(value_uint32(&r), (uint32_t)0xff);

  ASSERT_EQ(value_bit_xor(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT32);
  ASSERT_EQ(value_uint32(&r), (uint32_t)0xff);

  value_set_int64(&v1, 0xf0);
  value_set_int64(&v2, 0x0f);
  ASSERT_EQ(value_bit_and(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_INT64);
  ASSERT_EQ(value_int64(&r), 0);

  ASSERT_EQ(value_bit_or(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_INT64);
  ASSERT_EQ(value_int64(&r), (int64_t)0xff);

  ASSERT_EQ(value_bit_xor(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_INT64);
  ASSERT_EQ(value_int64(&r), (int64_t)0xff);

  value_set_uint64(&v1, 0xf0);
  value_set_uint64(&v2, 0x0f);
  ASSERT_EQ(value_bit_and(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT64);
  ASSERT_EQ(value_uint64(&r), 0);

  ASSERT_EQ(value_bit_or(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT64);
  ASSERT_EQ(value_uint64(&r), (uint64_t)0xff);

  ASSERT_EQ(value_bit_xor(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT64);
  ASSERT_EQ(value_uint64(&r), (uint64_t)0xff);

  value_set_int32(&v1, 0xf0);
  value_set_int64(&v2, 0x0f);
  ASSERT_EQ(value_bit_and(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_INT64);
  ASSERT_EQ(value_int64(&r), 0);

  value_set_uint8(&v1, 0xf0);
  value_set_uint32(&v2, 0x0f);
  ASSERT_EQ(value_bit_and(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_UINT32);
  ASSERT_EQ(value_uint64(&r), 0);

  value_set_bool(&v1, TRUE);
  value_set_bool(&v2, TRUE);
  ASSERT_EQ(value_bit_and(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_BOOL);
  ASSERT_EQ(value_bool(&r), TRUE);

  value_set_bool(&v1, TRUE);
  value_set_bool(&v2, TRUE);
  ASSERT_EQ(value_bit_or(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_BOOL);
  ASSERT_EQ(value_bool(&r), TRUE);

  value_set_bool(&v1, TRUE);
  value_set_bool(&v2, TRUE);
  ASSERT_EQ(value_bit_xor(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_BOOL);
  ASSERT_EQ(value_bool(&r), FALSE);

  value_set_bool(&v1, TRUE);
  value_set_bool(&v2, FALSE);
  ASSERT_EQ(value_bit_and(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_BOOL);
  ASSERT_EQ(value_bool(&r), FALSE);

  value_set_bool(&v1, TRUE);
  value_set_bool(&v2, FALSE);
  ASSERT_EQ(value_bit_or(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_BOOL);
  ASSERT_EQ(value_bool(&r), TRUE);

  value_set_bool(&v1, TRUE);
  value_set_bool(&v2, FALSE);
  ASSERT_EQ(value_bit_xor(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_BOOL);
  ASSERT_EQ(value_bool(&r), TRUE);

  value_set_bool(&v1, FALSE);
  value_set_bool(&v2, FALSE);
  ASSERT_EQ(value_bit_or(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_BOOL);
  ASSERT_EQ(value_bool(&r), FALSE);

  value_set_bool(&v1, FALSE);
  value_set_bool(&v2, FALSE);
  ASSERT_EQ(value_bit_xor(&v1, &v2, &r), RET_OK);
  ASSERT_EQ(r.type, VALUE_TYPE_BOOL);
  ASSERT_EQ(value_bool(&r), FALSE);
}

TEST(value, abs) {
  value_t v;
  value_t o;
  value_set_int(&v, -10);
  ASSERT_EQ(value_abs(&v, &o), RET_OK);
  ASSERT_EQ(o.type == v.type, TRUE);
  ASSERT_EQ(value_int(&o), 10);

  value_set_int8(&v, -10);
  ASSERT_EQ(value_abs(&v, &o), RET_OK);
  ASSERT_EQ(o.type == v.type, TRUE);
  ASSERT_EQ(value_int8(&o), 10);

  value_set_int16(&v, -10);
  ASSERT_EQ(value_abs(&v, &o), RET_OK);
  ASSERT_EQ(o.type == v.type, TRUE);
  ASSERT_EQ(value_int16(&o), 10);

  value_set_int32(&v, -10);
  ASSERT_EQ(value_abs(&v, &o), RET_OK);
  ASSERT_EQ(o.type == v.type, TRUE);
  ASSERT_EQ(value_int32(&o), 10);

  value_set_uint32(&v, 10);
  ASSERT_EQ(value_abs(&v, &o), RET_OK);
  ASSERT_EQ(o.type == v.type, TRUE);
  ASSERT_EQ(value_uint32(&o), 10);

  value_set_int64(&v, -10);
  ASSERT_EQ(value_abs(&v, &o), RET_OK);
  ASSERT_EQ(o.type == v.type, TRUE);
  ASSERT_EQ(value_int64(&o), 10);

  value_set_int64(&v, 10);
  ASSERT_EQ(value_abs(&v, &o), RET_OK);
  ASSERT_EQ(o.type == v.type, TRUE);
  ASSERT_EQ(value_int64(&o), 10);

  value_set_double(&v, 10);
  ASSERT_EQ(value_abs(&v, &o), RET_OK);
  ASSERT_EQ(o.type == v.type, TRUE);
  ASSERT_EQ(value_double(&o), 10);

  value_set_double(&v, -10);
  ASSERT_EQ(value_abs(&v, &o), RET_OK);
  ASSERT_EQ(o.type == v.type, TRUE);
  ASSERT_EQ(value_double(&o), 10);

  value_set_float(&v, 10);
  ASSERT_EQ(value_abs(&v, &o), RET_OK);
  ASSERT_EQ(o.type == v.type, TRUE);
  ASSERT_EQ(value_float(&o), 10);

  value_set_float(&v, -10);
  ASSERT_EQ(value_abs(&v, &o), RET_OK);
  ASSERT_EQ(o.type == v.type, TRUE);
  ASSERT_EQ(value_float(&o), 10);
}

TEST(value, add_int8) {
  value_t v1;
  value_t v2;
  value_t o;
  value_set_int8(&v1, -10);
  value_set_int8(&v2, -8);

  ASSERT_EQ(value_add(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_int8(&o), -18);

  ASSERT_EQ(value_sub(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_int8(&o), -2);

  ASSERT_EQ(value_mul(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_int8(&o), 80);

  ASSERT_EQ(value_div(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_int8(&o), 1);

  value_set_int8(&v1, -120);
  value_set_int8(&v2, -100);

  ASSERT_EQ(value_add(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == VALUE_TYPE_INT16, TRUE);
  ASSERT_EQ(value_int16(&o), -220);

  ASSERT_EQ(value_sub(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_int8(&o), -20);

  ASSERT_EQ(value_mul(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == VALUE_TYPE_INT16, TRUE);
  ASSERT_EQ(value_int16(&o), 120 * 100);

  ASSERT_EQ(value_div(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_int8(&o), 1);
}

TEST(value, add_int16) {
  value_t v1;
  value_t v2;
  value_t o;
  value_set_int16(&v1, -16);
  value_set_int16(&v2, -10);

  ASSERT_EQ(value_add(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_int16(&o), -26);

  ASSERT_EQ(value_sub(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_int16(&o), -6);

  ASSERT_EQ(value_mul(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_int16(&o), 160);

  ASSERT_EQ(value_div(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_int16(&o), 1);

  value_set_int16(&v1, -30000);
  value_set_int16(&v2, -20000);

  ASSERT_EQ(value_add(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == VALUE_TYPE_INT32, TRUE);
  ASSERT_EQ(value_int32(&o), -50000);

  ASSERT_EQ(value_sub(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_int16(&o), -10000);

  ASSERT_EQ(value_mul(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == VALUE_TYPE_INT32, TRUE);
  ASSERT_EQ(value_int32(&o), 30000 * 20000);

  ASSERT_EQ(value_div(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_int16(&o), 1);
}

TEST(value, add_int32) {
  value_t v1;
  value_t v2;
  value_t o;
  value_set_int32(&v1, -42);
  value_set_int32(&v2, -10);

  ASSERT_EQ(value_add(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_int32(&o), -52);

  ASSERT_EQ(value_sub(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_int32(&o), -32);

  ASSERT_EQ(value_mul(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_int32(&o), 420);

  ASSERT_EQ(value_div(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_int32(&o), 4);
}

TEST(value, add_int64) {
  value_t v1;
  value_t v2;
  value_t o;
  value_set_int64(&v1, -42);
  value_set_int64(&v2, -10);

  ASSERT_EQ(value_add(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_int64(&o), -52);

  ASSERT_EQ(value_sub(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_int64(&o), -32);

  ASSERT_EQ(value_mul(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_int64(&o), 420);

  ASSERT_EQ(value_div(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_int64(&o), 4);
}

TEST(value, add_double) {
  value_t v1;
  value_t v2;
  value_t o;
  value_set_double(&v1, -42);
  value_set_double(&v2, -10);

  ASSERT_EQ(value_add(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_double(&o), -52);

  ASSERT_EQ(value_sub(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_double(&o), -32);

  ASSERT_EQ(value_mul(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_double(&o), 420);

  ASSERT_EQ(value_div(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(tk_fequal(value_double(&o), 4.2), TRUE);
  ;
}

TEST(value, mod) {
  value_t v1;
  value_t v2;
  value_t o;
  value_set_int8(&v1, 10);
  value_set_int8(&v2, 6);
  ASSERT_EQ(value_mod(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_int8(&o), 4);

  value_set_uint8(&v1, 10);
  value_set_uint8(&v2, 6);
  ASSERT_EQ(value_mod(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_uint8(&o), 4);

  value_set_int16(&v1, 10);
  value_set_int16(&v2, 6);
  ASSERT_EQ(value_mod(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_int16(&o), 4);

  value_set_uint16(&v1, 10);
  value_set_uint16(&v2, 6);
  ASSERT_EQ(value_mod(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_uint16(&o), 4);

  value_set_int32(&v1, 10);
  value_set_int32(&v2, 6);
  ASSERT_EQ(value_mod(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_int32(&o), 4);

  value_set_uint32(&v1, 10);
  value_set_uint32(&v2, 6);
  ASSERT_EQ(value_mod(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_uint32(&o), 4);

  value_set_int64(&v1, 10);
  value_set_int64(&v2, 6);
  ASSERT_EQ(value_mod(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_int64(&o), 4);

  value_set_uint64(&v1, 10);
  value_set_uint64(&v2, 6);
  ASSERT_EQ(value_mod(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(o.type == v1.type, TRUE);
  ASSERT_EQ(value_uint64(&o), 4);
}

TEST(value, expt) {
  value_t v1;
  value_t v2;
  value_t o;
  value_set_int8(&v1, 10);
  value_set_int8(&v2, 3);
  ASSERT_EQ(value_expt(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(tk_fequal(value_double(&o), 1000), TRUE);
  ASSERT_EQ(o.type == VALUE_TYPE_DOUBLE, TRUE);

  value_set_int8(&v1, 10);
  value_set_int8(&v2, -3);
  ASSERT_EQ(value_expt(&v1, &v2, &o), RET_OK);
  ASSERT_EQ(tk_fequal(value_double(&o), 0.001), TRUE);
  ASSERT_EQ(o.type == VALUE_TYPE_DOUBLE, TRUE);
}

TEST(value, min) {
  int32_t i = 0;
  value_t v[10];
  value_t o;

  for (i = 0; i < ARRAY_SIZE(v); i++) {
    value_set_int8(v + i, i * ((i % 2 == 0) ? -1 : 1));
  }
  ASSERT_EQ(value_min(v, ARRAY_SIZE(v), &o), RET_OK);
  ASSERT_EQ(o.type, VALUE_TYPE_INT8);
  ASSERT_EQ(value_int8(&o), -8);

  for (i = 0; i < ARRAY_SIZE(v); i++) {
    value_set_uint8(v + i, i + 1);
  }
  ASSERT_EQ(value_min(v, ARRAY_SIZE(v), &o), RET_OK);
  ASSERT_EQ(o.type, VALUE_TYPE_UINT8);
  ASSERT_EQ(value_uint8(&o), 1);

  for (i = 0; i < ARRAY_SIZE(v); i++) {
    value_set_int16(v + i, i * ((i % 2 == 0) ? -1 : 1));
  }
  ASSERT_EQ(value_min(v, ARRAY_SIZE(v), &o), RET_OK);
  ASSERT_EQ(o.type, VALUE_TYPE_INT16);
  ASSERT_EQ(value_int16(&o), -8);

  for (i = 0; i < ARRAY_SIZE(v); i++) {
    value_set_uint16(v + i, i + 1);
  }
  ASSERT_EQ(value_min(v, ARRAY_SIZE(v), &o), RET_OK);
  ASSERT_EQ(o.type, VALUE_TYPE_UINT16);
  ASSERT_EQ(value_uint16(&o), 1);

  for (i = 0; i < ARRAY_SIZE(v); i++) {
    value_set_int32(v + i, i * ((i % 2 == 0) ? -1 : 1));
  }
  ASSERT_EQ(value_min(v, ARRAY_SIZE(v), &o), RET_OK);
  ASSERT_EQ(o.type, VALUE_TYPE_INT32);
  ASSERT_EQ(value_int32(&o), -8);

  for (i = 0; i < ARRAY_SIZE(v); i++) {
    value_set_uint32(v + i, i + 1);
  }
  ASSERT_EQ(value_min(v, ARRAY_SIZE(v), &o), RET_OK);
  ASSERT_EQ(o.type, VALUE_TYPE_UINT32);
  ASSERT_EQ(value_uint32(&o), 1);

  for (i = 0; i < ARRAY_SIZE(v); i++) {
    value_set_int64(v + i, i * ((i % 2 == 0) ? -1 : 1));
  }
  ASSERT_EQ(value_min(v, ARRAY_SIZE(v), &o), RET_OK);
  ASSERT_EQ(o.type, VALUE_TYPE_INT64);
  ASSERT_EQ(value_int64(&o), -8);

  for (i = 0; i < ARRAY_SIZE(v); i++) {
    value_set_uint64(v + i, i + 1);
  }
  ASSERT_EQ(value_min(v, ARRAY_SIZE(v), &o), RET_OK);
  ASSERT_EQ(o.type, VALUE_TYPE_UINT64);
  ASSERT_EQ(value_uint64(&o), 1);

  for (i = 0; i < ARRAY_SIZE(v); i++) {
    value_set_double(v + i, i + 1);
  }
  ASSERT_EQ(value_min(v, ARRAY_SIZE(v), &o), RET_OK);
  ASSERT_EQ(o.type, VALUE_TYPE_DOUBLE);
  ASSERT_EQ(value_double(&o), 1);

  const char* str[] = {"a", "b", "c", "0", "A", "B", "C", "1", "2", "3"};
  for (i = 0; i < ARRAY_SIZE(v); i++) {
    value_set_str(v + i, str[i]);
  }
  ASSERT_EQ(value_min(v, ARRAY_SIZE(v), &o), RET_OK);
  ASSERT_EQ(o.type, VALUE_TYPE_STRING);
  ASSERT_STREQ(value_str(&o), "0");
}

TEST(value, max) {
  int32_t i = 0;
  value_t v[10];
  value_t o;

  for (i = 0; i < ARRAY_SIZE(v); i++) {
    value_set_int8(v + i, i * ((i % 2 == 0) ? -1 : 1));
  }
  ASSERT_EQ(value_max(v, ARRAY_SIZE(v), &o), RET_OK);
  ASSERT_EQ(o.type, VALUE_TYPE_INT8);
  ASSERT_EQ(value_int8(&o), 9);

  for (i = 0; i < ARRAY_SIZE(v); i++) {
    value_set_uint8(v + i, i + 1);
  }
  ASSERT_EQ(value_max(v, ARRAY_SIZE(v), &o), RET_OK);
  ASSERT_EQ(o.type, VALUE_TYPE_UINT8);
  ASSERT_EQ(value_uint8(&o), 10);

  for (i = 0; i < ARRAY_SIZE(v); i++) {
    value_set_int16(v + i, i * ((i % 2 == 0) ? -1 : 1));
  }
  ASSERT_EQ(value_max(v, ARRAY_SIZE(v), &o), RET_OK);
  ASSERT_EQ(o.type, VALUE_TYPE_INT16);
  ASSERT_EQ(value_int16(&o), 9);

  for (i = 0; i < ARRAY_SIZE(v); i++) {
    value_set_uint16(v + i, i + 1);
  }
  ASSERT_EQ(value_max(v, ARRAY_SIZE(v), &o), RET_OK);
  ASSERT_EQ(o.type, VALUE_TYPE_UINT16);
  ASSERT_EQ(value_uint16(&o), 10);

  for (i = 0; i < ARRAY_SIZE(v); i++) {
    value_set_int32(v + i, i * ((i % 2 == 0) ? -1 : 1));
  }
  ASSERT_EQ(value_max(v, ARRAY_SIZE(v), &o), RET_OK);
  ASSERT_EQ(o.type, VALUE_TYPE_INT32);
  ASSERT_EQ(value_int32(&o), 9);

  for (i = 0; i < ARRAY_SIZE(v); i++) {
    value_set_uint32(v + i, i + 1);
  }
  ASSERT_EQ(value_max(v, ARRAY_SIZE(v), &o), RET_OK);
  ASSERT_EQ(o.type, VALUE_TYPE_UINT32);
  ASSERT_EQ(value_uint32(&o), 10);

  for (i = 0; i < ARRAY_SIZE(v); i++) {
    value_set_int64(v + i, i * ((i % 2 == 0) ? -1 : 1));
  }
  ASSERT_EQ(value_max(v, ARRAY_SIZE(v), &o), RET_OK);
  ASSERT_EQ(o.type, VALUE_TYPE_INT64);
  ASSERT_EQ(value_int64(&o), 9);

  for (i = 0; i < ARRAY_SIZE(v); i++) {
    value_set_uint64(v + i, i + 1);
  }
  ASSERT_EQ(value_max(v, ARRAY_SIZE(v), &o), RET_OK);
  ASSERT_EQ(o.type, VALUE_TYPE_UINT64);
  ASSERT_EQ(value_uint64(&o), 10);

  for (i = 0; i < ARRAY_SIZE(v); i++) {
    value_set_double(v + i, i + 1);
  }
  ASSERT_EQ(value_max(v, ARRAY_SIZE(v), &o), RET_OK);
  ASSERT_EQ(o.type, VALUE_TYPE_DOUBLE);
  ASSERT_EQ(value_double(&o), 10);

  const char* str[] = {"a", "b", "c", "0", "A", "B", "C", "1", "2", "3"};
  for (i = 0; i < ARRAY_SIZE(v); i++) {
    value_set_str(v + i, str[i]);
  }
  ASSERT_EQ(value_max(v, ARRAY_SIZE(v), &o), RET_OK);
  ASSERT_EQ(o.type, VALUE_TYPE_STRING);
  ASSERT_STREQ(value_str(&o), "c");
}

TEST(ValueTest, dup_wstr) {
  value_t v;
  const wchar_t* wstr = L"hello";

  ASSERT_EQ(value_dup_wstr(&v, wstr), &v);
  ASSERT_EQ(value_wstr(&v) != wstr, true);
  ASSERT_EQ(tk_wstr_eq(value_wstr(&v), wstr) == TRUE, true);
  ASSERT_EQ(v.free_handle == TRUE, true);

  value_reset(&v);

  ASSERT_EQ(v.free_handle == FALSE, true);
  ASSERT_EQ(v.value.wstr == NULL, true);
}
