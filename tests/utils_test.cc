#include <string>
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "gtest/gtest.h"

using std::string;

TEST(Utils, basic) {
  char str[32];

  ASSERT_EQ(tk_atoi("100"), 100);
  ASSERT_EQ(tk_watoi(L"100"), 100);
  ASSERT_EQ(tk_atof("100"), 100);
  ASSERT_EQ(tk_watof(L"100"), 100);

  ASSERT_EQ(strcmp(tk_itoa(str, sizeof(str), tk_atoi("100")), "100"), 0);
}

static void check_buff16(uint16_t* buff, uint16_t val, uint32_t size) {
  uint32_t i = 0;
  for (i = 0; i < size; i++) {
    ASSERT_EQ(buff[i], val);
  }
}

TEST(Utils, tk_memset16) {
  uint16_t buff[32];
  uint16_t val = 0x1234;

  memset(buff, 0x00, sizeof(buff));
  tk_memset16(buff, val, 1);
  check_buff16(buff, val, 1);

  memset(buff, 0x00, sizeof(buff));
  tk_memset16(buff, val, 7);
  check_buff16(buff, val, 7);

  memset(buff, 0x00, sizeof(buff));
  tk_memset16(buff + 1, val, 10);
  check_buff16(buff + 1, val, 10);

  memset(buff, 0x00, sizeof(buff));
  tk_memset16(buff + 1, val, 11);
  check_buff16(buff + 1, val, 11);

  memset(buff, 0x00, sizeof(buff));
  tk_memset16(buff, val, 10);
  check_buff16(buff, val, 10);

  memset(buff, 0x00, sizeof(buff));
  tk_memset16(buff, val, 11);
  check_buff16(buff, val, 11);
}

static void check_buff24(uint32_t* buff, uint32_t val, uint32_t size) {
  uint32_t i = 0;
  uint8_t* p = (uint8_t*)buff;
  uint8_t* src = (uint8_t*)&(val);

  for (i = 0; i < size; i++) {
    ASSERT_EQ(p[0], src[0]);
    ASSERT_EQ(p[1], src[1]);
    ASSERT_EQ(p[2], src[2]);
    p += 3;
  }
}

TEST(Utils, tk_memset24) {
  uint32_t buff[32];
  uint32_t val = 0x123456;

  memset(buff, 0x00, sizeof(buff));
  tk_memset24(buff, &val, 1);
  check_buff24(buff, val, 1);

  memset(buff, 0x00, sizeof(buff));
  tk_memset24(buff + 5, &val, 7);
  check_buff24(buff + 5, val, 7);

  memset(buff, 0x00, sizeof(buff));
  tk_memset24(buff + 7, &val, 10);
  check_buff24(buff + 7, val, 10);

  memset(buff, 0x00, sizeof(buff));
  tk_memset24(buff + 1, &val, 11);
  check_buff24(buff + 1, val, 11);

  memset(buff, 0x00, sizeof(buff));
  tk_memset24(buff + 3, &val, 10);
  check_buff24(buff + 3, val, 10);

  memset(buff, 0x00, sizeof(buff));
  tk_memset24(buff, &val, 11);
  check_buff24(buff, val, 11);
}

static void check_buff32(uint32_t* buff, uint32_t val, uint32_t size) {
  uint32_t i = 0;
  for (i = 0; i < size; i++) {
    ASSERT_EQ(buff[i], val);
  }
}

TEST(Utils, tk_memset32) {
  uint32_t buff[32];
  uint32_t val = 0x1234;

  memset(buff, 0x00, sizeof(buff));
  tk_memset32(buff, val, 1);
  check_buff32(buff, val, 1);

  memset(buff, 0x00, sizeof(buff));
  tk_memset32(buff, val, 7);
  check_buff32(buff, val, 7);

  memset(buff, 0x00, sizeof(buff));
  tk_memset32(buff + 1, val, 10);
  check_buff32(buff + 1, val, 10);

  memset(buff, 0x00, sizeof(buff));
  tk_memset32(buff + 1, val, 11);
  check_buff32(buff + 1, val, 11);

  memset(buff, 0x00, sizeof(buff));
  tk_memset32(buff, val, 10);
  check_buff32(buff, val, 10);

  memset(buff, 0x00, sizeof(buff));
  tk_memset32(buff, val, 11);
  check_buff32(buff, val, 11);
}

TEST(Utils, tk_memcpy16) {
  uint32_t i = 0;
  uint16_t dst[100];
  uint16_t src[100];

  tk_memset16(src, 0x1234, ARRAY_SIZE(src));
  for (i = 1; i < ARRAY_SIZE(src); i++) {
    tk_memcpy16(dst, src, i);
    ASSERT_EQ(memcmp(dst, src, i * 2), 0);
  }
}

