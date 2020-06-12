#include "tkc/utils.h"
#include "tkc/object_locker.h"
#include "tkc/object_default.h"
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
  const named_value_t* nv = (named_value_t*)data;

  str += nv->name;

  return RET_OK;
}

TEST(ObjectLocker, events) {
  value_t v;
  string log;
  object_t* obj = object_default_create();
  object_default_t* o = OBJECT_DEFAULT(obj);

  obj = object_locker_create(obj);
  emitter_on(EMITTER(obj), EVT_PROP_WILL_CHANGE, event_dump, &log);
  emitter_on(EMITTER(obj), EVT_PROP_CHANGED, event_dump, &log);
  emitter_on(EMITTER(obj), EVT_DESTROY, event_dump, &log);

  ASSERT_EQ(object_set_prop(obj, "6", value_set_int(&v, 50)), RET_OK);
  ASSERT_EQ(object_set_prop(obj, "8", value_set_int(&v, 50)), RET_OK);

  OBJECT_UNREF(o);
  OBJECT_UNREF(obj);

  ASSERT_EQ(log, "6:6:8:8:destroy:");
}

TEST(ObjectLocker, basic) {
  value_t v;
  string log;
  object_t* obj = object_default_create();
  object_default_t* o = OBJECT_DEFAULT(obj);

  obj = object_locker_create(obj);

  ASSERT_EQ(object_set_prop(obj, "5", value_set_int(&v, 50)), RET_OK);
  ASSERT_EQ(object_get_prop(obj, "5", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 50);

  ASSERT_EQ(object_set_prop(obj, "6", value_set_int(&v, 60)), RET_OK);
  ASSERT_EQ(object_get_prop(obj, "6", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 60);

  log = "";
  object_foreach_prop(obj, visit_dump, &log);
  ASSERT_EQ(log, "56");

  ASSERT_EQ(object_remove_prop(obj, "5"), RET_OK);
  ASSERT_EQ(object_get_prop(obj, "3", &v), RET_NOT_FOUND);
  ASSERT_EQ(object_remove_prop(obj, "3"), RET_NOT_FOUND);

  OBJECT_UNREF(o);
  OBJECT_UNREF(obj);
}
