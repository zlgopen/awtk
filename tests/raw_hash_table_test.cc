#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "tkc/raw_hash_table.h"
#include "gtest/gtest.h"

/* 键是size_t，值是void*，用整数转指针做值，所以不需要值销毁函数。 */
static void* raw_hash_table_test_value(size_t i) {
  return (void*)(uintptr_t)(i * 10 + 1);
}

/* 依次加入n个元素(键和值都由下标换算)，并校验元素个数逐个加1。 */
static void raw_hash_table_test_fill(tk_raw_hash_table_t* table, size_t n) {
  size_t i = 0;

  for (i = 0; i < n; i++) {
    ASSERT_EQ(tk_raw_hash_table_add(table, i, raw_hash_table_test_value(i)), RET_OK);
    ASSERT_EQ(table->size, i + 1);
  }
}

/* 校验表里正好是填充的n个元素(键和值都对)，且第n个键不存在。 */
static void raw_hash_table_test_expect(tk_raw_hash_table_t* table, size_t n) {
  size_t i = 0;

  ASSERT_EQ(table->size, n);
  for (i = 0; i < n; i++) {
    tk_raw_hash_table_entry_t* entry = tk_raw_hash_table_find(table, i);

    ASSERT_TRUE(entry != NULL);
    ASSERT_EQ(entry->key, i);
    ASSERT_EQ(entry->value, raw_hash_table_test_value(i));
  }
  ASSERT_TRUE(tk_raw_hash_table_find(table, n) == NULL);
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

TEST(RawHashTable, basic) {
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 200, NULL), RET_OK);
  ASSERT_EQ(TK_RAW_HASH_TABLE_BUCKET_COUNT(&table), 300u);

  raw_hash_table_test_fill(&table, 100);
  raw_hash_table_test_expect(&table, 100);

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

