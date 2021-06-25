#include "tkc/str.h"
#include "widgets/button.h"
#include "layer_window/layer.h"
#include "gtest/gtest.h"
#include "lcd_log.h"

TEST(Layer, basic) {
  canvas_t c;
  lcd_t* lcd = lcd_log_init(800, 600);
  canvas_init(&c, lcd, font_manager());
  layer_t* layer = layer_create("foo", &c, 10, 20, 60);

  ASSERT_EQ(layer->x, 10);
  ASSERT_EQ(layer->y, 20);
  ASSERT_EQ(layer->max_fps, 60);
  ASSERT_STREQ(layer->name, "foo");
  ASSERT_EQ(layer->canvas, &c);

  OBJECT_UNREF(layer);
}

TEST(Layer, dirty_rect) {
  canvas_t c;
  rect_t r = rect_init(10, 20, 30, 40);
  lcd_t* lcd = lcd_log_init(800, 600);
  canvas_init(&c, lcd, font_manager());
  layer_t* layer = layer_create("foo", &c, 10, 20, 60);

  ASSERT_EQ(layer_invalidate(layer, &r), RET_OK);
  ASSERT_EQ(layer->dirty_rects.max.w, 30);
  ASSERT_EQ(layer->dirty_rects.max.h, 40);
  
  r = rect_init(10, 20, 40, 50);
  ASSERT_EQ(layer_invalidate(layer, &r), RET_OK);
  ASSERT_EQ(layer->dirty_rects.max.w, 40);
  ASSERT_EQ(layer->dirty_rects.max.h, 50);

  ASSERT_EQ(layer_paint(layer), RET_OK);
  ASSERT_EQ(layer->dirty_rects.max.w, 0);
  ASSERT_EQ(layer->dirty_rects.max.h, 0);

  OBJECT_UNREF(layer);
}

TEST(Layer, widgets) {
  canvas_t c;
  rect_t r = rect_init(10, 20, 30, 40);
  widget_t* b1 = button_create(NULL, 0, 0, 100, 30);
  widget_t* b2 = button_create(NULL, 0, 40, 100, 30);
  lcd_t* lcd = lcd_log_init(800, 600);
  canvas_init(&c, lcd, font_manager());
  layer_t* layer = layer_create("foo", &c, 10, 20, 60);

  ASSERT_EQ(layer_invalidate(layer, &r), RET_OK);
  ASSERT_EQ(layer->dirty_rects.max.w, 30);
  ASSERT_EQ(layer->dirty_rects.max.h, 40);
  
  ASSERT_EQ(layer_add_widget(layer, b1), RET_OK);
  ASSERT_EQ(layer->widgets.size , 1);
  ASSERT_EQ(layer_add_widget(layer, b1), RET_OK);
  ASSERT_EQ(layer->widgets.size , 1);
  ASSERT_EQ(layer_add_widget(layer, b1), RET_OK);
  ASSERT_EQ(layer->widgets.size , 1);
  
  ASSERT_EQ(layer_add_widget(layer, b2), RET_OK);
  ASSERT_EQ(layer->widgets.size , 2);
  
  ASSERT_EQ(layer_remove_widget(layer, b2), RET_OK);
  ASSERT_EQ(layer->widgets.size , 1);
  
  ASSERT_EQ(layer_remove_widget(layer, b1), RET_OK);
  ASSERT_EQ(layer->widgets.size , 0);
  
  ASSERT_EQ(layer_add_widget(layer, b1), RET_OK);
  ASSERT_EQ(layer_add_widget(layer, b2), RET_OK);
  ASSERT_EQ(layer->widgets.size , 2);

  widget_destroy(b1);
  idle_dispatch();
  ASSERT_EQ(layer->widgets.size , 1);
  widget_destroy(b2);
  idle_dispatch();
  ASSERT_EQ(layer->widgets.size , 0);

  ASSERT_EQ(layer_paint(layer), RET_OK);
  ASSERT_EQ(layer->dirty_rects.max.w, 0);
  ASSERT_EQ(layer->dirty_rects.max.h, 0);

  OBJECT_UNREF(layer);
}

