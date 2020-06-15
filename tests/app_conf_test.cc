#include "gtest/gtest.h"
#include "conf_io/app_conf.h"
#include "conf_io/app_conf_init_ini.h"
#include "conf_io/app_conf_init_json.h"
#include "conf_io/app_conf_init_ubjson.h"
#include "tkc/object_default.h"

TEST(AppConf, basic) {
  object_t* obj = object_default_create();

  ASSERT_EQ(app_conf_set_instance(obj), RET_OK);

  ASSERT_EQ(app_conf_set_int("int", 123), RET_OK);
  ASSERT_EQ(app_conf_get_int("int", 0), 123);

  ASSERT_EQ(app_conf_set_int64("int64", 1234567890), RET_OK);
  ASSERT_EQ(app_conf_get_int64("int64", 0), 1234567890);

  ASSERT_EQ(app_conf_set_double("double", 123), RET_OK);
  ASSERT_EQ(app_conf_get_double("double", 0), 123);

  ASSERT_EQ(app_conf_set_str("str", "123"), RET_OK);
  ASSERT_STREQ(app_conf_get_str("str", NULL), "123");

  ASSERT_EQ(app_conf_deinit(), RET_OK);
}

static ret_t on_prop_changed(void* ctx, event_t* e) {
  int32_t* count = (int32_t*)ctx;
  prop_change_event_t* evt = prop_change_event_cast(e);

  *count = *count + 1;

  log_debug("prop %s changed\n", evt->name);

  return RET_OK;
}

TEST(AppConf, event) {
  int32_t count = 0;
  object_t* obj = object_default_create();

  ASSERT_EQ(app_conf_set_instance(obj), RET_OK);
  ASSERT_NE(app_conf_on_changed(on_prop_changed, &count), TK_INVALID_ID);

  ASSERT_EQ(app_conf_set_int("int", 123), RET_OK);
  ASSERT_EQ(app_conf_set_int64("int64", 1234567890), RET_OK);
  ASSERT_EQ(count, 2);

  ASSERT_EQ(app_conf_off_changed_by_ctx(&count), RET_OK);
  ASSERT_EQ(app_conf_set_double("double", 123), RET_OK);
  ASSERT_EQ(app_conf_set_str("str", "123"), RET_OK);
  ASSERT_EQ(count, 2);

  ASSERT_EQ(app_conf_deinit(), RET_OK);
}

TEST(AppConf, init) {
  app_conf_init_ini("conf_test");

  ASSERT_EQ(app_conf_set_int("int", 123), RET_OK);
  ASSERT_EQ(app_conf_set_int64("int64", 1234567890), RET_OK);
  ASSERT_EQ(app_conf_save(), RET_OK);

  ASSERT_EQ(app_conf_deinit(), RET_OK);
}

TEST(AppConf, wstr) {
  wchar_t str[32];
  app_conf_init_ini("conf_test");

  ASSERT_EQ(app_conf_set_wstr("wstr", L"abc"), RET_OK);
  ASSERT_EQ(app_conf_get_wstr("wstr", str, ARRAY_SIZE(str)), RET_OK);
  ASSERT_STREQ(app_conf_get_str("wstr", NULL), "abc");

  ASSERT_EQ(app_conf_deinit(), RET_OK);
}

TEST(AppConf, reload) {
  app_conf_init_ini("conf_test");

  ASSERT_EQ(app_conf_set_int("int", 123), RET_OK);
  ASSERT_EQ(app_conf_save(), RET_OK);

  ASSERT_EQ(app_conf_set_int("int", 456), RET_OK);
  ASSERT_EQ(app_conf_get_int("int", 0), 456);

  ASSERT_EQ(app_conf_reload(), RET_OK);
  ASSERT_EQ(app_conf_get_int("int", 0), 123);

  ASSERT_EQ(app_conf_deinit(), RET_OK);
}
