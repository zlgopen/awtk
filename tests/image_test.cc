#include "base/image.h"
#include "gtest/gtest.h"
#include "base/canvas.h"
#include "base/font_manager.h"
#include "font_dummy.h"
#include "lcd_log.h"

TEST(Image, basic) {
  value_t v;
  widget_t* img = image_create(NULL, 0, 0, 100, 100);
  value_set_str(&v, "repeat");
  ASSERT_EQ(widget_set_prop(img, "not exist", &v), RET_NOT_FOUND);
  ASSERT_EQ(widget_set_prop(img, "draw_type", &v), RET_OK);
  ASSERT_EQ(IMAGE(img)->draw_type, IMAGE_DRAW_REPEAT);

  ASSERT_EQ(image_set_draw_type(img, IMAGE_DRAW_REPEAT_X), RET_OK);
  ASSERT_EQ(IMAGE(img)->draw_type, IMAGE_DRAW_REPEAT_X);

  value_set_str(&v, "earth");
  ASSERT_EQ(widget_set_prop(img, "image", &v), RET_OK);
}
