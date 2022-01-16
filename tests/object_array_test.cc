#include "tkc/utils.h"
#include "tkc/object_default.h"
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
  const named_value_t* nv = (named_value_t*)data;
  const value_t* v = &(nv->value);

  if (v->type == VALUE_TYPE_STRING) {
    str += value_str(v);
  } else {
    char num[32];
    tk_snprintf(num, sizeof(num), "%d", value_int(v));
    str += num;
  }

  return RET_OK;
}

TEST(ObjectArray, events) {
  value_t v;
  string log;
  tk_object_t* obj = object_array_create();
  object_array_t* o = OBJECT_ARRAY(obj);

  emitter_on((emitter_t*)o, EVT_PROP_WILL_CHANGE, event_dump, &log);
  emitter_on((emitter_t*)o, EVT_PROP_CHANGED, event_dump, &log);
  emitter_on((emitter_t*)o, EVT_DESTROY, event_dump, &log);

  ASSERT_EQ(tk_object_set_prop(obj, "-1", value_set_int(&v, 50)), RET_OK);
  ASSERT_EQ(tk_object_set_prop(obj, "-1", value_set_int(&v, 50)), RET_OK);

  TK_OBJECT_UNREF(obj);

  ASSERT_EQ(log, "-1:-1:-1:-1:destroy:");
}

