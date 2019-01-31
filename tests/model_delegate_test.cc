#include "gtest/gtest.h"
#include "test_obj.inc"

#include <string>

using std::string;

TEST(ModelDelegate, i8) {
  model_t* model = test_obj_create_model();

  ASSERT_EQ(object_set_prop_int(OBJECT(model), "i8", 1), RET_OK);
  ASSERT_EQ(object_get_prop_int(OBJECT(model), "i8", 0), 1);

  object_unref(OBJECT(model));
}

TEST(ModelDelegate, u8) {
  model_t* model = test_obj_create_model();

  ASSERT_EQ(object_set_prop_int(OBJECT(model), "u8", 2), RET_OK);
  ASSERT_EQ(object_get_prop_int(OBJECT(model), "u8", 0), 2);

  object_unref(OBJECT(model));
}

TEST(ModelDelegate, i16) {
  model_t* model = test_obj_create_model();

  ASSERT_EQ(object_set_prop_int(OBJECT(model), "i16", 3), RET_OK);
  ASSERT_EQ(object_get_prop_int(OBJECT(model), "i16", 0), 3);

  object_unref(OBJECT(model));
}

TEST(ModelDelegate, u16) {
  model_t* model = test_obj_create_model();

  ASSERT_EQ(object_set_prop_int(OBJECT(model), "u16", 4), RET_OK);
  ASSERT_EQ(object_get_prop_int(OBJECT(model), "u16", 0), 4);

  object_unref(OBJECT(model));
}

TEST(ModelDelegate, i32) {
  model_t* model = test_obj_create_model();

  ASSERT_EQ(object_set_prop_int(OBJECT(model), "i32", 5), RET_OK);
  ASSERT_EQ(object_get_prop_int(OBJECT(model), "i32", 0), 5);

  object_unref(OBJECT(model));
}

TEST(ModelDelegate, u32) {
  model_t* model = test_obj_create_model();

  ASSERT_EQ(object_set_prop_int(OBJECT(model), "u32", 6), RET_OK);
  ASSERT_EQ(object_get_prop_int(OBJECT(model), "u32", 0), 6);

  object_unref(OBJECT(model));
}

TEST(ModelDelegate, i64) {
  model_t* model = test_obj_create_model();

  ASSERT_EQ(object_set_prop_int(OBJECT(model), "i64", 7), RET_OK);
  ASSERT_EQ(object_get_prop_int(OBJECT(model), "i64", 0), 7);

  object_unref(OBJECT(model));
}

TEST(ModelDelegate, u64) {
  model_t* model = test_obj_create_model();

  ASSERT_EQ(object_set_prop_int(OBJECT(model), "u64", 8), RET_OK);
  ASSERT_EQ(object_get_prop_int(OBJECT(model), "u64", 0), 8);

  object_unref(OBJECT(model));
}

TEST(ModelDelegate, b) {
  model_t* model = test_obj_create_model();

  ASSERT_EQ(object_set_prop_int(OBJECT(model), "b", TRUE), RET_OK);
  ASSERT_EQ(object_get_prop_int(OBJECT(model), "b", 0), TRUE);

  ASSERT_EQ(object_set_prop_int(OBJECT(model), "b", FALSE), RET_OK);
  ASSERT_EQ(object_get_prop_int(OBJECT(model), "b", 0), FALSE);

  object_unref(OBJECT(model));
}

TEST(ModelDelegate, f) {
  model_t* model = test_obj_create_model();

  ASSERT_EQ(object_set_prop_float(OBJECT(model), "f", 10), RET_OK);
  ASSERT_EQ(object_get_prop_float(OBJECT(model), "f", 0), 10);

  object_unref(OBJECT(model));
}

TEST(ModelDelegate, f32) {
  model_t* model = test_obj_create_model();

  ASSERT_EQ(object_set_prop_float(OBJECT(model), "f32", 11), RET_OK);
  ASSERT_EQ(object_get_prop_float(OBJECT(model), "f32", 0), 11);

  object_unref(OBJECT(model));
}

TEST(ModelDelegate, f64) {
  model_t* model = test_obj_create_model();

  ASSERT_EQ(object_set_prop_float(OBJECT(model), "f64", 12), RET_OK);
  ASSERT_EQ(object_get_prop_float(OBJECT(model), "f64", 0), 12);

  object_unref(OBJECT(model));
}

TEST(ModelDelegate, data) {
  model_t* model = test_obj_create_model();

  ASSERT_EQ(object_set_prop_str(OBJECT(model), "data", "123abc"), RET_OK);
  ASSERT_EQ(string(object_get_prop_str(OBJECT(model), "data")), string("123abc"));

  object_unref(OBJECT(model));
}

TEST(ModelDelegate, prop_not_exist) {
  model_t* model = test_obj_create_model();

  ASSERT_EQ(object_set_prop_str(OBJECT(model), "not prop", "123abc"), RET_NOT_FOUND);
  ASSERT_EQ(object_get_prop_str(OBJECT(model), "not prop"), (char*)NULL);

  object_unref(OBJECT(model));
}

TEST(ModelDelegate, command) {
  int32_t i = 0;
  model_t* model = test_obj_create_model();

  for (i = 0; i < MAX_SAVE_COUNT; i++) {
    ASSERT_EQ(object_can_exec(OBJECT(model), "save", NULL), TRUE);
    ASSERT_EQ(object_exec(OBJECT(model), "save", NULL), RET_OK);
  }
  ASSERT_EQ(object_can_exec(OBJECT(model), "save", NULL), FALSE);
  ASSERT_EQ(object_exec(OBJECT(model), "save", NULL), RET_FAIL);

  object_unref(OBJECT(model));
}

TEST(ModelDelegate, command_not_exist) {
  model_t* model = test_obj_create_model();

  ASSERT_EQ(object_can_exec(OBJECT(model), "load", NULL), FALSE);
  ASSERT_EQ(object_exec(OBJECT(model), "load", NULL), RET_NOT_FOUND);

  object_unref(OBJECT(model));
}

TEST(ModelDelegate, view_model) {
  value_t v;
  value_t v1;
  view_model_t* vm = test_obj_create_view_model();

  value_set_int(&v, 123);
  ASSERT_EQ(view_model_has_prop(vm, "u32"), TRUE);
  ASSERT_EQ(view_model_has_prop(vm, "u31"), FALSE);
  ASSERT_EQ(view_model_can_exec(vm, "save", NULL), TRUE);
  ASSERT_EQ(view_model_exec(vm, "save", NULL), RET_OK);
  
  ASSERT_EQ(view_model_can_exec(vm, "save", "1"), TRUE);
  ASSERT_EQ(view_model_exec(vm, "save", "1"), RET_OK);

  ASSERT_EQ(view_model_can_exec(vm, "load", NULL), FALSE);

  ASSERT_EQ(view_model_set_prop(vm, "u32", &v), RET_OK);
  ASSERT_EQ(view_model_get_prop(vm, "u32", &v1), RET_OK);
  ASSERT_EQ(value_int(&v), value_int(&v1));

  ASSERT_EQ(view_model_set_prop(vm, "i32", &v), RET_OK);
  ASSERT_EQ(view_model_get_prop(vm, "i32", &v1), RET_OK);
  ASSERT_EQ(value_int(&v), value_int(&v1));

  ASSERT_EQ(view_model_eval(vm, "$u32+$i32", &v1), RET_OK);
  ASSERT_EQ(2 * value_int(&v), value_int(&v1));

  object_unref(OBJECT(vm));
}
