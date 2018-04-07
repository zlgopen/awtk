#include "base/wstr.h"
#include "gtest/gtest.h"

static void testSetWStr(const char* utf8, const wchar_t* cstr) {
  wstr_t str;
  char sutf8[256];
  ASSERT_EQ(wstr_init(&str, 100), &str);
  ASSERT_EQ(str.capacity, 100);

  ASSERT_EQ(wstr_set(&str, cstr), RET_OK);
  ASSERT_EQ(str.size, wcslen(cstr));
  ASSERT_EQ(wcscmp(str.str, cstr), 0);

  ASSERT_EQ(wstr_get_utf8(&str, sutf8, sizeof(sutf8)), RET_OK);
  ASSERT_EQ(strcmp(utf8, sutf8), 0);

  ASSERT_EQ(wstr_reset(&str), RET_OK);
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
  ASSERT_EQ(wcscmp(str.str, cstr), 0);

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
  ASSERT_EQ(wcscmp(str.str, cstr), 0);

  ASSERT_EQ(wstr_remove(&str, offset, nr), ret);
  if (ret == RET_OK) {
    ASSERT_EQ(wcscmp(str.str, expected), 0);
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
  ASSERT_EQ(wcscmp(str.str, cstr), 0);

  ASSERT_EQ(wstr_insert(&str, offset, insert, wcslen(insert)), ret);
  if (ret == RET_OK) {
    ASSERT_EQ(wcscmp(str.str, expected), 0);
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
  ASSERT_EQ(wcscmp(str.str, cstr), 0);

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

TEST(WStr, float) {
  wstr_t str;
  float v1 = 123;
  float v2 = 0;
  ASSERT_EQ(wstr_init(&str, 100), &str);
  ASSERT_EQ(str.capacity, 100);

  ASSERT_EQ(wstr_from_float(&str, v1), RET_OK);
  ASSERT_EQ(wstr_to_float(&str, &v2), RET_OK);
  ASSERT_EQ(v1, v2);

  ASSERT_EQ(wstr_reset(&str), RET_OK);
}

TEST(WStr, value) {
  wstr_t str;
  value_t v1;
  int32_t vi;
  float vf;
  ASSERT_EQ(wstr_init(&str, 100), &str);
  ASSERT_EQ(str.capacity, 100);

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
  ASSERT_EQ(wcscmp(str.str, L"a12345"), 0);
  
  value_set_wstr(&v1, L"b12345");
  ASSERT_EQ(wstr_from_value(&str, &v1), RET_OK);
  ASSERT_EQ(wcscmp(str.str, L"b12345"), 0);

  ASSERT_EQ(wstr_reset(&str), RET_OK);
}
