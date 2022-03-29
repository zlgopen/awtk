#include "tkc/utils.h"
#include "tkc/object_date_time.h"
#include "tkc/object_default.h"
#include "gtest/gtest.h"
#include <stdlib.h>
#include <string>

using std::string;

static ret_t visit_dump(void* ctx, const void* data) {
  char buff[32];
  string& str = *(string*)ctx;
  const named_value_t* nv = (named_value_t*)data;

  str += nv->name + string(":") + value_str_ex(&(nv->value), buff, sizeof(buff) - 1);

  return RET_OK;
}

TEST(ObjectDateTime, basic) {
  string log;
  tk_object_t* obj = obj = object_date_time_create();

  ASSERT_EQ(tk_object_set_prop_uint32(obj, OBJECT_DATE_TIME_PROP_YEAR, 3000), RET_OK);
  ASSERT_EQ(tk_object_get_prop_uint32(obj, OBJECT_DATE_TIME_PROP_YEAR, 0), 3000);

  ASSERT_EQ(tk_object_set_prop_uint32(obj, OBJECT_DATE_TIME_PROP_MONTH, 3), RET_OK);
  ASSERT_EQ(tk_object_get_prop_uint32(obj, OBJECT_DATE_TIME_PROP_MONTH, 0), 3);

  ASSERT_EQ(tk_object_set_prop_uint32(obj, OBJECT_DATE_TIME_PROP_DAY, 4), RET_OK);
  ASSERT_EQ(tk_object_get_prop_uint32(obj, OBJECT_DATE_TIME_PROP_DAY, 0), 4);

  ASSERT_EQ(tk_object_set_prop_uint32(obj, OBJECT_DATE_TIME_PROP_HOUR, 5), RET_OK);
  ASSERT_EQ(tk_object_get_prop_uint32(obj, OBJECT_DATE_TIME_PROP_HOUR, 0), 5);

  ASSERT_EQ(tk_object_set_prop_uint32(obj, OBJECT_DATE_TIME_PROP_MINUTE, 6), RET_OK);
  ASSERT_EQ(tk_object_get_prop_uint32(obj, OBJECT_DATE_TIME_PROP_MINUTE, 0), 6);

  ASSERT_EQ(tk_object_set_prop_uint32(obj, OBJECT_DATE_TIME_PROP_SECOND, 7), RET_OK);
  ASSERT_EQ(tk_object_get_prop_uint32(obj, OBJECT_DATE_TIME_PROP_SECOND, 0), 7);

  ASSERT_EQ(tk_object_set_prop_uint32(obj, OBJECT_DATE_TIME_PROP_WDAY, 1), RET_OK);
  ASSERT_EQ(tk_object_get_prop_uint32(obj, OBJECT_DATE_TIME_PROP_WDAY, 0), 1);

  log = "";
  tk_object_foreach_prop(obj, visit_dump, &log);
  ASSERT_EQ(log, "year:3000month:3minute:6day:4hour:5second:7wday:1");

  TK_OBJECT_UNREF(obj);
}
