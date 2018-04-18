#include "lcd/lcd_mem.h"
#include "base/canvas.h"
#include "gtest/gtest.h"

TEST(LCDMem, draw_points) {
  int i = 0;
  canvas_t canvas;
  color_t color;
  font_manager_t font_manager;
  font_manager_init(&font_manager);
  point_t points[] = {{0, 0}, {100, 100},{100, 101},{100, 102}, {100, 103}, {199, 199}};
  color_t stroke_color = color_init(0xff, 0x0, 0x0, 0xff);
  lcd_t* lcd = lcd_mem_create(200, 200, TRUE);
  canvas_t* c = canvas_init(&canvas, lcd, &font_manager);

  ASSERT_EQ(c, &canvas);
  ASSERT_EQ(canvas_begin_frame(c, NULL), RET_OK);
  ASSERT_EQ(canvas_set_stroke_color(c, stroke_color), RET_OK);
  ASSERT_EQ(canvas_draw_points(c, points, ARRAY_SIZE(points)), RET_OK);
  
  for(i = 0; i < ARRAY_SIZE(points); i++) {
    point_t* p = points+i;
    color = lcd_get_point_color(c->lcd, p->x, p->y); 
    ASSERT_EQ(color.color, stroke_color.color);
    
    color = lcd_get_point_color(c->lcd, p->x+10, p->y); 
    ASSERT_EQ(color.color, 0);
  }

  lcd_destroy(lcd);
}

