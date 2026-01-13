#include "gtest/gtest.h"
#include "tkc/object_override.h"
#include "tkc/object_default.h"

static ret_t s_override_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  if (tk_str_eq(name, "override_prop")) {
    value_set_str(v, "override_value");
    return RET_OK;
  }
  return RET_NOT_FOUND;
}

static ret_t s_override_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  if (tk_str_eq(name, "override_prop")) {
    return RET_OK;
  }
  return RET_NOT_FOUND;
}

static bool_t s_override_can_exec(tk_object_t* obj, const char* name, const char* args) {
  if (tk_str_eq(name, "override_cmd")) {
    return TRUE;
  } else if (tk_str_eq(name, "override_cmd_ex")) {
    return TRUE;
  }
  return FALSE;
}

static ret_t s_override_exec(tk_object_t* obj, const char* name, const char* args) {
  if (tk_str_eq(name, "override_cmd")) {
    return RET_OK;
  }
  return RET_NOT_FOUND;
}

static ret_t s_override_exec_ex(tk_object_t* obj, const char* name, const char* args,
                                value_t* result) {
  if (tk_str_eq(name, "override_cmd_ex")) {
    value_set_str(result, "override_result");
    return RET_OK;
  }
  return RET_NOT_FOUND;
}

static const struct test_override_vtable_t : object_vtable_t {
  test_override_vtable_t() {
    type = "test_override";
    desc = "test_override";
    size = sizeof(tk_object_t);
    is_collection = FALSE;
    get_prop = s_override_get_prop;
    set_prop = s_override_set_prop;
    can_exec = s_override_can_exec;
    exec = s_override_exec;
    exec_ex = s_override_exec_ex;
  }
} s_test_override_vt;

TEST(ObjectOverride, basic) {
  tk_object_t* base_obj = object_default_create();
  tk_object_t* override_obj = object_override_create(base_obj, &s_test_override_vt);
  ASSERT_TRUE(override_obj != NULL);

  object_override_t* override = OBJECT_OVERRIDE(override_obj);
  ASSERT_TRUE(override != NULL);

  ASSERT_EQ(override->base_obj, base_obj);
  ASSERT_EQ(override->override_vt, &s_test_override_vt);

  TK_OBJECT_UNREF(override_obj);
  TK_OBJECT_UNREF(base_obj);
}

TEST(ObjectOverride, prop) {
  tk_object_t* base_obj = object_default_create();
  tk_object_t* override_obj = object_override_create(base_obj, &s_test_override_vt);
  value_t v1 = {};

  ASSERT_EQ(tk_object_get_prop(override_obj, "override_prop", &v1), RET_OK);
  ASSERT_STREQ(value_str(&v1), "override_value");

  ASSERT_EQ(tk_object_get_prop(override_obj, "non_exist_prop", &v1), RET_NOT_FOUND);

  TK_OBJECT_UNREF(override_obj);
  TK_OBJECT_UNREF(base_obj);
}

TEST(ObjectOverride, exec) {
  tk_object_t* base_obj = object_default_create();
  tk_object_t* override_obj = object_override_create(base_obj, &s_test_override_vt);
  value_t result = {};

  ASSERT_TRUE(tk_object_can_exec(override_obj, "override_cmd", NULL));
  ASSERT_EQ(tk_object_exec(override_obj, "override_cmd", NULL), RET_OK);

  ASSERT_TRUE(tk_object_can_exec(override_obj, "override_cmd_ex", NULL));
  ASSERT_EQ(tk_object_exec_ex(override_obj, "override_cmd_ex", NULL, &result), RET_OK);
  ASSERT_STREQ(value_str(&result), "override_result");
  value_reset(&result);

  ASSERT_FALSE(tk_object_can_exec(override_obj, "non_exist_cmd", NULL));
  ASSERT_EQ(tk_object_exec(override_obj, "non_exist_cmd", NULL), RET_NOT_FOUND);

  TK_OBJECT_UNREF(override_obj);
  TK_OBJECT_UNREF(base_obj);
}

TEST(ObjectOverride, clone) {
  tk_object_t* base_obj = object_default_create();
  tk_object_t* override_obj = object_override_create(base_obj, &s_test_override_vt);
  tk_object_t* clone_obj = NULL;
  value_t v = {};

  value_set_str(&v, "base_value");
  ASSERT_EQ(tk_object_set_prop(base_obj, "base_prop", &v), RET_OK);

  clone_obj = tk_object_clone(override_obj);
  ASSERT_TRUE(clone_obj != NULL);
  object_override_t* clone = OBJECT_OVERRIDE(clone_obj);
  ASSERT_NE(base_obj, clone->base_obj);

  ASSERT_EQ(tk_object_get_prop(clone_obj, "override_prop", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "override_value");

  TK_OBJECT_UNREF(clone_obj);
  TK_OBJECT_UNREF(override_obj);
  TK_OBJECT_UNREF(base_obj);
}

TEST(ObjectOverride, null_override_vt) {
  tk_object_t* base_obj = object_default_create();
  tk_object_t* override_obj = object_override_create(base_obj, NULL);
  value_t v1 = {};
  value_t v2 = {};

  value_set_str(&v1, "base_value");
  ASSERT_EQ(tk_object_set_prop(base_obj, "base_prop", &v1), RET_OK);
  ASSERT_EQ(tk_object_get_prop(override_obj, "base_prop", &v2), RET_OK);
  ASSERT_STREQ(value_str(&v1), value_str(&v2));

  TK_OBJECT_UNREF(override_obj);
  TK_OBJECT_UNREF(base_obj);
}
