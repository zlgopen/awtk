#include "widgets/view.h"
#include "gtest/gtest.h"

TEST(View, cast) {
  widget_t* w = view_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, view_cast(w));

  widget_destroy(w);
}

TEST(View, props) {
  widget_t* w = view_create(NULL, 10, 20, 30, 40);
  view_t* view = VIEW(w);
  widget_set_prop_str(w, WIDGET_PROP_DEFAULT_FOCUSED_CHILD, "aaa");
  ASSERT_STREQ(view->default_focused_child, "aaa");
  ASSERT_STREQ(widget_get_prop_str(w, WIDGET_PROP_DEFAULT_FOCUSED_CHILD, NULL), "aaa");

  widget_destroy(w);
}
