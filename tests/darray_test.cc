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

  ASSERT_EQ(darray.size, 0u);
  ASSERT_EQ(darray.capacity, 10u);

  darray_deinit(&darray);
}

TEST(DArrayTest, init0) {
  darray_t darray;
  darray_init(&darray, 0, NULL, NULL);

  ASSERT_EQ(darray.size, 0u);
  ASSERT_EQ(darray.capacity, 0u);
  ASSERT_EQ(darray.elms == NULL, TRUE);

  darray_deinit(&darray);
}

TEST(DArrayTest, insert) {
  darray_t darray;
  darray_init(&darray, 2, NULL, NULL);

  ASSERT_EQ(darray_insert(&darray, 0, tk_pointer_from_int(3)), RET_OK);
  ASSERT_EQ(darray_insert(&darray, 0, tk_pointer_from_int(2)), RET_OK);
  ASSERT_EQ(darray_insert(&darray, 0, tk_pointer_from_int(1)), RET_OK);
  ASSERT_EQ(darray_insert(&darray, 0, tk_pointer_from_int(0)), RET_OK);
  ASSERT_EQ(darray.size, 4u);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 0)), 0);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 1)), 1);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 2)), 2);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 3)), 3);

  ASSERT_EQ(darray_insert(&darray, 1, tk_pointer_from_int(111)), RET_OK);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 1)), 111);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 2)), 1);

  darray_deinit(&darray);
}

TEST(DArrayTest, create) {
  darray_t* darray = darray_create(10, NULL, NULL);
  ASSERT_EQ(darray->size, 0u);
  ASSERT_EQ(darray->capacity, 10u);
  darray_destroy(darray);
}

TEST(DArrayTest, create0) {
  darray_t* darray = darray_create(0, NULL, NULL);
  ASSERT_EQ(darray->size, 0u);
  ASSERT_EQ(darray->elms == NULL, true);
  ASSERT_EQ(darray->capacity, 0u);
  darray_destroy(darray);
}

TEST(DArrayTest, stack) {
  char* p = NULL;
  darray_t* darray = darray_create(10, NULL, NULL);
  ASSERT_EQ(darray->size, 0u);
  ASSERT_EQ(darray->capacity, 10u);

  ASSERT_EQ(darray_tail(darray), p);
  ASSERT_EQ(darray_head(darray), p);

  ASSERT_EQ(RET_OK, darray_push(darray, p + 1));
  ASSERT_EQ(darray_tail(darray), p + 1);
  ASSERT_EQ(darray_head(darray), p + 1);
  ASSERT_EQ(1, darray_count(darray, p + 1));

  ASSERT_EQ(darray_pop(darray), p + 1);
  ASSERT_EQ(darray->size, 0u);

  darray_destroy(darray);
}

TEST(DArrayTest, basic) {
  char* p = NULL;
  darray_t* darray = darray_create(10, NULL, NULL);
  ASSERT_EQ(darray->size, 0u);
  ASSERT_EQ(darray->capacity, 10u);

  ASSERT_EQ(RET_OK, darray_push(darray, p + 1));
  ASSERT_EQ(1, darray_count(darray, p + 1));

  ASSERT_EQ(RET_OK, darray_push(darray, p + 1));
  ASSERT_EQ(2, darray_count(darray, p + 1));

  ASSERT_EQ(RET_OK, darray_remove(darray, p + 1));
  ASSERT_EQ(1, darray_count(darray, p + 1));

  ASSERT_EQ(RET_OK, darray_push(darray, p + 1));
  ASSERT_EQ(2, darray_count(darray, p + 1));

  ASSERT_EQ(RET_OK, darray_remove_all(darray, NULL, p + 1));
  ASSERT_EQ(darray->size, 0u);
  ASSERT_EQ(0, darray_count(darray, p + 1));

  ASSERT_EQ(RET_OK, darray_push(darray, p + 1));
  ASSERT_EQ(RET_OK, darray_push(darray, p + 2));
  ASSERT_EQ(RET_OK, darray_push(darray, p + 3));
  ASSERT_EQ(RET_OK, darray_clear(darray));

  ASSERT_EQ(darray->size, 0u);

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
    ASSERT_EQ(i + 1u, darray.size);
  }

  for (i = 0; i < ARRAY_SIZE(cases); i++) {
    uint32_t k = ARRAY_SIZE(cases) - i - 1;
    void* data = darray_pop(&darray);
    ASSERT_EQ(data, p + cases[k]);
    ASSERT_EQ(k, darray.size);
  }

  darray_deinit(&darray);
}

TEST(DArrayTest, random) {
  uint32_t i = 0;
  uint32_t n = NR;
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
  ASSERT_EQ(darray.size, 0u);

  darray_deinit(&darray);
}

TEST(DArrayTest, inc) {
  uint32_t i = 0;
  uint32_t n = NR;
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
  ASSERT_EQ(darray.size, 0u);

  darray_deinit(&darray);
}