/* 单键上的各种操作：重复add失败、modify改值/键不存在失败、set写入值/键不存在时加入。 */
TEST(RawHashTable, one_key_ops) {
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, NULL), RET_OK);

  /* 键已存在时加入失败，原来的元素不受影响。 */
  ASSERT_EQ(tk_raw_hash_table_add(&table, 5, raw_hash_table_test_value(1)), RET_OK);
  ASSERT_EQ(table.size, 1u);
  ASSERT_EQ(tk_raw_hash_table_find(&table, 5)->value, raw_hash_table_test_value(1));
  ASSERT_EQ(tk_raw_hash_table_add(&table, 5, raw_hash_table_test_value(2)), RET_FAIL);
  ASSERT_EQ(table.size, 1u);
  ASSERT_EQ(TK_RAW_DARRAY_SIZE(table.buckets[tk_raw_hash_table_bucket_index(&table, 5)]), 1u);
  ASSERT_EQ(tk_raw_hash_table_find(&table, 5)->value, raw_hash_table_test_value(1));

  /* 修改已有的元素(size_t键，看不出换键)：元素个数和桶里元素个数都不变。 */
  ASSERT_EQ(tk_raw_hash_table_modify(&table, 5, raw_hash_table_test_value(2)), RET_OK);
  ASSERT_EQ(table.size, 1u);
  ASSERT_EQ(TK_RAW_DARRAY_SIZE(table.buckets[tk_raw_hash_table_bucket_index(&table, 5)]), 1u);
  ASSERT_EQ(tk_raw_hash_table_find(&table, 5)->value, raw_hash_table_test_value(2));

  /* 键不存在时修改失败，也不会新增元素。 */
  ASSERT_EQ(tk_raw_hash_table_modify(&table, 6, raw_hash_table_test_value(3)), RET_NOT_FOUND);
  ASSERT_EQ(table.size, 1u);
  ASSERT_TRUE(tk_raw_hash_table_find(&table, 6) == NULL);

  /* 键存在时set只改值，元素个数不变，桶里仍然只有一个元素。 */
  ASSERT_EQ(tk_raw_hash_table_set(&table, 5, raw_hash_table_test_value(3)), RET_OK);
  ASSERT_EQ(table.size, 1u);
  ASSERT_EQ(TK_RAW_DARRAY_SIZE(table.buckets[tk_raw_hash_table_bucket_index(&table, 5)]), 1u);
  ASSERT_EQ(tk_raw_hash_table_find(&table, 5)->value, raw_hash_table_test_value(3));

  ASSERT_EQ(tk_raw_hash_table_remove(&table, 5), RET_OK);
  ASSERT_EQ(table.size, 0u);
  ASSERT_EQ(TK_RAW_DARRAY_SIZE(table.buckets[tk_raw_hash_table_bucket_index(&table, 5)]), 0u);
  ASSERT_EQ(tk_raw_hash_table_remove(&table, 5), RET_NOT_FOUND);

  /* 键不存在时set加入。 */
  ASSERT_EQ(tk_raw_hash_table_set(&table, 7, raw_hash_table_test_value(4)), RET_OK);
  ASSERT_EQ(table.size, 1u);
  ASSERT_EQ(tk_raw_hash_table_find(&table, 7)->value, raw_hash_table_test_value(4));

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, remove) {
  size_t sum = 0;
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, NULL), RET_OK);

  raw_hash_table_test_fill(&table, 20);

  ASSERT_EQ(tk_raw_hash_table_remove(&table, 7), RET_OK);
  ASSERT_EQ(table.size, 19u);
  ASSERT_TRUE(tk_raw_hash_table_find(&table, 7) == NULL);
  ASSERT_EQ(tk_raw_hash_table_remove(&table, 7), RET_NOT_FOUND);
  ASSERT_EQ(table.size, 19u);

  ASSERT_EQ(tk_raw_hash_table_foreach(&table, raw_hash_table_test_sum_key, &sum), RET_OK);
  ASSERT_EQ(sum, 190 - 7);

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, clear) {
  size_t i = 0;
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, NULL), RET_OK);

  raw_hash_table_test_fill(&table, 20);
  ASSERT_EQ(table.size, 20u);

  /* 清空后桶数不变，但所有桶都被回收。 */
  ASSERT_EQ(tk_raw_hash_table_clear(&table), RET_OK);
  ASSERT_EQ(table.size, 0u);
  ASSERT_EQ(TK_RAW_HASH_TABLE_BUCKET_COUNT(&table), 15u);
  for (i = 0; i < TK_RAW_HASH_TABLE_BUCKET_COUNT(&table); i++) {
    ASSERT_TRUE(table.buckets[i] == NULL);
  }

  /* 清空后可以继续用。 */
  ASSERT_EQ(tk_raw_hash_table_add(&table, 1, raw_hash_table_test_value(1)), RET_OK);
  ASSERT_EQ(table.size, 1u);

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, init) {
  size_t i = 0;
  size_t index = 0;
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, NULL), RET_OK);
  ASSERT_TRUE(table.buckets != NULL);
  ASSERT_EQ(TK_RAW_HASH_TABLE_BUCKET_COUNT(&table), 15u);
  ASSERT_EQ(table.capacity, 10u);
  ASSERT_EQ(table.size, 0u);

  /* 一开始所有桶都是NULL。 */
  for (i = 0; i < TK_RAW_HASH_TABLE_BUCKET_COUNT(&table); i++) {
    ASSERT_TRUE(table.buckets[i] == NULL);
  }

  index = tk_raw_hash_table_bucket_index(&table, 11);
  ASSERT_EQ(index, tk_raw_hash_table_bucket_index(&table, 11));

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
  ASSERT_TRUE(table.buckets == NULL);
  ASSERT_EQ(table.size, 0u);
  ASSERT_EQ(table.capacity, 0u);

  ASSERT_NE(tk_raw_hash_table_init(&table, 0, NULL), RET_OK);
  ASSERT_TRUE(table.buckets == NULL);
  ASSERT_NE(tk_raw_hash_table_deinit(NULL), RET_OK);

  /* deinit之后可以重新init */
  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, NULL), RET_OK);
  ASSERT_TRUE(table.buckets != NULL);
  ASSERT_EQ(TK_RAW_HASH_TABLE_BUCKET_COUNT(&table), 15u);
  ASSERT_EQ(table.size, 0u);
  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
  ASSERT_TRUE(table.buckets == NULL);
}

