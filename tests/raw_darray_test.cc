#include "gtest/gtest.h"
#include "tkc/raw_darray.h"

TEST(RawDarray, basic) {
  TK_RAW_DARRAY_T(int) arr = TK_RAW_DARRAY_CREATE(int, 4);
  ASSERT_TRUE(arr != NULL);
  ASSERT_EQ(TK_RAW_DARRAY_SIZE(arr), 0u);

  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 0), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 1), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 2), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 3), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_SIZE(arr), 4);

  for (size_t i = 0; i < TK_RAW_DARRAY_SIZE(arr); i++) {
    ASSERT_EQ(arr[i], i);
    arr[i] *= 10;
  }

  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 40), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 50), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 60), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 70), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 80), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 90), RET_OK);

  for (size_t i = 0; i < TK_RAW_DARRAY_SIZE(arr); i++) {
    ASSERT_EQ(arr[i], i * 10);
  }

  ASSERT_EQ(TK_RAW_DARRAY_DESTROY(arr), RET_OK);
}

TEST(RawDarray, remove) {
  TK_RAW_DARRAY_T(int) arr = TK_RAW_DARRAY_CREATE(int, 4);
  ASSERT_TRUE(arr != NULL);

  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 0), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 1), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 2), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 3), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_SIZE(arr), 4);

  TK_RAW_DARRAY_REMOVE(arr, 1);
  ASSERT_EQ(TK_RAW_DARRAY_SIZE(arr), 3);

  int temp[] = {0, 2, 3, 4};
  for (size_t i = 0; i < TK_RAW_DARRAY_SIZE(arr); i++) {
    ASSERT_EQ(arr[i], temp[i]);
  }

  ASSERT_EQ(TK_RAW_DARRAY_DESTROY(arr), RET_OK);
}

TEST(RawDarray, insert) {
  TK_RAW_DARRAY_T(int) arr = TK_RAW_DARRAY_CREATE(int, 4);
  ASSERT_TRUE(arr != NULL);

  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 0), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 1), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 2), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 3), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_SIZE(arr), 4);

  TK_RAW_DARRAY_INSERT(arr, 3, 33);
  ASSERT_EQ(TK_RAW_DARRAY_SIZE(arr), 5);

  int temp[] = {0, 1, 2, 33, 3, 4};
  for (size_t i = 0; i < TK_RAW_DARRAY_SIZE(arr); i++) {
    ASSERT_EQ(arr[i], temp[i]);
  }

  ASSERT_EQ(TK_RAW_DARRAY_DESTROY(arr), RET_OK);
}

TEST(RawDarray, memset) {
  TK_RAW_DARRAY_T(int) arr = TK_RAW_DARRAY_CREATE(int, 4);
  ASSERT_TRUE(arr != NULL);

  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 0), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 1), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 2), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 3), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_SIZE(arr), 4);

  TK_RAW_DARRAY_RESIZE(arr, 100);
  ASSERT_EQ(TK_RAW_DARRAY_SIZE(arr), 100);

  memset(arr, 0, TK_RAW_DARRAY_SIZE(arr) * sizeof(*arr));

  int temp[100] = {0};
  ASSERT_EQ(memcmp(arr, temp, TK_RAW_DARRAY_SIZE(arr)), 0);

  ASSERT_EQ(TK_RAW_DARRAY_DESTROY(arr), RET_OK);
}

TEST(RawDarray, memcpy) {
  TK_RAW_DARRAY_T(int) arr = TK_RAW_DARRAY_CREATE(int, 0);
  ASSERT_TRUE(arr != NULL);

  int temp[] = {0, 1, 2, 3, 4};
  TK_RAW_DARRAY_RESIZE(arr, ARRAY_SIZE(temp));
  memcpy(arr, temp, ARRAY_SIZE(temp) * sizeof(*temp));

  for (size_t i = 0; i < TK_RAW_DARRAY_SIZE(arr); i++) {
    ASSERT_EQ(arr[i], temp[i]);
  }

  ASSERT_EQ(TK_RAW_DARRAY_DESTROY(arr), RET_OK);
}

static void raw_darray_func_arg_test(TK_RAW_DARRAY_T(int) * arr) {
  return_if_fail(arr != NULL && *arr != NULL);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(*arr, 40), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(*arr, 50), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(*arr, 60), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(*arr, 70), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(*arr, 80), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(*arr, 90), RET_OK);
}

TEST(RawDarray, func_arg) {
  TK_RAW_DARRAY_T(int) arr = TK_RAW_DARRAY_CREATE(int, 4);
  ASSERT_TRUE(arr != NULL);
  ASSERT_EQ(TK_RAW_DARRAY_SIZE(arr), 0u);

  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 0), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 1), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 2), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_PUSH(arr, 3), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_SIZE(arr), 4);

  for (size_t i = 0; i < TK_RAW_DARRAY_SIZE(arr); i++) {
    ASSERT_EQ(arr[i], i);
    arr[i] *= 10;
  }

  raw_darray_func_arg_test(&arr);

  for (size_t i = 0; i < TK_RAW_DARRAY_SIZE(arr); i++) {
    ASSERT_EQ(arr[i], i * 10);
  }

  ASSERT_EQ(TK_RAW_DARRAY_DESTROY(arr), RET_OK);
}
