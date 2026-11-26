#include <string.h>
#include <stdint.h>

#include "tkc/utils.h"
#include "tkc/named_value_hash.h"

static size_t s_key_destroy_count = 0;
static size_t s_value_destroy_count = 0;

/* 销毁键时用来检查"元素已经从桶里移除了"，实现在include之后 */
static void raw_hash_table_custom_note_destroy(void);

#define TK_RAW_HASH_TABLE_ENTRY_KEY_T char*
#define TK_RAW_HASH_TABLE_ENTRY_VALUE_T int32_t*
#define TK_RAW_HASH_TABLE_KEY_HASH(key) named_value_hash_get_hash_from_str(key)
#define TK_RAW_HASH_TABLE_KEY_COMPARE(entry_key, key) tk_strcmp(entry_key, key)

/* 键是tk_strdup加进来的，元素被删除时在这里释放 */
#define TK_RAW_HASH_TABLE_KEY_DESTROY(key) \
  do {                                     \
    s_key_destroy_count++;                 \
    raw_hash_table_custom_note_destroy();  \
    TKMEM_FREE(key);                       \
  } while (0)

#include "tkc/raw_hash_table.h"
#include "gtest/gtest.h"

static size_t s_size_in_destroy = 0;
static tk_raw_hash_table_t* s_destroy_table = NULL;

/* 元素被删除或者值被替换时释放值 */
static ret_t raw_hash_table_custom_value_destroy(int32_t* value) {
  s_value_destroy_count++;
  TKMEM_FREE(value);

  return RET_OK;
}

static void raw_hash_table_custom_note_destroy(void) {
  if (s_destroy_table != NULL) {
    s_size_in_destroy = s_destroy_table->size;
  }
}

static int32_t* raw_hash_table_custom_value(int32_t v) {
  int32_t* p = (int32_t*)TKMEM_ALLOC(sizeof(int32_t));
  *p = v;

  return p;
}

static ret_t raw_hash_table_custom_visit_remove_even(void* ctx, const void* data) {
  const tk_raw_hash_table_entry_t* entry = (const tk_raw_hash_table_entry_t*)data;

  return ((*(entry->value) % 2) == 0) ? RET_REMOVE : RET_OK;
}

static ret_t raw_hash_table_custom_visit_remove_all(void* ctx, const void* data) {
  return RET_REMOVE;
}

TEST(RawHashTable, str_key) {
  char* hello = tk_strdup("hello");
  tk_raw_hash_table_t table;
  tk_raw_hash_table_entry_t* entry = NULL;

  ASSERT_TRUE(hello != NULL);
  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, raw_hash_table_custom_value_destroy), RET_OK);
  ASSERT_TRUE(table.buckets != NULL);
  ASSERT_EQ(TK_RAW_HASH_TABLE_BUCKET_COUNT(&table), 15u);
  s_key_destroy_count = 0;
  s_value_destroy_count = 0;

  /* 键是堆上的副本，地址和字面量不同，但内容相同 */
  ASSERT_NE((const void*)hello, (const void*)"hello");
  ASSERT_EQ(tk_raw_hash_table_add(&table, hello, raw_hash_table_custom_value(1)), RET_OK);
  ASSERT_EQ(tk_raw_hash_table_add(&table, tk_strdup("world"), raw_hash_table_custom_value(2)),
            RET_OK);
  ASSERT_EQ(table.size, 2u);

  /* 键已存在时add失败：用字面量查找也能判定重复，失败时不接管值和键的所有权 */
  {
    int32_t* v = raw_hash_table_custom_value(3);

    ASSERT_EQ(tk_raw_hash_table_add(&table, "hello", v), RET_FAIL);
    ASSERT_EQ(table.size, 2u);
    TKMEM_FREE(v);
  }

  /* 键内容相同就能找到，和字符串的地址无关 */
  entry = tk_raw_hash_table_find(&table, "hello");
  ASSERT_TRUE(entry != NULL);
  ASSERT_STREQ(entry->key, "hello");
  ASSERT_EQ(*(entry->value), 1);

  /* modify只替换值：走value_destroy，键保留(KEY_DESTROY不会被调用) */
  ASSERT_EQ(tk_raw_hash_table_modify(&table, "hello", raw_hash_table_custom_value(9)), RET_OK);
  ASSERT_EQ(s_value_destroy_count, 1u);
  ASSERT_EQ(s_key_destroy_count, 0u);
  ASSERT_EQ(*(tk_raw_hash_table_find(&table, "hello")->value), 9);

  /* 用同一个指针modify：值没有变，不应该销毁它 */
  {
    int32_t* v = tk_raw_hash_table_find(&table, "hello")->value;

    ASSERT_EQ(tk_raw_hash_table_modify(&table, "hello", v), RET_OK);
    ASSERT_EQ(s_value_destroy_count, 1u);
    ASSERT_EQ(s_key_destroy_count, 0u);
    ASSERT_EQ(tk_raw_hash_table_find(&table, "hello")->value, v);
  }

  /* replace（已存在）也只替换值 */
  ASSERT_EQ(tk_raw_hash_table_replace(&table, "world", raw_hash_table_custom_value(8)), RET_OK);
  ASSERT_EQ(s_value_destroy_count, 2u);
  ASSERT_EQ(s_key_destroy_count, 0u);
  ASSERT_EQ(table.size, 2u);
  ASSERT_EQ(*(tk_raw_hash_table_find(&table, "world")->value), 8);

  /* replace（不存在）加入新元素，不销毁任何东西 */
  ASSERT_EQ(tk_raw_hash_table_replace(&table, tk_strdup("awtk"), raw_hash_table_custom_value(7)),
            RET_OK);
  ASSERT_EQ(s_value_destroy_count, 2u);
  ASSERT_EQ(s_key_destroy_count, 0u);
  ASSERT_EQ(table.size, 3u);

  /* remove：键和值各销毁一次 */
  ASSERT_EQ(tk_raw_hash_table_remove(&table, "awtk"), RET_OK);
  ASSERT_EQ(s_key_destroy_count, 1u);
  ASSERT_EQ(s_value_destroy_count, 3u);
  ASSERT_EQ(table.size, 2u);

  /* clear：剩下的2个元素，键和值各销毁2次 */
  ASSERT_EQ(tk_raw_hash_table_clear(&table), RET_OK);
  ASSERT_EQ(s_key_destroy_count, 3u);
  ASSERT_EQ(s_value_destroy_count, 5u);
  ASSERT_EQ(table.size, 0u);
  ASSERT_EQ(TK_RAW_HASH_TABLE_BUCKET_COUNT(&table), 15u);

  /* deinit：剩下的1个元素 */
  ASSERT_EQ(tk_raw_hash_table_add(&table, tk_strdup("end"), raw_hash_table_custom_value(6)),
            RET_OK);
  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
  ASSERT_TRUE(table.buckets == NULL);
  ASSERT_EQ(s_key_destroy_count, 4u);
  ASSERT_EQ(s_value_destroy_count, 6u);
}

