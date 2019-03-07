#include "base/system_info.h"
#include "gtest/gtest.h"

TEST(SystemInfo, basic) {
  system_info_t* info = system_info_create(APP_DESKTOP, "awtk", "awtk_dir");
  ASSERT_STREQ(info->app_name, "awtk");
  ASSERT_STREQ(info->app_root, "awtk_dir");

  ASSERT_EQ(info->app_type, APP_DESKTOP);
  ASSERT_STREQ(info->app_name, "awtk");
  ASSERT_STREQ(info->app_root, "awtk_dir");

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

  ASSERT_STREQ("portrait",
               object_get_prop_str(OBJECT(info), SYSTEM_INFO_PROP_LCD_ORIENTATION_NAME));

  ASSERT_EQ(system_info_set_lcd_w(info, 1000), RET_OK);
  ASSERT_STREQ("landscape",
               object_get_prop_str(OBJECT(info), SYSTEM_INFO_PROP_LCD_ORIENTATION_NAME));

  object_unref(OBJECT(info));
}
