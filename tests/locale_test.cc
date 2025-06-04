#include "base/locale_info.h"
#include "base/locale_info_xml.h"
#include "tkc/fs.h"
#include "gtest/gtest.h"

#include "common.h"
#include <string>

using std::string;

static string s_locale;

static ret_t test_on_locale_changed(void* ctx, event_t* e) {
  locale_info_t* locale_info = (locale_info_t*)ctx;

  (void)e;
  s_locale = string(locale_info->language) + "_" + string(locale_info->country);

  return RET_OK;
}

static const char* my_tr(const char* str) {
  if (tk_str_eq(str, "AAAA")) {
    return "4stars";
  } else {
    return NULL;
  }
}

TEST(Locale, basic) {
  const char* str = "ok";
  uint32_t id = 0;
  locale_info_t* locale_info = locale_info_create("en", "US");
  ASSERT_EQ(string("OK"), string(locale_info_tr(locale_info, str)));

  id = locale_info_on(locale_info, EVT_LOCALE_CHANGED, test_on_locale_changed, locale_info);
  ASSERT_EQ(id, 1u);
  ASSERT_EQ(locale_info_change(locale_info, "zh", "CN"), RET_OK);
  ASSERT_EQ(s_locale, "zh_CN");

  assert_str_eq(L"确定", locale_info_tr(locale_info, str));

  ASSERT_EQ(locale_info_off(locale_info, id), RET_OK);
  ASSERT_EQ(locale_info_change(locale_info, "en", "US"), RET_OK);
  ASSERT_EQ(s_locale, "zh_CN");
  ASSERT_EQ(string("OK"), string(locale_info_tr(locale_info, str)));

  locale_info_set_fallback_tr(locale_info, my_tr);
  ASSERT_EQ(string("4stars"), string(locale_info_tr(locale_info, "AAAA")));

  locale_info_destroy(locale_info);
}

static ret_t prepare_test_file(const char* file_name) {
  const char* content =
      "<string name=\"test\">\n<language name=\"en_US\">TEST</language>\n"
      "<language name=\"zh_CN\">测试</language>\n</string>\n"
      "<string name=\"cn\">\n<language name=\"en_US\">CN</language>\n"
      "<language name=\"zh_CN\">中文</language>\n</string>\n"
      "<string name=\"en\">\n<language name=\"en_US\">EN</language>\n"
      "<language name=\"zh_CN\">英文</language>\n</string>\n";

  return file_write(file_name, content, strlen(content));
}

TEST(Locale, xml_basic) {
  const char* file_name = "locale_info_xml_test.xml";
  prepare_test_file(file_name);

  const char* str = "test";
  locale_info_t* locale_info = locale_info_xml_create("en", "US");
  locale_info_xml_set_url(locale_info, file_name);

  ASSERT_EQ(string("TEST"), string(locale_info_tr(locale_info, str)));
  locale_info_change(locale_info, "zh", "CN");
  assert_str_eq(L"测试", locale_info_tr(locale_info, str));

  str = "cn";
  assert_str_eq(L"中文", locale_info_tr(locale_info, str));
  locale_info_change(locale_info, "en", "US");
  ASSERT_EQ(string("CN"), string(locale_info_tr(locale_info, str)));

  locale_info_xml_destroy(locale_info);
  file_remove(file_name);
}
