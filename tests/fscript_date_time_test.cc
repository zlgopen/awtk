#include "tkc/fscript.h"
#include "tkc/object_default.h"
#include "gtest/gtest.h"

TEST(FExpr, date_time_format1) {
  value_t v;
  tk_object_t* obj = object_default_create();

  fscript_eval(obj, "date_time_format(0, \"Y\")", &v);
  ASSERT_STREQ(value_str(&v), "1970");
  value_reset(&v);

  fscript_eval(obj, "date_time_format(0, \"Y-M-D\")", &v);
  ASSERT_STREQ(value_str(&v), "1970-1-1");
  value_reset(&v);

  fscript_eval(obj, "date_time_format(0, \"Y-M-D h:m:s\")", &v);
  ASSERT_STREQ(value_str(&v), "1970-1-1 0:0:0");
  value_reset(&v);

  TK_OBJECT_UNREF(obj);
}

TEST(FExpr, date_time_format2) {
  value_t v;
  tk_object_t* obj = object_default_create();

  fscript_eval(
      obj, "var dt = date_time_create(); date_time_from_time(dt, 61); date_time_format(dt, \"Y\")",
      &v);
  ASSERT_STREQ(value_str(&v), "1970");
  value_reset(&v);

  fscript_eval(
      obj,
      "var dt = date_time_create(); date_time_from_time(dt, 61); date_time_format(dt, \"Y-M-D\")",
      &v);
  ASSERT_STREQ(value_str(&v), "1970-1-1");
  value_reset(&v);

  fscript_eval(obj,
               "var dt = date_time_create(); date_time_from_time(dt, 61); date_time_format(dt,  "
               "\"Y-M-D h:m:s\")",
               &v);
  ASSERT_STREQ(value_str(&v), "1970-1-1 0:1:1");
  value_reset(&v);

  TK_OBJECT_UNREF(obj);
}
