#include "tkc/mem.h"
#include "tkc/utils.h"
#include "gtest/gtest.h"
#include "mvvm/model_delegate.h"

#include <string>

using std::string;

typedef struct _test_obj_t {
  int8_t i8;
  int16_t i16;
  int32_t i32;
  int64_t i64;
  uint8_t u8;
  uint16_t u16;
  uint32_t u32;
  uint64_t u64;
  float_t f;
  float f32;
  double_t f64;
  char* str;
  bool_t b;
  char data[32];

  int32_t save_count;
} test_obj_t;

static test_obj_t* test_obj_create(void) {
  return TKMEM_ZALLOC(test_obj_t);
}

#define MAX_SAVE_COUNT 100

static ret_t test_obj_save(test_obj_t* obj, const char* args) {
  if (obj->save_count < MAX_SAVE_COUNT) {
    obj->save_count++;

    return RET_OK;
  } else {
    return RET_FAIL;
  }
}

static bool_t test_obj_can_save(test_obj_t* obj, const char* args) {
  return obj->save_count < MAX_SAVE_COUNT;
}

static ret_t test_obj_get_data(test_obj_t* obj, value_t* v) {
  value_set_str(v, obj->data);

  return RET_OK;
}

static ret_t test_obj_set_data(test_obj_t* obj, value_t* v) {
  tk_strncpy(obj->data, value_str(v), sizeof(obj->data) - 1);

  return RET_OK;
}

static ret_t test_obj_destroy(test_obj_t* obj) {
  TKMEM_FREE(obj->str);
  TKMEM_FREE(obj);

  return RET_OK;
}

static model_t* test_obj_create_model(void) {
  test_obj_t* obj = test_obj_create();
  model_t* model = model_delegate_create(obj, (tk_destroy_t)test_obj_destroy);

  MODEL_SIMPLE_PROP(model, "i8", VALUE_TYPE_INT8, &(obj->i8));
  MODEL_SIMPLE_PROP(model, "i16", VALUE_TYPE_INT16, &(obj->i16));
  MODEL_SIMPLE_PROP(model, "i32", VALUE_TYPE_INT32, &(obj->i32));
  MODEL_SIMPLE_PROP(model, "i64", VALUE_TYPE_INT64, &(obj->i64));

  MODEL_SIMPLE_PROP(model, "u8", VALUE_TYPE_UINT8, &(obj->u8));
  MODEL_SIMPLE_PROP(model, "u16", VALUE_TYPE_UINT16, &(obj->u16));
  MODEL_SIMPLE_PROP(model, "u32", VALUE_TYPE_UINT32, &(obj->u32));
  MODEL_SIMPLE_PROP(model, "u64", VALUE_TYPE_UINT64, &(obj->u64));

  MODEL_SIMPLE_PROP(model, "f", VALUE_TYPE_FLOAT, &(obj->f));
  MODEL_SIMPLE_PROP(model, "f32", VALUE_TYPE_FLOAT32, &(obj->f32));
  MODEL_SIMPLE_PROP(model, "f64", VALUE_TYPE_DOUBLE, &(obj->f64));
  MODEL_SIMPLE_PROP(model, "b", VALUE_TYPE_BOOL, &(obj->b));

  MODEL_PROP(model, "data", test_obj_get_data, test_obj_set_data);
  MODEL_COMMAND(model, "save", test_obj_save, test_obj_can_save);

  /*test repeat*/
  MODEL_PROP(model, "data", test_obj_get_data, test_obj_set_data);
  MODEL_COMMAND(model, "save", test_obj_save, test_obj_can_save);

  return model;
}

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
