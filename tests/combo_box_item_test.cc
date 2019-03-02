#include "gtest/gtest.h"
#include "widgets/combo_box_item.h"

#include <string>

using std::string;

TEST(ComboBoxItem, basic) {
  value_t v1;
  value_t v2;
  widget_t* w = combo_box_item_create(NULL, 10, 20, 30, 40);

  value_set_int(&v1, 3);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_VALUE, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_VALUE, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_bool(&v1, true);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_CHECKED, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_CHECKED, &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));
  ASSERT_EQ(combo_box_item_cast(w), w);

  widget_destroy(w);
}

TEST(ComboBoxItem, value) {
  widget_t* w = combo_box_item_create(NULL, 10, 20, 30, 40);
  combo_box_item_t* item = COMBO_BOX_ITEM(w);

  combo_box_item_set_checked(w, TRUE);
  ASSERT_EQ(item->checked, TRUE);

  combo_box_item_set_value(w, 123);
  ASSERT_EQ(item->value, 123);

  widget_destroy(w);
}

#include "log_change_events.inc"

TEST(ComboBoxItem, event) {
  widget_t* w = combo_box_item_create(NULL, 0, 0, 100, 100);

  combo_box_item_set_checked(w, TRUE);

  s_log = "";
  widget_on(w, EVT_VALUE_WILL_CHANGE, on_change_events, NULL);
  widget_on(w, EVT_VALUE_CHANGED, on_change_events, NULL);

  combo_box_item_set_checked(w, TRUE);
  ASSERT_EQ(s_log, "");

  combo_box_item_set_checked(w, FALSE);
  ASSERT_EQ(s_log, "will_change;change;");

  widget_destroy(w);
}

TEST(ComboBoxItem, cast) {
  widget_t* w = combo_box_item_create(NULL, 0, 0, 100, 100);

  ASSERT_EQ(w, combo_box_item_cast(w));

  widget_destroy(w);
}