/* reserve：容量变大就加桶并rehash；已经够用时就什么都不做(不缩桶，也不改capacity)。 */
TEST(RawHashTable, reserve) {
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, NULL), RET_OK);
  raw_hash_table_test_fill(&table, 10);

  /* 变大：桶数按新容量换算，元素搬家后一个不少 */
  ASSERT_EQ(tk_raw_hash_table_reserve(&table, 100), RET_OK);
  ASSERT_EQ(table.capacity, 100u);
  ASSERT_EQ(TK_RAW_HASH_TABLE_BUCKET_COUNT(&table), 150u);
  raw_hash_table_test_expect(&table, 10);

  /* 变小但不小于size：已经够用，什么都不做，capacity和桶数都不变 */
  ASSERT_EQ(tk_raw_hash_table_reserve(&table, 10), RET_OK);
  ASSERT_EQ(table.capacity, 100u);
  ASSERT_EQ(TK_RAW_HASH_TABLE_BUCKET_COUNT(&table), 150u);

  /* 比size还小：失败，表不变 */
  ASSERT_NE(tk_raw_hash_table_reserve(&table, 9), RET_OK);
  ASSERT_EQ(table.capacity, 100u);
  ASSERT_EQ(TK_RAW_HASH_TABLE_BUCKET_COUNT(&table), 150u);
  raw_hash_table_test_expect(&table, 10);

  /* 非法参数 */
  ASSERT_NE(tk_raw_hash_table_reserve(NULL, 100), RET_OK);
  ASSERT_NE(tk_raw_hash_table_reserve(&table, 0), RET_OK);

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);

  /* 没init的表 */
  memset(&table, 0x00, sizeof(table));
  ASSERT_NE(tk_raw_hash_table_reserve(&table, 100), RET_OK);
}

