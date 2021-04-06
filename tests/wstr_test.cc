#include "tkc/wstr.h"
#include "tkc/mem.h"
#include "gtest/gtest.h"

static void testSetWStr(const char* utf8, const wchar_t* cstr) {
  wstr_t str;
  char sutf8[256];
  ASSERT_EQ(wstr_init(&str, 100), &str);
  ASSERT_EQ(str.capacity, 100);

  ASSERT_EQ(wstr_set(&str, cstr), RET_OK);
  ASSERT_EQ(str.size, wcslen(cstr));
  ASSERT_EQ(wcs_cmp(str.str, cstr), 0);

  ASSERT_EQ(wstr_get_utf8(&str, sutf8, sizeof(sutf8)), RET_OK);
  ASSERT_EQ(strcmp(utf8, sutf8), 0);

  ASSERT_EQ(wstr_reset(&str), RET_OK);
}

TEST(WStr, demo) {
  wstr_t s;
  wstr_init(&s, 0);

  wstr_append(&s, L"abc");
  wstr_append(&s, L"123");

  wstr_reset(&s);
}

TEST(WStr, basic) {
  testSetWStr("Hello", L"Hello");
  testSetWStr("中文", L"中文");
}

static void testSetUtf8(const char* utf8, const wchar_t* cstr) {
  wstr_t str;
  char sutf8[256];
  ASSERT_EQ(wstr_init(&str, 100), &str);
  ASSERT_EQ(str.capacity, 100);

  ASSERT_EQ(wstr_set_utf8(&str, utf8), RET_OK);
  ASSERT_EQ(str.size, wcslen(cstr));
  ASSERT_EQ(wcs_cmp(str.str, cstr), 0);

  ASSERT_EQ(wstr_get_utf8(&str, sutf8, sizeof(sutf8)), RET_OK);
  ASSERT_EQ(strcmp(utf8, sutf8), 0);

  ASSERT_EQ(wstr_reset(&str), RET_OK);
}

TEST(WStr, utf8) {
  testSetUtf8("Hello", L"Hello");
  testSetUtf8("中文", L"中文");
}

static void testRemove(const wchar_t* cstr, uint16_t offset, uint16_t nr, ret_t ret,
                       const wchar_t* expected) {
  wstr_t str;
  ASSERT_EQ(wstr_init(&str, 100), &str);
  ASSERT_EQ(str.capacity, 100);

  ASSERT_EQ(wstr_set(&str, cstr), RET_OK);
  ASSERT_EQ(str.size, wcslen(cstr));
  ASSERT_EQ(wcs_cmp(str.str, cstr), 0);

  ASSERT_EQ(wstr_remove(&str, offset, nr), ret);
  if (ret == RET_OK) {
    ASSERT_EQ(wcs_cmp(str.str, expected), 0);
  }

  ASSERT_EQ(wstr_reset(&str), RET_OK);
}

TEST(WStr, remove) {
  testRemove(L"abc", 1, 2, RET_OK, L"a");
  testRemove(L"abc", 0, 3, RET_OK, L"");
  testRemove(L"abc", 1, 3, RET_OK, L"a");
  testRemove(L"abc", 1, 9, RET_OK, L"a");
  testRemove(L"abc", 1, 9, RET_OK, L"a");
  testRemove(L"abc", 2, 9, RET_OK, L"ab");

  testRemove(L"汉字测试", 1, 2, RET_OK, L"汉试");
  testRemove(L"汉字测试", 0, 3, RET_OK, L"试");
  testRemove(L"汉字测试", 1, 3, RET_OK, L"汉");
  testRemove(L"汉字测试", 1, 9, RET_OK, L"汉");
  testRemove(L"汉字测试", 1, 9, RET_OK, L"汉");
  testRemove(L"汉字测试", 2, 9, RET_OK, L"汉字");
}

