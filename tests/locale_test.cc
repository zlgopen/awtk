#include "base/locale_info.h"
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

TEST(Locale, basic) {
  const char* str = "ok";
  uint32_t id = 0;
  locale_info_t* locale_info = locale_info_create("en", "US");
  ASSERT_EQ(string("OK"), string(locale_info_tr(locale_info, str)));

  id = locale_info_on(locale_info, EVT_LOCALE_CHANGED, test_on_locale_changed, locale_info);
  ASSERT_EQ(id, 1);
  ASSERT_EQ(locale_info_change(locale_info, "zh", "CN"), RET_OK);
  ASSERT_EQ(s_locale, "zh_CN");

  assert_str_eq(L"确定", locale_info_tr(locale_info, str));

  ASSERT_EQ(locale_info_off(locale_info, id), RET_OK);
  ASSERT_EQ(locale_info_change(locale_info, "en", "US"), RET_OK);
  ASSERT_EQ(s_locale, "zh_CN");
  ASSERT_EQ(string("OK"), string(locale_info_tr(locale_info, str)));

  locale_info_destroy(locale_info);
}
