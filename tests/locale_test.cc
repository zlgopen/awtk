#include "base/tklocale.h"
#include "gtest/gtest.h"

#include <string>

using std::string;

static string s_locale;

static ret_t test_on_locale_changed(void* ctx, event_t* e) {
  tklocale_t* tklocale = (tklocale_t*)ctx;

  (void)e;
  s_locale = string(tklocale->language) + "_" + string(tklocale->country);

  return RET_OK;
}

TEST(Locale, basic) {
  const char* str = "ok";
  uint32_t id = 0;
  tklocale_t* tklocale = tklocale_create("en", "US");
  ASSERT_EQ(string("OK"), string(tklocale_tr(tklocale, str)));

  id = tklocale_on(tklocale, EVT_LOCALE_CHANGED, test_on_locale_changed, tklocale);
  ASSERT_EQ(id, 1);
  ASSERT_EQ(tklocale_change(tklocale, "zh", "CN"), RET_OK);
  ASSERT_EQ(s_locale, "zh_CN");
  ASSERT_EQ(string("确定"), string(tklocale_tr(tklocale, str)));

  ASSERT_EQ(tklocale_off(tklocale, id), RET_OK);
  ASSERT_EQ(tklocale_change(tklocale, "en", "US"), RET_OK);
  ASSERT_EQ(s_locale, "zh_CN");
  ASSERT_EQ(string("OK"), string(tklocale_tr(tklocale, str)));

  tklocale_destroy(tklocale);
}
