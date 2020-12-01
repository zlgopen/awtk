#include "widgets/popup.h"
#include "gtest/gtest.h"

TEST(Popup, cast) {
  widget_t* w = popup_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, popup_cast(w));
  ASSERT_EQ(w, window_base_cast(w));

  widget_destroy(w);
}

TEST(Popup, basic) {
  widget_t* w = popup_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_CLOSE_WHEN_CLICK_OUTSIDE, TRUE), FALSE);
  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_CLOSE_WHEN_CLICK, TRUE), FALSE);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_CLOSE_WHEN_TIMEOUT, 1000), 0);

  ASSERT_EQ(widget_set_prop_bool(w, WIDGET_PROP_CLOSE_WHEN_CLICK_OUTSIDE, TRUE), RET_OK);
  ASSERT_EQ(widget_set_prop_bool(w, WIDGET_PROP_CLOSE_WHEN_CLICK, TRUE), RET_OK);
  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_CLOSE_WHEN_TIMEOUT, 1000), RET_OK);

  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_CLOSE_WHEN_CLICK_OUTSIDE, FALSE), TRUE);
  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_CLOSE_WHEN_CLICK, FALSE), TRUE);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_CLOSE_WHEN_TIMEOUT, 1000), 1000);

  widget_destroy(w);
}
