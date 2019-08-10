#include "gtest/gtest.h"
#include "lcd/lcd_mono.h"

#include <string>

using std::string;

static string s_log;
static ret_t lcd_log_flush(lcd_t* lcd) {
  return RET_OK;
}

TEST(View, draw_points) {
  uint32_t i = 0;
  uint32_t w = 128;
  uint32_t h = 64;

  lcd_t* lcd = lcd_mono_create(128, 64, lcd_log_flush);

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
