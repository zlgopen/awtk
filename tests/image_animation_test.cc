#include "base/canvas.h"
#include "base/widget.h"
#include "base/layout.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include "image_animation/image_animation.h"

#include <stdlib.h>
#include <string>
using std::string;

TEST(ImageAnimation, basic) {
  value_t v1;
  value_t v2;
  widget_t* w = image_animation_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_set_text(w, L"OK"), RET_OK);
  ASSERT_EQ(wcscmp(w->text.str, L"OK"), 0);
  ASSERT_EQ(widget_set_text(w, L"Cancel"), RET_OK);
  ASSERT_EQ(wcscmp(w->text.str, L"Cancel"), 0);

  value_set_str(&v1, "image");
  ASSERT_EQ(widget_set_prop(w, IMAGE_ANIMATION_PROP_IMAGE, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, IMAGE_ANIMATION_PROP_IMAGE, &v2), RET_OK);
  ASSERT_EQ(string(v1.value.str), string(v2.value.str));

  value_set_str(&v1, "1234");
  ASSERT_EQ(widget_set_prop(w, IMAGE_ANIMATION_PROP_SEQUENCE, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, IMAGE_ANIMATION_PROP_SEQUENCE, &v2), RET_OK);
  ASSERT_EQ(string(v1.value.str), string(v2.value.str));

  value_set_int(&v1, 200);
  ASSERT_EQ(widget_set_prop(w, IMAGE_ANIMATION_PROP_INTERVAL, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, IMAGE_ANIMATION_PROP_INTERVAL, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_bool(&v1, TRUE);
  ASSERT_EQ(widget_set_prop(w, IMAGE_ANIMATION_PROP_LOOP, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, IMAGE_ANIMATION_PROP_LOOP, &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));

  value_set_bool(&v1, TRUE);
  ASSERT_EQ(widget_set_prop(w, IMAGE_ANIMATION_PROP_AUTO_PLAY, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, IMAGE_ANIMATION_PROP_AUTO_PLAY, &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));

  widget_destroy(w);
}

TEST(ImageAnimation, clone) {
  value_t v1;

  widget_t* w1 = image_animation_create(NULL, 10, 20, 30, 40);

  value_set_int(&v1, 200);
  ASSERT_EQ(widget_set_prop(w1, IMAGE_ANIMATION_PROP_INTERVAL, &v1), RET_OK);
  widget_set_self_layout_params(w1, "1", "2", "3", "4");
  widget_set_children_layout_params(w1, "r0 c0 x10, y10, s10");
  ASSERT_EQ(image_animation_cast(w1), w1);

  widget_t* w2 = widget_clone(w1, NULL);
  ASSERT_EQ(widget_equal(w1, w2), TRUE);
  widget_destroy(w1);
  widget_destroy(w2);
}
