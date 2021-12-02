#include "tkc/fscript.h"
#include "tkc/object_default.h"
#include "gtest/gtest.h"

TEST(FExr, cksum) {
  value_t v;
  tk_object_t* obj = object_default_create();

  fscript_eval(obj, "cksum(\"1\")", &v);
  ASSERT_EQ(value_uint32(&v), 65486u);
  value_reset(&v);

  fscript_eval(obj, "cksum(\"123\")", &v);
  ASSERT_EQ(value_uint32(&v), 65385u);
  value_reset(&v);

  TK_OBJECT_UNREF(obj);
}

TEST(FExr, crc16) {
  value_t v;
  tk_object_t* obj = object_default_create();

  fscript_eval(obj, "crc16(\"1\")", &v);
  ASSERT_EQ(value_uint32(&v), 53362u);
  value_reset(&v);

  fscript_eval(obj, "crc16(\"123\")", &v);
  ASSERT_EQ(value_uint32(&v), 40116u);
  value_reset(&v);

  TK_OBJECT_UNREF(obj);
}

TEST(FExr, crc32) {
  value_t v;
  tk_object_t* obj = object_default_create();

  fscript_eval(obj, "crc32(\"1\")", &v);
  ASSERT_EQ(value_uint32(&v), 2212294583u);
  value_reset(&v);

  fscript_eval(obj, "crc32(\"123\")", &v);
  ASSERT_EQ(value_uint32(&v), 2286445522u);
  value_reset(&v);

  TK_OBJECT_UNREF(obj);
}