/* size将要超过capacity时的三种处理策略：忽略(默认)、返回失败、自动扩容。 */
TEST(RawHashTable, capacity_policy) {
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 2, NULL), RET_OK);
  ASSERT_EQ(table.capacity_policy, TK_RAW_HASH_TABLE_CAPACITY_POLICY_IGNORE);

  /* 默认忽略：超过capacity照样加入 */
  raw_hash_table_test_fill(&table, 3);
  ASSERT_EQ(table.size, 3u);
  ASSERT_EQ(table.capacity, 2u);

  /* 返回失败：设置策略本身总是成功，之后add/set才被拒 */
  ASSERT_EQ(tk_raw_hash_table_set_capacity_policy(&table, TK_RAW_HASH_TABLE_CAPACITY_POLICY_FAIL),
            RET_OK);
  ASSERT_EQ(table.capacity_policy, TK_RAW_HASH_TABLE_CAPACITY_POLICY_FAIL);
  ASSERT_EQ(tk_raw_hash_table_add(&table, 3, raw_hash_table_test_value(3)), RET_EXCEED_RANGE);
  ASSERT_EQ(tk_raw_hash_table_set(&table, 3, raw_hash_table_test_value(3)), RET_EXCEED_RANGE);
  ASSERT_TRUE(tk_raw_hash_table_find(&table, 3) == NULL);
  /* 键已存在的add不增长，不归策略管：只返回RET_FAIL */
  ASSERT_EQ(tk_raw_hash_table_add(&table, 2, raw_hash_table_test_value(2)), RET_FAIL);

  /* 腾出位置(size < capacity)就能加 */
  ASSERT_EQ(tk_raw_hash_table_remove(&table, 0), RET_OK);
  ASSERT_EQ(tk_raw_hash_table_remove(&table, 1), RET_OK);
  ASSERT_EQ(tk_raw_hash_table_add(&table, 3, raw_hash_table_test_value(3)), RET_OK);
  ASSERT_EQ(table.size, 2u);

  /* 自动扩容：设置时没超过capacity就不动，加入时超了才扩 */
  ASSERT_EQ(tk_raw_hash_table_set_capacity_policy(&table, TK_RAW_HASH_TABLE_CAPACITY_POLICY_GROW),
            RET_OK);
  ASSERT_EQ(table.capacity, 2u);
  ASSERT_EQ(tk_raw_hash_table_add(&table, 4, raw_hash_table_test_value(4)), RET_OK);
  ASSERT_EQ(table.capacity, 4u);
  ASSERT_EQ(TK_RAW_HASH_TABLE_BUCKET_COUNT(&table), 6u);
  ASSERT_EQ(tk_raw_hash_table_find(&table, 2)->value, raw_hash_table_test_value(2));

  /* 加到正好capacity，再加已存在的键：不增长，所以不扩容 */
  ASSERT_EQ(tk_raw_hash_table_add(&table, 5, raw_hash_table_test_value(5)), RET_OK);
  ASSERT_EQ(tk_raw_hash_table_add(&table, 2, raw_hash_table_test_value(2)), RET_FAIL);
  ASSERT_EQ(table.capacity, 4u);

  /* 桶级接口不管策略(cache_clock那类用法自己处理淘汰)：加到超过capacity也不扩、不报错 */
  ASSERT_EQ(tk_raw_hash_table_bucket_add(&table, tk_raw_hash_table_bucket_index(&table, 8), 8,
                                         raw_hash_table_test_value(8)),
            RET_OK);
  ASSERT_EQ(tk_raw_hash_table_bucket_add(&table, tk_raw_hash_table_bucket_index(&table, 9), 9,
                                         raw_hash_table_test_value(9)),
            RET_OK);
  ASSERT_EQ(table.size, 6u);
  ASSERT_EQ(table.capacity, 4u);
  ASSERT_EQ(TK_RAW_HASH_TABLE_BUCKET_COUNT(&table), 6u);

  /* 设置接口的边界：NULL失败；策略没变就不检查(已经超容量，检查的话会扩到10) */
  ASSERT_NE(tk_raw_hash_table_set_capacity_policy(NULL, TK_RAW_HASH_TABLE_CAPACITY_POLICY_GROW),
            RET_OK);
  ASSERT_EQ(tk_raw_hash_table_set_capacity_policy(&table, TK_RAW_HASH_TABLE_CAPACITY_POLICY_GROW),
            RET_OK);
  ASSERT_EQ(table.capacity, 4u);

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, shrink) {
  size_t i = 0;
  size_t n = 200;
  size_t buckets = 0;
  size_t empty = 0;
  size_t slack = 0;
  tk_raw_hash_table_t table;

  ASSERT_EQ(tk_raw_hash_table_init(&table, n, NULL), RET_OK);
  buckets = TK_RAW_HASH_TABLE_BUCKET_COUNT(&table);
  raw_hash_table_test_fill(&table, n);

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
  ASSERT_EQ(table.size, (size_t)(n / 2));
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
  ASSERT_EQ(table.size, (size_t)(n / 2 + 20));

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, foreach_add) {
  size_t i = 0;
  raw_hash_table_visit_add_ctx_t ctx;
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 100, NULL), RET_OK);

  raw_hash_table_test_fill(&table, 10);

  ctx.table = &table;
  ctx.nr = 0;
  ASSERT_EQ(tk_raw_hash_table_foreach(&table, raw_hash_table_test_visit_add, &ctx), RET_OK);

  /* 原来10个都访问到了，新加的那个可能也被访问到 */
  ASSERT_GE(ctx.nr, 10u);
  ASSERT_LE(ctx.nr, 11u);
  ASSERT_EQ(table.size, 11u);
  ASSERT_TRUE(tk_raw_hash_table_find(&table, 1000) != NULL);
  for (i = 0; i < 10; i++) {
    ASSERT_TRUE(tk_raw_hash_table_find(&table, i) != NULL);
  }

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, foreach_remove) {
  size_t sum = 0;
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 100, NULL), RET_OK);

  raw_hash_table_test_fill(&table, 100);

  ASSERT_EQ(tk_raw_hash_table_foreach(&table, raw_hash_table_test_remove_even_key, NULL), RET_OK);
  ASSERT_EQ(table.size, 50u);

  ASSERT_EQ(tk_raw_hash_table_foreach(&table, raw_hash_table_test_sum_key, &sum), RET_OK);
  ASSERT_EQ(sum, 2500);

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, foreach_visit_result) {
  size_t i = 0;
  size_t count = 0;
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, NULL), RET_OK);

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
  ASSERT_EQ(TK_RAW_HASH_TABLE_BUCKET_COUNT(&table), 1u);

  /* 只有一个桶，元素全挤在一起，桶内数组会扩容。 */
  raw_hash_table_test_fill(&table, 200);
  ASSERT_EQ(table.size, 200u);

  for (i = 0; i < 200; i++) {
    ASSERT_TRUE(tk_raw_hash_table_find(&table, i) != NULL);
  }

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

