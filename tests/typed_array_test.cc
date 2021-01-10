#include "tkc/typed_array.h"
#include "tkc/object_typed_array.h"
#include "gtest/gtest.h"

TEST(TypedArray, basic) {
  value_t v;
  typed_array_t* a = typed_array_create(VALUE_TYPE_UINT8, 0);
  ASSERT_EQ(a != NULL, true);

  ASSERT_EQ(typed_array_insert(a, 0, value_set_uint8(&v, 0)), RET_OK);
  ASSERT_EQ(typed_array_insert(a, 1, value_set_uint8(&v, 1)), RET_OK);
  ASSERT_EQ(typed_array_insert(a, 2, value_set_uint8(&v, 2)), RET_OK);
  ASSERT_EQ(a->size, 3);

  ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
  ASSERT_EQ(value_uint8(&v), 0);
  ASSERT_EQ(typed_array_get(a, 1, &v), RET_OK);
  ASSERT_EQ(value_uint8(&v), 1);
  ASSERT_EQ(typed_array_get(a, 2, &v), RET_OK);
  ASSERT_EQ(value_uint8(&v), 2);

  ASSERT_EQ(typed_array_set(a, 0, value_set_uint8(&v, 100)), RET_OK);
  ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
  ASSERT_EQ(value_uint8(&v), 100);

  ASSERT_EQ(typed_array_set(a, 1, value_set_uint8(&v, 200)), RET_OK);
  ASSERT_EQ(typed_array_get(a, 1, &v), RET_OK);
  ASSERT_EQ(value_uint8(&v), 200);

  ASSERT_EQ(typed_array_set(a, 2, value_set_uint8(&v, 230)), RET_OK);
  ASSERT_EQ(typed_array_get(a, 2, &v), RET_OK);
  ASSERT_EQ(value_uint8(&v), 230);

  ASSERT_EQ(typed_array_remove(a, 1), RET_OK);
  ASSERT_EQ(typed_array_get(a, 1, &v), RET_OK);
  ASSERT_EQ(value_uint8(&v), 230);
  ASSERT_EQ(a->size, 2);

  ASSERT_EQ(typed_array_remove(a, 0), RET_OK);
  ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
  ASSERT_EQ(value_uint8(&v), 230);
  ASSERT_EQ(a->size, 1);

  ASSERT_EQ(typed_array_remove(a, 0), RET_OK);
  ASSERT_EQ(a->size, 0);
  ASSERT_EQ(typed_array_remove(a, 0), RET_BAD_PARAMS);
  ASSERT_EQ(a->size, 0);

  ASSERT_EQ(typed_array_tail(a, &v), RET_BAD_PARAMS);

  typed_array_destroy(a);
}

TEST(TypedArray, push) {
  value_t v;
  uint8_t i = 0;
  typed_array_t* a = typed_array_create(VALUE_TYPE_UINT8, 0);
  ASSERT_EQ(a != NULL, true);
  for (i = 0; i < 255; i++) {
    ASSERT_EQ(a->size, i);
    ASSERT_EQ(typed_array_push(a, value_set_uint8(&v, i)), RET_OK);
    ASSERT_EQ(typed_array_tail(a, &v), RET_OK);
    ASSERT_EQ(value_uint8(&v), i);
  }

  for (; i > 0; i--) {
    ASSERT_EQ(a->size, i);
    ASSERT_EQ(typed_array_tail(a, &v), RET_OK);
    ASSERT_EQ(value_uint8(&v), i - 1);
    ASSERT_EQ(typed_array_pop(a, &v), RET_OK);
    ASSERT_EQ(value_uint8(&v), i - 1);
  }
  typed_array_destroy(a);
}

TEST(TypedArray, insert) {
  value_t v;
  uint8_t i = 0;
  typed_array_t* a = typed_array_create(VALUE_TYPE_UINT8, 0);
  ASSERT_EQ(a != NULL, true);
  for (i = 0; i < 255; i++) {
    ASSERT_EQ(a->size, i);
    ASSERT_EQ(typed_array_insert(a, 0, value_set_uint8(&v, i)), RET_OK);
    ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
    ASSERT_EQ(value_uint8(&v), i);
  }

  ASSERT_EQ(typed_array_clear(a), RET_OK);
  ASSERT_EQ(a->size, 0);

  typed_array_destroy(a);
}

