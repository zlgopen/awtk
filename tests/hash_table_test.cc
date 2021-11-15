#include "tkc/utils.h"
#include "tkc/hash_table.h"
#include "gtest/gtest.h"

static int int_compare(const void* a, const void* b) {
  return tk_pointer_to_int(a) - tk_pointer_to_int(b);
}

TEST(DHashTable, int_add_remove) {
  uint32_t i = 0;
  uint32_t n = 10000;
  hash_table_t* ht = hash_table_create(10, NULL, int_compare, hash_table_hash_int);

  for (i = 0; i < n; i++) {
    ASSERT_EQ(hash_table_add(ht, tk_pointer_from_int(i), TRUE), RET_OK);
    ASSERT_EQ(hash_table_count(ht, compare_always_equal, NULL), (i + 1));
    ASSERT_EQ(tk_pointer_to_int(hash_table_find(ht, int_compare, tk_pointer_from_int(i))), i);
  }

  for (i = 0; i < n; i++) {
    ASSERT_EQ(hash_table_remove(ht, int_compare, tk_pointer_from_int(i)), RET_OK);
    ASSERT_EQ(hash_table_count(ht, compare_always_equal, NULL), n - i - 1);
    ASSERT_EQ(tk_pointer_to_int(hash_table_find(ht, int_compare, tk_pointer_from_int(i))), 0);
  }

  hash_table_destroy(ht);
}

TEST(DHashTable, int_add_clear) {
  uint32_t i = 0;
  uint32_t n = 10000;
  hash_table_t* ht = hash_table_create(10, NULL, int_compare, hash_table_hash_int);

  for (i = 0; i < n; i++) {
    ASSERT_EQ(hash_table_add(ht, tk_pointer_from_int(i), TRUE), RET_OK);
    ASSERT_EQ(hash_table_count(ht, compare_always_equal, NULL), (i + 1));
    ASSERT_EQ(tk_pointer_to_int(hash_table_find(ht, int_compare, tk_pointer_from_int(i))), i);
  }

  ASSERT_EQ(hash_table_clear(ht), RET_OK);
  ASSERT_EQ(hash_table_count(ht, compare_always_equal, NULL), 0);

  hash_table_destroy(ht);
}

TEST(DHashTable, int_remove_all) {
  uint32_t i = 0;
  uint32_t n = 10000;
  hash_table_t* ht = hash_table_create(10, NULL, int_compare, hash_table_hash_int);

  for (i = 0; i < n; i++) {
    ASSERT_EQ(hash_table_add(ht, tk_pointer_from_int(i), TRUE), RET_OK);
    ASSERT_EQ(hash_table_count(ht, compare_always_equal, NULL), (i + 1));
    ASSERT_EQ(tk_pointer_to_int(hash_table_find(ht, int_compare, tk_pointer_from_int(i))), i);
  }

  ASSERT_EQ(hash_table_remove_all(ht, compare_always_equal, NULL), RET_OK);
  ASSERT_EQ(hash_table_count(ht, compare_always_equal, NULL), 0);

  hash_table_destroy(ht);
}

static ret_t visit_int(void* ctx, const void* data) {
  int32_t v = tk_pointer_to_int(data);
  str_t* str = (str_t*)ctx;
  str_append_char(str, ';');
  str_append_int(str, v);

  return RET_OK;
}

TEST(DHashTable, int_foreach) {
  uint32_t i = 0;
  uint32_t n = 20;
  str_t str;
  hash_table_t* ht = hash_table_create(10, NULL, int_compare, hash_table_hash_int);

  for (i = 0; i < n; i++) {
    ASSERT_EQ(hash_table_add(ht, tk_pointer_from_int(i), TRUE), RET_OK);
    ASSERT_EQ(hash_table_count(ht, compare_always_equal, NULL), (i + 1));
    ASSERT_EQ(tk_pointer_to_int(hash_table_find(ht, int_compare, tk_pointer_from_int(i))), i);
  }

  str_init(&str, 0);
  ASSERT_EQ(hash_table_foreach(ht, visit_int, &str), RET_OK);
  ASSERT_STREQ(str.str, ";0;10;1;11;2;12;3;13;4;14;5;15;6;16;7;17;8;18;9;19");
  str_reset(&str);

  hash_table_destroy(ht);
}

TEST(DHashTable, str_add_remove) {
  char str[32];
  uint32_t i = 0;
  uint32_t n = 10000;
  hash_table_t* ht =
      hash_table_create(10, default_destroy, (tk_compare_t)strcmp, hash_table_hash_str);

  for (i = 0; i < n; i++) {
    tk_snprintf(str, sizeof(str), "%u", i);
    ASSERT_EQ(hash_table_add(ht, tk_strdup(str), TRUE), RET_OK);
    ASSERT_EQ(hash_table_count(ht, compare_always_equal, NULL), (i + 1));
    ASSERT_STREQ((char*)hash_table_find(ht, (tk_compare_t)strcmp, str), str);
  }

  hash_table_destroy(ht);
}
