#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "tkc/raw_hash_table.h"
#include "gtest/gtest.h"

/* 键是size_t，值是void*，用整数转指针做值，所以不需要值销毁函数。 */
static void* raw_hash_table_test_value(size_t i) {
  return (void*)(uintptr_t)(i * 10 + 1);
}

static ret_t raw_hash_table_test_sum_key(void* ctx, const void* data) {
  const tk_raw_hash_table_entry_t* entry = (const tk_raw_hash_table_entry_t*)data;
  *(size_t*)ctx += entry->key;

  return RET_OK;
}

static ret_t raw_hash_table_test_remove_even_key(void* ctx, const void* data) {
  const tk_raw_hash_table_entry_t* entry = (const tk_raw_hash_table_entry_t*)data;

  return ((entry->key % 2) == 0) ? RET_REMOVE : RET_OK;
}

static ret_t raw_hash_table_test_visit_continue(void* ctx, const void* data) {
  (*(size_t*)ctx)++;

  return RET_CONTINUE;
}

static ret_t raw_hash_table_test_visit_skip(void* ctx, const void* data) {
  (*(size_t*)ctx)++;

  return RET_SKIP;
}

static ret_t raw_hash_table_test_visit_repeat(void* ctx, const void* data) {
  (*(size_t*)ctx)++;

  return RET_REPEAT;
}

static ret_t raw_hash_table_test_visit_fail(void* ctx, const void* data) {
  size_t* count = (size_t*)ctx;

  (*count)++;

  return *count >= 3 ? RET_FAIL : RET_OK;
}

typedef struct _raw_hash_table_visit_add_ctx_t {
  tk_raw_hash_table_t* table;
  size_t nr;
} raw_hash_table_visit_add_ctx_t;

static ret_t raw_hash_table_test_visit_add(void* ctx, const void* data) {
  raw_hash_table_visit_add_ctx_t* c = (raw_hash_table_visit_add_ctx_t*)ctx;

  c->nr++;
  if (c->nr == 1) {
    /* 遍历过程中往表里加元素，可能让桶内数组realloc */
    ENSURE(tk_raw_hash_table_add(c->table, 1000, raw_hash_table_test_value(1000)) == RET_OK);
  }

  return RET_OK;
}

TEST(RawHashTable, mix) {
  tk_raw_hash_table_t table;

  /* 相邻、bit只差一位的输入都要被打散，不能原样返回。 */
  ASSERT_NE(tk_raw_hash_table_mix(0), tk_raw_hash_table_mix(1));
  ASSERT_NE(tk_raw_hash_table_mix(1), tk_raw_hash_table_mix(2));
  ASSERT_NE(tk_raw_hash_table_mix(0), tk_raw_hash_table_mix(0x10000));

  /* 默认的键hash就是键本身，容器再mix一次。 */
  ASSERT_EQ(TK_RAW_HASH_TABLE_KEY_HASH(7), 7u);

  /* 键直接交给bucket_index(bucket_index内部会做hash)，下标必须落在桶数范围内。 */
  ASSERT_EQ(tk_raw_hash_table_init(&table, 100, NULL), RET_OK);
  ASSERT_LT(tk_raw_hash_table_bucket_index(&table, 7), TK_RAW_HASH_TABLE_BUCKET_COUNT(&table));
  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, bucket_count) {
  size_t i = 0;

  /* 桶数按负载因子2/3换算，是任意值(桶下标用乘法算，不需要凑2的幂或质数)。 */
  ASSERT_EQ(tk_raw_hash_table_bucket_count_for(0), 0u);
  ASSERT_EQ(tk_raw_hash_table_bucket_count_for(1), 1u);
  ASSERT_EQ(tk_raw_hash_table_bucket_count_for(2), 3u);
  ASSERT_EQ(tk_raw_hash_table_bucket_count_for(10), 15u);
  ASSERT_EQ(tk_raw_hash_table_bucket_count_for(100), 150u);
  ASSERT_EQ(tk_raw_hash_table_bucket_count_for(129), 193u);
  ASSERT_EQ(tk_raw_hash_table_bucket_count_for(300), 450u);

  /* 容量大于0时桶数不少于容量(负载因子不超过1)，保证桶数大于0。 */
  for (i = 1; i < 1000; i++) {
    size_t count = tk_raw_hash_table_bucket_count_for(i);

    ASSERT_GE(count, i);
  }
}