TEST(TypedArray, pointer) {
  typed_array_t* a = typed_array_create(VALUE_TYPE_POINTER, 0);
  ASSERT_EQ(a == NULL, true);
}

TEST(TypedArray, insert_uint64) {
  value_t v;
  uint8_t i = 0;
  typed_array_t* a = typed_array_create(VALUE_TYPE_UINT64, 0);
  ASSERT_EQ(a != NULL, true);
  ASSERT_EQ(a->element_size, sizeof(uint64_t));
  for (i = 0; i < 255; i++) {
    ASSERT_EQ(a->size, i);
    ASSERT_EQ(typed_array_insert(a, 0, value_set_uint64(&v, i)), RET_OK);
    ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
    ASSERT_EQ(value_uint64(&v), i);
  }

  ASSERT_EQ(typed_array_set(a, 10, value_set_uint64(&v, 0x1122334455667788)), RET_OK);
  ASSERT_EQ(typed_array_get(a, 10, &v), RET_OK);
  ASSERT_EQ(value_uint64(&v), 0x1122334455667788);
  ASSERT_EQ(typed_array_clear(a), RET_OK);
  ASSERT_EQ(a->size, 0);

  typed_array_destroy(a);
}

TEST(TypedArray, insert_double) {
  value_t v;
  uint8_t i = 0;
  typed_array_t* a = typed_array_create(VALUE_TYPE_DOUBLE, 10);
  ASSERT_EQ(a != NULL, true);
  ASSERT_EQ(a->element_size, sizeof(double_t));
  for (i = 0; i < 255; i++) {
    ASSERT_EQ(a->size, i);
    ASSERT_EQ(typed_array_insert(a, 0, value_set_double(&v, i)), RET_OK);
    ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
    ASSERT_EQ(value_double(&v), i);
  }

  ASSERT_EQ(typed_array_set(a, 10, value_set_double(&v, 0x112233)), RET_OK);
  ASSERT_EQ(typed_array_get(a, 10, &v), RET_OK);
  ASSERT_EQ(value_double(&v), 0x112233);
  ASSERT_EQ(typed_array_clear(a), RET_OK);
  ASSERT_EQ(a->size, 0);

  typed_array_destroy(a);
}

TEST(TypedArray, insert_uint32) {
  value_t v;
  uint8_t i = 0;
  typed_array_t* a = typed_array_create(VALUE_TYPE_UINT32, 0);
  ASSERT_EQ(a != NULL, true);
  ASSERT_EQ(a->element_size, sizeof(uint32_t));
  for (i = 0; i < 255; i++) {
    ASSERT_EQ(a->size, i);
    ASSERT_EQ(typed_array_insert(a, 0, value_set_uint32(&v, i)), RET_OK);
    ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
    ASSERT_EQ(value_uint32(&v), i);
  }

  ASSERT_EQ(typed_array_set(a, 10, value_set_uint32(&v, 0x11223344)), RET_OK);
  ASSERT_EQ(typed_array_get(a, 10, &v), RET_OK);
  ASSERT_EQ(value_uint32(&v), 0x11223344);
  ASSERT_EQ(typed_array_clear(a), RET_OK);
  ASSERT_EQ(a->size, 0);

  typed_array_destroy(a);
}

TEST(TypedArray, object1) {
  value_t v;
  uint8_t i = 0;
  object_t* obj = object_typed_array_create(VALUE_TYPE_UINT32, 0);
  typed_array_t* a = OBJECT_TYPED_ARRAY(obj)->arr;

  ASSERT_EQ(a != NULL, true);
  ASSERT_EQ(a->element_size, sizeof(uint32_t));
  for (i = 0; i < 255; i++) {
    ASSERT_EQ(a->size, i);
    ASSERT_EQ(typed_array_insert(a, 0, value_set_uint32(&v, i)), RET_OK);
    ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
    ASSERT_EQ(value_uint32(&v), i);
  }

  ASSERT_EQ(object_get_prop_int(obj, "size", 0), i);
  ASSERT_EQ(object_get_prop_int(obj, "bytes", 0), i * sizeof(uint32_t));
  ASSERT_EQ(object_get_prop_pointer(obj, "data"), (void*)a->data);

  OBJECT_UNREF(obj);
}
