#include "base/canvas.h"
#include "base/widget.h"
#include "base/layout.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "common.h"
#include "gtest/gtest.h"
#include "image_value/image_value.h"

#include <stdlib.h>
#include <string>
using std::string;

TEST(ImageValue, basic) {
  value_t v1;
  value_t v2;
  widget_t* w = image_value_create(NULL, 10, 20, 30, 40);

  value_set_str(&v1, "image");
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_IMAGE, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_IMAGE, &v2), RET_OK);
  ASSERT_EQ(string(v1.value.str), string(v2.value.str));

  value_set_str(&v1, "%2f");
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_FORMAT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_FORMAT, &v2), RET_OK);
  ASSERT_EQ(string(v1.value.str), string(v2.value.str));

  widget_destroy(w);
}

TEST(ImageValue, cast) {
  widget_t* w = image_value_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, image_value_cast(w));

  widget_destroy(w);
}

TEST(ImageValue, min_max) {
  widget_t* w = image_value_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_MIN, 1), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_MIN, 0), 1);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_MAX, 5), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_MAX, 0), 5);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_VALUE, 3), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_VALUE, 0), 3);

  widget_destroy(w);
}

TEST(ImageValue, add_delta_p) {
  widget_t* w = image_value_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_MIN, 1), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_MIN, 0), 1);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_MAX, 5), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_MAX, 0), 5);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_VALUE, 3), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_VALUE, 0), 3);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_CLICK_ADD_DELTA, 1), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_CLICK_ADD_DELTA, 0), 1);

  ASSERT_EQ(image_value_add_delta(w), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_VALUE, 0), 4);

  ASSERT_EQ(image_value_add_delta(w), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_VALUE, 0), 5);

  ASSERT_EQ(image_value_add_delta(w), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_VALUE, 0), 1);

  widget_destroy(w);
}

TEST(ImageValue, add_delta_n) {
  widget_t* w = image_value_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_MIN, 1), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_MIN, 0), 1);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_MAX, 5), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_MAX, 0), 5);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_VALUE, 3), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_VALUE, 0), 3);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_CLICK_ADD_DELTA, -1), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_CLICK_ADD_DELTA, 0), -1);

  ASSERT_EQ(image_value_add_delta(w), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_VALUE, 0), 2);

  ASSERT_EQ(image_value_add_delta(w), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_VALUE, 0), 1);

  ASSERT_EQ(image_value_add_delta(w), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_VALUE, 0), 5);

  widget_destroy(w);
}

#include "log_change_events.inc"

TEST(ImageValue, change_value) {
  widget_t* w = image_value_create(NULL, 10, 20, 30, 40);
  value_change_event_t evt;
  memset(&evt, 0x00, sizeof(evt));

  widget_on(w, EVT_VALUE_WILL_CHANGE, on_value_will_changed_accept, NULL);
  widget_on(w, EVT_VALUE_CHANGED, on_value_changed, &evt);
  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_VALUE, 3), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_VALUE, 0), 3);

  ASSERT_EQ(value_int(&(evt.old_value)), 0);
  ASSERT_EQ(value_int(&(evt.new_value)), 3);

  widget_destroy(w);
}

TEST(ImageValue, change_value_abort) {
  widget_t* w = image_value_create(NULL, 10, 20, 30, 40);
  value_change_event_t evt;
  memset(&evt, 0x00, sizeof(evt));

  widget_on(w, EVT_VALUE_WILL_CHANGE, on_value_will_changed_abort, NULL);
  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_VALUE, 3), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_VALUE, 3), 0);

  widget_destroy(w);
}

TEST(ImageValue, change_value_by_ui) {
  widget_t* w = image_value_create(NULL, 10, 20, 30, 40);
  value_change_event_t evt;
  memset(&evt, 0x00, sizeof(evt));

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_MIN, 1), RET_OK);
  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_MAX, 5), RET_OK);

  widget_on(w, EVT_VALUE_WILL_CHANGE, on_value_will_changed_accept, NULL);
  widget_on(w, EVT_VALUE_CHANGED, on_value_changed, &evt);
  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_CLICK_ADD_DELTA, 1), RET_OK);
  widget_dispatch_click(w);
  ASSERT_EQ(value_int(&(evt.old_value)), 0);
  ASSERT_EQ(value_int(&(evt.new_value)), 1);

  widget_destroy(w);
}