TEST(RawHashTable, bucket_index) {
  size_t i = 0;
  size_t count = 0;
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 100, NULL), RET_OK);
  count = TK_RAW_HASH_TABLE_BUCKET_COUNT(&table);

  /* 各种键(连续键、大键、指针值)算出的下标都要落在[0, 桶数) */
  for (i = 0; i < 10000; i++) {
    ASSERT_LT(tk_raw_hash_table_bucket_index(&table, i), count);
    ASSERT_LT(tk_raw_hash_table_bucket_index(&table, (SIZE_MAX >> 1) - i), count);
    ASSERT_LT(tk_raw_hash_table_bucket_index(&table, (size_t)&table + i * 64), count);
  }

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, value_compare) {
  void* a = (void*)(uintptr_t)0x1000;
  void* b = (void*)(uintptr_t)0x2000;

  /* 默认按值(指针)比较：相等返回0，否则返回非0。 */
  ASSERT_EQ(TK_RAW_HASH_TABLE_VALUE_COMPARE(a, a), 0);
  /* NULL在C++里是0，要和指针做有序比较得先转成void*，否则g++报ordered comparison of pointer with integer zero */
  ASSERT_EQ(TK_RAW_HASH_TABLE_VALUE_COMPARE((void*)NULL, (void*)NULL), 0);
  ASSERT_NE(TK_RAW_HASH_TABLE_VALUE_COMPARE(a, b), 0);
  ASSERT_NE(TK_RAW_HASH_TABLE_VALUE_COMPARE(a, (void*)NULL), 0);
}

TEST(RawHashTable, init) {
  size_t i = 0;
  size_t index = 0;
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, NULL), RET_OK);
  ASSERT_TRUE(table.buckets != NULL);
  ASSERT_EQ(TK_RAW_HASH_TABLE_BUCKET_COUNT(&table), 15u);
  ASSERT_EQ(tk_raw_hash_table_size(&table), 0u);

  /* 一开始所有桶都是NULL。 */
  for (i = 0; i < TK_RAW_HASH_TABLE_BUCKET_COUNT(&table); i++) {
    ASSERT_TRUE(table.buckets[i] == NULL);
  }

  index = tk_raw_hash_table_bucket_index(&table, 11);
  ASSERT_EQ(index, tk_raw_hash_table_bucket_index(&table, 11));
  ASSERT_EQ(index, tk_raw_hash_table_bucket_index(&table, 11));

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
  ASSERT_TRUE(table.buckets == NULL);

  ASSERT_NE(tk_raw_hash_table_init(&table, 0, NULL), RET_OK);
  ASSERT_TRUE(table.buckets == NULL);
  ASSERT_NE(tk_raw_hash_table_deinit(NULL), RET_OK);

  /* deinit之后可以重新init */
  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, NULL), RET_OK);
  ASSERT_TRUE(table.buckets != NULL);
  ASSERT_EQ(TK_RAW_HASH_TABLE_BUCKET_COUNT(&table), 15u);
  ASSERT_EQ(tk_raw_hash_table_size(&table), 0u);
  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
  ASSERT_TRUE(table.buckets == NULL);
}

