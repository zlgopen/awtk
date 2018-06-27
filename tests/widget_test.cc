
#include "base/canvas.h"
#include "base/widget.h"
#include "base/button.h"
#include "base/group_box.h"
#include "base/window.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include <stdlib.h>
#include "gtest/gtest.h"

TEST(Widget, moveresize) {
  widget_t* w = window_create(NULL, 0, 0, 400, 300);

  ASSERT_EQ(w->x, 0);
  ASSERT_EQ(w->y, 0);
  ASSERT_EQ(widget_move(w, 100, 200), RET_OK);
  ASSERT_EQ(w->x, 100);
  ASSERT_EQ(w->y, 200);

  ASSERT_EQ(widget_resize(w, 300, 400), RET_OK);
  ASSERT_EQ(w->w, 300);
  ASSERT_EQ(w->h, 400);

  ASSERT_EQ(widget_set_state(w, WIDGET_STATE_PRESSED), RET_OK);
  ASSERT_EQ(w->state, WIDGET_STATE_PRESSED);

  ASSERT_EQ(widget_set_enable(w, TRUE), RET_OK);
  ASSERT_EQ(w->enable, TRUE);

  ASSERT_EQ(widget_set_enable(w, FALSE), RET_OK);
  ASSERT_EQ(w->enable, FALSE);

  ASSERT_EQ(widget_set_visible(w, TRUE, TRUE), RET_OK);
  ASSERT_EQ(w->visible, TRUE);

  ASSERT_EQ(widget_set_visible(w, FALSE, TRUE), RET_OK);
  ASSERT_EQ(w->visible, FALSE);

  ASSERT_EQ(widget_set_focused(w, FALSE), RET_OK);
  ASSERT_EQ(w->focused, FALSE);

  widget_destroy(w);
}

TEST(Widget, props) {
  value_t v1;
  value_t v2;
  widget_t* w = window_create(NULL, 0, 0, 400, 300);

  value_set_str(&v1, "window");
  ASSERT_EQ(widget_set_prop(w, "name", &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, "name", &v2), RET_OK);
  ASSERT_EQ(v1.type, v2.type);
  ASSERT_EQ(strcmp(v1.value.str, v2.value.str), 0);

  value_set_int(&v1, 1);
  ASSERT_EQ(widget_set_prop(w, "x", &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, "x", &v2), RET_OK);
  ASSERT_EQ(v1.type, v2.type);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 2);
  ASSERT_EQ(widget_set_prop(w, "y", &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, "y", &v2), RET_OK);
  ASSERT_EQ(v1.type, v2.type);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 3);
  ASSERT_EQ(widget_set_prop(w, "w", &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, "w", &v2), RET_OK);
  ASSERT_EQ(v1.type, v2.type);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 4);
  ASSERT_EQ(widget_set_prop(w, "h", &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, "h", &v2), RET_OK);
  ASSERT_EQ(v1.type, v2.type);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  ASSERT_EQ(widget_set_prop(w, "hh", &v1), RET_NOT_FOUND);
  ASSERT_EQ(widget_get_prop(w, "hh", &v2), RET_NOT_FOUND);
  widget_destroy(w);
}

TEST(Widget, children) {
  widget_t* w = window_create(NULL, 0, 0, 400, 300);
  widget_t* c1 = button_create(w, 0, 0, 10, 10);
  widget_t* c2 = button_create(w, 0, 0, 10, 10);
  widget_t* c3 = button_create(w, 0, 0, 10, 10);
  widget_t* c4 = button_create(w, 0, 0, 10, 10);
  widget_t* c5 = button_create(w, 0, 0, 10, 10);
  widget_t* c6 = button_create(w, 0, 0, 10, 10);
  widget_t* group = group_box_create(w, 0, 0, 10, 10);

  ASSERT_EQ(widget_set_name(c1, "c1"), RET_OK);
  ASSERT_EQ(widget_set_name(c2, "c2"), RET_OK);
  ASSERT_EQ(widget_set_name(c3, "c3"), RET_OK);
  ASSERT_EQ(widget_set_name(c4, "c4"), RET_OK);
  ASSERT_EQ(widget_set_name(c5, "c5"), RET_OK);
  ASSERT_EQ(widget_set_name(c6, "c6"), RET_OK);
  ASSERT_EQ(widget_set_name(group, "group"), RET_OK);

  ASSERT_EQ(widget_lookup(w, "c1", FALSE), c1);
  ASSERT_EQ(widget_lookup(w, "c2", FALSE), c2);
  ASSERT_EQ(widget_lookup(w, "c3", FALSE), c3);

  ASSERT_EQ(widget_lookup(w, "c1", TRUE), c1);
  ASSERT_EQ(widget_lookup(w, "c2", TRUE), c2);
  ASSERT_EQ(widget_lookup(w, "c3", TRUE), c3);

  ASSERT_EQ(widget_lookup(w, "c4", TRUE), c4);
  ASSERT_EQ(widget_lookup(w, "c5", TRUE), c5);
  ASSERT_EQ(widget_lookup(w, "c6", TRUE), c6);

  widget_move_resize(c1, 10, 10, 20, 20);
  ASSERT_EQ(widget_find_target(w, 11, 11), c1);

  widget_move_resize(c2, 150, 10, 20, 20);
  ASSERT_EQ(widget_find_target(w, 151, 11), c2);

  widget_move_resize(c3, 150, 100, 20, 20);
  ASSERT_EQ(widget_find_target(w, 151, 100), c3);

  widget_destroy(w);
}

#ifdef WITH_VGCANVAS_LCD
TEST(Widget, rotation) {
  widget_t* w = button_create(NULL, 0, 0, 400, 300);
  ASSERT_EQ(w->rotation, 0);
  ASSERT_EQ(widget_set_rotation(w, 0.4), RET_OK);
  ASSERT_EQ(w->rotation, 0.4f);
  widget_destroy(w);
}

TEST(Widget, scale) {
  widget_t* w = button_create(NULL, 0, 0, 400, 300);
  ASSERT_EQ(w->scale_x, 1.0f);
  ASSERT_EQ(w->scale_y, 1.0f);

  ASSERT_EQ(widget_set_scale(w, 0.4, 4.0), RET_OK);
  ASSERT_EQ(w->scale_x, 0.4f);
  ASSERT_EQ(w->scale_y, 4.0f);

  widget_destroy(w);
}

TEST(Widget, anchor) {
  widget_t* w = button_create(NULL, 0, 0, 400, 300);
  ASSERT_EQ(w->anchor_x, 0.5f);
  ASSERT_EQ(w->anchor_y, 0.5f);

  ASSERT_EQ(widget_set_anchor(w, 0.1f, 1.0f), RET_OK);
  ASSERT_EQ(w->anchor_x, 0.1f);
  ASSERT_EQ(w->anchor_y, 1.0f);

  widget_destroy(w);
}
#endif /*WITH_VGCANVAS_LCD*/
