#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "gtest/gtest.h"

TEST(Utf8, ascii) {
  const char* str = "hello";
  const wchar_t* wstr = L"hello";
  char res_str[128];
  wchar_t res_wstr[128];

  ASSERT_EQ(wcscmp(tk_utf8_to_utf16(str, res_wstr, ARRAY_SIZE(res_wstr)), wstr), 0);
  ASSERT_EQ(strcmp(tk_utf8_from_utf16(wstr, res_str, ARRAY_SIZE(res_str)), str), 0);
}

static void dump_utf8(const char* str) {
  const char* p = str;
  log_debug("dump_utf8:%s\n", str);
  while (*p) {
    log_debug("%02x ", ((int)(*p) & 0xff));
    p++;
  }

  log_debug("\n");
}

static void dump_unicode(const wchar_t* str) {
  const wchar_t* p = str;
  log_debug("dump_unicode\n");
  while (*p) {
    log_debug("%04x ", (int)(*p));
    p++;
  }
  log_debug("\n");
}

TEST(Utf8, chinese) {
  /* 兼容非utf8编码的编译器，采用utf8编码初始化str，编码内容："中文" */
  char str[7] = {(char)0xe4, (char)0xb8, (char)0xad, (char)0xe6, (char)0x96, (char)0x87, 0};
  const wchar_t* wstr = L"中文";
  char res_str[128];
  wchar_t res_wstr[128];

  dump_utf8(str);
  dump_unicode(wstr);
  ASSERT_EQ(wcscmp(tk_utf8_to_utf16(str, res_wstr, ARRAY_SIZE(res_wstr)), wstr), 0);
  ASSERT_EQ(strcmp(tk_utf8_from_utf16(wstr, res_str, ARRAY_SIZE(res_str)), str), 0);

  dump_utf8(res_str);
  dump_unicode(res_wstr);
}

TEST(Utf8, dup) {
  /* 兼容非utf8编码的编译器，采用utf8编码初始化str，编码内容："中文" */
  char str[7] = {(char)0xe4, (char)0xb8, (char)0xad, (char)0xe6, (char)0x96, (char)0x87, 0};
  const wchar_t* wstr = L"中文";

  char* text = tk_utf8_dup_utf16(wstr, -1);
  ASSERT_STREQ(str, text);
  TKMEM_FREE(text);
}

TEST(Utf8, trim_invalid) {
  char text[32] = {0};
  /* 兼容非utf8编码的编译器，采用utf8编码初始化str，编码内容："中"、"中文" */
  char s[4] = {(char)0xe4, (char)0xb8, (char)0xad, 0};
  char str[7] = {(char)0xe4, (char)0xb8, (char)0xad, (char)0xe6, (char)0x96, (char)0x87, 0};

  memset(text, 0x00, sizeof(text));

  strncpy(text, str, 1);
  ASSERT_STREQ(tk_utf8_trim_invalid_char(text), "");

  strncpy(text, str, 2);
  ASSERT_STREQ(tk_utf8_trim_invalid_char(text), "");

  strncpy(text, str, 3);
  ASSERT_STREQ(tk_utf8_trim_invalid_char(text), s);

  strncpy(text, str, 4);
  ASSERT_STREQ(tk_utf8_trim_invalid_char(text), s);

  strncpy(text, str, 5);
  ASSERT_STREQ(tk_utf8_trim_invalid_char(text), s);

  strncpy(text, str, 6);
  ASSERT_STREQ(tk_utf8_trim_invalid_char(text), str);

  strncpy(text, str, 7);
  ASSERT_STREQ(tk_utf8_trim_invalid_char(text), str);

  strncpy(text, "abc", 4);
  ASSERT_STREQ(tk_utf8_trim_invalid_char(text), "abc");
}
