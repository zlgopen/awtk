#include "tkc/darray.h"
#include "tkc/utils.h"
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

  ASSERT_EQ(RET_OK, darray_remove_all(darray, NULL, p + 1));
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
  ASSERT_EQ(darray_remove_all(&darray, NULL, p + 2), RET_OK);
  ASSERT_EQ(darray.size, ARRAY_SIZE(cases) - 4);

  ASSERT_EQ(darray_remove_all(&darray, NULL, p + 1), RET_OK);
  ASSERT_EQ(darray.size, ARRAY_SIZE(cases) - 4 - 2);

  ASSERT_EQ(darray_remove_all(&darray, NULL, p + 12), RET_OK);
  ASSERT_EQ(darray.size, ARRAY_SIZE(cases) - 4 - 2 - 2);

  ASSERT_EQ(darray_remove_all(&darray, NULL, p + 13), RET_OK);
  ASSERT_EQ(darray.size, ARRAY_SIZE(cases) - 4 - 2 - 2 - 1);

  ASSERT_EQ(darray_remove_all(&darray, NULL, p + 14), RET_OK);
  ASSERT_EQ(darray.size, ARRAY_SIZE(cases) - 4 - 2 - 2 - 1 - 1);

  ASSERT_EQ(darray_remove_all(&darray, NULL, p + 33), RET_OK);
  ASSERT_EQ(darray.size, ARRAY_SIZE(cases) - 4 - 2 - 2 - 1 - 1 - 1);

  ASSERT_EQ(darray_remove_all(&darray, NULL, p + 34), RET_OK);
  ASSERT_EQ(darray.size, ARRAY_SIZE(cases) - 4 - 2 - 2 - 1 - 1 - 1 - 2);

  ASSERT_EQ(darray_remove_all(&darray, NULL, p + 0), RET_OK);
  ASSERT_EQ(darray.size, ARRAY_SIZE(cases) - 4 - 2 - 2 - 1 - 1 - 1 - 2);

  darray_deinit(&darray);
}

static int cmp_le(const void* a, const void* b) {
  int ia = tk_pointer_to_int((void*)a);
  int ib = tk_pointer_to_int((void*)b);

  if (ia <= ib) {
    return 0;
  }

  return -1;
}

TEST(DArrayTest, removeAll1) {
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
  ASSERT_EQ(darray_remove_all(&darray, cmp_le, tk_pointer_from_int(100)), RET_OK);
  ASSERT_EQ(darray.size, 1);

  darray_deinit(&darray);
}

TEST(DArrayTest, findAll) {
  uint32_t i = 0;
  char* p = NULL;
  darray_t darray;
  darray_t matched;
  int cases[] = {1, 2, 2, 2, 13, 14, 12, 41, 34, 34, 5, 563, 12, 1, 2, 33};
  darray_init(&darray, 10, NULL, NULL);

  for (i = 0; i < ARRAY_SIZE(cases); i++) {
    ASSERT_EQ(RET_OK, darray_push(&darray, p + cases[i]));
    ASSERT_EQ(i + 1, darray.size);
  }

  darray_init(&matched, 10, NULL, NULL);
  ASSERT_EQ(darray_find_all(&darray, cmp_le, tk_pointer_from_int(10), &matched), RET_OK);
  ASSERT_EQ(matched.size, 7);
  darray_clear(&matched);

  ASSERT_EQ(darray_find_all(&darray, cmp_le, tk_pointer_from_int(100), &matched), RET_OK);
  ASSERT_EQ(matched.size, ARRAY_SIZE(cases) - 1);
  darray_clear(&matched);

  darray_deinit(&matched);
  darray_deinit(&darray);
}

static int cmp_int(const void* a, const void* b) {
  int ia = tk_pointer_to_int(a);
  int ib = tk_pointer_to_int(b);

  return ia - ib;
}

