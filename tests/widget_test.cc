
#include "base/canvas.h"
#include "base/widget.h"
#include "base/button.h"
#include "base/label.h"
#include "base/group_box.h"
#include "base/window.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include <stdlib.h>
#include <string.h>
#include "gtest/gtest.h"

using std::string;

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

  widget_destroy(w);
}

TEST(Widget, custom_props) {
  value_t v1;
  value_t v2;
  widget_t* w = button_create(NULL, 0, 0, 400, 300);

  value_set_int(&v1, 123);
  ASSERT_EQ(widget_set_prop(w, "binding.value", &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, "binding.value", &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 1234);
  ASSERT_EQ(widget_set_prop(w, "binding.value", &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, "binding.value", &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_str(&v1, "1234");
  ASSERT_EQ(widget_set_prop(w, "binding.value", &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, "binding.value", &v2), RET_OK);
  ASSERT_EQ(strcmp(value_str(&v1), value_str(&v2)), 0);

  value_set_str(&v1, "1234");
  ASSERT_EQ(widget_set_prop(w, "binding.text", &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, "binding.text", &v2), RET_OK);
  ASSERT_EQ(strcmp(value_str(&v1), value_str(&v2)), 0);

  ASSERT_EQ(widget_get_prop(w, "hh", &v2), RET_NOT_FOUND);

  widget_destroy(w);
}

TEST(Widget, props_default) {
  value_t v1;
  widget_t* w = window_create(NULL, 0, 0, 400, 300);

  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_LAYOUT_W, &v1), RET_OK);
  ASSERT_EQ(value_int(&v1), w->w);

  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_LAYOUT_H, &v1), RET_OK);
  ASSERT_EQ(value_int(&v1), w->h);

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

static string s_event_log;

static ret_t on_button_events(void* ctx, event_t* e) {
  (void)ctx;
  switch (e->type) {
    case EVT_MOVE: {
      s_event_log += "move ";
      break;
    }
    case EVT_WILL_MOVE: {
      s_event_log += "will_move ";
      break;
    }
    case EVT_RESIZE: {
      s_event_log += "resize ";
      break;
    }
    case EVT_WILL_RESIZE: {
      s_event_log += "will_resize ";
      break;
    }
    case EVT_MOVE_RESIZE: {
      s_event_log += "move_resize ";
      break;
    }
    case EVT_WILL_MOVE_RESIZE: {
      s_event_log += "will_move_resize ";
      break;
    }
    case EVT_PROP_WILL_CHANGE: {
      prop_change_event_t* evt = (prop_change_event_t*)e;

      s_event_log += "prop_will_change ";
      s_event_log += evt->name;
      s_event_log += value_str(evt->value);
      s_event_log += " ";
      break;
    }
    case EVT_PROP_CHANGED: {
      prop_change_event_t* evt = (prop_change_event_t*)e;

      s_event_log += "prop_changed ";
      s_event_log += evt->name;
      s_event_log += value_str(evt->value);
      s_event_log += " ";
      break;
    } break;
  }
  return RET_OK;
}

TEST(Widget, move) {
  widget_t* w = button_create(NULL, 0, 0, 0, 0);

  s_event_log = "";
  widget_on(w, EVT_MOVE, on_button_events, NULL);
  widget_on(w, EVT_WILL_MOVE, on_button_events, NULL);

  widget_move(w, 100, 200);
  ASSERT_EQ(w->x, 100);
  ASSERT_EQ(w->y, 200);
  ASSERT_EQ(s_event_log, string("will_move move "));

  widget_destroy(w);
}

TEST(Widget, resize) {
  widget_t* w = button_create(NULL, 0, 0, 0, 0);

  s_event_log = "";
  widget_on(w, EVT_RESIZE, on_button_events, NULL);
  widget_on(w, EVT_WILL_RESIZE, on_button_events, NULL);

  widget_resize(w, 100, 200);
  ASSERT_EQ(w->w, 100);
  ASSERT_EQ(w->h, 200);
  ASSERT_EQ(s_event_log, string("will_resize resize "));

  widget_destroy(w);
}

TEST(Widget, move_resize) {
  widget_t* w = button_create(NULL, 0, 0, 0, 0);

  s_event_log = "";
  widget_on(w, EVT_MOVE_RESIZE, on_button_events, NULL);
  widget_on(w, EVT_WILL_MOVE_RESIZE, on_button_events, NULL);

  widget_move_resize(w, 100, 200, 300, 400);
  ASSERT_EQ(w->x, 100);
  ASSERT_EQ(w->y, 200);
  ASSERT_EQ(w->w, 300);
  ASSERT_EQ(w->h, 400);
  ASSERT_EQ(s_event_log, string("will_move_resize move_resize "));

  widget_destroy(w);
}

TEST(Widget, prop) {
  value_t v;
  widget_t* w = button_create(NULL, 0, 0, 0, 0);

  widget_on(w, EVT_PROP_WILL_CHANGE, on_button_events, NULL);
  widget_on(w, EVT_PROP_CHANGED, on_button_events, NULL);

  s_event_log = "";
  value_set_str(&v, "123");
  ASSERT_EQ(widget_set_prop(w, "name", &v), RET_OK);
  ASSERT_EQ(s_event_log, string("prop_will_change name123 prop_changed name123 "));

  widget_destroy(w);
}

TEST(Widget, dirty) {
  widget_t* w = window_create(NULL, 0, 0, 400, 300);
  widget_t* b1 = button_create(w, 0, 0, 100, 100);
  widget_invalidate(w, NULL);
  ASSERT_EQ(w->dirty, TRUE);
  ASSERT_EQ(b1->dirty, TRUE);
  widget_destroy_children(w);
  ASSERT_EQ(w->children->size, 0);

  widget_destroy(w);
}

TEST(Widget, grab) {
  widget_t* w = window_create(NULL, 0, 0, 400, 300);
  widget_t* b1 = button_create(w, 0, 0, 100, 100);
  widget_t* l1 = label_create(w, 0, 0, 10, 10);
  widget_t* l2 = label_create(w, 0, 0, 10, 10);
  widget_grab(b1, l1);
  ASSERT_EQ(b1->grab_widget, l1);
  ASSERT_EQ(w->grab_widget, b1);
  widget_ungrab(b1, l2);
  ASSERT_EQ(b1->grab_widget, l1);
  ASSERT_EQ(w->grab_widget, b1);

  widget_grab(b1, l2);
  ASSERT_EQ(b1->grab_widget, l2);
  ASSERT_EQ(w->grab_widget, b1);

  widget_destroy(w);
}
