#include "base/array.h"
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
  font_manager_init(&font_manager);
  canvas_init(&c, lcd, &font_manager);

  rect_init(r, 100, 100, 200, 200);
  canvas_begin_frame(&c, &r);

  lcd_log_reset(lcd);
  canvas_draw_hline(&c, 10, 20, 100);
  ASSERT_EQ(lcd_log_get_commands(lcd), "");

  lcd_log_reset(lcd);
  canvas_draw_hline(&c, 10, 310, 100);
  ASSERT_EQ(lcd_log_get_commands(lcd), "");

  lcd_log_reset(lcd);
  canvas_draw_hline(&c, 10, 110, 10);
  ASSERT_EQ(lcd_log_get_commands(lcd), "");

  lcd_log_reset(lcd);
  canvas_draw_hline(&c, 320, 110, 10);
  ASSERT_EQ(lcd_log_get_commands(lcd), "");

  lcd_log_reset(lcd);
  canvas_draw_hline(&c, 90, 110, 40);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dhl(100,110,30);");

  lcd_log_reset(lcd);
  canvas_draw_hline(&c, 110, 110, 40);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dhl(110,110,40);");

  lcd_log_reset(lcd);
  canvas_draw_hline(&c, 290, 110, 40);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dhl(290,110,10);");

  canvas_end_frame(&c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
}

