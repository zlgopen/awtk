#include "widgets/popup.h"
#include "widgets/button.h"
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

TEST(Popup, auto_scale_children_x) {
  widget_t* w = popup_create(NULL, 0, 0, 400, 600);
  widget_t* b = button_create(w, 10, 20, 30, 40);

  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_DESIGN_W, 0), 0);
  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_DESIGN_W, 200), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_DESIGN_W, 0), 200);

  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_DESIGN_H, 0), 0);
  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_DESIGN_H, 300), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_DESIGN_H, 0), 300);

  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_AUTO_SCALE_CHILDREN_X, TRUE), FALSE);
  ASSERT_EQ(widget_set_prop_bool(w, WIDGET_PROP_AUTO_SCALE_CHILDREN_X, TRUE), RET_OK);
  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_AUTO_SCALE_CHILDREN_X, FALSE), TRUE);

  widget_dispatch_simple_event(w, EVT_WINDOW_LOAD);
  ASSERT_EQ(b->x, 20);
  ASSERT_EQ(b->y, 20);
  ASSERT_EQ(b->w, 30);
  ASSERT_EQ(b->h, 40);

  widget_destroy(w);
}

TEST(Popup, auto_scale_children_xy) {
  widget_t* w = popup_create(NULL, 0, 0, 400, 600);
  widget_t* b = button_create(w, 10, 20, 30, 40);

  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_DESIGN_W, 0), 0);
  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_DESIGN_W, 200), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_DESIGN_W, 0), 200);

  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_DESIGN_H, 0), 0);
  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_DESIGN_H, 300), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_DESIGN_H, 0), 300);

  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_AUTO_SCALE_CHILDREN_X, TRUE), FALSE);
  ASSERT_EQ(widget_set_prop_bool(w, WIDGET_PROP_AUTO_SCALE_CHILDREN_X, TRUE), RET_OK);
  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_AUTO_SCALE_CHILDREN_X, FALSE), TRUE);

  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_AUTO_SCALE_CHILDREN_Y, TRUE), FALSE);
  ASSERT_EQ(widget_set_prop_bool(w, WIDGET_PROP_AUTO_SCALE_CHILDREN_Y, TRUE), RET_OK);
  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_AUTO_SCALE_CHILDREN_Y, FALSE), TRUE);

  widget_dispatch_simple_event(w, EVT_WINDOW_LOAD);
  ASSERT_EQ(b->x, 20);
  ASSERT_EQ(b->y, 40);
  ASSERT_EQ(b->w, 30);
  ASSERT_EQ(b->h, 40);

  widget_destroy(w);
}

TEST(Popup, auto_scale_children_xywh) {
  widget_t* w = popup_create(NULL, 0, 0, 400, 600);
  widget_t* b = button_create(w, 10, 20, 30, 40);

  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_DESIGN_W, 0), 0);
  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_DESIGN_W, 200), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_DESIGN_W, 0), 200);

  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_DESIGN_H, 0), 0);
  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_DESIGN_H, 300), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_DESIGN_H, 0), 300);

  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_AUTO_SCALE_CHILDREN_X, TRUE), FALSE);
  ASSERT_EQ(widget_set_prop_bool(w, WIDGET_PROP_AUTO_SCALE_CHILDREN_X, TRUE), RET_OK);
  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_AUTO_SCALE_CHILDREN_X, FALSE), TRUE);

  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_AUTO_SCALE_CHILDREN_Y, TRUE), FALSE);
  ASSERT_EQ(widget_set_prop_bool(w, WIDGET_PROP_AUTO_SCALE_CHILDREN_Y, TRUE), RET_OK);
  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_AUTO_SCALE_CHILDREN_Y, FALSE), TRUE);

  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_AUTO_SCALE_CHILDREN_W, TRUE), FALSE);
  ASSERT_EQ(widget_set_prop_bool(w, WIDGET_PROP_AUTO_SCALE_CHILDREN_W, TRUE), RET_OK);
  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_AUTO_SCALE_CHILDREN_W, FALSE), TRUE);

  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_AUTO_SCALE_CHILDREN_H, TRUE), FALSE);
  ASSERT_EQ(widget_set_prop_bool(w, WIDGET_PROP_AUTO_SCALE_CHILDREN_H, TRUE), RET_OK);
  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_AUTO_SCALE_CHILDREN_H, FALSE), TRUE);

  widget_dispatch_simple_event(w, EVT_WINDOW_LOAD);
  ASSERT_EQ(b->x, 20);
  ASSERT_EQ(b->y, 40);
  ASSERT_EQ(b->w, 60);
  ASSERT_EQ(b->h, 80);

  widget_destroy(w);
}
