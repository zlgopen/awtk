#include <stdint.h>
#include <string.h>

#include "tkc/utils.h"
#include "tkc/named_value_hash.h"

typedef struct _cache_clock_test_info_t {
  int32_t a;
  int32_t b;
} cache_clock_test_info_t;

static size_t s_destroy_count = 0;

#define TK_CACHE_CLOCK_KEY_T const char*
#define TK_CACHE_CLOCK_KEY_HASH(key) named_value_hash_get_hash_from_str(key)
#define TK_CACHE_CLOCK_KEY_COMPARE(entry_key, key) tk_strcmp(entry_key, key)
#define TK_CACHE_CLOCK_DATA_T cache_clock_test_info_t
/**
 * 结构体不支持==，要自己定义"是不是同一个对象"。
 * 故意只比a：b可以变而不触发销毁，用来验证"判成同一个对象也要把新数据写进去"。
 */
#define TK_CACHE_CLOCK_DATA_IS_SAME_OBJECT(entry_data, data) (((entry_data).a == (data).a) ? 1 : 0)

#include "tkc/cache_clock.h"
#include "gtest/gtest.h"

#define CACHE_CLOCK_TEST_KEY_MAX 1000

static char s_keys[CACHE_CLOCK_TEST_KEY_MAX][16];

/* 数据被淘汰/删除/替换时计数 */
static ret_t cache_clock_test_data_destroy(cache_clock_test_info_t data) {
  s_destroy_count++;
  (void)(data);

  return RET_OK;
}

static cache_clock_test_info_t cache_clock_test_info(int32_t a, int32_t b) {
  cache_clock_test_info_t info;

  info.a = a;
  info.b = b;

  return info;
}

/* 动态生成键：键的地址要稳定，所以放在静态数组里 */
static const char* cache_clock_test_key(size_t i) {
  tk_snprintf(s_keys[i], sizeof(s_keys[i]), "key%u", (unsigned)i);

  return s_keys[i];
}

/* 命中返回a字段，未命中返回-1 */
static int32_t cache_clock_test_get(tk_cache_clock_t* cache, const char* key) {
  cache_clock_test_info_t out;

  memset(&out, 0x00, sizeof(out));
  if (tk_cache_clock_get(cache, key, &out) == RET_OK) {
    return out.a;
  }

  return -1;
}

/* 校验自洽：桶里元素个数之和等于size，每个元素都能查回来，hand在桶数范围内。*/
/* 只读：不调用tk_cache_clock_get，免得顺手把访问位置1影响后面的断言。 */
static void cache_clock_test_check(tk_cache_clock_t* cache) {
  size_t i = 0, j = 0, n = 0;
  size_t buckets = TK_RAW_HASH_TABLE_BUCKET_COUNT(&(cache->table));

  for (i = 0; i < buckets; i++) {
    tk_raw_hash_table_bucket_t bucket = cache->table.buckets[i];

    for (j = 0; j < TK_RAW_DARRAY_SIZE(bucket); j++) {
      tk_raw_hash_table_entry_t* entry = tk_raw_hash_table_find(&(cache->table), bucket[j].key);

      n++;
      ASSERT_TRUE(entry != NULL);
      ASSERT_EQ(entry->value.a, bucket[j].value.a);
      ASSERT_EQ(entry->value.b, bucket[j].value.b);
    }
  }

  ASSERT_EQ(n, cache->table.size);
  ASSERT_LT(cache->hand, buckets);
}

/* 找一个落在第index个桶里的键(跳过skip1/skip2，可以为NULL)，找不到返回NULL。 */
static const char* cache_clock_test_key_in_bucket(tk_cache_clock_t* cache, size_t index,
                                                  const char* skip1, const char* skip2) {
  size_t i = 0;

  for (i = 0; i < CACHE_CLOCK_TEST_KEY_MAX; i++) {
    const char* key = cache_clock_test_key(i);

    if (tk_raw_hash_table_bucket_index(&(cache->table), key) != index) {
      continue;
    }
    if ((skip1 != NULL && tk_strcmp(key, skip1) == 0) ||
        (skip2 != NULL && tk_strcmp(key, skip2) == 0)) {
      continue;
    }

    return key;
  }

  return NULL;
}

