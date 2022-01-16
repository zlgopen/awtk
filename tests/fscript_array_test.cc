#include "tkc/fscript.h"
#include "tkc/object_default.h"
#include "gtest/gtest.h"

TEST(FScriptArray, size) {
  value_t v;
  tk_object_t* obj = object_default_create();

  fscript_eval(obj, "a=array_create();array_push(a, 1);array_push(a, 2);array_size(a)", &v);
  ASSERT_EQ(value_uint32(&v), 2);
  value_reset(&v);

  fscript_eval(obj, "a=array_create();array_is_empty(a)", &v);
  ASSERT_EQ(value_bool(&v), TRUE);
  value_reset(&v);

  fscript_eval(obj, "a=array_create();array_push(a, 1);array_push(a, 2);array_is_empty(a)", &v);
  ASSERT_EQ(value_bool(&v), FALSE);
  value_reset(&v);

  TK_OBJECT_UNREF(obj);
}