static void testInsert(const wchar_t* cstr, const wchar_t* insert, uint16_t offset, ret_t ret,
                       const wchar_t* expected) {
  wstr_t str;
  ASSERT_EQ(wstr_init(&str, 100), &str);
  ASSERT_EQ(str.capacity, 100);

  ASSERT_EQ(wstr_set(&str, cstr), RET_OK);
  ASSERT_EQ(str.size, wcslen(cstr));
  ASSERT_EQ(wcs_cmp(str.str, cstr), 0);

  ASSERT_EQ(wstr_insert(&str, offset, insert, wcslen(insert)), ret);
  if (ret == RET_OK) {
    ASSERT_EQ(wcs_cmp(str.str, expected), 0);
  }

  ASSERT_EQ(wstr_reset(&str), RET_OK);
}

TEST(WStr, insert) {
  testInsert(L"汉字测试", L"中国", 0, RET_OK, L"中国汉字测试");
  testInsert(L"汉字测试", L"中国", 1, RET_OK, L"汉中国字测试");
  testInsert(L"汉字测试", L"中国", 3, RET_OK, L"汉字测中国试");
  testInsert(L"汉字测试", L"中国", 4, RET_OK, L"汉字测试中国");
}

TEST(WStr, poppush) {
  wstr_t str;
  const wchar_t* cstr = L"汉字测试";
  ASSERT_EQ(wstr_init(&str, 100), &str);
  ASSERT_EQ(str.capacity, 100);

  ASSERT_EQ(wstr_set(&str, cstr), RET_OK);
  ASSERT_EQ(str.size, wcslen(cstr));
  ASSERT_EQ(wcs_cmp(str.str, cstr), 0);

  ASSERT_EQ(wstr_push(&str, 'a'), RET_OK);
  ASSERT_EQ(str.size, wcslen(cstr) + 1);
  ASSERT_EQ(str.str[str.size - 1], (wchar_t)'a');

  ASSERT_EQ(wstr_reset(&str), RET_OK);
}

TEST(WStr, int) {
  wstr_t str;
  int32_t v1 = 123;
  int32_t v2 = 0;
  ASSERT_EQ(wstr_init(&str, 100), &str);
  ASSERT_EQ(str.capacity, 100);

  ASSERT_EQ(wstr_from_int(&str, v1), RET_OK);
  ASSERT_EQ(wstr_to_int(&str, &v2), RET_OK);
  ASSERT_EQ(v1, v2);

  ASSERT_EQ(wstr_reset(&str), RET_OK);
}

TEST(WStr, double) {
  wstr_t str;
  double v1 = 123;
  double v2 = 0;
  ASSERT_EQ(wstr_init(&str, 100), &str);
  ASSERT_EQ(str.capacity, 100);

  ASSERT_EQ(wstr_from_float(&str, v1), RET_OK);
  ASSERT_EQ(wstr_to_float(&str, &v2), RET_OK);
  ASSERT_EQ(v1, v2);

  ASSERT_EQ(wstr_set(&str, L"1.01"), RET_OK);
  ASSERT_EQ(wstr_add_float(&str, 0.01), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"1.02"), 0);

  ASSERT_EQ(wstr_add_float(&str, 0.001), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"1.021"), 0);

  ASSERT_EQ(wstr_add_float(&str, 0.1), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"1.121"), 0);

  ASSERT_EQ(wstr_add_float(&str, 1), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"2.121"), 0);

  ASSERT_EQ(wstr_set(&str, L"1.01"), RET_OK);
  ASSERT_EQ(wstr_add_float(&str, -0.01), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"1.00"), 0);

  ASSERT_EQ(wstr_add_float(&str, -0.001), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"0.999"), 0);

  ASSERT_EQ(wstr_add_float(&str, -0.1), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"0.899"), 0);

  ASSERT_EQ(wstr_add_float(&str, 1), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"1.899"), 0);

  ASSERT_EQ(wstr_add_float(&str, -1), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"0.899"), 0);

  ASSERT_EQ(wstr_add_float(&str, 123), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"123.899"), 0);

  ASSERT_EQ(wstr_set(&str, L"1.010"), RET_OK);
  ASSERT_EQ(wstr_trim_float_zero(&str), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"1.01"), 0);

  ASSERT_EQ(wstr_set(&str, L"1.00"), RET_OK);
  ASSERT_EQ(wstr_trim_float_zero(&str), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"1"), 0);

  ASSERT_EQ(wstr_set(&str, L"100"), RET_OK);
  ASSERT_EQ(wstr_trim_float_zero(&str), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"100"), 0);

  ASSERT_EQ(wstr_set(&str, L"100.00000000"), RET_OK);
  ASSERT_EQ(wstr_trim_float_zero(&str), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"100"), 0);

  ASSERT_EQ(wstr_reset(&str), RET_OK);
}

