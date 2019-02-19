#include "tkc/darray.h"
#include "gtest/gtest.h"

#include <math.h>
#include <stdlib.h>

#define NR 1000

static void test_add(darray_t* darray, void* pv) {
  ASSERT_EQ(RET_OK, darray_push(darray, pv));
  ASSERT_EQ(pv, darray_find(darray, pv));
}

TEST(DArrayTest, init) {
  darray_t darray;
  darray_init(&darray, 10, NULL, NULL);

  ASSERT_EQ(darray.size, 0);
  ASSERT_EQ(darray.capacity, 10);

  darray_deinit(&darray);
}

TEST(DArrayTest, create) {
  darray_t* darray = darray_create(10, NULL, NULL);
  ASSERT_EQ(darray->size, 0);
  ASSERT_EQ(darray->capacity, 10);
  darray_destroy(darray);
}

TEST(DArrayTest, stack) {
  char* p = NULL;
  darray_t* darray = darray_create(10, NULL, NULL);
  ASSERT_EQ(darray->size, 0);
  ASSERT_EQ(darray->capacity, 10);

  ASSERT_EQ(darray_tail(darray), p);
  ASSERT_EQ(darray_head(darray), p);

  ASSERT_EQ(RET_OK, darray_push(darray, p + 1));
  ASSERT_EQ(darray_tail(darray), p + 1);
  ASSERT_EQ(darray_head(darray), p + 1);
  ASSERT_EQ(1, darray_count(darray, p + 1));

  ASSERT_EQ(darray_pop(darray), p + 1);
  ASSERT_EQ(darray->size, 0);

  darray_destroy(darray);
}

TEST(DArrayTest, basic) {
  char* p = NULL;
  darray_t* darray = darray_create(10, NULL, NULL);
  ASSERT_EQ(darray->size, 0);
  ASSERT_EQ(darray->capacity, 10);

  ASSERT_EQ(RET_OK, darray_push(darray, p + 1));
  ASSERT_EQ(1, darray_count(darray, p + 1));

  ASSERT_EQ(RET_OK, darray_push(darray, p + 1));
  ASSERT_EQ(2, darray_count(darray, p + 1));

  ASSERT_EQ(RET_OK, darray_remove(darray, p + 1));
  ASSERT_EQ(1, darray_count(darray, p + 1));

  ASSERT_EQ(RET_OK, darray_push(darray, p + 1));
  ASSERT_EQ(2, darray_count(darray, p + 1));

  ASSERT_EQ(RET_OK, darray_remove_all(darray, p + 1));
  ASSERT_EQ(darray->size, 0);
  ASSERT_EQ(0, darray_count(darray, p + 1));

  ASSERT_EQ(RET_OK, darray_push(darray, p + 1));
  ASSERT_EQ(RET_OK, darray_push(darray, p + 2));
  ASSERT_EQ(RET_OK, darray_push(darray, p + 3));
  ASSERT_EQ(RET_OK, darray_clear(darray));

  ASSERT_EQ(darray->size, 0);

  darray_destroy(darray);
}

TEST(DArrayTest, popPush) {
  uint32_t i = 0;
  char* p = NULL;
  darray_t darray;
  int cases[] = {1, 2, 13, 14, 12, 41, 34, 34, 5, 563, 12, 1, 2, 33};
  darray_init(&darray, 10, NULL, NULL);

  for (i = 0; i < ARRAY_SIZE(cases); i++) {
    ASSERT_EQ(RET_OK, darray_push(&darray, p + cases[i]));
    ASSERT_EQ(i + 1, darray.size);
  }

  for (i = 0; i < ARRAY_SIZE(cases); i++) {
    int k = ARRAY_SIZE(cases) - i - 1;
    void* data = darray_pop(&darray);
    ASSERT_EQ(data, p + cases[k]);
    ASSERT_EQ(k, darray.size);
  }

  darray_deinit(&darray);
}

