#include "base/array.h"
#include "gtest/gtest.h"

#include <math.h>
#include <stdlib.h>

#ifdef WIN32
#define random rand
#endif

#define NR 1000

static int pointer_cmp(const void* p1, const void* p2) {
  const char* p = NULL;
  long v1 = (const char*)p1 - p;
  long v2 = (const char*)p2 - p;

  return v1 - v2;
}

static void test_add(array_t* array, void* pv) {
  ASSERT_EQ(TRUE, array_push(array, pv));
  ASSERT_EQ(pv, array_find(array, pointer_cmp, pv));
}

TEST(OArrayTest, popPush) {
  uint32_t i = 0;
  char* p = NULL;
  array_t array;
  int cases[] = {1, 2, 13, 14, 12, 41, 34, 34, 5, 563, 12, 1, 2, 33};
  array_init(&array, 10);

  for (i = 0; i < ARRAY_SIZE(cases); i++) {
    ASSERT_EQ(TRUE, array_push(&array, p + cases[i]));
    ASSERT_EQ(i + 1, array.size);
  }

  for (i = 0; i < ARRAY_SIZE(cases); i++) {
    int k = ARRAY_SIZE(cases) - i - 1;
    void* data = array_pop(&array);
    ASSERT_EQ(data, p + cases[k]);
    ASSERT_EQ(k, array.size);
  }

  array_deinit(&array);
}

TEST(OArrayTest, random) {
  int i = 0;
  int n = NR;
  char* p = NULL;
  array_t array;
  array_init(&array, 10);

  for (i = 0; i < n; i++) {
    long v = random() + 1;
    void* pv = p + v;
    ASSERT_EQ(array.size, i);
    test_add(&array, pv);
  }

  for (i = 0; i < n; i++) {
    long k = random() % array.size;
    void* pv = array.elms[k];

    ASSERT_EQ(TRUE, array_remove(&array, pointer_cmp, pv));
  }
  ASSERT_EQ(array.size, 0);

  array_deinit(&array);
}

TEST(OArrayTest, inc) {
  int i = 0;
  int n = NR;
  char* p = NULL;
  array_t array;
  array_init(&array, 10);

  for (i = 0; i < n; i++) {
    void* pv = p + i;
    ASSERT_EQ(array.size, i);
    test_add(&array, pv);
  }

  for (i = 0; i < n; i++) {
    long k = random() % array.size;
    void* pv = array.elms[k];

    ASSERT_EQ(TRUE, array_remove(&array, pointer_cmp, pv));
  }
  ASSERT_EQ(array.size, 0);

  array_deinit(&array);
}

TEST(OArrayTest, dec) {
  int i = 0;
  char* p = NULL;
  int n = NR;
  array_t array;
  array_init(&array, 10);

  for (i = 0; i < n; i++) {
    void* pv = p + (n - i);
    ASSERT_EQ(array.size, i);
    test_add(&array, pv);
  }

  for (i = 0; i < n; i++) {
    long k = random() % array.size;
    void* pv = array.elms[k];

    ASSERT_EQ(pv, array_find(&array, pointer_cmp, pv));
    ASSERT_EQ(TRUE, array_remove(&array, pointer_cmp, pv));
    ASSERT_EQ(NULL, array_find(&array, pointer_cmp, pv));
  }
  ASSERT_EQ(array.size, 0);

  array_deinit(&array);
}

TEST(OArrayTest, removeAll) {
  uint32_t i = 0;
  char* p = NULL;
  array_t array;
  int cases[] = {1, 2, 2, 2, 13, 14, 12, 41, 34, 34, 5, 563, 12, 1, 2, 33};
  array_init(&array, 10);

  for (i = 0; i < ARRAY_SIZE(cases); i++) {
    ASSERT_EQ(TRUE, array_push(&array, p + cases[i]));
    ASSERT_EQ(i + 1, array.size);
  }

  ASSERT_EQ(array.size, ARRAY_SIZE(cases));
  ASSERT_EQ(array_remove_all(&array, NULL, p + 2, NULL), TRUE);
  ASSERT_EQ(array.size, ARRAY_SIZE(cases) - 4);

  ASSERT_EQ(array_remove_all(&array, NULL, p + 1, NULL), TRUE);
  ASSERT_EQ(array.size, ARRAY_SIZE(cases) - 4 - 2);

  ASSERT_EQ(array_remove_all(&array, NULL, p + 12, NULL), TRUE);
  ASSERT_EQ(array.size, ARRAY_SIZE(cases) - 4 - 2 - 2);

  ASSERT_EQ(array_remove_all(&array, NULL, p + 13, NULL), TRUE);
  ASSERT_EQ(array.size, ARRAY_SIZE(cases) - 4 - 2 - 2 - 1);

  ASSERT_EQ(array_remove_all(&array, NULL, p + 14, NULL), TRUE);
  ASSERT_EQ(array.size, ARRAY_SIZE(cases) - 4 - 2 - 2 - 1 - 1);

  ASSERT_EQ(array_remove_all(&array, NULL, p + 33, NULL), TRUE);
  ASSERT_EQ(array.size, ARRAY_SIZE(cases) - 4 - 2 - 2 - 1 - 1 - 1);

  ASSERT_EQ(array_remove_all(&array, NULL, p + 34, NULL), TRUE);
  ASSERT_EQ(array.size, ARRAY_SIZE(cases) - 4 - 2 - 2 - 1 - 1 - 1 - 2);

  ASSERT_EQ(array_remove_all(&array, NULL, p + 0, NULL), TRUE);
  ASSERT_EQ(array.size, ARRAY_SIZE(cases) - 4 - 2 - 2 - 1 - 1 - 1 - 2);

  array_deinit(&array);
}
