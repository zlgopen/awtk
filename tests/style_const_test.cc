#include "gtest/gtest.h"
#include "base/button.h"
#include "base/window.h"
#include "base/style_const.h"

TEST(StyleConst, basic) {
  widget_t* w = window_create(NULL, 10, 20, 30, 40);
  widget_t* b = button_create(w, 0, 0, 10, 10);
  style_t* s = style_const_create(b);

  ASSERT_EQ(s != NULL, true);

  style_destroy(s);
  widget_destroy(w);
}
