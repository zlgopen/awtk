#include "widgets/overlay.h"
#include "gtest/gtest.h"

TEST(Overlay, cast) {
  widget_t* w = overlay_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, overlay_cast(w));

  widget_destroy(w);
}

TEST(Overlay, props) {
  widget_t* w = overlay_create(NULL, 10, 20, 30, 40);
  overlay_t* overlay = OVERLAY(w);

  ASSERT_EQ(overlay->click_through, FALSE);
  widget_set_prop_bool(w, WIDGET_PROP_CLICK_THROUGH, TRUE);
  ASSERT_EQ(overlay->click_through, TRUE);
  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_CLICK_THROUGH, FALSE), TRUE);

  ASSERT_EQ(overlay->always_on_top, FALSE);
  widget_set_prop_bool(w, WIDGET_PROP_ALWAYS_ON_TOP, TRUE);
  ASSERT_EQ(overlay->always_on_top, TRUE);
  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_ALWAYS_ON_TOP, FALSE), TRUE);

  widget_destroy(w);
}
