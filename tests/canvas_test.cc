#include "base/canvas.h"
#include "base/font_manager.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>

TEST(Canvas, draw_hline) {
  rect_t r;
  canvas_t c;
  font_manager_t font_manager;
  lcd_t* lcd = lcd_log_init(800, 600);
  font_manager_init(&font_manager, NULL);
  canvas_init(&c, lcd, &font_manager);

  r = rect_init(100, 100, 200, 200);
  canvas_begin_frame(&c, &r, LCD_DRAW_NORMAL);

  lcd_log_reset(lcd);
  canvas_draw_hline(&c, 90, 110, 40);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dhl(100,110,30);");

  lcd_log_reset(lcd);
  canvas_draw_hline(&c, 110, 110, 40);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dhl(110,110,40);");

  lcd_log_reset(lcd);
  canvas_draw_hline(&c, 290, 110, 40);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dhl(290,110,10);");

  lcd_log_reset(lcd);
  canvas_draw_hline(&c, 290 + 40, 110, -40);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dhl(291,110,9);");

  canvas_end_frame(&c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
}

TEST(Canvas, draw_vline) {
  rect_t r;
  canvas_t c;
  font_manager_t font_manager;
  lcd_t* lcd = lcd_log_init(800, 600);
  font_manager_init(&font_manager, NULL);
  canvas_init(&c, lcd, &font_manager);

  r = rect_init(100, 100, 200, 200);
  canvas_begin_frame(&c, &r, LCD_DRAW_NORMAL);

  lcd_log_reset(lcd);
  canvas_draw_vline(&c, 110, 90, 40);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dvl(110,100,30);");

  lcd_log_reset(lcd);
  canvas_draw_vline(&c, 110, 110, 40);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dvl(110,110,40);");

  lcd_log_reset(lcd);
  canvas_draw_vline(&c, 110, 290, 40);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dvl(110,290,10);");

  lcd_log_reset(lcd);
  canvas_draw_vline(&c, 110, 290 + 40, -40);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dvl(110,291,9);");

  canvas_end_frame(&c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
}

TEST(Canvas, fill_rect) {
  rect_t r;
  canvas_t c;
  font_manager_t font_manager;
  lcd_t* lcd = lcd_log_init(800, 600);
  font_manager_init(&font_manager, NULL);
  canvas_init(&c, lcd, &font_manager);

  r = rect_init(100, 100, 200, 200);
  canvas_begin_frame(&c, &r, LCD_DRAW_NORMAL);

  lcd_log_reset(lcd);
  canvas_fill_rect(&c, 120, 310, 10, 10);
  ASSERT_EQ(lcd_log_get_commands(lcd), "");

  lcd_log_reset(lcd);
  canvas_fill_rect(&c, 110, 110, 40, 40);
  ASSERT_EQ(lcd_log_get_commands(lcd), "fr(110,110,40,40);");

  lcd_log_reset(lcd);
  canvas_fill_rect(&c, 180, 180, -40, -40);
  ASSERT_EQ(lcd_log_get_commands(lcd), "fr(141,141,40,40);");

  lcd_log_reset(lcd);
  canvas_fill_rect(&c, 90, 90, 40, 40);
  ASSERT_EQ(lcd_log_get_commands(lcd), "fr(100,100,30,30);");

  lcd_log_reset(lcd);
  canvas_fill_rect(&c, 90, 110, 40, 40);
  ASSERT_EQ(lcd_log_get_commands(lcd), "fr(100,110,30,40);");

  lcd_log_reset(lcd);
  canvas_fill_rect(&c, 290, 110, 40, 40);
  ASSERT_EQ(lcd_log_get_commands(lcd), "fr(290,110,10,40);");

  lcd_log_reset(lcd);
  canvas_fill_rect(&c, 290, 290, 40, 40);
  ASSERT_EQ(lcd_log_get_commands(lcd), "fr(290,290,10,10);");

  lcd_log_reset(lcd);
  canvas_fill_rect(&c, 90, 290, 40, 40);
  ASSERT_EQ(lcd_log_get_commands(lcd), "fr(100,290,30,10);");

  lcd_log_reset(lcd);
  canvas_fill_rect(&c, 90, 120, 40, 40);
  ASSERT_EQ(lcd_log_get_commands(lcd), "fr(100,120,30,40);");

  canvas_end_frame(&c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
}

TEST(Canvas, draw_points) {
  rect_t r;
  canvas_t c;
  font_manager_t font_manager;
  lcd_t* lcd = lcd_log_init(800, 600);
  font_manager_init(&font_manager, NULL);
  canvas_init(&c, lcd, &font_manager);
  point_t points[3];

#define POINTS3(x1, y1, x2, y2, x3, y3) \
  points[0].x = x1;                     \
  points[0].y = y1;                     \
  points[1].x = x2;                     \
  points[1].y = y2;                     \
  points[2].x = x3;                     \
  points[2].y = y3;
  r = rect_init(100, 100, 200, 200);
  canvas_begin_frame(&c, &r, LCD_DRAW_NORMAL);

  lcd_log_reset(lcd);
  POINTS3(0, 10, 100, 10, 320, 10);
  canvas_draw_points(&c, points, 3);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dps();");

  lcd_log_reset(lcd);
  POINTS3(0, 310, 100, 310, 320, 310);
  canvas_draw_points(&c, points, 3);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dps();");

  lcd_log_reset(lcd);
  POINTS3(10, 140, 140, 140, 320, 140);
  canvas_draw_points(&c, points, 3);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dps((140,140));");

  lcd_log_reset(lcd);
  POINTS3(100, 100, 200, 200, 300, 300);
  canvas_draw_points(&c, points, 3);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dps((100,100)(200,200));");

  canvas_end_frame(&c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
}

TEST(Canvas, draw_glyph) {
  rect_t r;
  canvas_t c;
  uint16_t font_size = 10;
  font_manager_t font_manager;
  lcd_t* lcd = lcd_log_init(800, 600);
  font_manager_init(&font_manager, NULL);
  canvas_init(&c, lcd, &font_manager);
  font_dummy_init();
  font_manager_add_font(&font_manager, font_dummy_0("demo0", font_size));

  r = rect_init(100, 100, 200, 200);
  canvas_begin_frame(&c, &r, LCD_DRAW_NORMAL);
  canvas_set_font(&c, "demo0", font_size);

  lcd_log_reset(lcd);
  canvas_draw_char(&c, 0, 95, 95);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(5,5,5,5,100,100);");

  lcd_log_reset(lcd);
  canvas_draw_char(&c, 0, 110, 95);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,5,10,5,110,100);");

  lcd_log_reset(lcd);
  canvas_draw_char(&c, 0, 110, 110);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,10,10,110,110);");

  lcd_log_reset(lcd);
  canvas_draw_char(&c, 0, 295, 110);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,5,10,295,110);");

  lcd_log_reset(lcd);
  canvas_draw_char(&c, 0, 95, 295);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(5,0,5,5,100,295);");

  canvas_end_frame(&c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
  canvas_reset(&c);
}

TEST(Canvas, draw_image) {
  rect_t r;
  rect_t s;
  rect_t d;
  canvas_t c;
  bitmap_t img;
  font_manager_t font_manager;
  font_manager_init(&font_manager, NULL);
  lcd_t* lcd = lcd_log_init(800, 600);
  canvas_init(&c, lcd, &font_manager);

  img.w = 32;
  img.h = 32;
  r = rect_init(100, 100, 200, 200);
  canvas_begin_frame(&c, &r, LCD_DRAW_NORMAL);

  lcd_log_reset(lcd);
  s = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w, img.h);
  canvas_draw_image(&c, &img, &s, &d);
  ASSERT_EQ(lcd_log_get_commands(lcd), "");

  lcd_log_reset(lcd);
  d = rect_init(90, 90, img.w, img.h);
  canvas_draw_image(&c, &img, &s, &d);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(10,10,22,22,100,100,22,22);");

  lcd_log_reset(lcd);
  d = rect_init(100, 90, img.w, img.h);
  canvas_draw_image(&c, &img, &s, &d);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,10,32,22,100,100,32,22);");

  lcd_log_reset(lcd);
  d = rect_init(290, 90, img.w, img.h);
  canvas_draw_image(&c, &img, &s, &d);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,10,10,22,290,100,10,22);");

  lcd_log_reset(lcd);
  d = rect_init(290, 290, img.w, img.h);
  canvas_draw_image(&c, &img, &s, &d);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,10,10,290,290,10,10);");

  s = rect_init(10, 10, 10, 10);

  lcd_log_reset(lcd);
  d = rect_init(95, 95, 10, 10);
  canvas_draw_image(&c, &img, &s, &d);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(15,15,5,5,100,100,5,5);");

  lcd_log_reset(lcd);
  d = rect_init(95, 100, 10, 10);
  canvas_draw_image(&c, &img, &s, &d);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(15,10,5,10,100,100,5,10);");

  lcd_log_reset(lcd);
  d = rect_init(100, 100, 10, 10);
  canvas_draw_image(&c, &img, &s, &d);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(10,10,10,10,100,100,10,10);");

  lcd_log_reset(lcd);
  d = rect_init(100, 295, 10, 10);
  canvas_draw_image(&c, &img, &s, &d);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(10,10,10,5,100,295,10,5);");

  lcd_log_reset(lcd);
  d = rect_init(290, 290, 10, 10);
  canvas_draw_image(&c, &img, &s, &d);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(10,10,10,10,290,290,10,10);");
  canvas_end_frame(&c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
  canvas_reset(&c);
}