TEST(RawHashTable, str_hash) {
  size_t i = 0;
  size_t j = 0;
  size_t used = 0;
  char buf[16];
  char other[16];
  char long_str[1001];
  tk_raw_hash_table_t table;

  /* 内容相同就是同一个hash，和字符串的地址无关 */
  tk_strcpy(buf, "hello");
  tk_strcpy(other, "hello");
  ASSERT_NE((const void*)buf, (const void*)other);
  ASSERT_EQ(named_value_hash_get_hash_from_str(buf), named_value_hash_get_hash_from_str(other));
  ASSERT_EQ(named_value_hash_get_hash_from_str(buf), named_value_hash_get_hash_from_str("hello"));

  /* 空串的hash是0 */
  ASSERT_EQ(named_value_hash_get_hash_from_str(""), 0u);

  /* 内容不同，hash不同；大小写敏感 */
  ASSERT_NE(named_value_hash_get_hash_from_str("abc"), named_value_hash_get_hash_from_str("abd"));
  ASSERT_NE(named_value_hash_get_hash_from_str("hello"),
            named_value_hash_get_hash_from_str("world"));
  ASSERT_NE(named_value_hash_get_hash_from_str("awtk"), named_value_hash_get_hash_from_str("AWTK"));

  /* 每个字符都参与hash：任意一位改动，hash都要变化 */
  for (i = 0; i < 6; i++) {
    char mut[16];

    tk_strcpy(mut, "abcdef");
    mut[i] = (mut[i] == 'z') ? 'y' : 'z';
    ASSERT_NE(named_value_hash_get_hash_from_str(mut),
              named_value_hash_get_hash_from_str("abcdef"));
  }

  /* 长字符串不会越界，结果稳定 */
  memset(long_str, 'x', sizeof(long_str) - 1);
  long_str[sizeof(long_str) - 1] = '\0';
  ASSERT_EQ(named_value_hash_get_hash_from_str(long_str),
            named_value_hash_get_hash_from_str(long_str));
  ASSERT_NE(named_value_hash_get_hash_from_str(long_str),
            named_value_hash_get_hash_from_str("xxx"));

  /* 用表自己的桶下标看分布：100个键要铺开，不能挤在少数几个桶里 */
  ASSERT_EQ(tk_raw_hash_table_init(&table, 100, raw_hash_table_custom_value_destroy), RET_OK);
  for (i = 0; i < 100; i++) {
    char key[16];

    tk_snprintf(key, sizeof(key), "key%d", (int)i);
    ASSERT_EQ(
        tk_raw_hash_table_add(&table, tk_strdup(key), raw_hash_table_custom_value((int32_t)i)),
        RET_OK);
  }

  for (j = 0; j < TK_RAW_HASH_TABLE_BUCKET_COUNT(&table); j++) {
    size_t n = TK_RAW_DARRAY_SIZE(table.buckets[j]);

    if (n > 0) {
      used++;
    }
    /* 150个桶装100个键，平均每个桶不到1个，不能有明显堆积 */
    ASSERT_LE(n, 10u);
  }
  ASSERT_GT(used, 50u);

  /* clear会销毁这100个键和值，后面的用例会重置计数 */
  ASSERT_EQ(tk_raw_hash_table_clear(&table), RET_OK);
  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}