TEST(WStr, value) {
  wstr_t str;
  value_t v1;
  int32_t vi;
  double vf;
  ASSERT_EQ(wstr_init(&str, 0), &str);
  ASSERT_EQ(str.capacity, 0);

  value_set_int(&v1, 123);
  ASSERT_EQ(wstr_from_value(&str, &v1), RET_OK);
  ASSERT_EQ(wstr_to_int(&str, &vi), RET_OK);
  ASSERT_EQ(value_int(&v1), vi);

  value_set_float(&v1, 1234);
  ASSERT_EQ(wstr_from_value(&str, &v1), RET_OK);
  ASSERT_EQ(wstr_to_float(&str, &vf), RET_OK);
  ASSERT_EQ(value_float(&v1), vf);

  value_set_str(&v1, "a12345");
  ASSERT_EQ(wstr_from_value(&str, &v1), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"a12345"), 0);

  value_set_wstr(&v1, L"b12345");
  ASSERT_EQ(wstr_from_value(&str, &v1), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"b12345"), 0);

  value_set_float32(&v1, 123);
  ASSERT_EQ(wstr_from_value(&str, &v1), RET_OK);
  ASSERT_EQ(wstr_to_int(&str, &vi), RET_OK);
  ASSERT_EQ(value_int(&v1), vi);

  value_set_double(&v1, 123);
  ASSERT_EQ(wstr_from_value(&str, &v1), RET_OK);
  ASSERT_EQ(wstr_to_int(&str, &vi), RET_OK);
  ASSERT_EQ(value_int(&v1), vi);

  ASSERT_EQ(wstr_reset(&str), RET_OK);
}

TEST(WStr, wcs_len) {
  ASSERT_EQ(wcs_len(L""), 0);
  ASSERT_EQ(wcs_len(L"a"), 1);
  ASSERT_EQ(wcs_len(L"abc中文测试123"), 10);
}

TEST(WStr, wcs_cpy) {
  wchar_t str[100];

  ASSERT_EQ(wcs_len(wcs_cpy(str, L"")), 0);
  ASSERT_EQ(wcs_len(wcs_cpy(str, L"a")), 1);
  ASSERT_EQ(wcs_len(wcs_cpy(str, L"abc中文测试123")), 10);
}

TEST(WStr, wcs_chr) {
  size_t i = 0;
  const wchar_t* str = L"abc中文测试123";
  size_t nr = wcslen(str);

  ASSERT_EQ(nr, wcs_len(str));
  for (i = 0; i < nr; i++) {
    ASSERT_EQ(wcs_chr(str, str[i]), wcschr(str, str[i]));
  }
}

TEST(WStr, wcs_dup) {
  wchar_t* str = NULL;

  str = wcs_dup(L"");
  ASSERT_EQ(wcs_cmp(str, L""), 0);
  TKMEM_FREE(str);

  str = wcs_dup(L"123");
  ASSERT_EQ(wcs_cmp(str, L"123"), 0);
  TKMEM_FREE(str);

  str = wcs_dup(L"abc中文测试123");
  ASSERT_EQ(wcs_cmp(str, L"abc中文测试123"), 0);
  TKMEM_FREE(str);
}

