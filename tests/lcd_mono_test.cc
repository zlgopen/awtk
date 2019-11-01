#include "gtest/gtest.h"
#include "lcd/lcd_mono.h"
#include "tkc/mem.h"

#include <string>

using std::string;

static string s_log;
static ret_t lcd_log_flush(lcd_t* lcd) {
  return RET_OK;
}

TEST(LcdMono, draw_points) {
  uint32_t i = 0;
  uint32_t w = 128;
  uint32_t h = 64;

  lcd_t* lcd = lcd_mono_create(w, h, lcd_log_flush, NULL, NULL);

  for (i = 0; i < 10000; i++) {
    point_t p;
    p.x = random() % w;
    p.y = random() % h;

    color_t c = color_init(1, 0, 0, 0xff);

    lcd_set_stroke_color(lcd, c);
    ASSERT_EQ(lcd_draw_points(lcd, &p, 1), RET_OK);
    ASSERT_EQ(lcd_get_point_color(lcd, p.x, p.y).color, c.color);

    c = color_init(0, 0, 0, 0xff);

    lcd_set_stroke_color(lcd, c);
    ASSERT_EQ(lcd_draw_points(lcd, &p, 1), RET_OK);
    ASSERT_EQ(lcd_get_point_color(lcd, p.x, p.y).color, c.color);
  }

  lcd_destroy(lcd);
}

TEST(LcdMono, draw_hline) {
  uint32_t i = 0;
  uint32_t j = 0;
  uint32_t w = 128;
  uint32_t h = 64;

  uint32_t x = 1;
  uint32_t line_size = w - x;

  lcd_t* lcd = lcd_mono_create(w, h, lcd_log_flush, NULL, NULL);

  for (i = 0; i < h; i++) {
    color_t c = color_init(1, 0, 0, 0xff);

    lcd_set_stroke_color(lcd, c);
    ASSERT_EQ(lcd_draw_hline(lcd, x, i, line_size), RET_OK);
    for (j = 0; j < line_size; j++) {
      ASSERT_EQ(lcd_get_point_color(lcd, x + j, i).color, c.color);
    }
  }
  lcd_destroy(lcd);
}

TEST(LcdMono, draw_vline) {
  uint32_t i = 0;
  uint32_t j = 0;
  uint32_t w = 128;
  uint32_t h = 64;

  uint32_t y = 1;
  uint32_t line_size = h - y;

  lcd_t* lcd = lcd_mono_create(w, h, lcd_log_flush, NULL, NULL);

  for (i = 0; i < w; i++) {
    color_t c = color_init(1, 0, 0, 0xff);

    lcd_set_stroke_color(lcd, c);
    ASSERT_EQ(lcd_draw_vline(lcd, i, y, line_size), RET_OK);
    for (j = 0; j < line_size; j++) {
      ASSERT_EQ(lcd_get_point_color(lcd, i, y + j).color, c.color);
    }
  }
  lcd_destroy(lcd);
}

static void test_fill_rect(lcd_t* lcd, uint32_t x, uint32_t y, uint32_t w, uint32_t h, color_t c) {
  uint32_t i = 0;
  uint32_t j = 0;

  lcd_set_fill_color(lcd, c);
  ASSERT_EQ(lcd_fill_rect(lcd, x, y, w, h), RET_OK);

  for (j = 0; j < h; j++) {
    for (i = 0; i < w; i++) {
      ASSERT_EQ(lcd_get_point_color(lcd, x + i, y + j).color, c.color);
    }
  }
}

TEST(LcdMono, fill_rect) {
  uint32_t w = 128;
  uint32_t h = 64;
  color_t bg = color_init(0, 0, 0, 0xff);
  color_t fg = color_init(1, 0, 0, 0xff);
  lcd_t* lcd = lcd_mono_create(w, h, lcd_log_flush, NULL, NULL);

  test_fill_rect(lcd, 0, 0, w, h, fg);
  test_fill_rect(lcd, 0, 0, w, h, bg);
  test_fill_rect(lcd, 0, 0, 1, 1, bg);
  test_fill_rect(lcd, 0, 0, 1, 1, fg);
  test_fill_rect(lcd, 0, 0, 1, 1, bg);
  test_fill_rect(lcd, 0, 0, 4, 3, fg);
  test_fill_rect(lcd, 0, 0, 4, 3, bg);

  lcd_destroy(lcd);
}

