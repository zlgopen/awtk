#include "keyboard/keyboard.h"
#include "gtest/gtest.h"

TEST(KeyBoard, basic) {
  value_t v1;
  value_t v2;
  widget_t* b = keyboard_create(NULL, 10, 20, 30, 40);

  value_set_str(&v1, "bottom_to_top");
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_OPEN_ANIM_HINT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_OPEN_ANIM_HINT, &v2), RET_OK);
  ASSERT_EQ(strcmp(v1.value.str, v2.value.str), 0);

  value_set_str(&v1, "fade");
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_CLOSE_ANIM_HINT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_CLOSE_ANIM_HINT, &v2), RET_OK);
  ASSERT_EQ(strcmp(v1.value.str, v2.value.str), 0);

  widget_destroy(b);
}

TEST(KeyBoard, cast) {
  widget_t* b = keyboard_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(b, keyboard_cast(b));
  ASSERT_EQ(b, window_base_cast(b));

  widget_destroy(b);
}

TEST(KeyBoard, is_keyboard) {
  widget_t* b = keyboard_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_is_keyboard(b), TRUE);

  widget_destroy(b);
}
