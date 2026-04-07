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
  ASSERT_EQ(a->size, 3u);

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
  ASSERT_EQ(a->size, 2u);

  ASSERT_EQ(typed_array_remove(a, 0), RET_OK);
  ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
  ASSERT_EQ(value_uint8(&v), 230);
  ASSERT_EQ(a->size, 1u);

  ASSERT_EQ(typed_array_remove(a, 0), RET_OK);
  ASSERT_EQ(a->size, 0u);
  ASSERT_EQ(typed_array_remove(a, 0), RET_BAD_PARAMS);
  ASSERT_EQ(a->size, 0u);

  ASSERT_EQ(typed_array_tail(a, &v), RET_BAD_PARAMS);

  typed_array_destroy(a);
}

TEST(TypedArray, push) {
  value_t v;
  uint8_t i = 0;
  typed_array_t* a = typed_array_create(VALUE_TYPE_UINT8, 0);
  ASSERT_EQ(a != NULL, true);
  for (i = 0; i < 255; i++) {
    ASSERT_EQ(a->size, (uint32_t)i);
    ASSERT_EQ(typed_array_push(a, value_set_uint8(&v, i)), RET_OK);
    ASSERT_EQ(typed_array_tail(a, &v), RET_OK);
    ASSERT_EQ(value_uint8(&v), i);
  }

  for (; i > 0; i--) {
    ASSERT_EQ(a->size, (uint32_t)i);
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
    ASSERT_EQ(a->size, (uint32_t)i);
    ASSERT_EQ(typed_array_insert(a, 0, value_set_uint8(&v, i)), RET_OK);
    ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
    ASSERT_EQ(value_uint8(&v), i);
  }

  ASSERT_EQ(typed_array_clear(a), RET_OK);
  ASSERT_EQ(a->size, 0u);

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
    ASSERT_EQ(a->size, (uint32_t)i);
    ASSERT_EQ(typed_array_insert(a, 0, value_set_uint64(&v, i)), RET_OK);
    ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
    ASSERT_EQ(value_uint64(&v), (uint64_t)i);
  }

  ASSERT_EQ(typed_array_set(a, 10, value_set_uint64(&v, 0x1122334455667788)), RET_OK);
  ASSERT_EQ(typed_array_get(a, 10, &v), RET_OK);
  ASSERT_EQ(value_uint64(&v), 0x1122334455667788u);
  ASSERT_EQ(typed_array_clear(a), RET_OK);
  ASSERT_EQ(a->size, 0u);

  typed_array_destroy(a);
}

TEST(TypedArray, insert_double) {
  value_t v;
  uint8_t i = 0;
  typed_array_t* a = typed_array_create(VALUE_TYPE_DOUBLE, 10);
  ASSERT_EQ(a != NULL, true);
  ASSERT_EQ(a->element_size, sizeof(double_t));
  for (i = 0; i < 255; i++) {
    ASSERT_EQ(a->size, (uint32_t)i);
    ASSERT_EQ(typed_array_insert(a, 0, value_set_double(&v, i)), RET_OK);
    ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
    ASSERT_EQ(value_double(&v), (double)i);
  }

  ASSERT_EQ(typed_array_set(a, 10, value_set_double(&v, 0x112233)), RET_OK);
  ASSERT_EQ(typed_array_get(a, 10, &v), RET_OK);
  ASSERT_EQ(value_double(&v), (double)0x112233);
  ASSERT_EQ(typed_array_clear(a), RET_OK);
  ASSERT_EQ(a->size, 0u);

  typed_array_destroy(a);
}

TEST(TypedArray, insert_uint32) {
  value_t v;
  uint8_t i = 0;
  typed_array_t* a = typed_array_create(VALUE_TYPE_UINT32, 0);
  ASSERT_EQ(a != NULL, true);
  ASSERT_EQ(a->element_size, sizeof(uint32_t));
  for (i = 0; i < 255; i++) {
    ASSERT_EQ(a->size, (uint32_t)i);
    ASSERT_EQ(typed_array_insert(a, 0, value_set_uint32(&v, i)), RET_OK);
    ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
    ASSERT_EQ(value_uint32(&v), (uint32_t)i);
  }

  ASSERT_EQ(typed_array_set(a, 10, value_set_uint32(&v, 0x11223344)), RET_OK);
  ASSERT_EQ(typed_array_get(a, 10, &v), RET_OK);
  ASSERT_EQ(value_uint32(&v), 0x11223344u);
  ASSERT_EQ(typed_array_clear(a), RET_OK);
  ASSERT_EQ(a->size, 0u);

  typed_array_destroy(a);
}

