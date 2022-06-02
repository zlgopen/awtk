#include "tkc/fscript.h"
#include "tkc/object_default.h"
#include "gtest/gtest.h"

TEST(FExr, json_load1) {
  value_t v;
  tk_object_t* obj = object_default_create();

  fscript_eval(obj, "a=json_load('{\"value\":123}');object_get(a, 'value')", &v);
  ASSERT_EQ(value_uint32(&v), 123);
  value_reset(&v);

  TK_OBJECT_UNREF(obj);
}

TEST(FExr, json_load2) {
  value_t v;
  tk_object_t* obj = object_default_create();

  fscript_eval(obj, "a=json_load('{\"value\":123}', 14);object_get(a, 'value')", &v);
  ASSERT_EQ(value_uint32(&v), 123);
  value_reset(&v);

  TK_OBJECT_UNREF(obj);
}

TEST(FExr, json_save) {
  value_t v;
  tk_object_t* obj = object_default_create();

  fscript_eval(obj, "a=json_load('{\"value\":123}');b=json_save_to_string(a);b", &v);
  ASSERT_STREQ(value_str(&v), "{\n    \"value\" : 123\n}");
  value_reset(&v);

  TK_OBJECT_UNREF(obj);
}
