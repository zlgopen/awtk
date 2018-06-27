#include "gtest/gtest.h"
#include "base/check_button.h"

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
}

TEST(CheckButton, radio) {
  widget_t* button = check_button_create_radio(NULL, 0, 0, 100, 100);

  ASSERT_EQ(CHECK_BUTTON(button)->radio, TRUE);
  ASSERT_EQ(CHECK_BUTTON(button)->value, FALSE);
}