TEST(RawHashTable, str_foreach_remove) {
  size_t i = 0;
  static const char* keys[] = {"k0", "k1", "k2", "k3", "k4", "k5"};
  tk_raw_hash_table_t table;

  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, raw_hash_table_custom_value_destroy), RET_OK);
  s_key_destroy_count = 0;
  s_value_destroy_count = 0;

  for (i = 0; i < ARRAY_SIZE(keys); i++) {
    ASSERT_EQ(
        tk_raw_hash_table_add(&table, tk_strdup(keys[i]), raw_hash_table_custom_value((int32_t)i)),
        RET_OK);
  }
  ASSERT_EQ(table.size, ARRAY_SIZE(keys));

  /* 删除值为偶数的元素：0/2/4，键和值各销毁3次 */
  ASSERT_EQ(tk_raw_hash_table_foreach(&table, raw_hash_table_custom_visit_remove_even, NULL),
            RET_OK);
  ASSERT_EQ(s_key_destroy_count, 3u);
  ASSERT_EQ(s_value_destroy_count, 3u);
  ASSERT_EQ(table.size, 3u);
  ASSERT_TRUE(tk_raw_hash_table_find(&table, "k1") != NULL);
  ASSERT_TRUE(tk_raw_hash_table_find(&table, "k0") == NULL);

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
  ASSERT_EQ(s_key_destroy_count, 6u);
  ASSERT_EQ(s_value_destroy_count, 6u);
}

/* rehash只搬家：不会调用自定义的键/值销毁函数，搬完还能按内容查到。 */
TEST(RawHashTable, reserve_rehash) {
  tk_raw_hash_table_t table;
  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, raw_hash_table_custom_value_destroy), RET_OK);
  s_key_destroy_count = 0;
  s_value_destroy_count = 0;

  ASSERT_EQ(tk_raw_hash_table_add(&table, tk_strdup("a"), raw_hash_table_custom_value(1)), RET_OK);
  ASSERT_EQ(tk_raw_hash_table_add(&table, tk_strdup("b"), raw_hash_table_custom_value(2)), RET_OK);

  /* 扩容rehash：一次销毁都不该有 */
  ASSERT_EQ(tk_raw_hash_table_reserve(&table, 100), RET_OK);
  ASSERT_EQ(s_key_destroy_count, 0u);
  ASSERT_EQ(s_value_destroy_count, 0u);
  ASSERT_EQ(*(tk_raw_hash_table_find(&table, "a")->value), 1);
  ASSERT_EQ(*(tk_raw_hash_table_find(&table, "b")->value), 2);

  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
  ASSERT_EQ(s_key_destroy_count, 2u);
  ASSERT_EQ(s_value_destroy_count, 2u);
}

TEST(RawHashTable, remove_order) {
  tk_raw_hash_table_t table;

  ASSERT_EQ(tk_raw_hash_table_init(&table, 10, raw_hash_table_custom_value_destroy), RET_OK);
  ASSERT_EQ(tk_raw_hash_table_add(&table, tk_strdup("a"), raw_hash_table_custom_value(1)), RET_OK);
  ASSERT_EQ(tk_raw_hash_table_add(&table, tk_strdup("b"), raw_hash_table_custom_value(2)), RET_OK);

  /* 销毁键时，元素应该已经从桶里移除了(所以表里只剩1个) */
  s_size_in_destroy = 100;
  s_destroy_table = &table;
  ASSERT_EQ(tk_raw_hash_table_remove(&table, "a"), RET_OK);
  ASSERT_EQ(s_size_in_destroy, 1u);
  ASSERT_EQ(table.size, 1u);

  /* foreach返回RET_REMOVE时也一样 */
  s_size_in_destroy = 100;
  ASSERT_EQ(tk_raw_hash_table_foreach(&table, raw_hash_table_custom_visit_remove_all, NULL),
            RET_OK);
  ASSERT_EQ(s_size_in_destroy, 0u);
  ASSERT_EQ(table.size, 0u);

  s_destroy_table = NULL;
  ASSERT_EQ(tk_raw_hash_table_deinit(&table), RET_OK);
}