TEST(CacheClock, basic) {
  cache_clock_test_info_t out;
  tk_cache_clock_t cache;

  s_destroy_count = 0;
  ASSERT_EQ(tk_cache_clock_init(&cache, 3, cache_clock_test_data_destroy), RET_OK);
  ASSERT_EQ(cache.table.size, 0u);
  ASSERT_EQ(cache.table.capacity, 3u);
  ASSERT_TRUE(cache.flags != NULL);

  ASSERT_EQ(tk_cache_clock_put(&cache, "a", cache_clock_test_info(100, 1)), RET_OK);
  ASSERT_EQ(cache.table.size, 1u);
  ASSERT_EQ(cache_clock_test_get(&cache, "a"), 100);

  /* 未命中返回RET_NOT_FOUND，且不改写out */
  out.a = 77;
  ASSERT_EQ(tk_cache_clock_get(&cache, "b", &out), RET_NOT_FOUND);
  ASSERT_EQ(out.a, 77);
  cache_clock_test_check(&cache);

  ASSERT_EQ(tk_cache_clock_deinit(&cache), RET_OK);
  ASSERT_EQ(s_destroy_count, 1u);
  ASSERT_EQ(cache.table.size, 0u);
  ASSERT_TRUE(cache.flags == NULL);
}

/* 淘汰：容量1时每次put淘汰上一个；反复churn后不会留下空桶(空桶当场整块回收)。 */
TEST(CacheClock, evict) {
  size_t i = 0;
  size_t live = 0;
  size_t buckets = 0;
  tk_cache_clock_t cache;

  s_destroy_count = 0;
  ASSERT_EQ(tk_cache_clock_init(&cache, 1, cache_clock_test_data_destroy), RET_OK);

  /* 容量1：每次put都淘汰上一个 */
  ASSERT_EQ(tk_cache_clock_put(&cache, "a", cache_clock_test_info(10, 0)), RET_OK);
  ASSERT_EQ(tk_cache_clock_put(&cache, "b", cache_clock_test_info(20, 0)), RET_OK);
  ASSERT_EQ(cache.table.size, 1u);
  ASSERT_EQ(s_destroy_count, 1u);
  ASSERT_EQ(cache_clock_test_get(&cache, "a"), -1);
  ASSERT_EQ(cache_clock_test_get(&cache, "b"), 20);
  cache_clock_test_check(&cache);

  ASSERT_EQ(tk_cache_clock_deinit(&cache), RET_OK);
  ASSERT_EQ(s_destroy_count, 2u);

  /* 长时间反复淘汰：淘汰把桶清空时立即整块回收，churn之后不会留下空桶 */
  s_destroy_count = 0;
  ASSERT_EQ(tk_cache_clock_init(&cache, 4, cache_clock_test_data_destroy), RET_OK);
  buckets = TK_RAW_HASH_TABLE_BUCKET_COUNT(&(cache.table));

  for (i = 0; i < CACHE_CLOCK_TEST_KEY_MAX; i++) {
    const char* key = cache_clock_test_key(i);

    ASSERT_EQ(tk_cache_clock_put(&cache, key, cache_clock_test_info((int32_t)i, 0)), RET_OK);
    ASSERT_LE(cache.table.size, 4u);
  }
  ASSERT_EQ(cache.table.size, 4u);
  cache_clock_test_check(&cache);

  /* 非NULL的桶里一定有元素：空桶在淘汰那一刻就还给分配器了 */
  for (i = 0; i < buckets; i++) {
    tk_raw_hash_table_bucket_t bucket = cache.table.buckets[i];

    ASSERT_TRUE(bucket == NULL || TK_RAW_DARRAY_SIZE(bucket) > 0);
    if (bucket != NULL) {
      live++;
    }
  }
  ASSERT_LE(live, cache.table.size);

  ASSERT_EQ(tk_cache_clock_deinit(&cache), RET_OK);
}

