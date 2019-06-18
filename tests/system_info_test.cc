#include "base/locale_info.h"
#include "base/system_info.h"
#include "gtest/gtest.h"
#include <string>
using std::string;

TEST(SystemInfo, basic) {
  system_info_t* info = system_info_create(APP_DESKTOP, "awtk", "./");

  ASSERT_EQ(info->app_type, APP_DESKTOP);
  ASSERT_STREQ(info->app_name, "awtk");
  ASSERT_EQ(strstr(info->app_root, "demos") != NULL, TRUE);

  ASSERT_EQ(system_info_set_lcd_w(info, 100), RET_OK);
  ASSERT_EQ(info->lcd_w, 100);
  ASSERT_EQ(info->lcd_w, object_get_prop_int(OBJECT(info), SYSTEM_INFO_PROP_LCD_W, 0));

  ASSERT_EQ(system_info_set_lcd_h(info, 200), RET_OK);
  ASSERT_EQ(info->lcd_h, 200);
  ASSERT_EQ(info->lcd_h, object_get_prop_int(OBJECT(info), SYSTEM_INFO_PROP_LCD_H, 0));

  ASSERT_EQ(system_info_set_lcd_type(info, LCD_FRAMEBUFFER), RET_OK);
  ASSERT_EQ(info->lcd_type, LCD_FRAMEBUFFER);
  ASSERT_EQ((int32_t)(info->lcd_type),
            object_get_prop_int(OBJECT(info), SYSTEM_INFO_PROP_LCD_TYPE, 111));

  ASSERT_EQ(system_info_set_device_pixel_ratio(info, 2), RET_OK);
  ASSERT_EQ(info->device_pixel_ratio, 2);
  ASSERT_EQ(info->device_pixel_ratio,
            object_get_prop_float(OBJECT(info), SYSTEM_INFO_PROP_DEVICE_PIXEL_RATIO, 0));

  ASSERT_EQ(system_info_set_lcd_orientation(info, LCD_ORIENTATION_180), RET_OK);
  ASSERT_EQ(info->lcd_orientation, LCD_ORIENTATION_180);
  ASSERT_EQ((int32_t)(info->lcd_orientation),
            object_get_prop_int(OBJECT(info), SYSTEM_INFO_PROP_LCD_ORIENTATION, 111));

  ASSERT_STREQ("portrait", object_get_prop_str(OBJECT(info), SYSTEM_INFO_PROP_DEVICE_ORIENTATION));

  ASSERT_EQ(system_info_set_lcd_w(info, 1000), RET_OK);
  ASSERT_STREQ("landscape", object_get_prop_str(OBJECT(info), SYSTEM_INFO_PROP_DEVICE_ORIENTATION));

  locale_info_change(locale_info(), "zh", "CN");
  ASSERT_STREQ("zh", object_get_prop_str(OBJECT(info), SYSTEM_INFO_PROP_LANGUAGE));
  ASSERT_STREQ("CN", object_get_prop_str(OBJECT(info), SYSTEM_INFO_PROP_COUNTRY));

  object_unref(OBJECT(info));
}

static ret_t on_expr(void* ctx, const void* data) {
  string& str = *(string*)ctx;

  if (str != "") {
    str += ";";
  }
  str += (char*)data;

  return RET_FAIL;
}

TEST(SystemInfo, expr) {
  string str;
  system_info_t* info = system_info_create(APP_DESKTOP, "awtk", "awtk_dir");

  locale_info_change(locale_info(), "zh", "CN");
  ASSERT_EQ(system_info_set_lcd_w(info, 100), RET_OK);
  ASSERT_EQ(system_info_set_lcd_h(info, 200), RET_OK);

  str = "";
  system_info_eval_exprs(info, "", on_expr, &str);
  ASSERT_EQ(str, string(""));

  str = "";
  system_info_eval_exprs(info, "abc", on_expr, &str);
  ASSERT_EQ(str, string("abc"));

  str = "";
  system_info_eval_exprs(info, "abc,123", on_expr, &str);
  ASSERT_EQ(str, string("abc;123"));

  str = "";
  system_info_eval_exprs(info, "abc_${lcd_w},123_${lcd_h}", on_expr, &str);
  ASSERT_EQ(str, string("abc_100;123_200"));

  str = "";
  system_info_eval_exprs(info, "abc_${lcd_w},123_${lcd_h},bg_${app_name}", on_expr, &str);
  ASSERT_EQ(str, string("abc_100;123_200;bg_awtk"));

  str = "";
  system_info_eval_exprs(info, "bg_${language}_${country},bg_${language},bg_${country}", on_expr,
                         &str);
  ASSERT_EQ(str, string("bg_zh_CN;bg_zh;bg_CN"));

  object_unref(OBJECT(info));
}
