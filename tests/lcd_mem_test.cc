#include "lcd/lcd_mem.h"
#include "base/canvas.h"
#include "gtest/gtest.h"

static void test_draw_points(canvas_t* c) {
  int i = 0;
  color_t color;
  color_t stroke_color = color_init(0xff, 0x0, 0x0, 0xff);
  point_t points[] = {{0, 0}, {100, 100},{100, 101},{100, 102}, {100, 103}, {199, 199}};
  
  ASSERT_EQ(canvas_set_stroke_color(c, stroke_color), RET_OK);
  ASSERT_EQ(canvas_draw_points(c, points, ARRAY_SIZE(points)), RET_OK);
  
  for(i = 0; i < ARRAY_SIZE(points); i++) {
    point_t* p = points+i;
    color = lcd_get_point_color(c->lcd, p->x, p->y); 
    ASSERT_EQ(color.color, stroke_color.color);
    
    color = lcd_get_point_color(c->lcd, p->x+10, p->y); 
    ASSERT_EQ(color.color, 0);
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

TEST(LCDMem, basic) {
  canvas_t canvas;
  font_manager_t font_manager;
  font_manager_init(&font_manager);
  lcd_t* lcd = lcd_mem_create(200, 200, TRUE);
  canvas_t* c = canvas_init(&canvas, lcd, &font_manager);

  ASSERT_EQ(c, &canvas);
  ASSERT_EQ(canvas_begin_frame(c, NULL), RET_OK);

  test_draw_points(c);
  test_fill_rect(c);
  test_stroke_rect(c);

  lcd_destroy(lcd);
}

