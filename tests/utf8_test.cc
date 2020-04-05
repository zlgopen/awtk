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
  const char* str = "中文";
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