TEST(DArrayTest, dec) {
  uint32_t i = 0;
  char* p = NULL;
  uint32_t n = NR;
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
  ASSERT_EQ(darray.size, 0u);

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
    ASSERT_EQ(i + 1u, darray.size);
  }

  ASSERT_EQ(darray.size, ARRAY_SIZE(cases));
  ASSERT_EQ(darray_remove_all(&darray, NULL, p + 2), RET_OK);
  ASSERT_EQ(darray.size, ARRAY_SIZE(cases) - 4u);

  ASSERT_EQ(darray_remove_all(&darray, NULL, p + 1), RET_OK);
  ASSERT_EQ(darray.size, ARRAY_SIZE(cases) - 4u - 2u);

  ASSERT_EQ(darray_remove_all(&darray, NULL, p + 12), RET_OK);
  ASSERT_EQ(darray.size, ARRAY_SIZE(cases) - 4u - 2u - 2u);

  ASSERT_EQ(darray_remove_all(&darray, NULL, p + 13), RET_OK);
  ASSERT_EQ(darray.size, ARRAY_SIZE(cases) - 4u - 2u - 2u - 1u);

  ASSERT_EQ(darray_remove_all(&darray, NULL, p + 14), RET_OK);
  ASSERT_EQ(darray.size, ARRAY_SIZE(cases) - 4u - 2u - 2u - 1u - 1u);

  ASSERT_EQ(darray_remove_all(&darray, NULL, p + 33), RET_OK);
  ASSERT_EQ(darray.size, ARRAY_SIZE(cases) - 4u - 2u - 2u - 1u - 1u - 1u);

  ASSERT_EQ(darray_remove_all(&darray, NULL, p + 34), RET_OK);
  ASSERT_EQ(darray.size, ARRAY_SIZE(cases) - 4u - 2u - 2u - 1u - 1u - 1u - 2u);

  ASSERT_EQ(darray_remove_all(&darray, NULL, p + 0), RET_OK);
  ASSERT_EQ(darray.size, ARRAY_SIZE(cases) - 4u - 2u - 2u - 1u - 1u - 1u - 2u);

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

  ASSERT_EQ(darray.size, (uint32_t)ARRAY_SIZE(cases));
  ASSERT_EQ(darray_remove_all(&darray, cmp_le, tk_pointer_from_int(100)), RET_OK);
  ASSERT_EQ(darray.size, 1u);

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
  ASSERT_EQ(matched.size, 7u);
  darray_clear(&matched);

  ASSERT_EQ(darray_find_all(&darray, cmp_le, tk_pointer_from_int(100), &matched), RET_OK);
  ASSERT_EQ(matched.size, ARRAY_SIZE(cases) - 1u);
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

  ASSERT_EQ(darray.size, 2u);

  p = (my_data_t*)darray_get(&darray, 0);
  ASSERT_STREQ(p->name, "jack");
  ASSERT_EQ(p->value, 100);

  p = (my_data_t*)darray_get(&darray, 1);
  ASSERT_STREQ(p->name, "tom");
  ASSERT_EQ(p->value, 10);

  darray_deinit(&darray);
}

TEST(DArrayTest, set) {
  darray_t darray;
  darray_init(&darray, 2, NULL, NULL);

  ASSERT_EQ(darray_insert(&darray, 0, tk_pointer_from_int(3)), RET_OK);
  ASSERT_EQ(darray_insert(&darray, 0, tk_pointer_from_int(2)), RET_OK);
  ASSERT_EQ(darray_insert(&darray, 0, tk_pointer_from_int(1)), RET_OK);
  ASSERT_EQ(darray_insert(&darray, 0, tk_pointer_from_int(0)), RET_OK);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 0)), 0);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 1)), 1);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 2)), 2);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 3)), 3);

  ASSERT_EQ(darray_set(&darray, 0, tk_pointer_from_int(100)), RET_OK);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 0)), 100);

  ASSERT_EQ(darray_set(&darray, 1, tk_pointer_from_int(200)), RET_OK);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 1)), 200);

  darray_deinit(&darray);
}

