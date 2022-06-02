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

TEST(Utf8, chinese2) {
  /* 4 utf8 */
  char buf[100] = {(char)0xf0, (char)0x90, (char)0xa4, (char)0x92, (char)0xf0,
                   (char)0x90, (char)0x87, (char)0xaf, 0};
  const char* str = buf;
  const wchar_t* wstr2 = L"𐤒𐇯";

  char res_str[128];
  wchar_t res_wstr[128];
  tk_utf8_to_utf16(str, res_wstr, ARRAY_SIZE(res_wstr));
  tk_utf8_from_utf16(res_wstr, res_str, ARRAY_SIZE(res_str));
  ASSERT_EQ(strcmp(res_str, str), 0);
  ASSERT_EQ(wcscmp(res_wstr, wstr2), 0);

#ifndef WIN32
  /* 
   字符值 0x200001 ~ 0x4000000  可以换 5个 utf8
   字符值 0x4000001 以上  可以换 6个 utf8
   上述字符值， 都无法用 utf16 表示，  utf16只能编码 小于 0x10FFFF 的字符值
   windows的 wchar_t 是 utf16, 因此 windows 下不就测了
   */
  if (sizeof(wchar_t) == 4) {
    /* 5 utf8 */
    char buf2[100] = {(char)0xf8, (char)0x88, (char)0x80, (char)0x80, (char)0x81, 0};
    str = buf2;
    tk_utf8_to_utf16(str, res_wstr, ARRAY_SIZE(res_wstr));
    tk_utf8_from_utf16(res_wstr, res_str, ARRAY_SIZE(res_str));
    ASSERT_EQ(strcmp(res_str, str), 0);

    /* 6 utf8 */
    char buf3[100] = {(char)0xfc, (char)0x84, (char)0x80, (char)0x80, (char)0x80, (char)0x81, 0};
    str = buf3;
    tk_utf8_to_utf16(str, res_wstr, ARRAY_SIZE(res_wstr));
    tk_utf8_from_utf16(res_wstr, res_str, ARRAY_SIZE(res_str));
    ASSERT_EQ(strcmp(res_str, str), 0);

    /* mixed */
    char buf4[100] = {'a',        'B',        (char)0xf0, (char)0x90, (char)0xa4, (char)0x92,
                      (char)0xf0, (char)0x90, (char)0x87, (char)0xaf, (char)0xfc, (char)0x84,
                      (char)0x80, (char)0x80, (char)0x80, (char)0x81, (char)0xf8, (char)0x88,
                      (char)0x80, (char)0x80, (char)0x81, 0};
    str = buf4;
    tk_utf8_to_utf16(str, res_wstr, ARRAY_SIZE(res_wstr));
    tk_utf8_from_utf16(res_wstr, res_str, ARRAY_SIZE(res_str));
    ASSERT_EQ(strcmp(res_str, str), 0);
  }
#endif
}

TEST(Utf8, out_len_invalid) {
  char str[7] = {0};
  char* result = tk_utf8_from_utf16(L"中文", str, 6);
  ASSERT_STREQ(result, NULL);
}

TEST(Utf8, dup) {
  /* 兼容非utf8编码的编译器，采用utf8编码初始化str，编码内容："中文" */
  char str[7] = {(char)0xe4, (char)0xb8, (char)0xad, (char)0xe6, (char)0x96, (char)0x87, 0};
  const wchar_t* wstr = L"中文";

  char* text = tk_utf8_dup_utf16(wstr, -1);
  ASSERT_STREQ(str, text);
  TKMEM_FREE(text);

  char str2[9] = {(char)0xf0, (char)0x90, (char)0xa4, (char)0x92, (char)0xf0,
                  (char)0x90, (char)0x87, (char)0xaf, 0};
  const wchar_t* wstr2 = L"𐤒𐇯";

  text = tk_utf8_dup_utf16(wstr2, -1);
  ASSERT_STREQ(str2, text);
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