TEST(WStr, push) {
  wstr_t str;
  int32_t v = 0;
  wstr_t* s = &str;
  ASSERT_EQ(wstr_init(&str, 0), &str);

  ASSERT_EQ(wstr_push(s, (wchar_t)'1'), RET_OK);
  ASSERT_EQ(s->size, 1);
  ASSERT_EQ(wstr_push(s, (wchar_t)'2'), RET_OK);
  ASSERT_EQ(s->size, 2);

  ASSERT_EQ(wstr_append_with_len(s, L"345", 3), RET_OK);
  ASSERT_EQ(s->size, 5);
  ASSERT_EQ(wstr_to_int(s, &v), RET_OK);
  ASSERT_EQ(v, 12345);
  wstr_reset(&str);
}

TEST(WStr, push_int) {
  wstr_t str;
  int32_t v = 0;
  wstr_t* s = &str;
  ASSERT_EQ(wstr_init(&str, 0), &str);

  ASSERT_EQ(wstr_push_int(s, "%d", 12345), RET_OK);
  ASSERT_EQ(s->size, 5);
  ASSERT_EQ(wstr_to_int(s, &v), RET_OK);
  ASSERT_EQ(v, 12345);
  s->size = 0;
  wstr_reset(&str);
}

TEST(WStr, append) {
  wstr_t str;

  wstr_init(&str, 0);
  ASSERT_EQ(wstr_append(&str, L"123"), RET_OK);
  ASSERT_EQ(str.size, 3);
  ASSERT_EQ(wstr_append(&str, L"abc"), RET_OK);
  ASSERT_EQ(str.size, 6);
  ASSERT_EQ(wcscmp(str.str, L"123abc"), 0);

  wstr_reset(&str);
}

TEST(WStr, append_len) {
  wstr_t str;

  wstr_init(&str, 0);
  ASSERT_EQ(wstr_append_with_len(&str, L"123123", 3), RET_OK);
  ASSERT_EQ(str.size, 3);
  ASSERT_EQ(wstr_append_with_len(&str, L"abcabc", 3), RET_OK);
  ASSERT_EQ(str.size, 6);
  ASSERT_EQ(wcscmp(str.str, L"123abc"), 0);

  wstr_reset(&str);
}

TEST(WStr, count) {
  wstr_t str;
  wstr_init(&str, 0);
  ASSERT_EQ(wstr_set(&str, L"123@123"), RET_OK);
  ASSERT_EQ(wstr_count_char(&str, '#'), 0);
  ASSERT_EQ(wstr_count_char(&str, '@'), 1);
  ASSERT_EQ(wstr_count_char(&str, '1'), 2);
  wstr_reset(&str);
}

TEST(WStr, set_utf8_with_len) {
  wstr_t str;

  wstr_init(&str, 0);
  ASSERT_EQ(wstr_set_utf8_with_len(&str, "123123", 0), RET_OK);
  ASSERT_EQ(str.size, 0);
  ASSERT_EQ(wcscmp(str.str, L""), 0);

  ASSERT_EQ(wstr_set_utf8_with_len(&str, "123123", 1), RET_OK);
  ASSERT_EQ(str.size, 1);
  ASSERT_EQ(wcscmp(str.str, L"1"), 0);

  ASSERT_EQ(wstr_set_utf8_with_len(&str, "123123", 3), RET_OK);
  ASSERT_EQ(str.size, 3);
  ASSERT_EQ(wcscmp(str.str, L"123"), 0);

  ASSERT_EQ(wstr_set_utf8_with_len(&str, "123123", 6), RET_OK);
  ASSERT_EQ(str.size, 6);
  ASSERT_EQ(wcscmp(str.str, L"123123"), 0);

  ASSERT_EQ(wstr_set_utf8_with_len(&str, "123123", 6), RET_OK);
  ASSERT_EQ(str.size, 6);
  ASSERT_EQ(wcscmp(str.str, L"123123"), 0);

  wstr_reset(&str);
}