TEST(TypedArray, object1) {
  value_t v;
  uint8_t i = 0;
  tk_object_t* obj = object_typed_array_create(VALUE_TYPE_UINT32, 0);
  typed_array_t* a = OBJECT_TYPED_ARRAY(obj)->arr;

  ASSERT_EQ(a != NULL, true);
  ASSERT_EQ(a->element_size, sizeof(uint32_t));
  for (i = 0; i < 255; i++) {
    ASSERT_EQ(a->size, (uint32_t)i);
    ASSERT_EQ(typed_array_insert(a, 0, value_set_uint32(&v, i)), RET_OK);
    ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
    ASSERT_EQ(value_uint32(&v), (uint32_t)i);
  }

  ASSERT_EQ(tk_object_get_prop_int(obj, "size", 0), (int32_t)i);
  ASSERT_EQ(tk_object_get_prop_int(obj, "bytes", 0), (int32_t)(i * sizeof(uint32_t)));
  ASSERT_EQ(tk_object_get_prop_pointer(obj, "data"), (void*)a->data);

  TK_OBJECT_UNREF(obj);
}

TEST(TypedArray, object2) {
  value_t v;
  uint8_t i = 0;
  tk_object_t* obj = object_typed_array_create(VALUE_TYPE_UINT32, 0);
  typed_array_t* a = OBJECT_TYPED_ARRAY(obj)->arr;

  ASSERT_EQ(a != NULL, true);
  ASSERT_EQ(a->element_size, sizeof(uint32_t));
  for (i = 0; i < 255; i++) {
    ASSERT_EQ(a->size, (uint32_t)i);
    ASSERT_EQ(typed_array_insert(a, 0, value_set_uint32(&v, i)), RET_OK);
    ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
    ASSERT_EQ(value_uint32(&v), (uint32_t)i);
  }

  ASSERT_EQ(tk_object_get_prop_int(obj, TK_OBJECT_PROP_SIZE, 0), (int32_t)i);
  ASSERT_EQ(tk_object_set_prop_int(obj, "[0]", 123), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(obj, "[0]", 0), 123);
  ASSERT_EQ(tk_object_set_prop_int(obj, "[1]", 1234), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(obj, "[1]", 0), 1234);

  TK_OBJECT_UNREF(obj);
}

TEST(TypedArray, insert_bool) {
  value_t v;
  uint8_t i = 0;
  typed_array_t* a = typed_array_create(VALUE_TYPE_BOOL, 0);
  ASSERT_EQ(a != NULL, true);
  ASSERT_EQ(a->element_size, sizeof(uint8_t));
  for (i = 0; i < 255; i++) {
    ASSERT_EQ(a->size, (uint32_t)i);
    ASSERT_EQ(typed_array_insert(a, 0, value_set_bool(&v, i % 2 == 1)), RET_OK);
    ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
    ASSERT_EQ(value_bool(&v), i % 2 == 1);
  }

  ASSERT_EQ(typed_array_clear(a), RET_OK);
  ASSERT_EQ(a->size, 0u);

  typed_array_destroy(a);
}

TEST(TypedArray, init_deinit_stack) {
  value_t v;
  typed_array_t a;

  ASSERT_EQ(typed_array_init(&a, VALUE_TYPE_INT32, 4), &a);
  ASSERT_EQ(a.type, VALUE_TYPE_INT32);
  ASSERT_EQ(a.element_size, (uint32_t)sizeof(int32_t));
  ASSERT_EQ(a.capacity, 4u);
  ASSERT_EQ(a.size, 0u);
  ASSERT_NE(a.data, nullptr);

  ASSERT_EQ(typed_array_push(&a, value_set_int32(&v, 7)), RET_OK);
  ASSERT_EQ(typed_array_push(&a, value_set_int32(&v, -3)), RET_OK);
  ASSERT_EQ(a.size, 2u);
  ASSERT_EQ(typed_array_get(&a, 0, &v), RET_OK);
  ASSERT_EQ(value_int32(&v), 7);
  ASSERT_EQ(typed_array_get(&a, 1, &v), RET_OK);
  ASSERT_EQ(value_int32(&v), -3);

  ASSERT_EQ(typed_array_deinit(&a), RET_OK);
  ASSERT_EQ(a.size, 0u);
  ASSERT_EQ(a.capacity, 0u);
  ASSERT_EQ(a.data, nullptr);
}

TEST(TypedArray, init_invalid) {
  typed_array_t a;

  ASSERT_EQ(typed_array_init(NULL, VALUE_TYPE_INT32, 0), nullptr);
  ASSERT_EQ(typed_array_init(&a, VALUE_TYPE_STRING, 0), nullptr);
}

