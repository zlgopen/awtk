#include "tkc/utils.h"
#include "tkc/object_array.h"
#include "gtest/gtest.h"
#include <stdlib.h>
#include <string>

using std::string;

static ret_t event_dump(void* ctx, event_t* e) {
  string& str = *(string*)ctx;

  if (e->type == EVT_PROP_WILL_CHANGE || e->type == EVT_PROP_CHANGED) {
    prop_change_event_t* evt = (prop_change_event_t*)e;
    str += evt->name;
    str += ":";
  } else if (e->type == EVT_DESTROY) {
    str += "destroy:";
  }

  return RET_OK;
}

static ret_t visit_dump(void* ctx, const void* data) {
  string& str = *(string*)ctx;
  const value_t* nv = (value_t*)data;
  if (nv->type == VALUE_TYPE_STRING) {
    str += value_str(nv);
  } else {
    char num[32];
    tk_snprintf(num, sizeof(num), "%d", value_int(nv));
    str += num;
  }

  return RET_OK;
}

TEST(ObjectArray, events) {
  value_t v;
  string log;
  object_t* obj = object_array_create();
  object_array_t* o = OBJECT_ARRAY(obj);

  emitter_on((emitter_t*)o, EVT_PROP_WILL_CHANGE, event_dump, &log);
  emitter_on((emitter_t*)o, EVT_PROP_CHANGED, event_dump, &log);
  emitter_on((emitter_t*)o, EVT_DESTROY, event_dump, &log);

  ASSERT_EQ(object_set_prop(obj, "-1", value_set_int(&v, 50)), RET_OK);
  ASSERT_EQ(object_set_prop(obj, "-1", value_set_int(&v, 50)), RET_OK);

  object_unref(obj);

  ASSERT_EQ(log, "-1:-1:-1:-1:destroy:");
}

TEST(ObjectArray, basic) {
  value_t v;
  string log;
  object_t* obj = object_array_create();
  object_array_t* o = OBJECT_ARRAY(obj);

  ASSERT_EQ(o->props_size, 0);

  ASSERT_EQ(object_set_prop(obj, "-1", value_set_int(&v, 0)), RET_OK);
  ASSERT_EQ(o->props_size, 1);
  ASSERT_EQ(object_get_prop(obj, "0", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 0);

  ASSERT_EQ(object_set_prop(obj, "-1", value_set_int(&v, 1)), RET_OK);
  ASSERT_EQ(o->props_size, 2);
  ASSERT_EQ(object_get_prop(obj, "1", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(object_set_prop(obj, "-1", value_set_int(&v, 2)), RET_OK);
  ASSERT_EQ(o->props_size, 3);
  ASSERT_EQ(object_get_prop(obj, "2", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);

  ASSERT_EQ(object_set_prop(obj, "-1", value_set_int(&v, 3)), RET_OK);
  ASSERT_EQ(o->props_size, 4);
  ASSERT_EQ(object_get_prop(obj, "3", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 3);

  ASSERT_EQ(object_set_prop(obj, "3", value_set_int(&v, 4)), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "size", -1), o->props_size);
  ASSERT_EQ(o->props_size, 4);
  ASSERT_EQ(object_get_prop(obj, "3", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 4);

  ASSERT_EQ(object_set_prop(obj, "2", value_set_int(&v, 5)), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "size", -1), o->props_size);
  ASSERT_EQ(o->props_size, 4);
  ASSERT_EQ(object_get_prop(obj, "2", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 5);

  ASSERT_NE(object_set_prop(obj, "9", value_set_int(&v, 90)), RET_OK);
  log = "";
  object_foreach_prop(obj, visit_dump, &log);
  ASSERT_EQ(log, "0154");

  ASSERT_EQ(object_remove_prop(obj, "3"), RET_OK);
  ASSERT_EQ(o->props_size, 3);
  ASSERT_EQ(object_get_prop(obj, "3", &v), RET_NOT_FOUND);
  ASSERT_EQ(object_remove_prop(obj, "3"), RET_NOT_FOUND);

  log = "";
  object_foreach_prop(obj, visit_dump, &log);
  ASSERT_EQ(log, "015");

  ASSERT_EQ(object_remove_prop(obj, "0"), RET_OK);
  ASSERT_EQ(o->props_size, 2);

  log = "";
  object_foreach_prop(obj, visit_dump, &log);
  ASSERT_EQ(log, "15");
  object_unref(obj);
}

TEST(ObjectArray, dup) {
  value_t v;
  string log;
  object_t* dup = NULL;
  object_t* obj = object_array_create();
  ASSERT_EQ(object_set_prop(obj, "-1", value_set_int(&v, 0)), RET_OK);
  ASSERT_EQ(object_set_prop(obj, "-1", value_set_int(&v, 1)), RET_OK);
  ASSERT_EQ(object_set_prop(obj, "-1", value_set_int(&v, 2)), RET_OK);
  ASSERT_EQ(object_set_prop(obj, "-1", value_set_int(&v, 3)), RET_OK);

  dup = object_array_clone(OBJECT_ARRAY(obj));
  
  log = "";
  object_foreach_prop(dup, visit_dump, &log);
  ASSERT_EQ(log, "0123");

  object_unref(obj);
  object_unref(dup);
}

