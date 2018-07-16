#include "base/image.h"
#include "gtest/gtest.h"
#include "base/canvas.h"
#include "base/font_manager.h"
#include "font_dummy.h"
#include "lcd_log.h"

TEST(Image, basic) {
  value_t v;
  value_t v1;
  widget_t* img = image_create(NULL, 0, 0, 100, 100);
  value_set_str(&v, "repeat");

  ASSERT_EQ(widget_set_prop(img, WIDGET_PROP_DRAW_TYPE, &v), RET_OK);
  ASSERT_EQ(IMAGE(img)->draw_type, IMAGE_DRAW_REPEAT);

  ASSERT_EQ(image_set_draw_type(img, IMAGE_DRAW_REPEAT_X), RET_OK);
  ASSERT_EQ(IMAGE(img)->draw_type, IMAGE_DRAW_REPEAT_X);

  value_set_str(&v, "repeat_y");
  ASSERT_EQ(widget_set_prop(img, WIDGET_PROP_DRAW_TYPE, &v), RET_OK);
  ASSERT_EQ(IMAGE(img)->draw_type, IMAGE_DRAW_REPEAT_Y);

  value_set_str(&v, "earth");
  ASSERT_EQ(widget_set_prop(img, WIDGET_PROP_IMAGE, &v), RET_OK);
  ASSERT_EQ(widget_get_prop(img, WIDGET_PROP_IMAGE, &v1), RET_OK);
  ASSERT_EQ(strcmp(value_str(&v), value_str(&v1)), 0);

  widget_destroy(img);
}