/* 桶是懒创建的：只有加入过元素的桶才分配；删元素后桶保留(复用同一块)，clear时才回收。 */
TEST(RawHashTable, bucket_lifecycle) {
  size_t i = 0;
  size_t index = 0;
  size_t non_null = 0;
  tk_raw_hash_table_bucket_t bucket = NULL;
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, NULL), RET_OK);

  index = tk_raw_hash_table_bucket_index(&table, 11);
  ASSERT_EQ(tk_raw_hash_table_add(&table, 11, raw_hash_table_test_value(11)), RET_OK);
  ASSERT_EQ(table.size, 1u);

  /* 只有加入元素的那个桶才创建raw_darray。 */
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
  bucket = table.buckets[index];
  ASSERT_TRUE(bucket != NULL);

  /* 删除元素后桶保留（回收只在clear/deinit时做），只是桶里没有元素了。 */
  ASSERT_EQ(tk_raw_hash_table_remove(&table, 11), RET_OK);
  ASSERT_TRUE(table.buckets[index] == bucket);
  ASSERT_EQ(TK_RAW_DARRAY_SIZE(table.buckets[index]), 0u);
  ASSERT_EQ(table.size, 0u);

  /* 再次加入同一个键，复用同一个桶，不重新分配。 */
  ASSERT_EQ(tk_raw_hash_table_add(&table, 11, raw_hash_table_test_value(2)), RET_OK);
  ASSERT_TRUE(table.buckets[index] == bucket);
  ASSERT_EQ(TK_RAW_DARRAY_SIZE(table.buckets[index]), 1u);

  /* clear时才回收桶。 */
  ASSERT_EQ(tk_raw_hash_table_clear(&table), RET_OK);
  ASSERT_TRUE(table.buckets[index] == NULL);
  ASSERT_EQ(TK_RAW_HASH_TABLE_BUCKET_COUNT(&table), 15u);

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

