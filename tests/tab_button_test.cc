#include <string>
#include "gtest/gtest.h"
#include "widgets/tab_button.h"
#include "widgets/tab_button_group.h"

TEST(TabButton, basic) {
  value_t v1;
  value_t v2;
  widget_t* g = tab_button_group_create(NULL, 0, 0, 100, 100);
  widget_t* w = tab_button_create(g, 0, 0, 100, 100);

  ASSERT_EQ(tab_button_set_value(w, TRUE), RET_OK);
  ASSERT_EQ(TAB_BUTTON(w)->value, TRUE);

  ASSERT_EQ(tab_button_set_value(w, FALSE), RET_OK);
  ASSERT_EQ(TAB_BUTTON(w)->value, FALSE);

  value_set_str(&v1, "icon");
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_ICON, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_ICON, &v2), RET_OK);
  ASSERT_EQ(strcmp(v1.value.str, v2.value.str), 0);

  value_set_str(&v1, "active_icon");
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_ACTIVE_ICON, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_ACTIVE_ICON, &v2), RET_OK);
  ASSERT_EQ(strcmp(v1.value.str, v2.value.str), 0);

  value_set_str(&v1, "tab_button_view_page1");
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_LOAD_UI, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_LOAD_UI, &v2), RET_OK);
  ASSERT_EQ(strcmp(v1.value.str, v2.value.str), 0);

  widget_destroy(g);
}

#include "log_change_events.inc"

TEST(TabButton, event) {
  widget_t* g = tab_button_group_create(NULL, 0, 0, 100, 100);
  widget_t* w = tab_button_create(g, 0, 0, 100, 100);

  tab_button_set_value(w, TRUE);

  s_log = "";
  widget_on(w, EVT_VALUE_WILL_CHANGE, on_change_events, NULL);
  widget_on(w, EVT_VALUE_CHANGED, on_change_events, NULL);

  tab_button_set_value(w, TRUE);
  ASSERT_EQ(s_log, "");

  tab_button_set_value(w, FALSE);
  ASSERT_EQ(s_log, "will_change;change;");

  widget_destroy(g);
}

TEST(TabButton, cast) {
  widget_t* g = tab_button_group_create(NULL, 0, 0, 100, 100);
  widget_t* w = tab_button_create(g, 0, 0, 100, 100);

  ASSERT_EQ(w, tab_button_cast(w));
  ASSERT_NE(w, tab_button_group_cast(w));

  widget_destroy(g);
}