TEST(Canvas, draw_vline) {
  rect_t r;
  canvas_t c;
  font_manager_t font_manager;
  lcd_t* lcd = lcd_log_init(800, 600);
  font_manager_init(&font_manager);
  canvas_init(&c, lcd, &font_manager);

  rect_init(r, 100, 100, 200, 200);
  canvas_begin_frame(&c, &r);

  lcd_log_reset(lcd);
  canvas_draw_vline(&c, 10, 20, 100);
  ASSERT_EQ(lcd_log_get_commands(lcd), "");

  lcd_log_reset(lcd);
  canvas_draw_vline(&c, 310, 20, 100);
  ASSERT_EQ(lcd_log_get_commands(lcd), "");

  lcd_log_reset(lcd);
  canvas_draw_vline(&c, 110, 10, 10);
  ASSERT_EQ(lcd_log_get_commands(lcd), "");

  lcd_log_reset(lcd);
  canvas_draw_vline(&c, 110, 320, 10);
  ASSERT_EQ(lcd_log_get_commands(lcd), "");

  lcd_log_reset(lcd);
  canvas_draw_vline(&c, 110, 90, 40);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dvl(110,100,30);");

  lcd_log_reset(lcd);
  canvas_draw_vline(&c, 110, 110, 40);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dvl(110,110,40);");

  lcd_log_reset(lcd);
  canvas_draw_vline(&c, 110, 290, 40);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dvl(110,290,10);");

  canvas_end_frame(&c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
}

TEST(Canvas, fill_rect) {
  rect_t r;
  canvas_t c;
  font_manager_t font_manager;
  lcd_t* lcd = lcd_log_init(800, 600);
  font_manager_init(&font_manager);
  canvas_init(&c, lcd, &font_manager);

  rect_init(r, 100, 100, 200, 200);
  canvas_begin_frame(&c, &r);

  lcd_log_reset(lcd);
  canvas_fill_rect(&c, 10, 10, 10, 10);
  ASSERT_EQ(lcd_log_get_commands(lcd), "");

  lcd_log_reset(lcd);
  canvas_fill_rect(&c, 10, 210, 10, 10);
  ASSERT_EQ(lcd_log_get_commands(lcd), "");

  lcd_log_reset(lcd);
  canvas_fill_rect(&c, 10, 310, 10, 10);
  ASSERT_EQ(lcd_log_get_commands(lcd), "");

  lcd_log_reset(lcd);
  canvas_fill_rect(&c, 310, 10, 10, 10);
  ASSERT_EQ(lcd_log_get_commands(lcd), "");

  lcd_log_reset(lcd);
  canvas_fill_rect(&c, 310, 210, 10, 10);
  ASSERT_EQ(lcd_log_get_commands(lcd), "");

  lcd_log_reset(lcd);
  canvas_fill_rect(&c, 310, 310, 10, 10);
  ASSERT_EQ(lcd_log_get_commands(lcd), "");

  lcd_log_reset(lcd);
  canvas_fill_rect(&c, 120, 10, 10, 10);
  ASSERT_EQ(lcd_log_get_commands(lcd), "");

  lcd_log_reset(lcd);
  canvas_fill_rect(&c, 120, 310, 10, 10);
  ASSERT_EQ(lcd_log_get_commands(lcd), "");

  lcd_log_reset(lcd);
  canvas_fill_rect(&c, 110, 110, 40, 40);
  ASSERT_EQ(lcd_log_get_commands(lcd), "fr(110,110,40,40);");

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
  font_manager_init(&font_manager);
  canvas_init(&c, lcd, &font_manager);
  point_t points[3];

#define POINTS3(x1, y1, x2, y2, x3, y3) \
  points[0].x = x1;                     \
  points[0].y = y1;                     \
  points[1].x = x2;                     \
  points[1].y = y2;                     \
  points[2].x = x3;                     \
  points[2].y = y3;
  rect_init(r, 100, 100, 200, 200);
  canvas_begin_frame(&c, &r);

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
  ASSERT_EQ(lcd_log_get_commands(lcd), "dps((100,100)(200,200)(300,300));");

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
  font_manager_init(&font_manager);
  canvas_init(&c, lcd, &font_manager);
  font_dummy_init();
  font_manager_add(&font_manager, font_dummy_0("demo0", font_size));

  rect_init(r, 100, 100, 200, 200);
  canvas_begin_frame(&c, &r);
  canvas_set_font(&c, "demo0", font_size);

  lcd_log_reset(lcd);
  canvas_draw_char(&c, 0, 0, 0);
  ASSERT_EQ(lcd_log_get_commands(lcd), "");

  lcd_log_reset(lcd);
  canvas_draw_char(&c, 0, 0, 120);
  ASSERT_EQ(lcd_log_get_commands(lcd), "");

  lcd_log_reset(lcd);
  canvas_draw_char(&c, 0, 120, 320);
  ASSERT_EQ(lcd_log_get_commands(lcd), "");

  lcd_log_reset(lcd);
  canvas_draw_char(&c, 0, 330, 320);
  ASSERT_EQ(lcd_log_get_commands(lcd), "");

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
}

TEST(Canvas, draw_image) {
  rect_t r;
  rect_t s;
  rect_t d;
  canvas_t c;
  bitmap_t img;
  font_manager_t font_manager;
  font_manager_init(&font_manager);
  lcd_t* lcd = lcd_log_init(800, 600);
  canvas_init(&c, lcd, &font_manager);

  img.w = 32;
  img.h = 32;
  rect_init(r, 100, 100, 200, 200);
  canvas_begin_frame(&c, &r);

  lcd_log_reset(lcd);
  rect_init(s, 0, 0, img.w, img.h);
  rect_init(d, 0, 0, img.w, img.h);
  canvas_draw_image(&c, &img, &s, &d);
  ASSERT_EQ(lcd_log_get_commands(lcd), "");

  lcd_log_reset(lcd);
  rect_init(d, 90, 90, img.w, img.h);
  canvas_draw_image(&c, &img, &s, &d);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(10,10,22,22,100,100,22,22);");

  lcd_log_reset(lcd);
  rect_init(d, 100, 90, img.w, img.h);
  canvas_draw_image(&c, &img, &s, &d);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,10,32,22,100,100,32,22);");

  lcd_log_reset(lcd);
  rect_init(d, 290, 90, img.w, img.h);
  canvas_draw_image(&c, &img, &s, &d);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,10,10,22,290,100,10,22);");

  lcd_log_reset(lcd);
  rect_init(d, 290, 290, img.w, img.h);
  canvas_draw_image(&c, &img, &s, &d);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(0,0,10,10,290,290,10,10);");

  rect_init(s, 10, 10, 10, 10);

  lcd_log_reset(lcd);
  rect_init(d, 95, 95, 10, 10);
  canvas_draw_image(&c, &img, &s, &d);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(15,15,5,5,100,100,5,5);");

  lcd_log_reset(lcd);
  rect_init(d, 95, 100, 10, 10);
  canvas_draw_image(&c, &img, &s, &d);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(15,10,5,10,100,100,5,10);");

  lcd_log_reset(lcd);
  rect_init(d, 100, 100, 10, 10);
  canvas_draw_image(&c, &img, &s, &d);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(10,10,10,10,100,100,10,10);");

  lcd_log_reset(lcd);
  rect_init(d, 100, 295, 10, 10);
  canvas_draw_image(&c, &img, &s, &d);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(10,10,10,5,100,295,10,5);");

  lcd_log_reset(lcd);
  rect_init(d, 290, 290, 10, 10);
  canvas_draw_image(&c, &img, &s, &d);
  ASSERT_EQ(lcd_log_get_commands(lcd), "dg(10,10,10,10,290,290,10,10);");
  canvas_end_frame(&c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
}