TEST(DArrayTest, random) {
  int i = 0;
  int n = NR;
  char* p = NULL;
  darray_t darray;
  darray_init(&darray, 10, NULL, NULL);

  for (i = 0; i < n; i++) {
    long v = random() + 1;
    void* pv = p + v;
    ASSERT_EQ(darray.size, i);
    test_add(&darray, pv);
  }

  for (i = 0; i < n; i++) {
    long k = random() % darray.size;
    void* pv = darray.elms[k];

    ASSERT_EQ(RET_OK, darray_remove(&darray, pv));
  }
  ASSERT_EQ(darray.size, 0);

  darray_deinit(&darray);
}

TEST(DArrayTest, inc) {
  int i = 0;
  int n = NR;
  char* p = NULL;
  darray_t darray;
  darray_init(&darray, 10, NULL, NULL);

  for (i = 0; i < n; i++) {
    void* pv = p + i;
    ASSERT_EQ(darray.size, i);
    test_add(&darray, pv);
  }

  for (i = 0; i < n; i++) {
    long k = random() % darray.size;
    void* pv = darray.elms[k];

    ASSERT_EQ(RET_OK, darray_remove(&darray, pv));
  }
  ASSERT_EQ(darray.size, 0);

  darray_deinit(&darray);
}

TEST(DArrayTest, dec) {
  int i = 0;
  char* p = NULL;
  int n = NR;
  darray_t darray;
  darray_init(&darray, 10, NULL, NULL);

  for (i = 0; i < n; i++) {
    void* pv = p + (n - i);
    ASSERT_EQ(darray.size, i);
    test_add(&darray, pv);
  }

  for (i = 0; i < n; i++) {
    long k = random() % darray.size;
    void* pv = darray.elms[k];

    ASSERT_EQ(pv, darray_find(&darray, pv));
    ASSERT_EQ(RET_OK, darray_remove(&darray, pv));
    ASSERT_EQ(NULL, darray_find(&darray, pv));
  }
  ASSERT_EQ(darray.size, 0);

  darray_deinit(&darray);
}

TEST(DArrayTest, removeAll) {
  uint32_t i = 0;
  char* p = NULL;
  darray_t darray;
  int cases[] = {1, 2, 2, 2, 13, 14, 12, 41, 34, 34, 5, 563, 12, 1, 2, 33};
  darray_init(&darray, 10, NULL, NULL);

  for (i = 0; i < ARRAY_SIZE(cases); i++) {
    ASSERT_EQ(RET_OK, darray_push(&darray, p + cases[i]));
    ASSERT_EQ(i + 1, darray.size);
  }

  ASSERT_EQ(darray.size, ARRAY_SIZE(cases));
  ASSERT_EQ(darray_remove_all(&darray, p + 2), RET_OK);
  ASSERT_EQ(darray.size, ARRAY_SIZE(cases) - 4);

  ASSERT_EQ(darray_remove_all(&darray, p + 1), RET_OK);
  ASSERT_EQ(darray.size, ARRAY_SIZE(cases) - 4 - 2);

  ASSERT_EQ(darray_remove_all(&darray, p + 12), RET_OK);
  ASSERT_EQ(darray.size, ARRAY_SIZE(cases) - 4 - 2 - 2);

  ASSERT_EQ(darray_remove_all(&darray, p + 13), RET_OK);
  ASSERT_EQ(darray.size, ARRAY_SIZE(cases) - 4 - 2 - 2 - 1);

  ASSERT_EQ(darray_remove_all(&darray, p + 14), RET_OK);
  ASSERT_EQ(darray.size, ARRAY_SIZE(cases) - 4 - 2 - 2 - 1 - 1);

  ASSERT_EQ(darray_remove_all(&darray, p + 33), RET_OK);
  ASSERT_EQ(darray.size, ARRAY_SIZE(cases) - 4 - 2 - 2 - 1 - 1 - 1);

  ASSERT_EQ(darray_remove_all(&darray, p + 34), RET_OK);
  ASSERT_EQ(darray.size, ARRAY_SIZE(cases) - 4 - 2 - 2 - 1 - 1 - 1 - 2);

  ASSERT_EQ(darray_remove_all(&darray, p + 0), RET_OK);
  ASSERT_EQ(darray.size, ARRAY_SIZE(cases) - 4 - 2 - 2 - 1 - 1 - 1 - 2);

  darray_deinit(&darray);
}
