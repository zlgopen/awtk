#include "gtest/gtest.h"
#include "tkc/object_overload.h"
#include "tkc/object_default.h"

static ret_t s_overload_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  if (tk_str_eq(name, "overloaded_prop")) {
    value_set_str(v, "overloaded_value");
    return RET_OK;
  }
  return RET_NOT_FOUND;
}

static ret_t s_overload_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  if (tk_str_eq(name, "overloaded_prop")) {
    return RET_OK;
  }
  return RET_NOT_FOUND;
}

static bool_t s_overload_can_exec(tk_object_t* obj, const char* name, const char* args) {
  if (tk_str_eq(name, "overloaded_cmd")) {
    return TRUE;
  } else if (tk_str_eq(name, "overloaded_cmd_ex")) {
    return TRUE;
  }
  return FALSE;
}

static ret_t s_overload_exec(tk_object_t* obj, const char* name, const char* args) {
  if (tk_str_eq(name, "overloaded_cmd")) {
    return RET_OK;
  }
  return RET_NOT_FOUND;
}

static ret_t s_overload_exec_ex(tk_object_t* obj, const char* name, const char* args,
                                value_t* result) {
  if (tk_str_eq(name, "overloaded_cmd_ex")) {
    value_set_str(result, "overloaded_result");
    return RET_OK;
  }
  return RET_NOT_FOUND;
}

static const struct test_overload_vtable_t : object_vtable_t {
  test_overload_vtable_t() {
    type = "test_overload";
    desc = "test_overload";
    size = sizeof(tk_object_t);
    is_collection = FALSE;
    get_prop = s_overload_get_prop;
    set_prop = s_overload_set_prop;
    can_exec = s_overload_can_exec;
    exec = s_overload_exec;
    exec_ex = s_overload_exec_ex;
  }
} s_test_overload_vt;

TEST(ObjectOverload, basic) {
  tk_object_t* base_obj = object_default_create();
  tk_object_t* overload_obj = object_overload_create(base_obj, &s_test_overload_vt);
  ASSERT_TRUE(overload_obj != NULL);

  object_overload_t* overload = OBJECT_OVERLOAD(overload_obj);
  ASSERT_TRUE(overload != NULL);

  ASSERT_EQ(overload->base_obj, base_obj);
  ASSERT_EQ(overload->overload_vt, &s_test_overload_vt);

  TK_OBJECT_UNREF(overload_obj);
  TK_OBJECT_UNREF(base_obj);
}

TEST(ObjectOverload, prop) {
  tk_object_t* base_obj = object_default_create();
  tk_object_t* overload_obj = object_overload_create(base_obj, &s_test_overload_vt);
  value_t v1 = {};

  ASSERT_EQ(tk_object_get_prop(overload_obj, "overloaded_prop", &v1), RET_OK);
  ASSERT_STREQ(value_str(&v1), "overloaded_value");

  ASSERT_EQ(tk_object_get_prop(overload_obj, "non_exist_prop", &v1), RET_NOT_FOUND);

  TK_OBJECT_UNREF(overload_obj);
  TK_OBJECT_UNREF(base_obj);
}

TEST(ObjectOverload, exec) {
  tk_object_t* base_obj = object_default_create();
  tk_object_t* overload_obj = object_overload_create(base_obj, &s_test_overload_vt);
  value_t result = {};

  ASSERT_TRUE(tk_object_can_exec(overload_obj, "overloaded_cmd", NULL));
  ASSERT_EQ(tk_object_exec(overload_obj, "overloaded_cmd", NULL), RET_OK);

  ASSERT_TRUE(tk_object_can_exec(overload_obj, "overloaded_cmd_ex", NULL));
  ASSERT_EQ(tk_object_exec_ex(overload_obj, "overloaded_cmd_ex", NULL, &result), RET_OK);
  ASSERT_STREQ(value_str(&result), "overloaded_result");
  value_reset(&result);

  ASSERT_FALSE(tk_object_can_exec(overload_obj, "non_exist_cmd", NULL));
  ASSERT_EQ(tk_object_exec(overload_obj, "non_exist_cmd", NULL), RET_NOT_FOUND);

  TK_OBJECT_UNREF(overload_obj);
  TK_OBJECT_UNREF(base_obj);
}

TEST(ObjectOverload, clone) {
  tk_object_t* base_obj = object_default_create();
  tk_object_t* overload_obj = object_overload_create(base_obj, &s_test_overload_vt);
  tk_object_t* clone_obj = NULL;
  value_t v = {};

  value_set_str(&v, "base_value");
  ASSERT_EQ(tk_object_set_prop(base_obj, "base_prop", &v), RET_OK);

  clone_obj = tk_object_clone(overload_obj);
  ASSERT_TRUE(clone_obj != NULL);
  object_overload_t* clone = OBJECT_OVERLOAD(clone_obj);
  ASSERT_NE(base_obj, clone->base_obj);

  ASSERT_EQ(tk_object_get_prop(clone_obj, "overloaded_prop", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "overloaded_value");

  TK_OBJECT_UNREF(clone_obj);
  TK_OBJECT_UNREF(overload_obj);
  TK_OBJECT_UNREF(base_obj);
}

TEST(ObjectOverload, null_overload_vt) {
  tk_object_t* base_obj = object_default_create();
  tk_object_t* overload_obj = object_overload_create(base_obj, NULL);
  value_t v1 = {};
  value_t v2 = {};

  value_set_str(&v1, "base_value");
  ASSERT_EQ(tk_object_set_prop(base_obj, "base_prop", &v1), RET_OK);
  ASSERT_EQ(tk_object_get_prop(overload_obj, "base_prop", &v2), RET_OK);
  ASSERT_STREQ(value_str(&v1), value_str(&v2));

  TK_OBJECT_UNREF(overload_obj);
  TK_OBJECT_UNREF(base_obj);
}
