#include "base/window.h"
#include "base/canvas.h"
#include "base/widget.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>

extern "C" widget_t* window_design_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
extern "C" image_manager_t* t_image_manager;
extern "C" locale_info_t* t_locale_info;
extern "C" assets_manager_t* t_assets_manager;
extern "C" font_manager_t* t_font_manager;

TEST(WindowDesign, locale_info) {
  widget_t* w = window_design_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_get_locale_info(w), t_locale_info);

  widget_destroy(w);
}

TEST(WindowDesign, image_manager) {
  widget_t* w = window_design_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_get_image_manager(w), t_image_manager);

  widget_destroy(w);
}

TEST(WindowDesign, assets_manager) {
  widget_t* w = window_design_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_get_assets_manager(w), t_assets_manager);

  widget_destroy(w);
}

TEST(WindowDesign, font_manager) {
  widget_t* w = window_design_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_get_font_manager(w), t_font_manager);

  widget_destroy(w);
}