TEST(ImageValue, paint) {
  canvas_t c;
  font_manager_t font_manager;
  rect_t r = rect_init(0, 0, 800, 600);
  lcd_t* lcd = lcd_log_init(800, 600);
  font_manager_init(&font_manager, NULL);
  canvas_init(&c, lcd, &font_manager);
  widget_t* w = image_value_create(NULL, 10, 10, 200, 80);

  canvas_begin_frame(&c, &r, LCD_DRAW_NORMAL);
  widget_set_prop_int(w, WIDGET_PROP_VALUE, 100);
  widget_set_prop_str(w, WIDGET_PROP_FORMAT, "%02d");
  widget_set_prop_str(w, WIDGET_PROP_IMAGE, "num_");

  lcd_log_reset(lcd);
  widget_on_paint_self(w, &c);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,30,48,55,16,30,48);dg(0,0,30,48,85,16,30,48);dg(0,0,30,48,115,16,30,48);");
  lcd_log_reset(lcd);

  widget_set_style_str(w, "text_align_h", "left");
  widget_set_style_str(w, "text_align_v", "top");
  widget_on_paint_self(w, &c);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,30,48,2,2,30,48);dg(0,0,30,48,32,2,30,48);dg(0,0,30,48,62,2,30,48);");
  lcd_log_reset(lcd);

  widget_set_style_str(w, "text_align_h", "left");
  widget_set_style_str(w, "text_align_v", "middle");
  widget_on_paint_self(w, &c);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,30,48,2,16,30,48);dg(0,0,30,48,32,16,30,48);dg(0,0,30,48,62,16,30,48);");
  lcd_log_reset(lcd);

  widget_set_style_str(w, "text_align_h", "left");
  widget_set_style_str(w, "text_align_v", "bottom");
  widget_on_paint_self(w, &c);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,30,48,2,30,30,48);dg(0,0,30,48,32,30,30,48);dg(0,0,30,48,62,30,30,48);");
  lcd_log_reset(lcd);

  widget_set_style_str(w, "text_align_h", "center");
  widget_set_style_str(w, "text_align_v", "top");
  widget_on_paint_self(w, &c);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,30,48,55,2,30,48);dg(0,0,30,48,85,2,30,48);dg(0,0,30,48,115,2,30,48);");
  lcd_log_reset(lcd);

  widget_set_style_str(w, "text_align_h", "center");
  widget_set_style_str(w, "text_align_v", "middle");
  widget_on_paint_self(w, &c);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,30,48,55,16,30,48);dg(0,0,30,48,85,16,30,48);dg(0,0,30,48,115,16,30,48);");
  lcd_log_reset(lcd);

  widget_set_style_str(w, "text_align_h", "center");
  widget_set_style_str(w, "text_align_v", "bottom");
  widget_on_paint_self(w, &c);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,30,48,55,30,30,48);dg(0,0,30,48,85,30,30,48);dg(0,0,30,48,115,30,30,48);");
  lcd_log_reset(lcd);

  widget_set_style_str(w, "text_align_h", "right");
  widget_set_style_str(w, "text_align_v", "top");
  widget_on_paint_self(w, &c);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,30,48,108,2,30,48);dg(0,0,30,48,138,2,30,48);dg(0,0,30,48,168,2,30,48);");
  lcd_log_reset(lcd);

  widget_set_style_str(w, "text_align_h", "right");
  widget_set_style_str(w, "text_align_v", "middle");
  widget_on_paint_self(w, &c);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,30,48,108,16,30,48);dg(0,0,30,48,138,16,30,48);dg(0,0,30,48,168,16,30,48);");
  lcd_log_reset(lcd);

  widget_set_style_str(w, "text_align_h", "right");
  widget_set_style_str(w, "text_align_v", "bottom");
  widget_on_paint_self(w, &c);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,30,48,108,30,30,48);dg(0,0,30,48,138,30,30,48);dg(0,0,30,48,168,30,30,48);");
  lcd_log_reset(lcd);

  widget_set_style_str(w, "text_align_h", "center");
  widget_set_style_str(w, "text_align_v", "middle");
  widget_set_style_str(w, "fg_image_draw_type", "scale");
  widget_on_paint_self(w, &c);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,30,48,2,2,65,76);dg(0,0,30,48,67,2,65,76);dg(0,0,30,48,132,2,65,76);");
  lcd_log_reset(lcd);

  widget_set_style_str(w, "fg_image_draw_type", "scale_w");
  widget_on_paint_self(w, &c);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,5,30,42,2,0,65,92);dg(0,5,30,42,67,0,65,92);dg(0,5,30,42,132,0,65,92);");
  lcd_log_reset(lcd);

  widget_set_style_str(w, "fg_image_draw_type", "scale_h");
  widget_on_paint_self(w, &c);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,30,48,29,2,47,76);dg(0,0,30,48,76,2,47,76);dg(0,0,30,48,123,2,47,76);");
  lcd_log_reset(lcd);

  widget_set_style_str(w, "fg_image_draw_type", "scale_auto");
  widget_on_paint_self(w, &c);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,30,48,29,2,47,76);dg(0,0,30,48,76,2,47,76);dg(0,0,30,48,123,2,47,76);");
  lcd_log_reset(lcd);

  canvas_end_frame(&c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
  widget_destroy(w);
}