TEST(DArrayTest, insert_order) {
  darray_t darray;
  darray_init(&darray, 2, NULL, NULL);

  ASSERT_EQ(darray_sorted_insert(&darray, tk_pointer_from_int(3), NULL, true), RET_OK);
  ASSERT_EQ(darray_sorted_insert(&darray, tk_pointer_from_int(1), NULL, true), RET_OK);
  ASSERT_EQ(darray_sorted_insert(&darray, tk_pointer_from_int(2), NULL, true), RET_OK);
  ASSERT_EQ(darray_sorted_insert(&darray, tk_pointer_from_int(0), NULL, true), RET_OK);
  ASSERT_EQ(darray_sorted_insert(&darray, tk_pointer_from_int(6), NULL, true), RET_OK);
  ASSERT_EQ(darray_sorted_insert(&darray, tk_pointer_from_int(5), NULL, true), RET_OK);
  ASSERT_EQ(darray_sorted_insert(&darray, tk_pointer_from_int(4), NULL, true), RET_OK);
  ASSERT_EQ(darray.size, 7u);

  ASSERT_EQ(darray_sorted_insert(&darray, tk_pointer_from_int(3), NULL, true), RET_OK);
  ASSERT_EQ(darray.size, 7u);
  ASSERT_EQ(darray_sorted_insert(&darray, tk_pointer_from_int(1), NULL, true), RET_OK);
  ASSERT_EQ(darray.size, 7u);
  ASSERT_EQ(darray_sorted_insert(&darray, tk_pointer_from_int(2), NULL, true), RET_OK);
  ASSERT_EQ(darray.size, 7u);
  ASSERT_EQ(darray_sorted_insert(&darray, tk_pointer_from_int(0), NULL, true), RET_OK);
  ASSERT_EQ(darray.size, 7u);
  ASSERT_EQ(darray_sorted_insert(&darray, tk_pointer_from_int(6), NULL, true), RET_OK);
  ASSERT_EQ(darray.size, 7u);
  ASSERT_EQ(darray_sorted_insert(&darray, tk_pointer_from_int(5), NULL, true), RET_OK);
  ASSERT_EQ(darray.size, 7u);
  ASSERT_EQ(darray_sorted_insert(&darray, tk_pointer_from_int(4), NULL, true), RET_OK);
  ASSERT_EQ(darray.size, 7u);

  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 0)), 0);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 1)), 1);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 2)), 2);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 3)), 3);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 4)), 4);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 5)), 5);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 6)), 6);

  ASSERT_EQ(darray_sorted_insert(&darray, tk_pointer_from_int(3), NULL, false), RET_OK);
  ASSERT_EQ(darray.size, 8u);
  ASSERT_EQ(darray_sorted_insert(&darray, tk_pointer_from_int(1), NULL, false), RET_OK);
  ASSERT_EQ(darray.size, 9u);
  ASSERT_EQ(darray_sorted_insert(&darray, tk_pointer_from_int(2), NULL, false), RET_OK);
  ASSERT_EQ(darray.size, 10u);
  ASSERT_EQ(darray_sorted_insert(&darray, tk_pointer_from_int(0), NULL, false), RET_OK);
  ASSERT_EQ(darray.size, 11u);
  ASSERT_EQ(darray_sorted_insert(&darray, tk_pointer_from_int(6), NULL, false), RET_OK);
  ASSERT_EQ(darray.size, 12u);
  ASSERT_EQ(darray_sorted_insert(&darray, tk_pointer_from_int(5), NULL, false), RET_OK);
  ASSERT_EQ(darray.size, 13u);
  ASSERT_EQ(darray_sorted_insert(&darray, tk_pointer_from_int(4), NULL, false), RET_OK);
  ASSERT_EQ(darray.size, 14u);

  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 0)), 0);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 1)), 0);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 2)), 1);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 3)), 1);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 4)), 2);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 5)), 2);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 6)), 3);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 7)), 3);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 8)), 4);
  ASSERT_EQ(tk_pointer_to_int(darray_get(&darray, 9)), 4);

  darray_deinit(&darray);
}

TEST(DArrayTest, insert1) {
  darray_t darray;
  char name[32];
  uint32_t i = 0;
  uint32_t n = 1000;

  darray_init(&darray, 2, default_destroy, NULL);

  for (i = 0; i < n; i++) {
    tk_snprintf(name, sizeof(name), "%u", i);
    ASSERT_EQ(darray_sorted_insert(&darray, tk_strdup(name), (tk_compare_t)strcmp, TRUE), RET_OK);
    ASSERT_STREQ((char*)darray_bsearch(&darray, (tk_compare_t)strcmp, name), name);
  }

  darray_deinit(&darray);
}

TEST(DArrayTest, insert2) {
  darray_t darray;
  uint32_t i = 0;
  uint32_t n = 1000;

  darray_init(&darray, 2, NULL, NULL);

  for (i = 0; i < n; i++) {
    ASSERT_EQ(darray_sorted_insert(&darray, tk_pointer_from_int(i), NULL, TRUE), RET_OK);
    ASSERT_EQ(darray_bsearch_index(&darray, NULL, tk_pointer_from_int(i)), (int32_t)i);
  }

  for (i = n; i > 0; i--) {
    ASSERT_EQ(darray_sorted_insert(&darray, tk_pointer_from_int(i), NULL, TRUE), RET_OK);
    ASSERT_EQ(darray_bsearch_index(&darray, NULL, tk_pointer_from_int(i)), (int32_t)i);
  }

  darray_deinit(&darray);
}

TEST(DArrayTest, push_unique) {
  darray_t darray;
  uint32_t i = 0;
  uint32_t n = 1000;

  darray_init(&darray, 2, NULL, NULL);

  for (i = 0; i < n; i++) {
    ASSERT_EQ(darray_push_unique(&darray, tk_pointer_from_int(i)), RET_OK);
    ASSERT_NE(darray_push_unique(&darray, tk_pointer_from_int(i)), RET_OK);
    ASSERT_EQ(darray.size, i + 1);
  }

  darray_deinit(&darray);
}
