#include "base/locale.h"
#include "gtest/gtest.h"

#include <string>

using std::string;

static string s_locale;

static ret_t test_on_locale_changed(void* ctx, event_t* e) {
  tklocale_t* locale = (tklocale_t*)ctx;

  (void)e;
  s_locale = string(locale->language) + "_" + string(locale->country);

  return RET_OK;
}

TEST(Locale, basic) {
  const char* str = "ok";
  uint32_t id = 0;
  tklocale_t* locale = locale_create("en", "US");
  ASSERT_EQ(string("OK"), string(locale_tr(locale, str)));

  id = locale_on(locale, EVT_LOCALE_CHANGED, test_on_locale_changed, locale);
  ASSERT_EQ(id, 1);
  ASSERT_EQ(locale_change(locale, "zh", "CN"), RET_OK);
  ASSERT_EQ(s_locale, "zh_CN");
  ASSERT_EQ(string("确定"), string(locale_tr(locale, str)));

  ASSERT_EQ(locale_off(locale, id), RET_OK);
  ASSERT_EQ(locale_change(locale, "en", "US"), RET_OK);
  ASSERT_EQ(s_locale, "zh_CN");
  ASSERT_EQ(string("OK"), string(locale_tr(locale, str)));

  locale_destroy(locale);
}