TEST(CacheClock, update) {
  cache_clock_test_info_t info;
  cache_clock_test_info_t out;
  tk_cache_clock_t cache;

  s_destroy_count = 0;
  ASSERT_EQ(tk_cache_clock_init(&cache, 3, cache_clock_test_data_destroy), RET_OK);

  info = cache_clock_test_info(1, 2);
  ASSERT_EQ(tk_cache_clock_put(&cache, "a", info), RET_OK);

  /* 数据是按值拷贝进去的，之后改局部变量不影响缓存 */
  info.a = 10;
  info.b = 20;
  ASSERT_EQ(tk_cache_clock_get(&cache, "a", &out), RET_OK);
  ASSERT_EQ(out.a, 1);
  ASSERT_EQ(out.b, 2);

  /* 同一个键只占一个元素，旧数据由销毁函数销毁 */
  ASSERT_EQ(tk_cache_clock_put(&cache, "a", info), RET_OK);
  ASSERT_EQ(cache.table.size, 1u);
  ASSERT_EQ(s_destroy_count, 1u);
  ASSERT_EQ(cache_clock_test_get(&cache, "a"), 10);
  cache_clock_test_check(&cache);

  /* 值和旧的一样：只置访问位，不销毁(调用方把取出来的数据写回去是安全的) */
  ASSERT_EQ(tk_cache_clock_put(&cache, "a", info), RET_OK);
  ASSERT_EQ(cache.table.size, 1u);
  ASSERT_EQ(s_destroy_count, 1u);
  ASSERT_EQ(cache_clock_test_get(&cache, "a"), 10);
  cache_clock_test_check(&cache);

  /* 比较相等(只比a)但数据不同：不销毁旧数据，新数据也必须写进去 */
  info.b = 99;
  ASSERT_EQ(tk_cache_clock_put(&cache, "a", info), RET_OK);
  ASSERT_EQ(cache.table.size, 1u);
  ASSERT_EQ(s_destroy_count, 1u);
  ASSERT_EQ(tk_cache_clock_get(&cache, "a", &out), RET_OK);
  ASSERT_EQ(out.a, 10);
  ASSERT_EQ(out.b, 99);
  cache_clock_test_check(&cache);

  ASSERT_EQ(tk_cache_clock_deinit(&cache), RET_OK);
  ASSERT_EQ(s_destroy_count, 2u);
}

TEST(CacheClock, remove_clear) {
  tk_cache_clock_t cache;

  s_destroy_count = 0;
  ASSERT_EQ(tk_cache_clock_init(&cache, 10, cache_clock_test_data_destroy), RET_OK);

  ASSERT_EQ(tk_cache_clock_put(&cache, "a", cache_clock_test_info(1, 0)), RET_OK);
  ASSERT_EQ(tk_cache_clock_put(&cache, "b", cache_clock_test_info(2, 0)), RET_OK);
  ASSERT_EQ(tk_cache_clock_put(&cache, "c", cache_clock_test_info(3, 0)), RET_OK);

  ASSERT_EQ(tk_cache_clock_remove(&cache, "b"), RET_OK);
  ASSERT_EQ(cache.table.size, 2u);
  ASSERT_EQ(s_destroy_count, 1u);
  ASSERT_EQ(cache_clock_test_get(&cache, "b"), -1);
  ASSERT_EQ(tk_cache_clock_remove(&cache, "b"), RET_NOT_FOUND);
  cache_clock_test_check(&cache);

  ASSERT_EQ(tk_cache_clock_clear(&cache), RET_OK);
  ASSERT_EQ(cache.table.size, 0u);
  ASSERT_EQ(s_destroy_count, 3u);
  ASSERT_EQ(cache.hand, 0u);
  cache_clock_test_check(&cache);

  /* clear之后可以继续用 */
  ASSERT_EQ(tk_cache_clock_put(&cache, "g", cache_clock_test_info(7, 0)), RET_OK);
  ASSERT_EQ(cache_clock_test_get(&cache, "g"), 7);

  ASSERT_EQ(tk_cache_clock_deinit(&cache), RET_OK);
  ASSERT_EQ(s_destroy_count, 4u);
}

TEST(CacheClock, collide_bucket) {
  const char* first = NULL;
  const char* second = NULL;
  const char* other = NULL;
  tk_cache_clock_t cache;

  s_destroy_count = 0;
  ASSERT_EQ(tk_cache_clock_init(&cache, 2, cache_clock_test_data_destroy), RET_OK);

  /* 找两个落在同一个桶(桶0)的键，再找一个落在别的桶(桶1)的键 */
  first = cache_clock_test_key_in_bucket(&cache, 0, NULL, NULL);
  second = cache_clock_test_key_in_bucket(&cache, 0, first, NULL);
  other = cache_clock_test_key_in_bucket(&cache, 1, first, second);
  ASSERT_TRUE(first != NULL && second != NULL && other != NULL);

  ASSERT_EQ(tk_cache_clock_put(&cache, first, cache_clock_test_info(1, 1)), RET_OK);
  ASSERT_EQ(tk_cache_clock_put(&cache, second, cache_clock_test_info(2, 2)), RET_OK);
  ASSERT_EQ(cache.table.size, 2u);

  /* 满了：淘汰桶0里最早插入的那个(first) */
  ASSERT_EQ(tk_cache_clock_put(&cache, other, cache_clock_test_info(3, 3)), RET_OK);
  ASSERT_EQ(cache.table.size, 2u);
  ASSERT_EQ(s_destroy_count, 1u);
  ASSERT_EQ(cache_clock_test_get(&cache, first), -1);
  ASSERT_EQ(cache_clock_test_get(&cache, second), 2);
  ASSERT_EQ(cache_clock_test_get(&cache, other), 3);

  /* 淘汰的是桶0，hand前移一个桶 */
  ASSERT_EQ(cache.hand, 1u);
  cache_clock_test_check(&cache);

  ASSERT_EQ(tk_cache_clock_deinit(&cache), RET_OK);
}

