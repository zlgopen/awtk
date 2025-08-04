#include "gtest/gtest.h"
#include "tkc/fscript.h"
#include "tkc/object.h"
#include "tkc/object_default.h"

typedef struct _object_custom_t {
  tk_object_t obj;
} object_custom_t;

static object_custom_t* object_custom_cast(tk_object_t* obj);

#define OBJECT_CUSTOM(obj) object_custom_cast(obj)

static bool_t object_custom_can_cmd0(tk_object_t* obj, const char* args) {
  object_custom_t* model = OBJECT_CUSTOM(obj);
  return_value_if_fail(model != NULL, FALSE);

  return TRUE;
}

static ret_t object_custom_cmd0(tk_object_t* obj, const char* args) {
  object_custom_t* model = OBJECT_CUSTOM(obj);
  return_value_if_fail(model != NULL, RET_BAD_PARAMS);

  return RET_OK;
}

static bool_t object_custom_can_cmd1(tk_object_t* obj, const char* args) {
  object_custom_t* model = OBJECT_CUSTOM(obj);
  return_value_if_fail(model != NULL, FALSE);

  return TRUE;
}

static ret_t object_custom_cmd1(tk_object_t* obj, const char* args, value_t* result) {
  object_custom_t* model = OBJECT_CUSTOM(obj);
  return_value_if_fail(model != NULL && result != NULL, RET_BAD_PARAMS);

  value_set_str(result, "ok");

  return RET_OK;
}

static ret_t object_custom_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  tk_object_t* sub = NULL;
  const char* sub_name = NULL;
  object_custom_t* model = OBJECT_CUSTOM(obj);
  return_value_if_fail(model != NULL, RET_BAD_PARAMS);

  sub = tk_object_get_child_object(obj, name, &sub_name);
  if (sub != NULL) {
    return tk_object_get_prop(sub, sub_name, v);
  }

  // begin the dispatch of get_prop
  // end the dispatch of get_prop

  return ret;
}

static ret_t object_custom_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  tk_object_t* sub = NULL;
  const char* sub_name = NULL;
  object_custom_t* model = OBJECT_CUSTOM(obj);
  return_value_if_fail(model != NULL, RET_BAD_PARAMS);

  sub = tk_object_get_child_object(obj, name, &sub_name);
  if (sub != NULL) {
    return tk_object_set_prop(sub, sub_name, v);
  }

  // begin the dispatch of set_prop
  // end the dispatch of set_prop

  return ret;
}

static bool_t object_custom_can_exec(tk_object_t* obj, const char* name, const char* args) {
  bool_t ret = FALSE;
  tk_object_t* sub = NULL;
  const char* sub_name = NULL;
  object_custom_t* model = OBJECT_CUSTOM(obj);
  return_value_if_fail(model != NULL, FALSE);

  sub = tk_object_get_child_object(obj, name, &sub_name);
  if (sub != NULL) {
    return tk_object_can_exec(sub, sub_name, args);
  }

  // begin the dispatch of can_exec
  if (tk_str_eq(name, "cmd0")) {
    ret = object_custom_can_cmd0(obj, args);
  } else if (tk_str_eq(name, "cmd1")) {
    ret = object_custom_can_cmd1(obj, args);
  }
  // end the dispatch of can_exec

  return ret;
}

static ret_t object_custom_exec(tk_object_t* obj, const char* name, const char* args) {
  ret_t ret = RET_NOT_FOUND;
  tk_object_t* sub = NULL;
  const char* sub_name = NULL;
  object_custom_t* model = OBJECT_CUSTOM(obj);
  return_value_if_fail(model != NULL, RET_BAD_PARAMS);

  sub = tk_object_get_child_object(obj, name, &sub_name);
  if (sub != NULL) {
    return tk_object_exec(sub, sub_name, args);
  }

  // begin the dispatch of exec
  if (tk_str_eq(name, "cmd0")) {
    ret = object_custom_cmd0(obj, args);
  }
  // end the dispatch of exec

  return ret;
}

