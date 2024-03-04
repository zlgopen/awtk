#include "widgets/group_box.h"
#include "widgets/check_button.h"
#include "gtest/gtest.h"

TEST(GroupBox, cast) {
  widget_t* w = group_box_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, group_box_cast(w));

  widget_destroy(w);
  idle_dispatch();
}

static ret_t on_value_changed(void* ctx, event_t* e) {
  uint32_t* value = (uint32_t*)ctx;
  value_change_event_t* evt = (value_change_event_t*)e;
  *value = value_uint32(&(evt->new_value));

  return RET_OK;
}

TEST(GroupBox, radio_button) {
  uint32_t value1 = 0;
  uint32_t value2 = 0;
  widget_t* w = group_box_create(NULL, 10, 20, 30, 40);
  widget_t* r0 = check_button_create_radio(w, 10, 20, 30, 40);
  widget_t* r1 = check_button_create_radio(w, 10, 20, 30, 40);
  widget_t* r2 = check_button_create_radio(w, 10, 30, 30, 40);
  widget_t* r3 = check_button_create_radio(w, 10, 40, 30, 40);
  group_box_t* group_box = GROUP_BOX(w);

  widget_on(w, EVT_VALUE_WILL_CHANGE, on_value_changed, &value1);
  widget_on(w, EVT_VALUE_CHANGED, on_value_changed, &value2);

  widget_set_value_int(w, 0);
  ASSERT_EQ(widget_get_value_int(r0), TRUE);
  ASSERT_EQ(value1, 0);
  ASSERT_EQ(value2, 0);
  ASSERT_EQ(group_box->value, 0);

  widget_set_value_int(w, 1);
  ASSERT_EQ(widget_get_value_int(r1), TRUE);
  ASSERT_EQ(value1, 1);
  ASSERT_EQ(value2, 1);
  ASSERT_EQ(group_box->value, 1);

  widget_set_value_int(w, 2);
  ASSERT_EQ(widget_get_value_int(r2), TRUE);
  ASSERT_EQ(value1, 2);
  ASSERT_EQ(value2, 2);
  ASSERT_EQ(group_box->value, 2);

  widget_set_value_int(w, 3);
  ASSERT_EQ(widget_get_value_int(r3), TRUE);
  ASSERT_EQ(value1, 3);
  ASSERT_EQ(value2, 3);
  ASSERT_EQ(group_box->value, 3);

  widget_set_value_int(r0, 1);
  ASSERT_EQ(value1, 0);
  ASSERT_EQ(value2, 0);
  ASSERT_EQ(group_box->value, 0);

  widget_set_value_int(r3, 1);
  ASSERT_EQ(value1, 3);
  ASSERT_EQ(value2, 3);
  ASSERT_EQ(group_box->value, 3);

  widget_destroy(w);
  idle_dispatch();
}