TEST(ObjectArray, basic) {
  value_t v;
  string log;
  tk_object_t* obj = object_array_create();
  object_array_t* o = OBJECT_ARRAY(obj);

  ASSERT_EQ(o->size, 0u);

  ASSERT_EQ(tk_object_set_prop(obj, "-1", value_set_int(&v, 0)), RET_OK);
  ASSERT_EQ(o->size, 1u);
  ASSERT_EQ(tk_object_get_prop(obj, "0", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 0);
  ASSERT_EQ(tk_object_get_prop(obj, "[0]", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 0);

  ASSERT_EQ(tk_object_set_prop(obj, "-1", value_set_int(&v, 1)), RET_OK);
  ASSERT_EQ(o->size, 2u);
  ASSERT_EQ(tk_object_get_prop(obj, "1", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);
  ASSERT_EQ(tk_object_get_prop(obj, "[1]", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(tk_object_set_prop(obj, "-1", value_set_int(&v, 2)), RET_OK);
  ASSERT_EQ(o->size, 3u);
  ASSERT_EQ(tk_object_get_prop(obj, "2", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);
  ASSERT_EQ(tk_object_get_prop(obj, "[2]", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);

  ASSERT_EQ(tk_object_set_prop(obj, "-1", value_set_int(&v, 3)), RET_OK);
  ASSERT_EQ(o->size, 4u);
  ASSERT_EQ(tk_object_get_prop(obj, "3", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 3);
  ASSERT_EQ(tk_object_get_prop(obj, "[3]", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 3);

  ASSERT_EQ(tk_object_set_prop(obj, "3", value_set_int(&v, 4)), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(obj, "size", -1), (int32_t)o->size);
  ASSERT_EQ(tk_object_get_prop_int(obj, "#size", -1), (int32_t)o->size);
  ASSERT_EQ(o->size, 4u);
  ASSERT_EQ(tk_object_get_prop(obj, "3", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 4);
  ASSERT_EQ(tk_object_get_prop(obj, "[3]", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 4);

  ASSERT_EQ(tk_object_set_prop(obj, "2", value_set_int(&v, 5)), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(obj, "size", -1), (int32_t)o->size);
  ASSERT_EQ(tk_object_get_prop_int(obj, "#size", -1), (int32_t)o->size);
  ASSERT_EQ(o->size, 4u);
  ASSERT_EQ(tk_object_get_prop(obj, "2", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 5);
  ASSERT_EQ(tk_object_get_prop(obj, "[2]", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 5);

  ASSERT_NE(tk_object_set_prop(obj, "9", value_set_int(&v, 90)), RET_OK);
  log = "";
  tk_object_foreach_prop(obj, visit_dump, &log);
  ASSERT_EQ(log, "0154");

  ASSERT_EQ(tk_object_remove_prop(obj, "3"), RET_OK);
  ASSERT_EQ(o->size, 3u);
  ASSERT_EQ(tk_object_get_prop(obj, "3", &v), RET_NOT_FOUND);
  ASSERT_EQ(tk_object_remove_prop(obj, "3"), RET_NOT_FOUND);

  ASSERT_EQ(tk_object_set_prop(obj, "-1", value_set_int(&v, 3)), RET_OK);
  ASSERT_EQ(o->size, 4u);
  ASSERT_EQ(tk_object_remove_prop(obj, "[3]"), RET_OK);
  ASSERT_EQ(o->size, 3u);
  ASSERT_EQ(tk_object_get_prop(obj, "[3]", &v), RET_NOT_FOUND);
  ASSERT_EQ(tk_object_remove_prop(obj, "[3]"), RET_NOT_FOUND);

  log = "";
  tk_object_foreach_prop(obj, visit_dump, &log);
  ASSERT_EQ(log, "015");

  ASSERT_EQ(tk_object_remove_prop(obj, "0"), RET_OK);
  ASSERT_EQ(o->size, 2u);

  log = "";
  tk_object_foreach_prop(obj, visit_dump, &log);
  ASSERT_EQ(log, "15");
  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, clone) {
  value_t v;
  string log;
  tk_object_t* dup = NULL;
  tk_object_t* obj = object_array_create();
  ASSERT_EQ(tk_object_set_prop(obj, "-1", value_set_int(&v, 0)), RET_OK);
  ASSERT_EQ(tk_object_set_prop(obj, "-1", value_set_int(&v, 1)), RET_OK);
  ASSERT_EQ(tk_object_set_prop(obj, "-1", value_set_int(&v, 2)), RET_OK);
  ASSERT_EQ(tk_object_set_prop(obj, "-1", value_set_int(&v, 3)), RET_OK);

  dup = object_array_clone(obj);

  log = "";
  tk_object_foreach_prop(dup, visit_dump, &log);
  ASSERT_EQ(log, "0123");

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(dup);
}

TEST(ObjectArray, path) {
  value_t v;
  tk_object_t* root = object_default_create();
  tk_object_t* obja = object_array_create();
  tk_object_t* obja1 = object_default_create();
  tk_object_t* obja2 = object_default_create();
  tk_object_t* objb = object_array_create();
  tk_object_t* objb1 = object_default_create();
  tk_object_t* objb2 = object_default_create();

  ASSERT_EQ(tk_object_set_prop_object(root, "a", obja), RET_OK);
  ASSERT_EQ(tk_object_set_prop_object(root, "b", objb), RET_OK);

  ASSERT_EQ(tk_object_set_prop_object(obja, "-1", obja1), RET_OK);
  ASSERT_EQ(tk_object_set_prop_int(obja1, "value", 456), RET_OK);
  ASSERT_EQ(tk_object_get_prop_by_path(root, "a.0.value", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 456);

  ASSERT_EQ(tk_object_set_prop_object(obja, "-1", obja2), RET_OK);
  ASSERT_EQ(tk_object_set_prop_int(obja2, "value", 56), RET_OK);
  ASSERT_EQ(tk_object_get_prop_by_path(root, "a.1.value", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 56);

  ASSERT_EQ(tk_object_set_prop_object(objb, "-1", objb1), RET_OK);
  ASSERT_EQ(tk_object_set_prop_int(objb1, "value", 56), RET_OK);
  ASSERT_EQ(tk_object_get_prop_by_path(root, "b.0.value", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 56);

  ASSERT_EQ(tk_object_set_prop_object(objb, "-1", objb2), RET_OK);
  ASSERT_EQ(tk_object_set_prop_int(objb2, "value", 156), RET_OK);
  ASSERT_EQ(tk_object_get_prop_by_path(root, "b.1.value", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 156);

  ASSERT_NE(tk_object_get_prop_by_path(root, "a.a.value", &v), RET_OK);
  ASSERT_NE(tk_object_get_prop_by_path(root, "b.a.value", &v), RET_OK);
  ASSERT_NE(tk_object_get_prop_by_path(root, "c.value", &v), RET_OK);

  TK_OBJECT_UNREF(root);
  TK_OBJECT_UNREF(obja);
  TK_OBJECT_UNREF(obja1);
  TK_OBJECT_UNREF(obja2);
  TK_OBJECT_UNREF(objb);
  TK_OBJECT_UNREF(objb1);
  TK_OBJECT_UNREF(objb2);
}

TEST(ObjectArray, insert) {
  value_t v;
  tk_object_t* obj = object_array_create();
  object_array_t* o = OBJECT_ARRAY(obj);

  ASSERT_EQ(object_array_insert(obj, 0, value_set_str(&v, "c")), RET_OK);
  ASSERT_STREQ(tk_object_get_prop_str(obj, "0"), "c");
  ASSERT_EQ(o->size, 1u);

  ASSERT_EQ(object_array_insert(obj, 1, value_set_str(&v, "d")), RET_OK);
  ASSERT_STREQ(tk_object_get_prop_str(obj, "1"), "d");
  ASSERT_EQ(o->size, 2u);

  ASSERT_EQ(object_array_insert(obj, 2, value_set_str(&v, "e")), RET_OK);
  ASSERT_STREQ(tk_object_get_prop_str(obj, "2"), "e");
  ASSERT_EQ(o->size, 3u);

  ASSERT_EQ(object_array_insert(obj, 0, value_set_str(&v, "b")), RET_OK);
  ASSERT_STREQ(tk_object_get_prop_str(obj, "0"), "b");
  ASSERT_EQ(o->size, 4u);

  ASSERT_EQ(object_array_insert(obj, 0, value_set_str(&v, "a")), RET_OK);
  ASSERT_STREQ(tk_object_get_prop_str(obj, "0"), "a");
  ASSERT_EQ(o->size, 5u);

  ASSERT_EQ(object_array_push(obj, value_set_str(&v, "hello")), RET_OK);
  ASSERT_STREQ(tk_object_get_prop_str(obj, "5"), "hello");
  ASSERT_EQ(o->size, 6u);

  ASSERT_EQ(object_array_pop(obj, &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "hello");
  value_reset(&v);

  ASSERT_EQ(object_array_pop(obj, &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "e");
  value_reset(&v);

  ASSERT_EQ(object_array_remove(obj, 0), RET_OK);
  ASSERT_STREQ(tk_object_get_prop_str(obj, "0"), "b");

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, index_of) {
  value_t v;
  tk_object_t* obj = object_array_create();

  value_set_int(&v, 10);
  ASSERT_EQ(object_array_index_of(obj, &v), -1);

  value_set_int(&v, 10);
  ASSERT_EQ(object_array_last_index_of(obj, &v), -1);

  value_set_int(&v, 10);
  object_array_push(obj, &v);
  ASSERT_EQ(object_array_index_of(obj, &v), 0);
  ASSERT_EQ(object_array_last_index_of(obj, &v), 0);

  value_set_int(&v, 20);
  object_array_push(obj, &v);
  value_set_int(&v, 30);
  object_array_push(obj, &v);

  value_set_int(&v, 30);
  ASSERT_EQ(object_array_index_of(obj, &v), 2);

  value_set_int(&v, 10);
  ASSERT_EQ(object_array_index_of(obj, &v), 0);

  value_set_int(&v, 50);
  ASSERT_EQ(object_array_index_of(obj, &v), -1);

  value_set_int(&v, 10);
  object_array_push(obj, &v);
  value_set_int(&v, 20);
  object_array_push(obj, &v);
  value_set_int(&v, 30);
  object_array_push(obj, &v);

  value_set_int(&v, 10);
  ASSERT_EQ(object_array_last_index_of(obj, &v), 3);

  value_set_int(&v, 20);
  ASSERT_EQ(object_array_last_index_of(obj, &v), 4);

  value_set_int(&v, 30);
  ASSERT_EQ(object_array_last_index_of(obj, &v), 5);

  value_set_int(&v, 300);
  ASSERT_EQ(object_array_last_index_of(obj, &v), -1);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, shift) {
  value_t v;
  tk_object_t* obj = object_array_create();

  value_set_int(&v, 10);
  object_array_push(obj, &v);
  value_set_int(&v, 20);
  object_array_push(obj, &v);
  value_set_int(&v, 30);
  object_array_push(obj, &v);

  ASSERT_EQ(object_array_shift(obj, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 10);

  ASSERT_EQ(object_array_shift(obj, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 20);

  ASSERT_EQ(object_array_shift(obj, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 30);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, get_and_remove) {
  value_t v;
  tk_object_t* obj = object_array_create();

  value_set_int(&v, 10);
  object_array_push(obj, &v);
  value_set_int(&v, 20);
  object_array_push(obj, &v);
  value_set_int(&v, 30);
  object_array_push(obj, &v);

  ASSERT_EQ(object_array_get_and_remove(obj, 1, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 20);

  ASSERT_EQ(object_array_get_and_remove(obj, 1, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 30);

  ASSERT_EQ(object_array_get_and_remove(obj, 0, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 10);

  ASSERT_NE(object_array_get_and_remove(obj, 0, &v), RET_OK);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, create_with_str1) {
  value_t v;
  tk_object_t* obj = object_array_create_with_str("1,2,3,4", ",", VALUE_TYPE_INT32);
  object_array_t* o = OBJECT_ARRAY(obj);

  ASSERT_EQ(o->size, 4u);
  ASSERT_EQ(object_array_get(obj, 0, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(object_array_get(obj, 1, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);

  ASSERT_EQ(object_array_get(obj, 2, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 3);

  ASSERT_EQ(object_array_get(obj, 3, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 4);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, create_with_str2) {
  value_t v;
  tk_object_t* obj = object_array_create_with_str("1, 2, 3, 4, ", ", ", VALUE_TYPE_INT32);
  object_array_t* o = OBJECT_ARRAY(obj);

  ASSERT_EQ(o->size, 4u);
  ASSERT_EQ(object_array_get(obj, 0, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(object_array_get(obj, 1, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);

  ASSERT_EQ(object_array_get(obj, 2, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 3);

  ASSERT_EQ(object_array_get(obj, 3, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 4);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, create_with_str3) {
  value_t v;
  tk_object_t* obj = object_array_create_with_str("1", ", ", VALUE_TYPE_INT32);
  object_array_t* o = OBJECT_ARRAY(obj);

  ASSERT_EQ(o->size, 1u);
  ASSERT_EQ(object_array_get(obj, 0, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, create_with_str4) {
  value_t v;
  tk_object_t* obj = object_array_create_with_str("1, ", ", ", VALUE_TYPE_INT32);
  object_array_t* o = OBJECT_ARRAY(obj);

  ASSERT_EQ(o->size, 1u);
  ASSERT_EQ(object_array_get(obj, 0, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, create_with_str5) {
  value_t v;
  tk_object_t* obj = object_array_create_with_str("abc,123", ",", VALUE_TYPE_STRING);
  object_array_t* o = OBJECT_ARRAY(obj);

  ASSERT_EQ(o->size, 2u);
  ASSERT_EQ(object_array_get(obj, 0, &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "abc");

  ASSERT_EQ(object_array_get(obj, 1, &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "123");

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, join0) {
  str_t s;
  tk_object_t* obj = object_array_create_with_str("", ",", VALUE_TYPE_STRING);

  str_init(&s, 100);
  ASSERT_EQ(object_array_join(obj, ",", &s), RET_OK);
  ASSERT_STREQ(s.str, "");
  str_reset(&s);
  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, join1) {
  str_t s;
  tk_object_t* obj = object_array_create_with_str("abc", ",", VALUE_TYPE_STRING);

  str_init(&s, 100);
  ASSERT_EQ(object_array_join(obj, ",", &s), RET_OK);
  ASSERT_STREQ(s.str, "abc");
  str_reset(&s);
  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, join2) {
  str_t s;
  tk_object_t* obj = object_array_create_with_str("a,b,c", ",", VALUE_TYPE_STRING);

  str_init(&s, 100);
  ASSERT_EQ(object_array_join(obj, "; ", &s), RET_OK);
  ASSERT_STREQ(s.str, "a; b; c");
  str_reset(&s);
  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, dup0) {
  tk_object_t* dup = NULL;
  tk_object_t* obj = object_array_create_with_str("", ",", VALUE_TYPE_INT32);

  dup = object_array_dup(obj, 0, 0);
  ASSERT_EQ(dup != NULL, true);
  TK_OBJECT_UNREF(dup);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, dup1) {
  value_t v;
  tk_object_t* dup = NULL;
  tk_object_t* obj = object_array_create_with_str("1,2,3", ",", VALUE_TYPE_INT32);

  dup = object_array_dup(obj, 0, 1);
  ASSERT_EQ(dup != NULL, true);
  ASSERT_EQ(OBJECT_ARRAY(dup)->size, 1u);
  ASSERT_EQ(object_array_get(dup, 0, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);
  TK_OBJECT_UNREF(dup);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, dup2) {
  value_t v;
  tk_object_t* dup = NULL;
  tk_object_t* obj = object_array_create_with_str("1,2,3", ",", VALUE_TYPE_INT32);

  dup = object_array_dup(obj, 0, 2);
  ASSERT_EQ(dup != NULL, true);
  ASSERT_EQ(OBJECT_ARRAY(dup)->size, 2u);
  ASSERT_EQ(object_array_get(dup, 0, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);
  ASSERT_EQ(object_array_get(dup, 1, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);
  TK_OBJECT_UNREF(dup);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, dup3) {
  value_t v;
  tk_object_t* dup = NULL;
  tk_object_t* obj = object_array_create_with_str("1,2,3", ",", VALUE_TYPE_INT32);

  dup = object_array_dup(obj, 1, 3);
  ASSERT_EQ(dup != NULL, true);
  ASSERT_EQ(OBJECT_ARRAY(dup)->size, 2u);
  ASSERT_EQ(object_array_get(dup, 0, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);
  ASSERT_EQ(object_array_get(dup, 1, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 3);
  TK_OBJECT_UNREF(dup);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, sort_int1) {
  value_t v;
  tk_object_t* obj = object_array_create_with_str("2,1,3", ",", VALUE_TYPE_INT32);

  object_array_sort_as_int(obj, TRUE);
  ASSERT_EQ(object_array_get(obj, 0, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);
  ASSERT_EQ(object_array_get(obj, 1, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);
  ASSERT_EQ(object_array_get(obj, 2, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 3);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, sort_int2) {
  value_t v;
  tk_object_t* obj = object_array_create_with_str("3,2,1", ",", VALUE_TYPE_INT32);

  object_array_sort_as_int(obj, TRUE);
  ASSERT_EQ(object_array_get(obj, 0, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);
  ASSERT_EQ(object_array_get(obj, 1, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);
  ASSERT_EQ(object_array_get(obj, 2, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 3);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, sort_int3) {
  value_t v;
  tk_object_t* obj = object_array_create_with_str("1,2,3", ",", VALUE_TYPE_INT32);

  object_array_sort_as_int(obj, TRUE);
  ASSERT_EQ(object_array_get(obj, 0, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);
  ASSERT_EQ(object_array_get(obj, 1, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);
  ASSERT_EQ(object_array_get(obj, 2, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 3);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, sort_int4) {
  value_t v;
  tk_object_t* obj = object_array_create_with_str("1,2,3", ",", VALUE_TYPE_INT32);

  object_array_sort_as_int(obj, FALSE);
  ASSERT_EQ(object_array_get(obj, 0, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 3);
  ASSERT_EQ(object_array_get(obj, 1, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);
  ASSERT_EQ(object_array_get(obj, 2, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, sort_double1) {
  value_t v;
  tk_object_t* obj = object_array_create_with_str("1,2,3", ",", VALUE_TYPE_DOUBLE);

  object_array_sort_as_double(obj, TRUE);
  ASSERT_EQ(object_array_get(obj, 0, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);
  ASSERT_EQ(object_array_get(obj, 1, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);
  ASSERT_EQ(object_array_get(obj, 2, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 3);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, sort_double2) {
  value_t v;
  tk_object_t* obj = object_array_create_with_str("1,2,3", ",", VALUE_TYPE_DOUBLE);

  object_array_sort_as_double(obj, FALSE);
  ASSERT_EQ(object_array_get(obj, 0, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 3);
  ASSERT_EQ(object_array_get(obj, 1, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);
  ASSERT_EQ(object_array_get(obj, 2, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, sort_str1) {
  value_t v;
  tk_object_t* obj = object_array_create_with_str("a1,B2,c3", ",", VALUE_TYPE_STRING);

  object_array_sort_as_str(obj, TRUE, FALSE);
  ASSERT_EQ(object_array_get(obj, 0, &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "B2");
  ASSERT_EQ(object_array_get(obj, 1, &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "a1");

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, sort_str2) {
  value_t v;
  tk_object_t* obj = object_array_create_with_str("a1,B2,c3", ",", VALUE_TYPE_STRING);

  object_array_sort_as_str(obj, TRUE, TRUE);
  ASSERT_EQ(object_array_get(obj, 0, &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "a1");
  ASSERT_EQ(object_array_get(obj, 1, &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "B2");

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, sort_str3) {
  value_t v;
  tk_object_t* obj = object_array_create_with_str("a1,B2,c3", ",", VALUE_TYPE_STRING);

  object_array_sort_as_str(obj, FALSE, TRUE);
  ASSERT_EQ(object_array_get(obj, 0, &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "c3");
  ASSERT_EQ(object_array_get(obj, 1, &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "B2");

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, min1) {
  value_t v;
  tk_object_t* obj = object_array_create_with_str("123,412,199,1,10000,2", ",", VALUE_TYPE_DOUBLE);
  ASSERT_EQ(object_array_min(obj, &v), RET_OK);
  ASSERT_EQ(value_double(&v), 1);
  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, min2) {
  value_t v;
  tk_object_t* obj = object_array_create_with_str("", ",", VALUE_TYPE_DOUBLE);
  ASSERT_EQ(object_array_min(obj, &v), RET_OK);
  ASSERT_EQ(value_double(&v), 0);
  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, max1) {
  value_t v;
  tk_object_t* obj = object_array_create_with_str("123,412,199,1,10000,2", ",", VALUE_TYPE_DOUBLE);
  ASSERT_EQ(object_array_max(obj, &v), RET_OK);
  ASSERT_EQ(value_double(&v), 10000);
  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, max2) {
  value_t v;
  tk_object_t* obj = object_array_create_with_str("", ",", VALUE_TYPE_DOUBLE);
  ASSERT_EQ(object_array_max(obj, &v), RET_OK);
  ASSERT_EQ(value_double(&v), 0);
  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, sum1) {
  value_t v;
  tk_object_t* obj = object_array_create_with_str("1,2,3,4", ",", VALUE_TYPE_DOUBLE);
  ASSERT_EQ(object_array_sum(obj, &v), RET_OK);
  ASSERT_EQ(value_double(&v), 10);
  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, sum2) {
  value_t v;
  tk_object_t* obj = object_array_create_with_str("", ",", VALUE_TYPE_DOUBLE);
  ASSERT_EQ(object_array_sum(obj, &v), RET_OK);
  ASSERT_EQ(value_double(&v), 0);
  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, avg1) {
  value_t v;
  tk_object_t* obj = object_array_create_with_str("0,1,2,3,4", ",", VALUE_TYPE_DOUBLE);
  ASSERT_EQ(object_array_avg(obj, &v), RET_OK);
  ASSERT_EQ(value_double(&v), 2);
  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, avg2) {
  value_t v;
  tk_object_t* obj = object_array_create_with_str("", ",", VALUE_TYPE_DOUBLE);
  ASSERT_EQ(object_array_avg(obj, &v), RET_OK);
  ASSERT_EQ(value_double(&v), 0);
  TK_OBJECT_UNREF(obj);
}

TEST(ObjectArray, reverse) {
  str_t s;
  tk_object_t* obj = object_array_create_with_str("0", ",", VALUE_TYPE_INT32);

  str_init(&s, 100);
  ASSERT_EQ(object_array_reverse(obj), RET_OK);
  ASSERT_EQ(object_array_join(obj, ",", &s), RET_OK);
  ASSERT_STREQ(s.str, "0");
  str_clear(&s);
  TK_OBJECT_UNREF(obj);

  obj = object_array_create_with_str("0,1", ",", VALUE_TYPE_INT32);
  ASSERT_EQ(object_array_reverse(obj), RET_OK);
  ASSERT_EQ(object_array_join(obj, ",", &s), RET_OK);
  ASSERT_STREQ(s.str, "1,0");
  str_clear(&s);
  TK_OBJECT_UNREF(obj);

  obj = object_array_create_with_str("0,1,2", ",", VALUE_TYPE_INT32);
  ASSERT_EQ(object_array_reverse(obj), RET_OK);
  ASSERT_EQ(object_array_join(obj, ",", &s), RET_OK);
  ASSERT_STREQ(s.str, "2,1,0");
  str_reset(&s);
  TK_OBJECT_UNREF(obj);
}
