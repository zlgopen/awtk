#include "gtest/gtest.h"
#include "widgets/button.h"
#include "base/window.h"
#include "base/style_const.h"
#include "widgets/combo_box_item.h"
#include <string>

using std::string;

TEST(StyleConst, basic) {
  widget_t* w = window_create(NULL, 10, 20, 30, 40);
  widget_t* b = button_create(w, 0, 0, 10, 10);
  style_t* s = style_const_create(b);

  ASSERT_EQ(s != NULL, true);

  style_destroy(s);
  widget_destroy(w);
}

TEST(StyleConst, combo_box_item) {
  const char* icon = NULL;
  widget_t* w = window_create(NULL, 10, 20, 30, 40);
  widget_t* b = combo_box_item_create(w, 0, 0, 10, 10);
  style_t* s = style_const_create(b);

  ASSERT_EQ(style_is_mutable(s), FALSE);
  style_notify_widget_state_changed(s, b);
  icon = style_get_str(s, STYLE_ID_ICON, NULL);
  ASSERT_EQ(string(icon), "empty");

  combo_box_item_set_checked(b, TRUE);
  style_notify_widget_state_changed(s, b);
  icon = style_get_str(s, STYLE_ID_ICON, NULL);
  ASSERT_EQ(string(icon), "check");

  style_destroy(s);
  widget_destroy(w);
}
