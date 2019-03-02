#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/model_delegate.h"
#include "mvvm/base/view_model_normal.h"

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
  int delta = args != NULL ? tk_atoi(args) : 1;
  if (obj->save_count < MAX_SAVE_COUNT) {
    obj->save_count += delta;

    return RET_OK;
  } else {
    return RET_FAIL;
  }
}

static bool_t test_obj_can_save(test_obj_t* obj, const char* args) {
  int delta = args != NULL ? tk_atoi(args) : 1;
  return (obj->save_count + delta) <= MAX_SAVE_COUNT;
}

static ret_t test_obj_get_data(test_obj_t* obj, value_t* v) {
  value_set_str(v, obj->data);

  return RET_OK;
}

static ret_t test_obj_get_save_count(test_obj_t* obj, value_t* v) {
  value_set_int(v, obj->save_count);

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
  MODEL_PROP(model, "save_count", test_obj_get_save_count, NULL);
  MODEL_COMMAND(model, "save", test_obj_save, test_obj_can_save);

  /*test repeat*/
  MODEL_PROP(model, "data", test_obj_get_data, test_obj_set_data);
  MODEL_COMMAND(model, "save", test_obj_save, test_obj_can_save);

  return model;
}

static view_model_t* test_obj_create_view_model(void) {
  model_t* model = test_obj_create_model();
  return view_model_normal_create(model);
}
