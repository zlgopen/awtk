#include <string>
#include "gtest/gtest.h"
#include "widgets/button.h"
#include "widgets/check_button.h"
#include "widgets/group_box.h"

using std::string;

static ret_t on_changed(void* ctx, event_t* e) {
  int* count = (int*)ctx;
  if (e->type == EVT_VALUE_CHANGED) {
    *count += 1;
  }

  return RET_OK;
}

TEST(CheckButton, basic) {
  uint32_t id = 0;
  int count = 0;
  widget_t* button = check_button_create(NULL, 0, 0, 100, 100);

  ASSERT_EQ(CHECK_BUTTON(button)->radio, FALSE);
  ASSERT_EQ(CHECK_BUTTON(button)->value, FALSE);

  id = widget_on(button, EVT_VALUE_CHANGED, on_changed, &count);
  ASSERT_EQ(id > 0, TRUE);

  ASSERT_EQ(check_button_set_value(button, TRUE), RET_OK);
  ASSERT_EQ(CHECK_BUTTON(button)->value, TRUE);
  ASSERT_EQ(count, 1);
  ASSERT_EQ(check_button_set_value(button, FALSE), RET_OK);
  ASSERT_EQ(CHECK_BUTTON(button)->value, FALSE);
  ASSERT_EQ(count, 2);

  ASSERT_EQ(widget_off(button, id), RET_OK);

  ASSERT_EQ(check_button_set_value(button, TRUE), RET_OK);
  ASSERT_EQ(CHECK_BUTTON(button)->value, TRUE);
  ASSERT_EQ(count, 2);
  ASSERT_EQ(check_button_set_value(button, FALSE), RET_OK);
  ASSERT_EQ(CHECK_BUTTON(button)->value, FALSE);
  ASSERT_EQ(count, 2);

  widget_destroy(button);
}

TEST(CheckButton, radio) {
  widget_t* button = check_button_create_radio(NULL, 0, 0, 100, 100);

  ASSERT_EQ(check_button_cast(button), button);
  ASSERT_EQ(CHECK_BUTTON(button)->radio, TRUE);
  ASSERT_EQ(CHECK_BUTTON(button)->value, FALSE);

  widget_destroy(button);
}

#include "log_change_events.inc"

TEST(CheckButton, event) {
  widget_t* w = check_button_create_radio(NULL, 0, 0, 100, 100);

  check_button_set_value(w, TRUE);

  s_log = "";
  widget_on(w, EVT_VALUE_WILL_CHANGE, on_change_events, NULL);
  widget_on(w, EVT_VALUE_CHANGED, on_change_events, NULL);

  check_button_set_value(w, TRUE);
  ASSERT_EQ(s_log, "");

  check_button_set_value(w, FALSE);
  ASSERT_EQ(s_log, "will_change;change;");

  widget_destroy(w);
}

TEST(CheckButton, radio_cast) {
  widget_t* w = check_button_create_radio(NULL, 0, 0, 100, 100);
  ASSERT_EQ(w, check_button_cast(w));
  widget_destroy(w);
}

TEST(CheckButton, check_cast) {
  widget_t* w = check_button_create(NULL, 0, 0, 100, 100);
  ASSERT_EQ(w, check_button_cast(w));
  ASSERT_NE(w, button_cast(w));
  widget_destroy(w);
}

TEST(CheckButton, radio1) {
  widget_t* g = group_box_create(NULL, 0, 0, 100, 100);
  widget_t* b1 = check_button_create_radio(g, 0, 0, 100, 100);
  widget_t* b2 = check_button_create_radio(g, 0, 0, 100, 100);
  widget_t* b3 = check_button_create_radio(g, 0, 0, 100, 100);

  ASSERT_EQ(check_button_set_value(b3, TRUE), RET_OK);
  ASSERT_EQ(check_button_get_checked_button(b1), b3);
  ASSERT_EQ(check_button_get_checked_button(b2), b3);
  ASSERT_EQ(check_button_get_checked_button(b3), b3);

  ASSERT_EQ(check_button_set_value(b1, TRUE), RET_OK);
  ASSERT_EQ(check_button_get_checked_button(b1), b1);
  ASSERT_EQ(check_button_get_checked_button(b2), b1);
  ASSERT_EQ(check_button_get_checked_button(b3), b1);

  ASSERT_EQ(check_button_set_value(b2, TRUE), RET_OK);
  ASSERT_EQ(check_button_get_checked_button(b1), b2);
  ASSERT_EQ(check_button_get_checked_button(b2), b2);
  ASSERT_EQ(check_button_get_checked_button(b3), b2);

  widget_destroy(g);
}
