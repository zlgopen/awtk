#include "base/canvas.h"
#include "base/pixel.h"
#include "gtest/gtest.h"
#include "tkc/mem.h"
#include "lcd/lcd_mem_bgr565.h"
#include "lcd/lcd_mem_rgb565.h"
#include "lcd/lcd_mem_bgr888.h"
#include "lcd/lcd_mem_rgb888.h"
#include "lcd/lcd_mem_bgra8888.h"
#include "lcd/lcd_mem_rgba8888.h"

static void test_draw_points(canvas_t* c) {
  uint32_t i = 0;
  color_t color;
  color_t stroke_color = color_init(0xf8, 0x0, 0x0, 0xff);
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
  color_t fill_color = color_init(0xf8, 0x0, 0x0, 0xff);

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
  color_t stroke_color = color_init(0xf8, 0x0, 0x0, 0xff);
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

  canvas_reset(c);
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

  canvas_reset(c);
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

  canvas_reset(c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
}

TEST(LCDMem, rgb888) {
  canvas_t canvas;
  font_manager_t font_manager;
  font_manager_init(&font_manager, NULL);
  lcd_t* lcd = lcd_mem_rgb888_create(200, 200, TRUE);
  canvas_t* c = canvas_init(&canvas, lcd, &font_manager);

  test_draw(c);

  canvas_reset(c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
}

TEST(LCDMem, bgr565) {
  canvas_t canvas;
  font_manager_t font_manager;
  font_manager_init(&font_manager, NULL);
  lcd_t* lcd = lcd_mem_bgr565_create(200, 200, TRUE);
  canvas_t* c = canvas_init(&canvas, lcd, &font_manager);

  test_draw(c);

  canvas_reset(c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
}

TEST(LCDMem, rgb565) {
  canvas_t canvas;
  font_manager_t font_manager;
  font_manager_init(&font_manager, NULL);
  lcd_t* lcd = lcd_mem_rgb565_create(200, 200, TRUE);
  canvas_t* c = canvas_init(&canvas, lcd, &font_manager);

  test_draw(c);

  canvas_reset(c);
  font_manager_deinit(&font_manager);
  lcd_destroy(lcd);
}

TEST(LCDMem, resize) {
  lcd_t* lcd = lcd_mem_bgra8888_create(100, 200, TRUE);
  lcd_mem_t* mem = (lcd_mem_t*)lcd;

  lcd_resize(lcd, 200, 100, 0);
  ASSERT_EQ(lcd->w, 200);
  ASSERT_EQ(lcd->h, 100);
  ASSERT_EQ(mem->line_length, 200u * 4u);

  lcd_destroy(lcd);
}

TEST(LCDMem, blend_pixel_alpha_saturate) {
  uint32_t x = 0;
  uint32_t y = 0;
  uint32_t w = 4;
  uint32_t h = 4;
  color_t color;
  point_t point = {1, 1};
  lcd_t* lcd = lcd_mem_bgra8888_create(w, h, TRUE);
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  uint8_t* fbuff = lcd_mem_get_offline_fb(mem);
  uint32_t line_length = mem->line_length;

  ASSERT_EQ(line_length, w * (uint32_t)pixel_bgra8888_BPP);

  /* 目标像素的 alpha 为 0xf4，画笔的 alpha 为 0xf8，合成后的 alpha 为 0x100，必须饱和到 0xff */
  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++) {
      pixel_bgra8888_t* p = (pixel_bgra8888_t*)(fbuff + y * line_length + x * pixel_bgra8888_BPP);
      p->b = 200;
      p->g = 150;
      p->r = 100;
      p->a = 0xf4;
    }
  }

  ASSERT_EQ(lcd_set_stroke_color(lcd, color_init(10, 20, 30, 0xf8)), RET_OK);
  ASSERT_EQ(lcd_draw_points(lcd, &point, 1), RET_OK);

  color = lcd_get_point_color(lcd, point.x, point.y);
  ASSERT_EQ(color.rgba.r, 12);
  ASSERT_EQ(color.rgba.g, 22);
  ASSERT_EQ(color.rgba.b, 33);
  ASSERT_EQ(color.rgba.a, 0xff);

  /* 其它的像素保持不变 */
  color = lcd_get_point_color(lcd, 0, 0);
  ASSERT_EQ(color.rgba.r, 100);
  ASSERT_EQ(color.rgba.g, 150);
  ASSERT_EQ(color.rgba.b, 200);
  ASSERT_EQ(color.rgba.a, 0xf4);

  lcd_destroy(lcd);
}
