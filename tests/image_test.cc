#include "widgets/image.h"
#include "gtest/gtest.h"
#include "base/canvas.h"
#include "base/font_manager.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "ui_loader/ui_serializer.h"

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

TEST(Image, rotation) {
  image_t* w = IMAGE(image_create(NULL, 0, 0, 400, 300));
  image_base_t* image_base = IMAGE_BASE(w);

  ASSERT_EQ(image_base->rotation, 0);
  ASSERT_EQ(image_set_rotation(WIDGET(w), 0.4), RET_OK);
  ASSERT_EQ(image_base->rotation, 0.4f);
  widget_destroy(WIDGET(w));
}

TEST(Image, scale) {
  image_t* w = IMAGE(image_create(NULL, 0, 0, 400, 300));
  image_base_t* image_base = IMAGE_BASE(w);

  ASSERT_EQ(image_base->scale_x, 1.0f);
  ASSERT_EQ(image_base->scale_y, 1.0f);

  ASSERT_EQ(image_set_scale(WIDGET(w), 0.4, 4.0), RET_OK);
  ASSERT_EQ(image_base->scale_x, 0.4f);
  ASSERT_EQ(image_base->scale_y, 4.0f);

  widget_destroy(WIDGET(w));
}

TEST(Image, anchor) {
  image_t* w = IMAGE(image_create(NULL, 0, 0, 400, 300));
  image_base_t* image_base = IMAGE_BASE(w);

  ASSERT_EQ(image_base->anchor_x, 0.5f);
  ASSERT_EQ(image_base->anchor_y, 0.5f);

  ASSERT_EQ(image_set_anchor(WIDGET(w), 0.1f, 1.0f), RET_OK);
  ASSERT_EQ(image_base->anchor_x, 0.1f);
  ASSERT_EQ(image_base->anchor_y, 1.0f);

  widget_destroy(WIDGET(w));
}

TEST(Image, animator) {
  widget_t* w = (image_create(NULL, 0, 0, 400, 300));

  ASSERT_EQ(widget_create_animator(w, "move(y_from=0, y_to=128)"), RET_OK);
  ASSERT_EQ(widget_start_animator(w, "move"), RET_OK);
  ASSERT_EQ(widget_pause_animator(w, "move"), RET_OK);
  ASSERT_EQ(widget_find_animator(w, "move") != NULL, TRUE);
  ASSERT_EQ(widget_destroy_animator(w, "move"), RET_OK);

  ASSERT_EQ(widget_find_animator(w, "move") == NULL, TRUE);

  widget_destroy((w));
}

TEST(Image, cast) {
  widget_t* w = image_create(NULL, 0, 0, 400, 300);

  ASSERT_EQ(w, image_cast(w));
  ASSERT_EQ(w, image_base_cast(w));

  widget_destroy(w);
}

TEST(Image, clone) {
  str_t str;
  widget_t* w2 = NULL;
  widget_t* w1 = image_create(NULL, 0, 0, 400, 300);

  ASSERT_EQ(image_set_draw_type(w1, IMAGE_DRAW_REPEAT_X), RET_OK);
  ASSERT_EQ(widget_set_prop_str(w1, WIDGET_PROP_IMAGE, "earth"), RET_OK);

  str_init(&str, 0);
  w2 = widget_clone(w1, NULL);
  log_debug("==================================\n");
  widget_to_xml(w1, &str);
  log_debug("w1:%s\n", str.str);

  str_set(&str, "");
  widget_to_xml(w2, &str);
  log_debug("w2:%s\n", str.str);
  log_debug("==================================\n");

  ASSERT_EQ(widget_equal(w1, w2), TRUE);

  widget_destroy(w1);
  widget_destroy(w2);
  str_reset(&str);
}