TEST(RawHashTable, lazy_bucket) {
  size_t i = 0;
  size_t index = 0;
  size_t non_null = 0;
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, NULL), RET_OK);
  ASSERT_TRUE(table.buckets != NULL);

  ASSERT_EQ(tk_raw_hash_table_add(&table, 11, raw_hash_table_test_value(11)), RET_OK);
  ASSERT_EQ(tk_raw_hash_table_size(&table), 1u);

  /* 只有加入元素的那个桶才创建raw_darray。 */
  index = tk_raw_hash_table_bucket_index(&table, 11);
  for (i = 0; i < TK_RAW_HASH_TABLE_BUCKET_COUNT(&table); i++) {
    if (table.buckets[i] != NULL) {
      non_null++;
      ASSERT_EQ(i, index);
      ASSERT_EQ(TK_RAW_DARRAY_SIZE(table.buckets[i]), 1u);
      ASSERT_EQ(table.buckets[i][0].key, 11u);
      ASSERT_EQ(table.buckets[i][0].value, raw_hash_table_test_value(11));
    }
  }
  ASSERT_EQ(non_null, 1u);

  /* 删除元素后桶保留（回收只在clear/deinit时做），只是桶里没有元素了。 */
  ASSERT_EQ(tk_raw_hash_table_remove(&table, 11), RET_OK);
  ASSERT_TRUE(table.buckets[index] != NULL);
  ASSERT_EQ(TK_RAW_DARRAY_SIZE(table.buckets[index]), 0u);
  ASSERT_EQ(tk_raw_hash_table_size(&table), 0u);

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, basic) {
  size_t i = 0;
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 200, NULL), RET_OK);
  ASSERT_TRUE(table.buckets != NULL);
  ASSERT_EQ(TK_RAW_HASH_TABLE_BUCKET_COUNT(&table), 300u);

  for (i = 0; i < 100; i++) {
    ASSERT_EQ(tk_raw_hash_table_add(&table, i, raw_hash_table_test_value(i)), RET_OK);
    ASSERT_EQ(tk_raw_hash_table_size(&table), i + 1);
  }
  ASSERT_GT(tk_raw_hash_table_size(&table), 0u);

  for (i = 0; i < 100; i++) {
    tk_raw_hash_table_entry_t* entry = tk_raw_hash_table_find(&table, i);
    ASSERT_TRUE(entry != NULL);
    ASSERT_EQ(entry->key, i);
    ASSERT_EQ(entry->value, raw_hash_table_test_value(i));
  }

  ASSERT_TRUE(tk_raw_hash_table_find(&table, 100) == NULL);

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, unique_key) {
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, NULL), RET_OK);
  ASSERT_TRUE(table.buckets != NULL);

  ASSERT_EQ(tk_raw_hash_table_add(&table, 5, raw_hash_table_test_value(1)), RET_OK);
  ASSERT_EQ(tk_raw_hash_table_size(&table), 1u);

  /* 键已存在时加入失败，原来的元素不受影响。 */
  ASSERT_EQ(tk_raw_hash_table_add(&table, 5, raw_hash_table_test_value(2)), RET_FAIL);
  ASSERT_EQ(tk_raw_hash_table_size(&table), 1u);
  ASSERT_EQ(TK_RAW_DARRAY_SIZE(table.buckets[tk_raw_hash_table_bucket_index(&table, 5)]), 1u);
  ASSERT_EQ(tk_raw_hash_table_find(&table, 5)->value, raw_hash_table_test_value(1));

  ASSERT_EQ(tk_raw_hash_table_remove(&table, 5), RET_OK);
  ASSERT_EQ(tk_raw_hash_table_size(&table), 0u);
  ASSERT_EQ(TK_RAW_DARRAY_SIZE(table.buckets[tk_raw_hash_table_bucket_index(&table, 5)]), 0u);
  ASSERT_EQ(tk_raw_hash_table_remove(&table, 5), RET_NOT_FOUND);

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, modify) {
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, NULL), RET_OK);
  ASSERT_TRUE(table.buckets != NULL);

  ASSERT_EQ(tk_raw_hash_table_add(&table, 5, raw_hash_table_test_value(1)), RET_OK);
  ASSERT_EQ(tk_raw_hash_table_size(&table), 1u);
  ASSERT_EQ(tk_raw_hash_table_find(&table, 5)->value, raw_hash_table_test_value(1));

  /* 修改已有的值，元素个数不变。 */
  ASSERT_EQ(tk_raw_hash_table_modify(&table, 5, raw_hash_table_test_value(2)), RET_OK);
  ASSERT_EQ(tk_raw_hash_table_size(&table), 1u);
  ASSERT_EQ(TK_RAW_DARRAY_SIZE(table.buckets[tk_raw_hash_table_bucket_index(&table, 5)]), 1u);
  ASSERT_EQ(tk_raw_hash_table_find(&table, 5)->value, raw_hash_table_test_value(2));

  /* 键不存在时修改失败，也不会新增元素。 */
  ASSERT_EQ(tk_raw_hash_table_modify(&table, 6, raw_hash_table_test_value(3)), RET_NOT_FOUND);
  ASSERT_EQ(tk_raw_hash_table_size(&table), 1u);
  ASSERT_TRUE(tk_raw_hash_table_find(&table, 6) == NULL);

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, replace) {
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, NULL), RET_OK);
  ASSERT_TRUE(table.buckets != NULL);

  /* 键不存在时加入。 */
  ASSERT_EQ(tk_raw_hash_table_replace(&table, 5, raw_hash_table_test_value(1)), RET_OK);
  ASSERT_EQ(tk_raw_hash_table_size(&table), 1u);
  ASSERT_EQ(tk_raw_hash_table_find(&table, 5)->value, raw_hash_table_test_value(1));

  /* 键存在时替换，元素个数不变，桶里仍然只有一个元素。 */
  ASSERT_EQ(tk_raw_hash_table_replace(&table, 5, raw_hash_table_test_value(2)), RET_OK);
  ASSERT_EQ(tk_raw_hash_table_size(&table), 1u);
  ASSERT_EQ(TK_RAW_DARRAY_SIZE(table.buckets[tk_raw_hash_table_bucket_index(&table, 5)]), 1u);
  ASSERT_EQ(tk_raw_hash_table_find(&table, 5)->value, raw_hash_table_test_value(2));

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, remove) {
  size_t i = 0;
  size_t sum = 0;
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, NULL), RET_OK);
  ASSERT_TRUE(table.buckets != NULL);

  for (i = 0; i < 20; i++) {
    ASSERT_EQ(tk_raw_hash_table_add(&table, i, raw_hash_table_test_value(i)), RET_OK);
  }

  ASSERT_EQ(tk_raw_hash_table_remove(&table, 7), RET_OK);
  ASSERT_EQ(tk_raw_hash_table_size(&table), 19u);
  ASSERT_TRUE(tk_raw_hash_table_find(&table, 7) == NULL);
  ASSERT_EQ(tk_raw_hash_table_remove(&table, 7), RET_NOT_FOUND);
  ASSERT_EQ(tk_raw_hash_table_size(&table), 19u);

  ASSERT_EQ(tk_raw_hash_table_foreach(&table, raw_hash_table_test_sum_key, &sum), RET_OK);
  ASSERT_EQ(sum, 190 - 7);

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, bucket_reuse) {
  size_t index = 0;
  tk_raw_hash_table_t table;
  tk_raw_hash_table_bucket_t bucket = NULL;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, NULL), RET_OK);
  ASSERT_TRUE(table.buckets != NULL);

  index = tk_raw_hash_table_bucket_index(&table, 5);
  ASSERT_EQ(tk_raw_hash_table_add(&table, 5, raw_hash_table_test_value(1)), RET_OK);
  bucket = table.buckets[index];
  ASSERT_TRUE(bucket != NULL);

  /* 删除元素后桶保留（回收只在clear/deinit时做）。 */
  ASSERT_EQ(tk_raw_hash_table_remove(&table, 5), RET_OK);
  ASSERT_TRUE(table.buckets[index] == bucket);
  ASSERT_EQ(TK_RAW_DARRAY_SIZE(table.buckets[index]), 0u);

  /* 再次加入同一个键，复用同一个桶，不重新分配。 */
  ASSERT_EQ(tk_raw_hash_table_add(&table, 5, raw_hash_table_test_value(2)), RET_OK);
  ASSERT_TRUE(table.buckets[index] == bucket);
  ASSERT_EQ(TK_RAW_DARRAY_SIZE(table.buckets[index]), 1u);

  /* clear时才回收桶。 */
  ASSERT_EQ(tk_raw_hash_table_clear(&table), RET_OK);
  ASSERT_TRUE(table.buckets[index] == NULL);
  ASSERT_EQ(TK_RAW_HASH_TABLE_BUCKET_COUNT(&table), 15u);

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, foreach_add) {
  size_t i = 0;
  raw_hash_table_visit_add_ctx_t ctx;
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 100, NULL), RET_OK);
  ASSERT_TRUE(table.buckets != NULL);

  for (i = 0; i < 10; i++) {
    ASSERT_EQ(tk_raw_hash_table_add(&table, i, raw_hash_table_test_value(i)), RET_OK);
  }

  ctx.table = &table;
  ctx.nr = 0;
  ASSERT_EQ(tk_raw_hash_table_foreach(&table, raw_hash_table_test_visit_add, &ctx), RET_OK);

  /* 原来10个都访问到了，新加的那个可能也被访问到 */
  ASSERT_GE(ctx.nr, 10u);
  ASSERT_LE(ctx.nr, 11u);
  ASSERT_EQ(tk_raw_hash_table_size(&table), 11u);
  ASSERT_TRUE(tk_raw_hash_table_find(&table, 1000) != NULL);
  for (i = 0; i < 10; i++) {
    ASSERT_TRUE(tk_raw_hash_table_find(&table, i) != NULL);
  }

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, null_table) {
  size_t sum = 0;
  tk_raw_hash_table_t table;

  memset(&table, 0x00, sizeof(table));

  /* table指针本身为NULL */
  ASSERT_NE(tk_raw_hash_table_init(NULL, 10, NULL), RET_OK);

  /* table还没init：查询按空表处理，修改类操作返回错误 */
  ASSERT_NE(tk_raw_hash_table_add(&table, 1, NULL), RET_OK);
  ASSERT_NE(tk_raw_hash_table_modify(&table, 1, NULL), RET_OK);
  ASSERT_NE(tk_raw_hash_table_replace(&table, 1, NULL), RET_OK);
  ASSERT_NE(tk_raw_hash_table_remove(&table, 1), RET_OK);
  ASSERT_NE(tk_raw_hash_table_clear(&table), RET_OK);
  ASSERT_NE(tk_raw_hash_table_deinit(&table), RET_OK);
  ASSERT_TRUE(tk_raw_hash_table_find(&table, 1) == NULL);
  ASSERT_EQ(tk_raw_hash_table_size(&table), 0u);
  ASSERT_NE(tk_raw_hash_table_foreach(&table, raw_hash_table_test_sum_key, &sum), RET_OK);
  ASSERT_EQ(sum, 0u);

  /* 初始化之后还没有元素：遍历直接返回RET_OK */
  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, NULL), RET_OK);
  ASSERT_EQ(tk_raw_hash_table_foreach(&table, raw_hash_table_test_sum_key, &sum), RET_OK);
  ASSERT_EQ(sum, 0u);
  ASSERT_EQ(tk_raw_hash_table_size(&table), 0u);

  /* 遍历函数为NULL */
  ASSERT_NE(tk_raw_hash_table_foreach(&table, NULL, NULL), RET_OK);

  /* deinit之后可以重新init，且table为NULL时修改类操作失败 */
  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
  ASSERT_NE(tk_raw_hash_table_add(&table, 1, NULL), RET_OK);

  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, NULL), RET_OK);
  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, stress) {
  size_t i = 0, k = 0, op = 0, total = 0;
  size_t expect[200];
  size_t expect_val[200];
  tk_raw_hash_table_t table;

  ASSERT_EQ(tk_raw_hash_table_init(&table, 200, NULL), RET_OK);
  ASSERT_TRUE(table.buckets != NULL);
  ASSERT_EQ(TK_RAW_HASH_TABLE_BUCKET_COUNT(&table), 300u);
  memset(expect, 0, sizeof(expect));
  memset(expect_val, 0, sizeof(expect_val));

  srand(1234);
  for (op = 0; op < 2000; op++) {
    uint32_t action = (uint32_t)(rand() % 4);
    k = (size_t)(rand() % 200);

    switch (action) {
      case 0:
      case 1: {
        ret_t ret = tk_raw_hash_table_add(&table, k, raw_hash_table_test_value(op));
        ASSERT_EQ(ret, expect[k] ? RET_FAIL : RET_OK);
        if (ret == RET_OK) {
          expect[k] = 1;
          expect_val[k] = op;
        }
        break;
      }
      case 2: {
        ret_t ret = tk_raw_hash_table_modify(&table, k, raw_hash_table_test_value(op));
        ASSERT_EQ(ret, expect[k] ? RET_OK : RET_NOT_FOUND);
        if (ret == RET_OK) {
          expect_val[k] = op;
        }
        break;
      }
      default: {
        if (expect[k]) {
          ASSERT_EQ(tk_raw_hash_table_remove(&table, k), RET_OK);
          expect[k] = 0;
        } else {
          ASSERT_EQ(tk_raw_hash_table_remove(&table, k), RET_NOT_FOUND);
          ASSERT_EQ(tk_raw_hash_table_replace(&table, k, raw_hash_table_test_value(op)), RET_OK);
          expect[k] = 1;
          expect_val[k] = op;
        }
        break;
      }
    }

    /* 每次操作后全量校验 */
    total = 0;
    for (i = 0; i < 200; i++) {
      tk_raw_hash_table_entry_t* entry = tk_raw_hash_table_find(&table, i);

      if (expect[i]) {
        total++;
        ASSERT_TRUE(entry != NULL);
        ASSERT_EQ(entry->key, i);
        ASSERT_EQ(entry->value, raw_hash_table_test_value(expect_val[i]));
      } else {
        ASSERT_TRUE(entry == NULL);
      }
    }
    ASSERT_EQ(tk_raw_hash_table_size(&table), total);
    ASSERT_EQ(TK_RAW_HASH_TABLE_BUCKET_COUNT(&table), 300u);

    /* 同一个桶里不能有重复的键 */
    for (i = 0; i < TK_RAW_HASH_TABLE_BUCKET_COUNT(&table); i++) {
      size_t a = 0, b = 0, n = TK_RAW_DARRAY_SIZE(table.buckets[i]);

      for (a = 0; a < n; a++) {
        for (b = a + 1; b < n; b++) {
          ASSERT_NE(table.buckets[i][a].key, table.buckets[i][b].key);
        }
      }
    }
  }

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, clear) {
  size_t i = 0;
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, NULL), RET_OK);
  ASSERT_TRUE(table.buckets != NULL);

  for (i = 0; i < 20; i++) {
    ASSERT_EQ(tk_raw_hash_table_add(&table, i, raw_hash_table_test_value(i)), RET_OK);
  }
  ASSERT_EQ(tk_raw_hash_table_size(&table), 20u);

  /* 清空后桶数不变，但所有桶都被回收。 */
  ASSERT_EQ(tk_raw_hash_table_clear(&table), RET_OK);
  ASSERT_EQ(tk_raw_hash_table_size(&table), 0u);
  ASSERT_EQ(TK_RAW_HASH_TABLE_BUCKET_COUNT(&table), 15u);
  for (i = 0; i < TK_RAW_HASH_TABLE_BUCKET_COUNT(&table); i++) {
    ASSERT_TRUE(table.buckets[i] == NULL);
  }

  /* 清空后可以继续用。 */
  ASSERT_EQ(tk_raw_hash_table_add(&table, 1, raw_hash_table_test_value(1)), RET_OK);
  ASSERT_EQ(tk_raw_hash_table_size(&table), 1u);

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, foreach_remove) {
  size_t i = 0;
  size_t sum = 0;
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 100, NULL), RET_OK);
  ASSERT_TRUE(table.buckets != NULL);

  for (i = 0; i < 100; i++) {
    ASSERT_EQ(tk_raw_hash_table_add(&table, i, raw_hash_table_test_value(i)), RET_OK);
  }

  ASSERT_EQ(tk_raw_hash_table_foreach(&table, raw_hash_table_test_remove_even_key, NULL), RET_OK);
  ASSERT_EQ(tk_raw_hash_table_size(&table), 50u);

  ASSERT_EQ(tk_raw_hash_table_foreach(&table, raw_hash_table_test_sum_key, &sum), RET_OK);
  ASSERT_EQ(sum, 2500);

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, foreach_visit_result) {
  size_t i = 0;
  size_t count = 0;
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, NULL), RET_OK);
  ASSERT_TRUE(table.buckets != NULL);

  for (i = 0; i < 10; i++) {
    ASSERT_EQ(tk_raw_hash_table_add(&table, i, NULL), RET_OK);
  }

  ASSERT_EQ(tk_raw_hash_table_foreach(&table, raw_hash_table_test_visit_continue, &count), RET_OK);
  ASSERT_EQ(count, 10u);

  count = 0;
  ASSERT_EQ(tk_raw_hash_table_foreach(&table, raw_hash_table_test_visit_skip, &count), RET_OK);
  ASSERT_EQ(count, 10u);

  /* RET_REPEAT不支持，告警后按RET_OK处理 */
  count = 0;
  ASSERT_EQ(tk_raw_hash_table_foreach(&table, raw_hash_table_test_visit_repeat, &count), RET_OK);
  ASSERT_EQ(count, 10u);

  /* 其它返回值终止遍历，并把它返回给调用者 */
  count = 0;
  ASSERT_EQ(tk_raw_hash_table_foreach(&table, raw_hash_table_test_visit_fail, &count), RET_FAIL);
  ASSERT_EQ(count, 3u);

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, many_in_one_bucket) {
  size_t i = 0;
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 1, NULL), RET_OK);
  ASSERT_TRUE(table.buckets != NULL);
  ASSERT_EQ(TK_RAW_HASH_TABLE_BUCKET_COUNT(&table), 1u);

  /* 只有一个桶，元素全挤在一起，桶内数组会扩容。 */
  for (i = 0; i < 200; i++) {
    ASSERT_EQ(tk_raw_hash_table_add(&table, i, raw_hash_table_test_value(i)), RET_OK);
  }
  ASSERT_EQ(tk_raw_hash_table_size(&table), 200u);

  for (i = 0; i < 200; i++) {
    ASSERT_TRUE(tk_raw_hash_table_find(&table, i) != NULL);
  }

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, hash) {
  size_t i = 0;
  size_t max_chain = 0;
  tk_raw_hash_table_t table;

  /* 连续的键要散开到很多桶里，不能挤在少数几个桶里。 */
  ASSERT_EQ(tk_raw_hash_table_init(&table, 1000, NULL), RET_OK);
  ASSERT_TRUE(table.buckets != NULL);
  for (i = 0; i < 1000; i++) {
    ASSERT_EQ(tk_raw_hash_table_add(&table, i, NULL), RET_OK);
  }
  for (i = 0; i < TK_RAW_HASH_TABLE_BUCKET_COUNT(&table); i++) {
    size_t n = TK_RAW_DARRAY_SIZE(table.buckets[i]);
    if (n > max_chain) {
      max_chain = n;
    }
  }
  /* 负载0.66下，理想hash的最大链长一般在7以内。 */
  ASSERT_LE(max_chain, 10u);
  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, error) {
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, NULL), RET_OK);
  ASSERT_TRUE(table.buckets != NULL);

  ASSERT_NE(tk_raw_hash_table_add(NULL, 1, NULL), RET_OK);
  ASSERT_TRUE(tk_raw_hash_table_find(NULL, 1) == NULL);
  ASSERT_NE(tk_raw_hash_table_modify(NULL, 1, NULL), RET_OK);
  ASSERT_NE(tk_raw_hash_table_replace(NULL, 1, NULL), RET_OK);
  ASSERT_NE(tk_raw_hash_table_remove(NULL, 1), RET_OK);
  ASSERT_NE(tk_raw_hash_table_clear(NULL), RET_OK);
  ASSERT_EQ(tk_raw_hash_table_size(NULL), 0u);
  ASSERT_NE(tk_raw_hash_table_foreach(NULL, raw_hash_table_test_sum_key, NULL), RET_OK);
  ASSERT_NE(tk_raw_hash_table_foreach(&table, NULL, NULL), RET_OK);

  /* 内部函数：桶的地址为NULL */
  ASSERT_NE(tk_raw_hash_table_bucket_add(NULL, 1, NULL), RET_OK);
  ASSERT_NE(tk_raw_hash_table_shrink(NULL), RET_OK);

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);

  /* deinit之后桶数组为NULL：shrink返回错误，不能崩 */
  ASSERT_NE(tk_raw_hash_table_shrink(&table), RET_OK);
}