TEST(TypedArray, extend_grow_and_noop) {
  value_t v;
  typed_array_t* a = typed_array_create(VALUE_TYPE_UINT32, 0);
  ASSERT_NE(a, nullptr);

  ASSERT_EQ(typed_array_push(a, value_set_uint32(&v, 11)), RET_OK);
  ASSERT_EQ(typed_array_push(a, value_set_uint32(&v, 22)), RET_OK);
  ASSERT_EQ(a->size, 2u);
  uint32_t cap_before = a->capacity;
  ASSERT_GT(cap_before, 1u);

  ASSERT_EQ(typed_array_extend(a, 1u), RET_OK);
  ASSERT_EQ(a->capacity, cap_before);
  ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
  ASSERT_EQ(value_uint32(&v), 11u);
  ASSERT_EQ(typed_array_get(a, 1, &v), RET_OK);
  ASSERT_EQ(value_uint32(&v), 22u);

  ASSERT_EQ(typed_array_extend(a, 200u), RET_OK);
  ASSERT_EQ(a->capacity, 200u);
  ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
  ASSERT_EQ(value_uint32(&v), 11u);
  ASSERT_EQ(typed_array_get(a, 1, &v), RET_OK);
  ASSERT_EQ(value_uint32(&v), 22u);

  typed_array_destroy(a);
}

/* Mirrors typed_array_extend_delta: new_cap = (size + 1) * 1.2F when growing.
 * Catches accidental revert to (size + 1) + 1.2F (e.g. cap after 3 pushes would be 4 vs 3). */
TEST(TypedArray, extend_delta_growth_formula) {
  value_t v;
  typed_array_t* a = typed_array_create(VALUE_TYPE_UINT32, 0);
  ASSERT_NE(a, nullptr);

  for (uint32_t n = 0; n < 20; n++) {
    uint32_t sz_before = a->size;
    uint32_t cap_before = a->capacity;
    uint32_t expected_cap =
        (sz_before + 1u > cap_before) ? (uint32_t)((sz_before + 1u) * 1.2F) : cap_before;

    ASSERT_EQ(typed_array_push(a, value_set_uint32(&v, n)), RET_OK);
    ASSERT_EQ(a->size, sz_before + 1u);
    ASSERT_EQ(a->capacity, expected_cap);
    ASSERT_EQ(typed_array_get(a, sz_before, &v), RET_OK);
    ASSERT_EQ(value_uint32(&v), n);
  }

  typed_array_destroy(a);
}

TEST(TypedArray, insert_clamp_index) {
  value_t v;
  typed_array_t* a = typed_array_create(VALUE_TYPE_UINT32, 0);
  ASSERT_NE(a, nullptr);

  ASSERT_EQ(typed_array_insert(a, 999u, value_set_uint32(&v, 100)), RET_OK);
  ASSERT_EQ(a->size, 1u);
  ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
  ASSERT_EQ(value_uint32(&v), 100u);

  ASSERT_EQ(typed_array_push(a, value_set_uint32(&v, 200)), RET_OK);
  ASSERT_EQ(typed_array_push(a, value_set_uint32(&v, 300)), RET_OK);
  ASSERT_EQ(a->size, 3u);

  ASSERT_EQ(typed_array_insert(a, 1000u, value_set_uint32(&v, 400)), RET_OK);
  ASSERT_EQ(a->size, 4u);
  ASSERT_EQ(typed_array_get(a, 2, &v), RET_OK);
  ASSERT_EQ(value_uint32(&v), 300u);
  ASSERT_EQ(typed_array_get(a, 3, &v), RET_OK);
  ASSERT_EQ(value_uint32(&v), 400u);

  typed_array_destroy(a);
}