TEST(CacheClock, evict_many) {
  size_t i = 0;
  tk_cache_clock_t cache;

  s_destroy_count = 0;
  ASSERT_EQ(tk_cache_clock_init(&cache, 100, cache_clock_test_data_destroy), RET_OK);

  /* 放1000个：size不超过容量，淘汰次数=1000-100 */
  for (i = 0; i < CACHE_CLOCK_TEST_KEY_MAX; i++) {
    const char* key = cache_clock_test_key(i);

    ASSERT_EQ(tk_cache_clock_put(&cache, key, cache_clock_test_info((int32_t)i, 0)), RET_OK);
    ASSERT_LE(cache.table.size, 100u);
  }

  ASSERT_EQ(cache.table.size, 100u);
  ASSERT_EQ(s_destroy_count, 900u);
  cache_clock_test_check(&cache);

  /* 刚写入的那个一定还在(它所在的桶被置了二次机会位) */
  ASSERT_EQ(cache_clock_test_get(&cache, cache_clock_test_key(CACHE_CLOCK_TEST_KEY_MAX - 1)),
            (int32_t)(CACHE_CLOCK_TEST_KEY_MAX - 1));

  ASSERT_EQ(tk_cache_clock_deinit(&cache), RET_OK);
  ASSERT_EQ(s_destroy_count, 1000u);
}

TEST(CacheClock, shrink) {
  size_t i = 0;
  tk_cache_clock_t cache;

  s_destroy_count = 0;
  ASSERT_EQ(tk_cache_clock_init(&cache, 10, cache_clock_test_data_destroy), RET_OK);

  for (i = 0; i < 10; i++) {
    const char* key = cache_clock_test_key(i);

    ASSERT_EQ(tk_cache_clock_put(&cache, key, cache_clock_test_info((int32_t)i, 0)), RET_OK);
  }
  ASSERT_EQ(cache.table.size, 10u);

  ASSERT_EQ(tk_cache_clock_shrink(&cache, 4), RET_OK);
  ASSERT_EQ(cache.table.size, 4u);
  ASSERT_EQ(s_destroy_count, 6u);
  cache_clock_test_check(&cache);

  /* 缩到和当前个数一样、或者比当前还大：都是no-op */
  ASSERT_EQ(tk_cache_clock_shrink(&cache, 4), RET_OK);
  ASSERT_EQ(cache.table.size, 4u);
  ASSERT_EQ(tk_cache_clock_shrink(&cache, 100), RET_OK);
  ASSERT_EQ(cache.table.size, 4u);
  ASSERT_EQ(s_destroy_count, 6u);

  ASSERT_EQ(tk_cache_clock_shrink(&cache, 0), RET_OK);
  ASSERT_EQ(cache.table.size, 0u);
  ASSERT_EQ(s_destroy_count, 10u);
  cache_clock_test_check(&cache);

  ASSERT_EQ(tk_cache_clock_deinit(&cache), RET_OK);
}

