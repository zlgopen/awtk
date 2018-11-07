#include "base/canvas.h"
#include "gtest/gtest.h"
#include "lcd/lcd_mem_bgr888.h"
#include "lcd/lcd_mem_bgra8888.h"
#include "lcd/lcd_mem_rgba8888.h"

static void test_draw_points(canvas_t* c) {
  int i = 0;
  color_t color;
  color_t stroke_color = color_init(0xff, 0x0, 0x0, 0xff);
  point_t points[] = {{0, 0}, {100, 100}, {100, 101}, {100, 102}, {100, 103}, {199, 199}};

  ASSERT_EQ(canvas_set_stroke_color(c, stroke_color), RET_OK);
  ASSERT_EQ(canvas_draw_points(c, points, ARRAY_SIZE(points)), RET_OK);

  for (i = 0; i < ARRAY_SIZE(points); i++) {
    point_t* p = points + i;
    memset(&color, 0x00, sizeof(color));
    color = lcd_get_point_color(c->lcd, p->x, p->y);
    ASSERT_EQ(color.color, stroke_color.color);
  }
}

static void test_fill_rect(canvas_t* c) {
  color_t color;
  lcd_t* lcd = c->lcd;
  color_t fill_color = color_init(0xff, 0xff, 0x0, 0xff);

  ASSERT_EQ(canvas_set_fill_color(c, fill_color), RET_OK);
  ASSERT_EQ(canvas_fill_rect(c, 0, 0, lcd->w, lcd->h), RET_OK);
  color = lcd_get_point_color(c->lcd, 0, 0);
  ASSERT_EQ(color.color, fill_color.color);

  color = lcd_get_point_color(c->lcd, 10, 10);
  ASSERT_EQ(color.color, fill_color.color);

  color = lcd_get_point_color(c->lcd, 199, 199);
  ASSERT_EQ(color.color, fill_color.color);
}

static void test_stroke_rect(canvas_t* c) {
  color_t color;
  lcd_t* lcd = c->lcd;
  color_t stroke_color = color_init(0xff, 0x0, 0x0, 0xff);
  ASSERT_EQ(canvas_set_stroke_color(c, stroke_color), RET_OK);

  ASSERT_EQ(canvas_stroke_rect(c, 0, 0, lcd->w, lcd->h), RET_OK);
  color = lcd_get_point_color(c->lcd, 0, 0);
  ASSERT_EQ(color.color, stroke_color.color);

  color = lcd_get_point_color(c->lcd, 0, 199);
  ASSERT_EQ(color.color, stroke_color.color);

  color = lcd_get_point_color(c->lcd, 199, 0);
  ASSERT_EQ(color.color, stroke_color.color);

  color = lcd_get_point_color(c->lcd, 199, 199);
  ASSERT_EQ(color.color, stroke_color.color);
}

static void test_draw(canvas_t* c) {
  ASSERT_EQ(canvas_begin_frame(c, NULL, LCD_DRAW_NORMAL), RET_OK);
  test_draw_points(c);
  test_fill_rect(c);
  test_stroke_rect(c);
}

TEST(LCDMem, rgba8888) {
  canvas_t canvas;
  font_manager_t font_manager;
  font_manager_init(&font_manager, NULL);
  lcd_t* lcd = lcd_mem_rgba8888_create(200, 200, TRUE);
  canvas_t* c = canvas_init(&canvas, lcd, &font_manager);

  test_draw(c);

  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
}

TEST(LCDMem, bgra8888) {
  canvas_t canvas;
  font_manager_t font_manager;
  font_manager_init(&font_manager, NULL);
  lcd_t* lcd = lcd_mem_bgra8888_create(200, 200, TRUE);
  canvas_t* c = canvas_init(&canvas, lcd, &font_manager);

  test_draw(c);

  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
}

TEST(LCDMem, bgr888) {
  canvas_t canvas;
  font_manager_t font_manager;
  font_manager_init(&font_manager, NULL);
  lcd_t* lcd = lcd_mem_bgr888_create(200, 200, TRUE);
  canvas_t* c = canvas_init(&canvas, lcd, &font_manager);

  test_draw(c);

  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
}

TEST(LCDMem, resize) {
  lcd_t* lcd = lcd_mem_bgra8888_create(100, 200, TRUE);
  lcd_mem_t* mem = (lcd_mem_t*)lcd;

  lcd_resize(lcd, 200, 100, 0);
  ASSERT_EQ(lcd->w, 200);
  ASSERT_EQ(lcd->h, 100);
  ASSERT_EQ(mem->line_length, 200 * 4);

  lcd_destroy(lcd);
}