TEST(TypedArray, bad_params) {
  value_t v;
  typed_array_t* a = typed_array_create(VALUE_TYPE_UINT32, 0);
  ASSERT_NE(a, nullptr);

  ASSERT_EQ(typed_array_get(NULL, 0, &v), RET_BAD_PARAMS);
  ASSERT_EQ(typed_array_get(a, 0, NULL), RET_BAD_PARAMS);
  ASSERT_EQ(typed_array_get(a, 0, &v), RET_BAD_PARAMS);

  ASSERT_EQ(typed_array_set(NULL, 0, &v), RET_BAD_PARAMS);
  ASSERT_EQ(typed_array_set(a, 0, NULL), RET_BAD_PARAMS);
  ASSERT_EQ(typed_array_set(a, 0, &v), RET_BAD_PARAMS);

  ASSERT_EQ(typed_array_insert(NULL, 0, &v), RET_BAD_PARAMS);
  ASSERT_EQ(typed_array_insert(a, 0, NULL), RET_BAD_PARAMS);

  ASSERT_EQ(typed_array_remove(NULL, 0), RET_BAD_PARAMS);
  ASSERT_EQ(typed_array_remove(a, 0), RET_BAD_PARAMS);

  ASSERT_EQ(typed_array_pop(NULL, &v), RET_BAD_PARAMS);
  ASSERT_EQ(typed_array_pop(a, NULL), RET_BAD_PARAMS);
  ASSERT_EQ(typed_array_pop(a, &v), RET_BAD_PARAMS);

  ASSERT_EQ(typed_array_tail(NULL, &v), RET_BAD_PARAMS);
  ASSERT_EQ(typed_array_tail(a, NULL), RET_BAD_PARAMS);
  ASSERT_EQ(typed_array_tail(a, &v), RET_BAD_PARAMS);

  ASSERT_EQ(typed_array_push(NULL, &v), RET_BAD_PARAMS);
  ASSERT_EQ(typed_array_push(a, NULL), RET_BAD_PARAMS);

  ASSERT_EQ(typed_array_extend(NULL, 10), RET_BAD_PARAMS);
  ASSERT_EQ(typed_array_clear(NULL), RET_BAD_PARAMS);
  ASSERT_EQ(typed_array_deinit(NULL), RET_BAD_PARAMS);
  ASSERT_EQ(typed_array_destroy(NULL), RET_BAD_PARAMS);

  ASSERT_EQ(typed_array_push(a, value_set_uint32(&v, 1)), RET_OK);
  ASSERT_EQ(typed_array_get(a, 10, &v), RET_BAD_PARAMS);
  ASSERT_EQ(typed_array_set(a, 10, value_set_uint32(&v, 2)), RET_BAD_PARAMS);
  ASSERT_EQ(typed_array_remove(a, 1), RET_BAD_PARAMS);

  typed_array_destroy(a);
}

TEST(TypedArray, scalar_types_roundtrip) {
  value_t v;
  typed_array_t* a;

  a = typed_array_create(VALUE_TYPE_INT8, 0);
  ASSERT_NE(a, nullptr);
  ASSERT_EQ(typed_array_push(a, value_set_int8(&v, -42)), RET_OK);
  ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
  ASSERT_EQ(value_int8(&v), -42);
  typed_array_destroy(a);

  a = typed_array_create(VALUE_TYPE_INT16, 0);
  ASSERT_NE(a, nullptr);
  ASSERT_EQ(typed_array_push(a, value_set_int16(&v, -10000)), RET_OK);
  ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
  ASSERT_EQ(value_int16(&v), -10000);
  typed_array_destroy(a);

  a = typed_array_create(VALUE_TYPE_UINT16, 0);
  ASSERT_NE(a, nullptr);
  ASSERT_EQ(typed_array_push(a, value_set_uint16(&v, 50000)), RET_OK);
  ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
  ASSERT_EQ(value_uint16(&v), 50000);
  typed_array_destroy(a);

  a = typed_array_create(VALUE_TYPE_INT32, 0);
  ASSERT_NE(a, nullptr);
  ASSERT_EQ(typed_array_push(a, value_set_int32(&v, -2000000000)), RET_OK);
  ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
  ASSERT_EQ(value_int32(&v), -2000000000);
  typed_array_destroy(a);

  a = typed_array_create(VALUE_TYPE_INT64, 0);
  ASSERT_NE(a, nullptr);
  ASSERT_EQ(typed_array_push(a, value_set_int64(&v, -((int64_t)1 << 40))), RET_OK);
  ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
  ASSERT_EQ(value_int64(&v), -((int64_t)1 << 40));
  typed_array_destroy(a);

  a = typed_array_create(VALUE_TYPE_FLOAT, 0);
  ASSERT_NE(a, nullptr);
  ASSERT_EQ(typed_array_push(a, value_set_float(&v, (float_t)2.5)), RET_OK);
  ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
  ASSERT_DOUBLE_EQ((double)value_float(&v), 2.5);
  typed_array_destroy(a);

  a = typed_array_create(VALUE_TYPE_FLOAT32, 0);
  ASSERT_NE(a, nullptr);
  ASSERT_EQ(typed_array_push(a, value_set_float32(&v, 3.125f)), RET_OK);
  ASSERT_EQ(typed_array_get(a, 0, &v), RET_OK);
  ASSERT_FLOAT_EQ(value_float32(&v), 3.125f);
  typed_array_destroy(a);
}