static void gen_data(uint8_t* data, uint32_t w, uint32_t h) {
  uint32_t x = 0;
  uint32_t y = 0;
  uint32_t i = 0;

  for (i = 0; i < w * h; i++) {
    x = random() % w;
    y = random() % h;

    ASSERT_EQ(bitmap_mono_set_pixel((uint8_t*)(data), w, h, x, y, FALSE), RET_OK);
    ASSERT_EQ(bitmap_mono_get_pixel(data, w, h, x, y), FALSE);

    ASSERT_EQ(bitmap_mono_set_pixel((uint8_t*)(data), w, h, x, y, TRUE), RET_OK);
    ASSERT_EQ(bitmap_mono_get_pixel(data, w, h, x, y), TRUE);
  }

  return;
}

static void lcd_check_data(lcd_t* lcd, uint8_t* data, uint32_t x, uint32_t y, uint32_t w,
                           uint32_t h) {
  uint32_t i = 0;
  uint32_t j = 0;
  for (j = 0; j < h; j++) {
    for (i = 0; i < w; i++) {
      color_t gc = color_init(bitmap_mono_get_pixel(data, w, h, i, j), 0, 0, 0xff);
      color_t c = lcd_get_point_color(lcd, x + i, y + j);

      ASSERT_EQ(gc.color, c.color);
    }
  }
}

static void glyph_gen(glyph_t* g, uint32_t w, uint32_t h) {
  memset(g, 0x00, sizeof(glyph_t));

  g->w = w;
  g->h = h;
  g->data = bitmap_mono_create_data(w, h);
  gen_data((uint8_t*)(g->data), w, h);

  return;
}

static void test_draw_glyph(lcd_t* lcd, uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
  glyph_t g;
  glyph_gen(&g, w, h);
  rect_t r = rect_init(0, 0, w, h);
  color_t c = color_init(0xff, 0xff, 0xff, 0xff);

  lcd_set_text_color(lcd, c);
  ASSERT_EQ(lcd_draw_glyph(lcd, &g, &r, x, y), RET_OK);
  lcd_check_data(lcd, (uint8_t*)(g.data), x, y, w, h);

  TKMEM_FREE(g.data);
}

TEST(LcdMono, draw_glyph) {
  uint32_t w = 128;
  uint32_t h = 64;
  lcd_t* lcd = lcd_mono_create(w, h, lcd_log_flush, NULL, NULL);

  test_draw_glyph(lcd, 0, 0, 2, 2);
  test_draw_glyph(lcd, 0, 0, 10, 10);
  test_draw_glyph(lcd, 1, 0, 10, 10);
  test_draw_glyph(lcd, 1, 1, 10, 10);
  test_draw_glyph(lcd, 10, 10, 10, 10);
  test_draw_glyph(lcd, 10, 10, 4, 4);

  lcd_destroy(lcd);
}

static void bitmap_gen(bitmap_t* b, uint32_t w, uint32_t h) {
  uint8_t* data = NULL;
  memset(b, 0x00, sizeof(bitmap_t));

  b->w = w;
  b->h = h;
  b->format = BITMAP_FMT_MONO;
  data = bitmap_mono_create_data(w, h);

  gen_data((uint8_t*)(data), w, h);
  b->buffer = GRAPHIC_BUFFER_CREATE_WITH_DATA(data, w, h, BITMAP_FMT_MONO);

  return;
}

static void test_draw_bitmap(lcd_t* lcd, uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
  bitmap_t b;
  bitmap_gen(&b, w, h);
  rect_t r = rect_init(0, 0, w, h);
  rect_t d = rect_init(x, y, w, h);
  uint8_t* bdata = bitmap_lock_buffer_for_write(&b);
  ASSERT_EQ(lcd_draw_image(lcd, &b, &r, &d), RET_OK);
  lcd_check_data(lcd, (uint8_t*)(bdata), x, y, w, h);
  bitmap_unlock_buffer(&b);
  graphic_buffer_destroy(b.buffer);
}

TEST(LcdMono, draw_bitmap) {
  uint32_t w = 128;
  uint32_t h = 64;
  lcd_t* lcd = lcd_mono_create(w, h, lcd_log_flush, NULL, NULL);

  test_draw_bitmap(lcd, 0, 0, 2, 2);
  test_draw_bitmap(lcd, 0, 0, 10, 10);
  test_draw_bitmap(lcd, 1, 0, 10, 10);
  test_draw_bitmap(lcd, 1, 1, 10, 10);
  test_draw_bitmap(lcd, 10, 10, 10, 10);
  test_draw_bitmap(lcd, 10, 10, 4, 4);

  lcd_destroy(lcd);
}