TEST(Utils, tk_memcpy32) {
  uint32_t i = 0;
  uint32_t dst[100];
  uint32_t src[100];

  tk_memset32(src, 0x12345678, ARRAY_SIZE(src));
  for (i = 1; i < ARRAY_SIZE(src); i++) {
    tk_memcpy32(dst, src, i);
    ASSERT_EQ(memcmp(dst, src, i * 4), 0);
  }
}

TEST(Utils, tk_strncpy) {
  char dst[32];
  const char* str = "hello world";

  ASSERT_EQ(string(tk_strncpy(dst, str, 1)), string("h"));
  ASSERT_EQ(string(tk_strncpy(dst, str, strlen(str))), string(str));
  ASSERT_EQ(string(tk_strncpy(dst, str, strlen(str) + 1)), string(str));
}

TEST(Utils, filename_to_name) {
  char name[TK_NAME_LEN + 1];

  filename_to_name("test.png", name, TK_NAME_LEN);
  ASSERT_EQ(string(name), string("test"));

  filename_to_name("/a/test.png", name, TK_NAME_LEN);
  ASSERT_EQ(string(name), string("test"));
}

TEST(Utils, tk_skip_to_num) {
  ASSERT_EQ(tk_atoi(tk_skip_to_num("123")), 123);
  ASSERT_EQ(tk_atoi(tk_skip_to_num("abc123")), 123);
  ASSERT_EQ(tk_atoi(tk_skip_to_num("abc:@#123")), 123);
}

TEST(Utils, sscanf) {
  int r = 0;
  int g = 0;
  int b = 0;

  tk_sscanf("#123456", "#%02x%02x%02x", &r, &g, &b);
  ASSERT_EQ(r, 0x12);
  ASSERT_EQ(g, 0x34);
  ASSERT_EQ(b, 0x56);
}

TEST(Utils, xml_file_expand_read) {
  str_t s;
  str_init(&s, 0);

  ASSERT_EQ(xml_file_expand_read("./tests/testdata/main.xml", &s), RET_OK);
  ASSERT_EQ(string(s.str), "<window><button />\n<label />\n</window>\n");

  ASSERT_EQ(xml_file_expand_read("./tests/testdata/button.xml", &s), RET_OK);
  ASSERT_EQ(string(s.str), "<button />\n");

  str_reset(&s);
}

TEST(Utils, str_append) {
  char str[6];
  memset(str, 0x00, sizeof(str));
  ASSERT_EQ(tk_str_append(str, sizeof(str), "1"), RET_OK);
  ASSERT_EQ(string(str), string("1"));

  ASSERT_EQ(tk_str_append(str, sizeof(str), "2"), RET_OK);
  ASSERT_EQ(string(str), string("12"));

  ASSERT_EQ(tk_str_append(str, sizeof(str), "3"), RET_OK);
  ASSERT_EQ(string(str), string("123"));

  ASSERT_EQ(tk_str_append(str, sizeof(str), "4"), RET_OK);
  ASSERT_EQ(string(str), string("1234"));

  ASSERT_EQ(tk_str_append(str, sizeof(str), "5"), RET_OK);
  ASSERT_EQ(string(str), string("12345"));

  ASSERT_EQ(tk_str_append(str, sizeof(str), "6"), RET_FAIL);
  ASSERT_EQ(string(str), string("12345"));
}

TEST(Utils, tk_str_copy) {
  char* p = NULL;
  char* str = (char*)TKMEM_ALLOC(6);

  p = tk_str_copy(str, "abc");
  ASSERT_EQ(str, p);

  p = tk_str_copy(str, "abcdef");
  ASSERT_NE(str, p);
  ASSERT_EQ(string(p), "abcdef");
  str = p;

  p = tk_str_copy(str, "abcdef");
  ASSERT_EQ(str, p);
  ASSERT_EQ(string(p), "abcdef");
  str = p;

  p = tk_str_copy(str, "abcdef123");
  ASSERT_NE(str, p);
  ASSERT_EQ(string(p), "abcdef123");
  str = p;

  TKMEM_FREE(str);
}

TEST(Utils, tk_strdup) {
  char* str = tk_strdup("abc");
  ASSERT_EQ(string(str), "abc");

  TKMEM_FREE(str);
}

TEST(Utils, tk_wstrdup) {
  wchar_t* str = tk_wstrdup(L"abc");
  ASSERT_EQ(memcmp(str, L"abc", sizeof(wchar_t) * 3), 0);

  TKMEM_FREE(str);
}