TEST(Canvas, draw_image_patch3_x_scale_y) {
  rect_t r;
  rect_t d;
  canvas_t c;
  bitmap_t img;
  font_manager_t font_manager;
  font_manager_init(&font_manager, NULL);
  lcd_t* lcd = lcd_log_init(800, 600);
  canvas_init(&c, lcd, &font_manager);

  img.w = 32;
  img.h = 32;
  r = rect_init(0, 0, 320, 480);
  canvas_begin_frame(&c, &r, LCD_DRAW_NORMAL);

  lcd_log_reset(lcd);
  d = rect_init(0, 0, img.w, img.h);
  ASSERT_EQ(canvas_draw_image_patch3_x_scale_y(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,10,32,0,0,10,32);dg(10,0,12,32,10,0,12,32);dg(22,0,10,32,22,0,10,32);");

  lcd_log_reset(lcd);
  d = rect_init(0, 0, img.w * 3, img.h * 2);
  ASSERT_EQ(canvas_draw_image_patch3_x_scale_y(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,10,32,0,0,10,64);dg(10,0,12,32,10,0,76,64);dg(22,0,10,32,86,0,10,64);");

  lcd_log_reset(lcd);
  d = rect_init(0, 0, img.w * 3, img.h * 2);
  ASSERT_EQ(canvas_draw_image_ex(&c, &img, IMAGE_DRAW_PATCH3_X_SCALE_Y, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,10,32,0,0,10,64);dg(10,0,12,32,10,0,76,64);dg(22,0,10,32,86,0,10,64);");

  lcd_log_reset(lcd);
  d = rect_init(0, 0, img.w * 3, img.h / 2);
  ASSERT_EQ(canvas_draw_image_patch3_x_scale_y(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,10,32,0,0,10,16);dg(10,0,12,32,10,0,76,16);dg(22,0,10,32,86,0,10,16);");

  canvas_end_frame(&c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
  canvas_reset(&c);
}

TEST(Canvas, draw_image_patch3_y_scale_x) {
  rect_t r;
  rect_t d;
  canvas_t c;
  bitmap_t img;
  font_manager_t font_manager;
  font_manager_init(&font_manager, NULL);
  lcd_t* lcd = lcd_log_init(800, 600);
  canvas_init(&c, lcd, &font_manager);

  img.w = 32;
  img.h = 32;
  r = rect_init(0, 0, 320, 480);
  canvas_begin_frame(&c, &r, LCD_DRAW_NORMAL);

  lcd_log_reset(lcd);
  d = rect_init(0, 0, img.w, img.h);
  ASSERT_EQ(canvas_draw_image_patch3_y_scale_x(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,32,10,0,0,32,10);dg(0,10,32,12,0,10,32,12);dg(0,22,32,10,0,22,32,10);");

  lcd_log_reset(lcd);
  d = rect_init(0, 0, img.w * 2, img.h * 3);
  ASSERT_EQ(canvas_draw_image_patch3_y_scale_x(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,32,10,0,0,64,10);dg(0,10,32,12,0,10,64,76);dg(0,22,32,10,0,86,64,10);");

  lcd_log_reset(lcd);
  d = rect_init(0, 0, img.w / 2, img.h * 3);
  ASSERT_EQ(canvas_draw_image_patch3_y_scale_x(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,32,10,0,0,16,10);dg(0,10,32,12,0,10,16,76);dg(0,22,32,10,0,86,16,10);");

  lcd_log_reset(lcd);
  d = rect_init(0, 0, img.w / 2, img.h * 3);
  ASSERT_EQ(canvas_draw_image_ex(&c, &img, IMAGE_DRAW_PATCH3_Y_SCALE_X, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,32,10,0,0,16,10);dg(0,10,32,12,0,10,16,76);dg(0,22,32,10,0,86,16,10);");

  canvas_end_frame(&c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
  canvas_reset(&c);
}

TEST(Canvas, draw_image_patch9) {
  rect_t r;
  rect_t d;
  canvas_t c;
  bitmap_t img;
  font_manager_t font_manager;
  font_manager_init(&font_manager, NULL);
  lcd_t* lcd = lcd_log_init(800, 600);
  canvas_init(&c, lcd, &font_manager);

  img.w = 32;
  img.h = 32;
  r = rect_init(0, 0, 320, 480);
  canvas_begin_frame(&c, &r, LCD_DRAW_NORMAL);

  lcd_log_reset(lcd);
  d = rect_init(0, 0, img.w, img.h);
  ASSERT_EQ(canvas_draw_image_patch9(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,10,10,0,0,10,10);dg(22,0,10,10,22,0,10,10);dg(0,22,10,10,0,22,10,10);dg(22,22,"
            "10,10,22,22,10,10);dg(10,0,12,10,10,0,12,10);dg(10,22,12,10,10,22,12,10);dg(0,10,10,"
            "12,0,10,10,12);dg(22,10,10,12,22,10,10,12);dg(10,10,12,12,10,10,12,12);");

  lcd_log_reset(lcd);
  d = rect_init(0, 0, img.w * 2, img.h * 3);
  ASSERT_EQ(canvas_draw_image_patch9(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,10,10,0,0,10,10);dg(22,0,10,10,54,0,10,10);dg(0,22,10,10,0,86,10,10);dg(22,22,"
            "10,10,54,86,10,10);dg(10,0,12,10,10,0,44,10);dg(10,22,12,10,10,86,44,10);dg(0,10,10,"
            "12,0,10,10,76);dg(22,10,10,12,54,10,10,76);dg(10,10,12,12,10,10,44,76);");

  lcd_log_reset(lcd);
  d = rect_init(0, 0, img.w * 2, img.h * 3);
  ASSERT_EQ(canvas_draw_image_ex(&c, &img, IMAGE_DRAW_PATCH9, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,10,10,0,0,10,10);dg(22,0,10,10,54,0,10,10);dg(0,22,10,10,0,86,10,10);dg(22,22,"
            "10,10,54,86,10,10);dg(10,0,12,10,10,0,44,10);dg(10,22,12,10,10,86,44,10);dg(0,10,10,"
            "12,0,10,10,76);dg(22,10,10,12,54,10,10,76);dg(10,10,12,12,10,10,44,76);");

  lcd_log_reset(lcd);
  d = rect_init(0, 0, img.w / 2, img.h * 3);
  ASSERT_EQ(canvas_draw_image_patch9(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,5,10,0,0,5,10);dg(27,0,5,10,11,0,5,10);dg(0,22,5,10,0,86,5,10);dg(27,22,5,10,"
            "11,86,5,10);dg(5,0,22,10,5,0,6,10);dg(5,22,22,10,5,86,6,10);dg(0,10,5,12,0,10,5,76);"
            "dg(27,10,5,12,11,10,5,76);dg(5,10,22,12,5,10,6,76);");

  canvas_end_frame(&c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
  canvas_reset(&c);
}

TEST(Canvas, draw_image_repeat_x) {
  rect_t r;
  rect_t d;
  canvas_t c;
  bitmap_t img;
  font_manager_t font_manager;
  font_manager_init(&font_manager, NULL);
  lcd_t* lcd = lcd_log_init(800, 600);
  canvas_init(&c, lcd, &font_manager);

  img.w = 32;
  img.h = 32;
  r = rect_init(0, 0, 320, 480);
  canvas_begin_frame(&c, &r, LCD_DRAW_NORMAL);

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w / 2, img.h);
  ASSERT_EQ(canvas_draw_image_repeat_x(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,16,32,0,0,16,32);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(2, 2, img.w / 3, img.h);
  ASSERT_EQ(canvas_draw_image_repeat_x(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,10,32,2,2,10,32);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(2, 2, img.w * 3, img.h);
  ASSERT_EQ(canvas_draw_image_repeat_x(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,32,32,2,2,32,32);dg(0,0,32,32,34,2,32,32);dg(0,0,32,32,66,2,32,32);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w + 10, img.h / 2);
  ASSERT_EQ(canvas_draw_image_repeat_x(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,32,16,0,0,32,16);dg(0,0,10,16,32,0,10,16);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w + 10, img.h / 2);
  ASSERT_EQ(canvas_draw_image_ex(&c, &img, IMAGE_DRAW_REPEAT_X, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,32,16,0,0,32,16);dg(0,0,10,16,32,0,10,16);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w * 2 + 10, img.h + 10);
  ASSERT_EQ(canvas_draw_image_repeat_x(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,32,32,0,0,32,42);dg(0,0,32,32,32,0,32,42);dg(0,0,10,32,64,0,10,42);");

  canvas_end_frame(&c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
  canvas_reset(&c);
}

TEST(Canvas, draw_image_repeat_y) {
  rect_t r;
  rect_t d;
  canvas_t c;
  bitmap_t img;
  font_manager_t font_manager;
  font_manager_init(&font_manager, NULL);
  lcd_t* lcd = lcd_log_init(800, 600);
  canvas_init(&c, lcd, &font_manager);

  img.w = 32;
  img.h = 32;
  r = rect_init(0, 0, 320, 480);
  canvas_begin_frame(&c, &r, LCD_DRAW_NORMAL);

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w, img.h / 2);
  ASSERT_EQ(canvas_draw_image_repeat_y(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,32,16,0,0,32,16);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w + 10, img.h / 2);
  ASSERT_EQ(canvas_draw_image_repeat_y(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,32,16,0,0,42,16);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(2, 2, img.w + 10, img.h / 2);
  ASSERT_EQ(canvas_draw_image_repeat_y(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,32,16,2,2,42,16);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(2, 2, img.w + 10, img.h * 3);
  ASSERT_EQ(canvas_draw_image_repeat_y(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,32,32,2,2,42,32);dg(0,0,32,32,2,34,42,32);dg(0,0,32,32,2,66,42,32);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w + 10, img.h * 2 + 10);
  ASSERT_EQ(canvas_draw_image_repeat_y(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,32,32,0,0,42,32);dg(0,0,32,32,0,32,42,32);dg(0,0,32,10,0,64,42,10);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w + 10, img.h * 2 + 10);
  ASSERT_EQ(canvas_draw_image_ex(&c, &img, IMAGE_DRAW_REPEAT_Y, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,32,32,0,0,42,32);dg(0,0,32,32,0,32,42,32);dg(0,0,32,10,0,64,42,10);");

  canvas_end_frame(&c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
  canvas_reset(&c);
}

TEST(Canvas, draw_image_repeat_y_inverse) {
  rect_t r;
  rect_t d;
  canvas_t c;
  bitmap_t img;
  font_manager_t font_manager;
  font_manager_init(&font_manager, NULL);
  lcd_t* lcd = lcd_log_init(800, 600);
  canvas_init(&c, lcd, &font_manager);

  img.w = 32;
  img.h = 32;
  r = rect_init(0, 0, 320, 480);
  canvas_begin_frame(&c, &r, LCD_DRAW_NORMAL);

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w, img.h / 2);
  ASSERT_EQ(canvas_draw_image_repeat_y_inverse(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,16,32,16,0,0,32,16);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w + 10, img.h / 2);
  ASSERT_EQ(canvas_draw_image_repeat_y_inverse(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,16,32,16,0,0,42,16);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(2, 2, img.w + 10, img.h / 2);
  ASSERT_EQ(canvas_draw_image_repeat_y_inverse(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,16,32,16,2,2,42,16);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(2, 2, img.w + 10, img.h * 3);
  ASSERT_EQ(canvas_draw_image_repeat_y_inverse(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,32,32,2,66,42,32);dg(0,0,32,32,2,34,42,32);dg(0,0,32,32,2,2,42,32);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w + 10, img.h * 2 + 10);
  ASSERT_EQ(canvas_draw_image_repeat_y_inverse(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,32,32,0,42,42,32);dg(0,0,32,32,0,10,42,32);dg(0,22,32,10,0,0,42,10);");

  canvas_end_frame(&c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
  canvas_reset(&c);
}

TEST(Canvas, draw_image_repeat) {
  rect_t r;
  rect_t d;
  canvas_t c;
  bitmap_t img;
  font_manager_t font_manager;
  font_manager_init(&font_manager, NULL);
  lcd_t* lcd = lcd_log_init(800, 600);
  canvas_init(&c, lcd, &font_manager);

  img.w = 32;
  img.h = 32;
  r = rect_init(0, 0, 320, 480);
  canvas_begin_frame(&c, &r, LCD_DRAW_NORMAL);

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w / 2, img.h / 2);
  ASSERT_EQ(canvas_draw_image_repeat(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,16,16,0,0,16,16);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w + 10, img.h / 2);
  ASSERT_EQ(canvas_draw_image_repeat(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,32,16,0,0,32,16);dg(0,0,10,16,32,0,10,16);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w + 10, img.h / 2);
  ASSERT_EQ(canvas_draw_image_ex(&c, &img, IMAGE_DRAW_REPEAT, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,32,16,0,0,32,16);dg(0,0,10,16,32,0,10,16);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w * 2 + 10, img.h * 2 + 10);
  ASSERT_EQ(canvas_draw_image_repeat(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,32,32,0,0,32,32);dg(0,0,32,32,32,0,32,32);dg(0,0,10,32,64,0,10,32);dg(0,0,32,"
            "32,0,32,32,32);dg(0,0,32,32,32,32,32,32);dg(0,0,10,32,64,32,10,32);dg(0,0,32,10,0,64,"
            "32,10);dg(0,0,32,10,32,64,32,10);dg(0,0,10,10,64,64,10,10);");

  canvas_end_frame(&c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
  canvas_reset(&c);
}

TEST(Canvas, draw_image_repeat9) {
  rect_t r;
  rect_t d;
  canvas_t c;
  bitmap_t img;
  font_manager_t font_manager;
  font_manager_init(&font_manager, NULL);
  lcd_t* lcd = lcd_log_init(800, 600);
  canvas_init(&c, lcd, &font_manager);

  img.w = 32;
  img.h = 32;
  r = rect_init(0, 0, 320, 480);
  canvas_begin_frame(&c, &r, LCD_DRAW_NORMAL);

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w / 2, img.h / 2);
  ASSERT_EQ(canvas_draw_image_repeat9(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,8,8,0,0,8,8);dg(24,0,8,8,8,0,8,8);dg(0,24,8,8,0,8,8,8);dg(24,24,8,8,8,8,8,8);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w + 10, img.h / 2);
  ASSERT_EQ(canvas_draw_image_repeat9(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,16,8,0,0,16,8);dg(16,0,16,8,26,0,16,8);dg(0,24,16,8,0,8,16,8);dg(16,24,16,8,26,"
            "8,16,8);"
            "dg(15,0,2,8,16,0,2,8);dg(15,0,2,8,18,0,2,8);dg(15,0,2,8,20,0,2,8);dg(15,0,2,8,22,0,2,"
            "8);dg(15,0,2,8,24,0,2,8);"
            "dg(15,24,2,8,16,8,2,8);dg(15,24,2,8,18,8,2,8);dg(15,24,2,8,20,8,2,8);dg(15,24,2,8,22,"
            "8,2,8);dg(15,24,2,8,24,8,2,8);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w + 10, img.h / 2);
  ASSERT_EQ(canvas_draw_image_ex(&c, &img, IMAGE_DRAW_REPEAT9, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,16,8,0,0,16,8);dg(16,0,16,8,26,0,16,8);dg(0,24,16,8,0,8,16,8);dg(16,24,16,8,26,"
            "8,16,8);"
            "dg(15,0,2,8,16,0,2,8);dg(15,0,2,8,18,0,2,8);dg(15,0,2,8,20,0,2,8);dg(15,0,2,8,22,0,2,"
            "8);dg(15,0,2,8,24,0,2,8);"
            "dg(15,24,2,8,16,8,2,8);dg(15,24,2,8,18,8,2,8);dg(15,24,2,8,20,8,2,8);dg(15,24,2,8,22,"
            "8,2,8);dg(15,24,2,8,24,8,2,8);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w + 15, img.h + 15);
  ASSERT_EQ(canvas_draw_image_repeat9(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,16,16,0,0,16,16);dg(16,0,16,16,31,0,16,16);dg(0,16,16,16,0,31,16,16);dg(16,16,"
            "16,16,31,31,16,16);"
            "dg(15,0,2,16,16,0,2,16);dg(15,0,2,16,18,0,2,16);dg(15,0,2,16,20,0,2,16);dg(15,0,2,16,"
            "22,0,2,16);dg(15,0,2,16,24,0,2,16);dg(15,0,2,16,26,0,2,16);dg(15,0,2,16,28,0,2,16);dg("
            "15,0,1,16,30,0,1,16);"
            "dg(15,16,2,16,16,31,2,16);dg(15,16,2,16,18,31,2,16);dg(15,16,2,16,20,31,2,16);dg(15,"
            "16,2,16,22,31,2,16);dg(15,16,2,16,24,31,2,16);dg(15,16,2,16,26,31,2,16);dg(15,16,2,16,"
            "28,31,2,16);dg(15,16,1,16,30,31,1,16);"
            "dg(0,15,16,2,0,16,16,2);dg(0,15,16,2,0,18,16,2);dg(0,15,16,2,0,20,16,2);dg(0,15,16,2,"
            "0,22,16,2);dg(0,15,16,2,0,24,16,2);dg(0,15,16,2,0,26,16,2);dg(0,15,16,2,0,28,16,2);dg("
            "0,15,16,1,0,30,16,1);"
            "dg(16,15,16,2,31,16,16,2);dg(16,15,16,2,31,18,16,2);dg(16,15,16,2,31,20,16,2);dg(16,"
            "15,16,2,31,22,16,2);dg(16,15,16,2,31,24,16,2);dg(16,15,16,2,31,26,16,2);dg(16,15,16,2,"
            "31,28,16,2);dg(16,15,16,1,31,30,16,1);"
            "dg(15,15,2,2,16,16,2,2);dg(15,15,2,2,18,16,2,2);dg(15,15,2,2,20,16,2,2);dg(15,15,2,2,"
            "22,16,2,2);dg(15,15,2,2,24,16,2,2);dg(15,15,2,2,26,16,2,2);dg(15,15,2,2,28,16,2,2);dg("
            "15,15,1,2,30,16,1,2);dg(15,15,2,2,16,18,2,2);dg(15,15,2,2,18,18,2,2);dg(15,15,2,2,20,"
            "18,2,2);dg(15,15,2,2,22,18,2,2);dg(15,15,2,2,24,18,2,2);dg(15,15,2,2,26,18,2,2);dg(15,"
            "15,2,2,28,18,2,2);dg(15,15,1,2,30,18,1,2);dg(15,15,2,2,16,20,2,2);dg(15,15,2,2,18,20,"
            "2,2);dg(15,15,2,2,20,20,2,2);dg(15,15,2,2,22,20,2,2);dg(15,15,2,2,24,20,2,2);dg(15,15,"
            "2,2,26,20,2,2);dg(15,15,2,2,28,20,2,2);dg(15,15,1,2,30,20,1,2);dg(15,15,2,2,16,22,2,2)"
            ";dg(15,15,2,2,18,22,2,2);dg(15,15,2,2,20,22,2,2);dg(15,15,2,2,22,22,2,2);dg(15,15,2,2,"
            "24,22,2,2);dg(15,15,2,2,26,22,2,2);dg(15,15,2,2,28,22,2,2);dg(15,15,1,2,30,22,1,2);dg("
            "15,15,2,2,16,24,2,2);dg(15,15,2,2,18,24,2,2);dg(15,15,2,2,20,24,2,2);dg(15,15,2,2,22,"
            "24,2,2);dg(15,15,2,2,24,24,2,2);dg(15,15,2,2,26,24,2,2);dg(15,15,2,2,28,24,2,2);dg(15,"
            "15,1,2,30,24,1,2);dg(15,15,2,2,16,26,2,2);dg(15,15,2,2,18,26,2,2);dg(15,15,2,2,20,26,"
            "2,2);dg(15,15,2,2,22,26,2,2);dg(15,15,2,2,24,26,2,2);dg(15,15,2,2,26,26,2,2);dg(15,15,"
            "2,2,28,26,2,2);dg(15,15,1,2,30,26,1,2);dg(15,15,2,2,16,28,2,2);dg(15,15,2,2,18,28,2,2)"
            ";dg(15,15,2,2,20,28,2,2);dg(15,15,2,2,22,28,2,2);dg(15,15,2,2,24,28,2,2);dg(15,15,2,2,"
            "26,28,2,2);dg(15,15,2,2,28,28,2,2);dg(15,15,1,2,30,28,1,2);dg(15,15,2,1,16,30,2,1);dg("
            "15,15,2,1,18,30,2,1);dg(15,15,2,1,20,30,2,1);dg(15,15,2,1,22,30,2,1);dg(15,15,2,1,24,"
            "30,2,1);dg(15,15,2,1,26,30,2,1);dg(15,15,2,1,28,30,2,1);dg(15,15,1,1,30,30,1,1);");

  canvas_end_frame(&c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
  canvas_reset(&c);
}

TEST(Canvas, draw_image_repeat3_x) {
  rect_t r;
  rect_t d;
  canvas_t c;
  bitmap_t img;
  font_manager_t font_manager;
  font_manager_init(&font_manager, NULL);
  lcd_t* lcd = lcd_log_init(800, 600);
  canvas_init(&c, lcd, &font_manager);

  img.w = 32;
  img.h = 32;
  r = rect_init(0, 0, 320, 480);
  canvas_begin_frame(&c, &r, LCD_DRAW_NORMAL);

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w / 2, img.h / 2);
  ASSERT_EQ(canvas_draw_image_repeat3_x(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,8,24,0,0,8,24);dg(24,0,8,24,8,0,8,24);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w + 10, img.h / 2);
  ASSERT_EQ(canvas_draw_image_repeat3_x(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,16,24,0,0,16,24);dg(16,0,2,24,16,0,2,24);dg(16,0,2,24,18,0,2,24);dg(16,0,2,24,"
            "20,0,2,24);dg(16,0,2,24,22,0,2,24);dg(16,0,2,24,24,0,2,24);dg(16,0,16,24,26,0,16,24)"
            ";");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w + 10, img.h / 2);
  ASSERT_EQ(canvas_draw_image_ex(&c, &img, IMAGE_DRAW_REPEAT3_X, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,16,24,0,0,16,24);dg(16,0,2,24,16,0,2,24);dg(16,0,2,24,18,0,2,24);dg(16,0,2,24,"
            "20,0,2,24);dg(16,0,2,24,22,0,2,24);dg(16,0,2,24,24,0,2,24);dg(16,0,16,24,26,0,16,24)"
            ";");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w + 15, img.h + 15);
  ASSERT_EQ(canvas_draw_image_repeat3_x(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,16,32,0,7,16,32);dg(16,0,2,32,16,7,2,32);dg(16,0,2,32,18,7,2,32);dg(16,0,2,32,"
            "20,7,2,32);dg(16,0,2,32,22,7,2,32);dg(16,0,2,32,24,7,2,32);dg(16,0,2,32,26,7,2,32);dg("
            "16,0,2,32,28,7,2,32);dg(16,0,1,32,30,7,1,32);dg(16,0,16,32,31,7,16,32);");

  canvas_end_frame(&c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
  canvas_reset(&c);
}

TEST(Canvas, draw_image_repeat3_y) {
  rect_t r;
  rect_t d;
  canvas_t c;
  bitmap_t img;
  font_manager_t font_manager;
  font_manager_init(&font_manager, NULL);
  lcd_t* lcd = lcd_log_init(800, 600);
  canvas_init(&c, lcd, &font_manager);

  img.w = 32;
  img.h = 32;
  r = rect_init(0, 0, 320, 480);
  canvas_begin_frame(&c, &r, LCD_DRAW_NORMAL);

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w / 2, img.h / 2);
  ASSERT_EQ(canvas_draw_image_repeat3_y(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,24,8,0,0,24,8);dg(0,24,24,8,0,8,24,8);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w / 2, img.h + 10);
  ASSERT_EQ(canvas_draw_image_repeat3_y(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,24,16,0,0,24,16);dg(0,15,24,2,0,16,24,2);dg(0,15,24,2,0,18,24,2);dg(0,15,24,2,"
            "0,20,24,2);dg(0,15,24,2,0,22,24,2);dg(0,15,24,2,0,24,24,2);dg(0,16,24,16,0,26,24,16)"
            ";");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w / 2, img.h + 10);
  ASSERT_EQ(canvas_draw_image_ex(&c, &img, IMAGE_DRAW_REPEAT3_Y, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,24,16,0,0,24,16);dg(0,15,24,2,0,16,24,2);dg(0,15,24,2,0,18,24,2);dg(0,15,24,2,"
            "0,20,24,2);dg(0,15,24,2,0,22,24,2);dg(0,15,24,2,0,24,24,2);dg(0,16,24,16,0,26,24,16)"
            ";");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w + 15, img.h + 15);
  ASSERT_EQ(canvas_draw_image_repeat3_y(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd),
            "dg(0,0,32,16,7,0,32,16);dg(0,15,32,2,7,16,32,2);dg(0,15,32,2,7,18,32,2);dg(0,15,32,2,"
            "7,20,32,2);dg(0,15,32,2,7,22,32,2);dg(0,15,32,2,7,24,32,2);dg(0,15,32,2,7,26,32,2);dg("
            "0,15,32,2,7,28,32,2);dg(0,15,32,1,7,30,32,1);dg(0,16,32,16,7,31,32,16);");

  canvas_end_frame(&c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
  canvas_reset(&c);
}

TEST(Canvas, draw_image_scale_w) {
  rect_t r;
  rect_t d;
  canvas_t c;
  bitmap_t img;
  font_manager_t font_manager;
  font_manager_init(&font_manager, NULL);
  lcd_t* lcd = lcd_log_init(800, 600);
  canvas_init(&c, lcd, &font_manager);

  img.w = 32;
  img.h = 32;
  r = rect_init(0, 0, 320, 480);
  canvas_begin_frame(&c, &r, LCD_DRAW_NORMAL);

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w / 2, img.h / 2);
  ASSERT_EQ(canvas_draw_image_scale_w(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,32,32,0,0,16,16);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w + 10, img.h / 2);
  ASSERT_EQ(canvas_draw_image_scale_w(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,32,12,0,0,42,16);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w * 2, img.h * 2 + 10);
  ASSERT_EQ(canvas_draw_image_scale_w(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,32,32,0,0,64,64);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w * 2, img.h * 2 + 10);
  ASSERT_EQ(canvas_draw_image_ex(&c, &img, IMAGE_DRAW_SCALE_W, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,32,32,0,0,64,64);");

  canvas_end_frame(&c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
  canvas_reset(&c);
}

TEST(Canvas, draw_image_scale_h) {
  rect_t r;
  rect_t d;
  canvas_t c;
  bitmap_t img;
  font_manager_t font_manager;
  font_manager_init(&font_manager, NULL);
  lcd_t* lcd = lcd_log_init(800, 600);
  canvas_init(&c, lcd, &font_manager);

  img.w = 32;
  img.h = 32;
  r = rect_init(0, 0, 320, 480);
  canvas_begin_frame(&c, &r, LCD_DRAW_NORMAL);

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w / 2, img.h / 2);
  ASSERT_EQ(canvas_draw_image_scale_h(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,32,32,0,0,16,16);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w + 10, img.h / 2);
  ASSERT_EQ(canvas_draw_image_scale_h(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,32,32,0,0,16,16);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w * 2, img.h * 2 + 10);
  ASSERT_EQ(canvas_draw_image_scale_h(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,27,32,0,0,64,74);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w * 2, img.h * 2 + 10);
  ASSERT_EQ(canvas_draw_image_ex(&c, &img, IMAGE_DRAW_SCALE_H, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,27,32,0,0,64,74);");

  canvas_end_frame(&c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
  canvas_reset(&c);
}

TEST(Canvas, draw_image_scale) {
  rect_t r;
  rect_t d;
  canvas_t c;
  bitmap_t img;
  font_manager_t font_manager;
  font_manager_init(&font_manager, NULL);
  lcd_t* lcd = lcd_log_init(800, 600);
  canvas_init(&c, lcd, &font_manager);

  img.w = 32;
  img.h = 32;
  r = rect_init(0, 0, 320, 480);
  canvas_begin_frame(&c, &r, LCD_DRAW_NORMAL);

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w / 2, img.h / 2);
  ASSERT_EQ(canvas_draw_image_scale(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,32,32,0,0,16,16);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w + 10, img.h / 2);
  ASSERT_EQ(canvas_draw_image_scale(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,32,32,13,0,16,16);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w * 2, img.h * 2 + 10);
  ASSERT_EQ(canvas_draw_image_scale(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,32,32,0,5,64,64);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w * 2, img.h * 2 + 10);
  ASSERT_EQ(canvas_draw_image_ex(&c, &img, IMAGE_DRAW_SCALE_AUTO, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,32,32,0,5,64,64);");

  canvas_end_frame(&c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
  canvas_reset(&c);
}

TEST(Canvas, draw_image_center) {
  rect_t r;
  rect_t d;
  canvas_t c;
  bitmap_t img;
  font_manager_t font_manager;
  font_manager_init(&font_manager, NULL);
  lcd_t* lcd = lcd_log_init(800, 600);
  canvas_init(&c, lcd, &font_manager);

  img.w = 32;
  img.h = 32;
  r = rect_init(0, 0, 320, 480);
  canvas_begin_frame(&c, &r, LCD_DRAW_NORMAL);

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w, img.h);
  ASSERT_EQ(canvas_draw_image_center(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,32,32,0,0,32,32);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w + 10, img.h + 10);
  ASSERT_EQ(canvas_draw_image_center(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,32,32,5,5,32,32);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w * 2, img.h * 2 + 10);
  ASSERT_EQ(canvas_draw_image_center(&c, &img, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,32,32,16,21,32,32);");

  lcd_log_reset(lcd);
  r = rect_init(0, 0, img.w, img.h);
  d = rect_init(0, 0, img.w * 2, img.h * 2 + 10);
  ASSERT_EQ(canvas_draw_image_ex(&c, &img, IMAGE_DRAW_CENTER, &d), RET_OK);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,32,32,16,21,32,32);");

  canvas_end_frame(&c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
  canvas_reset(&c);
}
