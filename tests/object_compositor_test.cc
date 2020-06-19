#include "tkc/utils.h"
#include "tkc/object_compositor.h"
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

TEST(ObjectComposite, events) {
  value_t v;
  string log;
  object_t* obj1 = object_default_create();
  object_t* obj2 = object_default_create();
  object_t* obj = object_compositor_create(obj1, obj2);

  emitter_on(EMITTER(obj), EVT_PROP_WILL_CHANGE, event_dump, &log);
  emitter_on(EMITTER(obj), EVT_PROP_CHANGED, event_dump, &log);
  emitter_on(EMITTER(obj), EVT_DESTROY, event_dump, &log);

  ASSERT_EQ(object_set_prop(obj, "6", value_set_int(&v, 50)), RET_OK);
  ASSERT_EQ(object_set_prop(obj, "8", value_set_int(&v, 50)), RET_OK);

  OBJECT_UNREF(obj);
  OBJECT_UNREF(obj1);
  OBJECT_UNREF(obj2);

  ASSERT_EQ(log, "6:6:8:8:destroy:");
}

TEST(ObjectComposite, basic) {
  value_t v;
  string log;
  object_t* obj1 = object_default_create();
  object_t* obj2 = object_default_create();
  object_t* obj = object_compositor_create(obj1, obj2);

  ASSERT_EQ(object_set_prop(obj, "5", value_set_int(&v, 50)), RET_OK);
  ASSERT_EQ(object_get_prop(obj, "5", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 50);
  ASSERT_EQ(object_get_prop(obj1, "5", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 50);
  ASSERT_NE(object_get_prop(obj2, "5", &v), RET_OK);

  ASSERT_EQ(object_set_prop(obj, "6", value_set_int(&v, 60)), RET_OK);
  ASSERT_EQ(object_get_prop(obj, "6", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 60);

  log = "";
  object_foreach_prop(obj, visit_dump, &log);
  ASSERT_EQ(log, "56");

  ASSERT_EQ(object_set_prop(obj2, "7", value_set_int(&v, 70)), RET_OK);
  ASSERT_EQ(object_get_prop(obj, "7", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 70);
  ASSERT_NE(object_get_prop(obj1, "7", &v), RET_OK);

  ASSERT_EQ(object_remove_prop(obj, "5"), RET_OK);
  ASSERT_EQ(object_remove_prop(obj, "7"), RET_OK);
  ASSERT_NE(object_remove_prop(obj2, "7"), RET_OK);

  ASSERT_EQ(object_get_prop(obj, "3", &v), RET_NOT_FOUND);
  ASSERT_EQ(object_remove_prop(obj, "3"), RET_NOT_FOUND);

  OBJECT_UNREF(obj);
  OBJECT_UNREF(obj1);
  OBJECT_UNREF(obj2);
}
