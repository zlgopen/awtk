#include "base/lcd.h"
#include "tkc/utils.h"
#include "gtest/gtest.h"

static rect_t s_dirty_rect;
static int32_t s_canvas_ptr = 0;
static uint32_t s_line_length = 0;

static ret_t lcd_log_set_line_length(lcd_t* lcd, uint32_t line_length) {
  s_line_length = line_length;
  return RET_OK;
}

static ret_t lcd_log_set_canvas(lcd_t* lcd, canvas_t* c) {
  s_canvas_ptr = tk_pointer_to_int(c);
  return RET_OK;
}

static ret_t lcd_log_begin_frame(lcd_t* lcd, const rect_t* dirty_rect) {
  s_dirty_rect.x = dirty_rect->x;
  s_dirty_rect.y = dirty_rect->y;
  s_dirty_rect.w = dirty_rect->w;
  s_dirty_rect.h = dirty_rect->h;
  return RET_OK;
}

TEST(lcd, base) {
  rect_t r;
  rect_t r1;
  lcd_t lcd;

  memset(&lcd, 0x0, sizeof(lcd_t));
  r = rect_init(100, 100, 200, 200);

  lcd.w = 800;
  lcd.h = 480;
  lcd.type = LCD_COMPOSITOR;
  lcd.set_canvas = lcd_log_set_canvas;
  lcd.begin_frame = lcd_log_begin_frame;
  lcd.set_line_length = lcd_log_set_line_length;

  ASSERT_EQ(lcd_is_compositor(&lcd) == TRUE, true);
  ASSERT_EQ(lcd_get_type(&lcd) == LCD_COMPOSITOR, true);
  ASSERT_EQ(lcd_set_line_length(&lcd, 100), RET_OK);
  ASSERT_EQ(s_line_length, 100);

  ASSERT_EQ(lcd_set_canvas(&lcd, (canvas_t*)tk_pointer_from_int(0xee)), RET_OK);
  ASSERT_EQ(s_canvas_ptr, 0xee);

  ASSERT_EQ(lcd_begin_frame(&lcd, &r, LCD_DRAW_OFFLINE), RET_OK);
  ASSERT_EQ(lcd_get_dirty_rect(&lcd, &r1), RET_OK);

  ASSERT_EQ(s_dirty_rect.x, r1.x);
  ASSERT_EQ(s_dirty_rect.y, r1.y);
  ASSERT_EQ(s_dirty_rect.w, r1.w);
  ASSERT_EQ(s_dirty_rect.h, r1.h);
}
