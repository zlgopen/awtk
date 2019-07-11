#include "widgets/tab_button_group.h"
#include "gtest/gtest.h"

TEST(TabButtonGroup, cast) {
  widget_t* w = tab_button_group_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, tab_button_group_cast(w));
  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_COMPACT, FALSE), FALSE);
  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_SCROLLABLE, FALSE), FALSE);

  ASSERT_EQ(widget_set_prop_bool(w, WIDGET_PROP_COMPACT, TRUE), FALSE);
  ASSERT_EQ(widget_set_prop_bool(w, WIDGET_PROP_SCROLLABLE, TRUE), FALSE);

  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_COMPACT, FALSE), TRUE);
  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_SCROLLABLE, FALSE), TRUE);

  widget_destroy(w);
}