TEST(DArrayTest, sort) {
  uint32_t i = 0;
  char* p = NULL;
  darray_t darray;
  int cases[] = {100, 1, 2, 2, 13, 14, 12, 4};
  darray_init(&darray, 10, NULL, NULL);

  for (i = 0; i < ARRAY_SIZE(cases); i++) {
    ASSERT_EQ(RET_OK, darray_push(&darray, p + cases[i]));
    ASSERT_EQ(i + 1, darray.size);
  }

  ASSERT_EQ(darray_sort(&darray, cmp_int), RET_OK);
  ASSERT_EQ(tk_pointer_to_int(darray.elms[0]), 1);
  ASSERT_EQ(tk_pointer_to_int(darray.elms[1]), 2);
  ASSERT_EQ(tk_pointer_to_int(darray.elms[2]), 2);
  ASSERT_EQ(tk_pointer_to_int(darray.elms[3]), 4);
  ASSERT_EQ(tk_pointer_to_int(darray.elms[4]), 12);
  ASSERT_EQ(tk_pointer_to_int(darray.elms[5]), 13);
  ASSERT_EQ(tk_pointer_to_int(darray.elms[6]), 14);
  ASSERT_EQ(tk_pointer_to_int(darray.elms[7]), 100);

  ASSERT_EQ(darray_bsearch_index(&darray, NULL, tk_pointer_from_int(1)), 0);
  ASSERT_EQ(darray_bsearch_index(&darray, NULL, tk_pointer_from_int(4)), 3);
  ASSERT_EQ(darray_bsearch_index(&darray, NULL, tk_pointer_from_int(12)), 4);
  ASSERT_EQ(darray_bsearch_index(&darray, NULL, tk_pointer_from_int(13)), 5);
  ASSERT_EQ(darray_bsearch_index(&darray, NULL, tk_pointer_from_int(14)), 6);
  ASSERT_EQ(darray_bsearch_index(&darray, NULL, tk_pointer_from_int(100)), 7);

  darray_deinit(&darray);
}

TEST(DArrayTest, bsearch) {
  darray_t darray;
  darray_init(&darray, 10, NULL, NULL);

  darray_push(&darray, tk_pointer_from_int(1));
  ASSERT_EQ(darray_bsearch_index(&darray, NULL, tk_pointer_from_int(1)), 0);

  darray_push(&darray, tk_pointer_from_int(2));
  ASSERT_EQ(darray_bsearch_index(&darray, NULL, tk_pointer_from_int(2)), 1);

  darray_push(&darray, tk_pointer_from_int(3));
  ASSERT_EQ(darray_bsearch_index(&darray, NULL, tk_pointer_from_int(3)), 2);

  ASSERT_EQ(darray_bsearch_index(&darray, NULL, tk_pointer_from_int(30)), -1);

  darray_deinit(&darray);
}

#include "tkc/mem.h"

typedef struct _my_data_t {
  char name[TK_NAME_LEN + 1];
  int value;
} my_data_t;

static my_data_t* my_data_create(const char* name, int value) {
  my_data_t* data = TKMEM_ZALLOC(my_data_t);
  return_value_if_fail(data != NULL, NULL);

  tk_strncpy(data->name, name, TK_NAME_LEN);
  data->value = value;

  return data;
}

static ret_t my_data_destroy(my_data_t* data) {
  TKMEM_FREE(data);

  return RET_OK;
}

TEST(DArrayTest, struct) {
  darray_t darray;
  my_data_t* p = NULL;
  darray_init(&darray, 10, (tk_destroy_t)my_data_destroy, NULL);

  darray_push(&darray, my_data_create("jack", 100));
  darray_push(&darray, my_data_create("tom", 10));

  ASSERT_EQ(darray.size, 2);

  p = (my_data_t*)darray_get(&darray, 0);
  ASSERT_STREQ(p->name, "jack");
  ASSERT_EQ(p->value, 100);

  p = (my_data_t*)darray_get(&darray, 1);
  ASSERT_STREQ(p->name, "tom");
  ASSERT_EQ(p->value, 10);

  darray_deinit(&darray);
}