/* shrink：空桶整块回收、桶内数组缩到实际大小，元素及其值都不变 */
TEST(RawHashTable, shrink) {
  size_t i = 0;
  size_t n = 200;
  size_t buckets = 0;
  size_t empty = 0;
  size_t slack = 0;
  tk_raw_hash_table_t table;

  ASSERT_EQ(tk_raw_hash_table_init(&table, n, NULL), RET_OK);
  buckets = TK_RAW_HASH_TABLE_BUCKET_COUNT(&table);
  for (i = 0; i < n; i++) {
    ASSERT_EQ(tk_raw_hash_table_add(&table, i, raw_hash_table_test_value(i)), RET_OK);
  }

  /* 删掉一半：桶里会留下空桶和闲置槽位 */
  for (i = 0; i < n; i += 2) {
    ASSERT_EQ(tk_raw_hash_table_remove(&table, i), RET_OK);
  }
  for (i = 0; i < buckets; i++) {
    tk_raw_hash_table_bucket_t bucket = table.buckets[i];
    size_t size = TK_RAW_DARRAY_SIZE(bucket);

    if (bucket != NULL && size == 0) {
      empty++;
    } else if (size > 0 && TK_RAW_DARRAY_CAPACITY(bucket) != size) {
      slack++;
    }
  }
  ASSERT_TRUE(empty > 0 || slack > 0);

  /* 回收：桶个数不变，空桶置空，非空桶缩到实际大小 */
  ASSERT_EQ(tk_raw_hash_table_shrink(&table), RET_OK);
  ASSERT_EQ(TK_RAW_HASH_TABLE_BUCKET_COUNT(&table), buckets);
  for (i = 0; i < buckets; i++) {
    tk_raw_hash_table_bucket_t bucket = table.buckets[i];

    ASSERT_TRUE(bucket == NULL || TK_RAW_DARRAY_SIZE(bucket) > 0);
    ASSERT_EQ(TK_RAW_DARRAY_CAPACITY(bucket), TK_RAW_DARRAY_SIZE(bucket));
  }

  /* 元素和值不变 */
  ASSERT_EQ(tk_raw_hash_table_size(&table), (size_t)(n / 2));
  for (i = 0; i < n; i++) {
    tk_raw_hash_table_entry_t* entry = tk_raw_hash_table_find(&table, i);

    if ((i % 2) == 0) {
      ASSERT_TRUE(entry == NULL);
    } else {
      ASSERT_TRUE(entry != NULL);
      ASSERT_EQ(entry->value, raw_hash_table_test_value(i));
    }
  }

  /* 缩容之后继续加：桶内数组能重新增长 */
  for (i = n; i < n + 20; i++) {
    ASSERT_EQ(tk_raw_hash_table_add(&table, i, raw_hash_table_test_value(i)), RET_OK);
  }
  ASSERT_EQ(tk_raw_hash_table_size(&table), (size_t)(n / 2 + 20));

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}
