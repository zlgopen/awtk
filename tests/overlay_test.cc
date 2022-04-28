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

  ASSERT_EQ(widget_is_overlay(w), TRUE);
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

TEST(Overlay, copy) {
  widget_t* w = overlay_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_set_prop_bool(w, WIDGET_PROP_STRONGLY_FOCUS, TRUE), RET_OK);
  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_DESIGN_W, 400), RET_OK);
  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_DESIGN_H, 300), RET_OK);
  ASSERT_EQ(widget_set_prop_str(w, WIDGET_PROP_THEME, "hello"), RET_OK);
  ASSERT_EQ(widget_set_prop_str(w, WIDGET_PROP_ANIM_HINT, "popup"), RET_OK);
  widget_t* w1 = widget_clone(w, NULL);

  ASSERT_EQ(widget_get_prop_bool(w1, WIDGET_PROP_STRONGLY_FOCUS, FALSE), TRUE);
  ASSERT_EQ(widget_get_prop_int(w1, WIDGET_PROP_DESIGN_W, 0), 400);
  ASSERT_EQ(widget_get_prop_int(w1, WIDGET_PROP_DESIGN_H, 0), 300);
  ASSERT_STREQ(widget_get_prop_str(w1, WIDGET_PROP_THEME, NULL), "hello");
  ASSERT_STREQ(widget_get_prop_str(w1, WIDGET_PROP_ANIM_HINT, NULL), "popup");

  widget_destroy(w);
  widget_destroy(w1);
}