/* 直接用桶级接口增删，size也要跟着变(cache_clock那类用法依赖它)。 */
TEST(RawHashTable, bucket_size) {
  size_t index = 0;
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, NULL), RET_OK);

  index = tk_raw_hash_table_bucket_index(&table, 11);
  ASSERT_EQ(tk_raw_hash_table_bucket_add(&table, index, 11, raw_hash_table_test_value(11)), RET_OK);
  ASSERT_EQ(table.size, 1u);

  /* 参数不对时不改个数 */
  ASSERT_NE(tk_raw_hash_table_bucket_add(&table, TK_RAW_HASH_TABLE_BUCKET_COUNT(&table), 12,
                                         raw_hash_table_test_value(12)),
            RET_OK);
  ASSERT_EQ(table.size, 1u);

  ASSERT_EQ(tk_raw_hash_table_bucket_remove(&table, index, 0), RET_OK);
  ASSERT_EQ(table.size, 0u);
  ASSERT_NE(tk_raw_hash_table_bucket_remove(&table, index, 0), RET_OK);
  ASSERT_EQ(table.size, 0u);

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, bucket_shrink) {
  size_t i = 0;
  size_t n = 200;
  size_t index = 0;
  size_t buckets = 0;
  size_t key0 = 0;
  size_t key1 = 0;
  tk_raw_hash_table_entry_t* entry = NULL;
  tk_raw_hash_table_bucket_t bucket = NULL;
  tk_raw_hash_table_t table;

  ASSERT_EQ(tk_raw_hash_table_init(&table, n, NULL), RET_OK);
  buckets = TK_RAW_HASH_TABLE_BUCKET_COUNT(&table);
  raw_hash_table_test_fill(&table, n);

  /* 找一个只有一个元素的桶：删掉它的键以后桶就空了(remove自己不回收，桶槽位还在) */
  for (index = 0; index < buckets; index++) {
    if (TK_RAW_DARRAY_SIZE(table.buckets[index]) == 1) {
      break;
    }
  }
  ASSERT_LT(index, buckets);
  key0 = table.buckets[index][0].key;
  ASSERT_EQ(tk_raw_hash_table_remove(&table, key0), RET_OK);
  ASSERT_TRUE(table.buckets[index] != NULL);
  ASSERT_EQ(TK_RAW_DARRAY_SIZE(table.buckets[index]), 0u);
  ASSERT_EQ(table.size, (size_t)(n - 1));

  /* 空桶：整块回收，槽位恢复成没用过；重复调用是no-op */
  ASSERT_EQ(tk_raw_hash_table_bucket_shrink(&table, index), RET_OK);
  ASSERT_TRUE(table.buckets[index] == NULL);
  ASSERT_EQ(tk_raw_hash_table_bucket_shrink(&table, index), RET_OK);

  /* 非空桶：缩到实际大小 */
  for (index = 0; index < buckets; index++) {
    if (TK_RAW_DARRAY_SIZE(table.buckets[index]) >= 2) {
      break;
    }
  }
  ASSERT_LT(index, buckets);
  bucket = table.buckets[index];
  key1 = bucket[0].key;
  ASSERT_EQ(tk_raw_hash_table_remove(&table, key1), RET_OK);
  ASSERT_EQ(table.size, (size_t)(n - 2));

  /* 删元素不会收缩桶内数组：容量仍然比实际大小大 */
  ASSERT_TRUE(TK_RAW_DARRAY_CAPACITY(table.buckets[index]) >
              TK_RAW_DARRAY_SIZE(table.buckets[index]));
  ASSERT_EQ(tk_raw_hash_table_bucket_shrink(&table, index), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_CAPACITY(table.buckets[index]), TK_RAW_DARRAY_SIZE(table.buckets[index]));

  /* 桶的个数不变，元素和值都不动 */
  ASSERT_EQ(TK_RAW_HASH_TABLE_BUCKET_COUNT(&table), buckets);
  for (i = 0; i < n; i++) {
    entry = tk_raw_hash_table_find(&table, i);

    if (i == key0 || i == key1) {
      ASSERT_TRUE(entry == NULL);
    } else {
      ASSERT_TRUE(entry != NULL);
      ASSERT_EQ(entry->value, raw_hash_table_test_value(i));
    }
  }

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

/* 非法用法：NULL指针、没init/已deinit的表、越界的桶下标。这些只断言"不是RET_OK"。 */
TEST(RawHashTable, invalid_usage) {
  size_t sum = 0;
  tk_raw_hash_table_t table;

  /* table还没init：查询按空表处理，修改类操作返回错误 */
  memset(&table, 0x00, sizeof(table));
  ASSERT_NE(tk_raw_hash_table_add(&table, 1, NULL), RET_OK);
  ASSERT_NE(tk_raw_hash_table_modify(&table, 1, NULL), RET_OK);
  ASSERT_NE(tk_raw_hash_table_set(&table, 1, NULL), RET_OK);
  ASSERT_NE(tk_raw_hash_table_remove(&table, 1), RET_OK);
  ASSERT_NE(tk_raw_hash_table_clear(&table), RET_OK);
  ASSERT_NE(tk_raw_hash_table_deinit(&table), RET_OK);
  ASSERT_TRUE(tk_raw_hash_table_find(&table, 1) == NULL);
  ASSERT_EQ(table.size, 0u);
  ASSERT_NE(tk_raw_hash_table_foreach(&table, raw_hash_table_test_sum_key, &sum), RET_OK);
  ASSERT_EQ(sum, 0u);

  /* table指针本身为NULL */
  ASSERT_NE(tk_raw_hash_table_init(NULL, 10, NULL), RET_OK);
  ASSERT_NE(tk_raw_hash_table_add(NULL, 1, NULL), RET_OK);
  ASSERT_TRUE(tk_raw_hash_table_find(NULL, 1) == NULL);
  ASSERT_NE(tk_raw_hash_table_modify(NULL, 1, NULL), RET_OK);
  ASSERT_NE(tk_raw_hash_table_set(NULL, 1, NULL), RET_OK);
  ASSERT_NE(tk_raw_hash_table_remove(NULL, 1), RET_OK);
  ASSERT_NE(tk_raw_hash_table_clear(NULL), RET_OK);
  ASSERT_NE(tk_raw_hash_table_foreach(NULL, raw_hash_table_test_sum_key, NULL), RET_OK);

  /* 内部函数：NULL表、NULL桶地址 */
  ASSERT_NE(tk_raw_hash_table_bucket_add(NULL, 0, 1, NULL), RET_OK);
  ASSERT_NE(tk_raw_hash_table_bucket_modify(NULL, NULL, 1, NULL), RET_OK);
  ASSERT_NE(tk_raw_hash_table_shrink(NULL), RET_OK);
  ASSERT_NE(tk_raw_hash_table_bucket_shrink(NULL, 0), RET_OK);

  /* 初始化之后还没有元素：遍历直接返回RET_OK；遍历函数为NULL要报错 */
  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, NULL), RET_OK);
  ASSERT_EQ(tk_raw_hash_table_foreach(&table, raw_hash_table_test_sum_key, &sum), RET_OK);
  ASSERT_EQ(sum, 0u);
  ASSERT_EQ(table.size, 0u);
  ASSERT_NE(tk_raw_hash_table_foreach(&table, NULL, NULL), RET_OK);

  /* 内部函数：桶下标越界 */
  ASSERT_NE(tk_raw_hash_table_bucket_shrink(&table, TK_RAW_HASH_TABLE_BUCKET_COUNT(&table)),
            RET_OK);

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);

  /* deinit之后桶数组为NULL：修改类操作和shrink都返回错误，不能崩；重新init后可以继续用 */
  ASSERT_NE(tk_raw_hash_table_shrink(&table), RET_OK);
  ASSERT_NE(tk_raw_hash_table_bucket_shrink(&table, 0), RET_OK);
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
          ASSERT_EQ(tk_raw_hash_table_set(&table, k, raw_hash_table_test_value(op)), RET_OK);
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
    ASSERT_EQ(table.size, total);
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