static ret_t object_custom_exec_ex(tk_object_t* obj, const char* name, const char* args,
                                   value_t* result) {
  ret_t ret = RET_NOT_FOUND;
  tk_object_t* sub = NULL;
  const char* sub_name = NULL;
  object_custom_t* model = OBJECT_CUSTOM(obj);
  return_value_if_fail(model != NULL && result != NULL, RET_BAD_PARAMS);

  sub = tk_object_get_child_object(obj, name, &sub_name);
  if (sub != NULL) {
    return tk_object_exec(sub, sub_name, args);
  }

  // begin the dispatch of exec
  if (tk_str_eq(name, "cmd1")) {
    ret = object_custom_cmd1(obj, args, result);
  }
  // end the dispatch of exec

  return ret;
}

static ret_t object_custom_on_destroy_impl(tk_object_t* obj) {
  object_custom_t* model = OBJECT_CUSTOM(obj);
  return_value_if_fail(model != NULL, RET_BAD_PARAMS);

  return RET_OK;
}

static const struct object_custom_vtable_t : object_vtable_t {
  object_custom_vtable_t() {
    type = "object_custom_t";
    desc = "object_custom_t";
    size = sizeof(object_custom_t);
    is_collection = FALSE;
    on_destroy = object_custom_on_destroy_impl;
    get_prop = object_custom_get_prop;
    set_prop = object_custom_set_prop;
    can_exec = object_custom_can_exec;
    exec = object_custom_exec;
    exec_ex = object_custom_exec_ex;
  }
} s_object_custom_vtable;

tk_object_t* object_custom_create(void) {
  tk_object_t* obj = tk_object_create(&s_object_custom_vtable);
  object_custom_t* model = (object_custom_t*)(obj);
  return_value_if_fail(model != NULL, NULL);

  return obj;
}

static object_custom_t* object_custom_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_custom_vtable, NULL);
  return (object_custom_t*)(obj);
}

/*************************************************************************************/

TEST(ObjectCustom, exec) {
  tk_object_t* obj = object_custom_create();

  ASSERT_EQ(tk_object_can_exec(obj, "cmd0", NULL), TRUE);
  ASSERT_EQ(tk_object_exec(obj, "cmd0", NULL), RET_OK);

  ASSERT_EQ(tk_object_can_exec(obj, "cmd1", NULL), TRUE);
  ASSERT_EQ(tk_object_exec(obj, "cmd1", NULL), RET_OK);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectCustom, exec_ex) {
  tk_object_t* obj = object_custom_create();
  value_t result = {};

  ASSERT_EQ(tk_object_can_exec(obj, "cmd0", NULL), TRUE);
  ASSERT_EQ(tk_object_exec_ex(obj, "cmd0", NULL, &result), RET_OK);
  ASSERT_EQ(value_int(&result), RET_OK);

  ASSERT_EQ(tk_object_can_exec(obj, "cmd1", NULL), TRUE);
  ASSERT_EQ(tk_object_exec_ex(obj, "cmd1", NULL, &result), RET_OK);
  ASSERT_STREQ(value_str(&result), "ok");

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectCustom, fscript1) {
  tk_object_t* obj_fscript = object_default_create();
  tk_object_t* obj = object_custom_create();
  value_t result = {};

  tk_object_set_prop_object(obj_fscript, "self", obj);

  fscript_eval(obj_fscript, "self.exec_ex('cmd1', '')", &result);
  ASSERT_STREQ(value_str(&result), "ok");
  value_reset(&result);
  
  fscript_eval(obj_fscript, "self.exec_ex('cmd_not_exist', '')", &result);
  ASSERT_EQ(value_int(&result), RET_NOT_FOUND);
  value_reset(&result);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(obj_fscript);
}

TEST(ObjectCustom, fscript2) {
  tk_object_t* obj_fscript = object_default_create();
  tk_object_t* obj = object_custom_create();
  value_t result = {};

  tk_object_set_prop_object(obj_fscript, "self", obj);

  fscript_eval(obj_fscript, "self.exec('cmd0', '')", &result);
  ASSERT_EQ(value_int(&result), RET_OK);
  
  fscript_eval(obj_fscript, "self.exec('cmd_not_exist', '')", &result);
  ASSERT_EQ(value_int(&result), RET_NOT_FOUND);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(obj_fscript);
}
