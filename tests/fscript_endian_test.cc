#include "tkc/fscript.h"
#include "tkc/object_default.h"
#include "gtest/gtest.h"

TEST(FExr, is_little) {
  value_t v;
  object_t* obj = object_default_create();

  fscript_eval(obj, "is_little()", &v);
  ASSERT_EQ(value_bool(&v), TRUE);
  value_reset(&v);

  OBJECT_UNREF(obj);
}

TEST(FExr, endian_htons) {
  value_t v;
  object_t* obj = object_default_create();

  fscript_eval(obj, "htons(0x1122)", &v);
  ASSERT_EQ(value_int(&v), 0x2211);
  value_reset(&v);

  OBJECT_UNREF(obj);
}

TEST(FExr, endian_htonl) {
  value_t v;
  object_t* obj = object_default_create();

  fscript_eval(obj, "htonl(0x11223344)", &v);
  ASSERT_EQ(value_uint32(&v), 0x44332211);
  value_reset(&v);

  OBJECT_UNREF(obj);
}

TEST(FExr, endian_htonf) {
  value_t v;
  object_t* obj = object_default_create();

  fscript_eval(obj, "ntohf(htonf(100))", &v);
  ASSERT_EQ(value_int(&v), 100);
  value_reset(&v);

  OBJECT_UNREF(obj);
}

TEST(FExr, endian_ntohs) {
  value_t v;
  object_t* obj = object_default_create();

  fscript_eval(obj, "ntohs(0x1122)", &v);
  ASSERT_EQ(value_int(&v), 0x2211);
  value_reset(&v);

  OBJECT_UNREF(obj);
}

TEST(FExr, endian_ntohl) {
  value_t v;
  object_t* obj = object_default_create();

  fscript_eval(obj, "ntohl(0x11223344)", &v);
  ASSERT_EQ(value_uint32(&v), 0x44332211);
  value_reset(&v);

  OBJECT_UNREF(obj);
}

TEST(FExr, endian_ntohll) {
  value_t v;
  object_t* obj = object_default_create();

  fscript_eval(obj, "ntohll(0x1122334455667788)", &v);
  ASSERT_EQ(value_uint64(&v), 0x8877665544332211);
  value_reset(&v);

  OBJECT_UNREF(obj);
}