TEST(RawHashTable, hash) {
  size_t i = 0;
  size_t max_chain = 0;
  tk_raw_hash_table_t table;

  /* 连续的键要散开到很多桶里，不能挤在少数几个桶里。 */
  ASSERT_EQ(tk_raw_hash_table_init(&table, 1000, NULL), RET_OK);
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

TEST(RawHashTable, value_is_same_object) {
  void* a = (void*)(uintptr_t)0x1000;
  void* b = (void*)(uintptr_t)0x2000;

  /* 默认按值(指针)比较：相同返回真，否则返回假。 */
  ASSERT_TRUE(TK_RAW_HASH_TABLE_VALUE_IS_SAME_OBJECT(a, a, NULL));
  /* NULL在C++里是0，要和指针比较得先转成void*，否则g++报comparison between pointer and integer */
  ASSERT_TRUE(TK_RAW_HASH_TABLE_VALUE_IS_SAME_OBJECT((void*)NULL, (void*)NULL, NULL));
  ASSERT_FALSE(TK_RAW_HASH_TABLE_VALUE_IS_SAME_OBJECT(a, b, NULL));
  ASSERT_FALSE(TK_RAW_HASH_TABLE_VALUE_IS_SAME_OBJECT(a, (void*)NULL, NULL));
}