TEST(CacheClock, shrink_reclaim_buckets) {
  size_t i = 0;
  size_t slack = 0;
  size_t buckets = 0;
  tk_cache_clock_t cache;

  ASSERT_EQ(tk_cache_clock_init(&cache, 20, cache_clock_test_data_destroy), RET_OK);
  buckets = TK_RAW_HASH_TABLE_BUCKET_COUNT(&(cache.table));

  /* 反复插入/淘汰：桶内会留下闲置槽位，空桶也不会被回收 */
  for (i = 0; i < CACHE_CLOCK_TEST_KEY_MAX; i++) {
    const char* key = cache_clock_test_key(i);

    ASSERT_EQ(tk_cache_clock_put(&cache, key, cache_clock_test_info((int32_t)i, 0)), RET_OK);
  }

  /* churn之后桶里会留下闲置槽位(容量只按需增长、从不收缩)；空桶在淘汰那一刻就已经回收了 */
  for (i = 0; i < buckets; i++) {
    tk_raw_hash_table_bucket_t bucket = cache.table.buckets[i];

    ASSERT_TRUE(bucket == NULL || TK_RAW_DARRAY_SIZE(bucket) > 0);
    if (bucket != NULL && TK_RAW_DARRAY_CAPACITY(bucket) > TK_RAW_DARRAY_SIZE(bucket)) {
      slack++;
    }
  }
  ASSERT_TRUE(slack > 0);

  /* 缩到当前个数：不淘汰元素，只回收闲置内存 */
  ASSERT_EQ(tk_cache_clock_shrink(&cache, cache.table.size), RET_OK);
  ASSERT_EQ(cache.table.size, 20u);
  cache_clock_test_check(&cache);

  for (i = 0; i < buckets; i++) {
    tk_raw_hash_table_bucket_t bucket = cache.table.buckets[i];

    /* 空桶被回收(桶槽位为NULL)，非空桶缩到实际大小 */
    ASSERT_TRUE(bucket == NULL || TK_RAW_DARRAY_SIZE(bucket) > 0);
    ASSERT_EQ(TK_RAW_DARRAY_CAPACITY(bucket), TK_RAW_DARRAY_SIZE(bucket));
  }

  /* 缩到0：所有桶都回收 */
  ASSERT_EQ(tk_cache_clock_shrink(&cache, 0), RET_OK);
  for (i = 0; i < buckets; i++) {
    ASSERT_TRUE(cache.table.buckets[i] == NULL);
  }

  ASSERT_EQ(tk_cache_clock_deinit(&cache), RET_OK);
}

/* 非法/边界用法：没init、已deinit、NULL参数、NULL数据。这些只断言"不是RET_OK"。 */
TEST(CacheClock, invalid_usage) {
  cache_clock_test_info_t out;
  tk_cache_clock_t cache;

  s_destroy_count = 0;

  /* 没init就put：不能崩，返回错误 */
  memset(&cache, 0x00, sizeof(cache));
  ASSERT_NE(tk_cache_clock_put(&cache, "a", cache_clock_test_info(1, 0)), RET_OK);
  ASSERT_NE(tk_cache_clock_clear(&cache), RET_OK);
  ASSERT_NE(tk_cache_clock_get(&cache, "a", &out), RET_OK);

  /* NULL指针 */
  ASSERT_NE(tk_cache_clock_init(NULL, 10, NULL), RET_OK);
  ASSERT_NE(tk_cache_clock_init(&cache, 0, NULL), RET_OK);
  ASSERT_NE(tk_cache_clock_get(NULL, "a", &out), RET_OK);
  ASSERT_NE(tk_cache_clock_put(NULL, "a", cache_clock_test_info(1, 0)), RET_OK);
  ASSERT_NE(tk_cache_clock_remove(NULL, "a"), RET_OK);
  ASSERT_NE(tk_cache_clock_clear(NULL), RET_OK);
  ASSERT_NE(tk_cache_clock_shrink(NULL, 0), RET_OK);
  ASSERT_NE(tk_cache_clock_deinit(NULL), RET_OK);

  /* init之后是空缓存：clear/remove/get/shrink 都安全 */
  ASSERT_EQ(tk_cache_clock_init(&cache, 2, cache_clock_test_data_destroy), RET_OK);
  ASSERT_EQ(tk_cache_clock_clear(&cache), RET_OK);
  ASSERT_EQ(tk_cache_clock_remove(&cache, "a"), RET_NOT_FOUND);
  ASSERT_EQ(tk_cache_clock_get(&cache, "a", &out), RET_NOT_FOUND);
  ASSERT_EQ(tk_cache_clock_shrink(&cache, 0), RET_OK);
  ASSERT_EQ(cache.table.size, 0u);
  cache_clock_test_check(&cache);
  ASSERT_NE(tk_cache_clock_get(&cache, "a", NULL), RET_OK);
  ASSERT_EQ(tk_cache_clock_deinit(&cache), RET_OK);

  /* deinit之后再put：返回错误，不崩；deinit是幂等的 */
  ASSERT_NE(tk_cache_clock_put(&cache, "a", cache_clock_test_info(1, 0)), RET_OK);
  ASSERT_EQ(tk_cache_clock_deinit(&cache), RET_OK);
  ASSERT_EQ(s_destroy_count, 0u);
}
