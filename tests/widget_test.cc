
#include "base/canvas.h"
#include "base/widget.h"
#include "widgets/button.h"
#include "widgets/label.h"
#include "widgets/group_box.h"
#include "widgets/window.h"
#include "base/style_const.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include <stdlib.h>
#include <string.h>
#include "gtest/gtest.h"

using std::string;

TEST(Widget, basic1) {
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

TEST(Widget, basic2) {
  value_t t;
  value_t f;
  widget_t* w = window_create(NULL, 0, 0, 400, 300);

  value_set_str(&t, "true");
  value_set_str(&f, "false");

  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_ENABLE, &t), RET_OK);
  ASSERT_EQ(w->enable, TRUE);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_ENABLE, &f), RET_OK);
  ASSERT_EQ(w->enable, FALSE);

  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_VISIBLE, &t), RET_OK);
  ASSERT_EQ(w->visible, TRUE);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_VISIBLE, &f), RET_OK);
  ASSERT_EQ(w->visible, FALSE);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_X, 11), RET_OK);
  ASSERT_EQ(w->x, 11);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_X, 0), 11);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_Y, 22), RET_OK);
  ASSERT_EQ(w->y, 22);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_Y, 0), 22);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_W, 33), RET_OK);
  ASSERT_EQ(w->w, 33);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_W, 0), 33);

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_H, 44), RET_OK);
  ASSERT_EQ(w->h, 44);
  ASSERT_EQ(widget_get_prop_int(w, WIDGET_PROP_H, 0), 44);

  ASSERT_EQ(widget_set_prop_str(w, WIDGET_PROP_STYLE, "default"), RET_OK);
  ASSERT_EQ(strcmp(w->style, "default"), 0);
  ASSERT_EQ(strcmp(widget_get_prop_str(w, WIDGET_PROP_STYLE, ""), "default"), 0);

  ASSERT_EQ(widget_set_prop_str(w, WIDGET_PROP_NAME, "name"), RET_OK);
  ASSERT_EQ(strcmp(w->name, "name"), 0);
  ASSERT_EQ(strcmp(widget_get_prop_str(w, WIDGET_PROP_NAME, ""), "name"), 0);

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

TEST(Widget, index_of) {
  widget_t* w = window_create(NULL, 0, 0, 400, 300);
  widget_t* b1 = button_create(w, 0, 0, 100, 100);
  widget_t* l1 = label_create(w, 0, 0, 10, 10);
  widget_t* l2 = label_create(w, 0, 0, 10, 10);
  widget_t* l3 = label_create(w, 0, 0, 10, 10);
  ASSERT_EQ(widget_index_of(b1), 0);
  ASSERT_EQ(widget_index_of(l1), 1);
  ASSERT_EQ(widget_index_of(l2), 2);
  ASSERT_EQ(widget_index_of(l3), 3);

  widget_destroy(w);
}

static ret_t dummy_on_timer(const timer_info_t* info) {
  return RET_REPEAT;
}

TEST(Widget, timer) {
  widget_t* w = button_create(NULL, 0, 0, 400, 300);
  uint32_t id = widget_add_timer(w, dummy_on_timer, 100);
  ASSERT_EQ(timer_find(id)->id, id);

  widget_destroy(w);

  ASSERT_EQ(timer_find(id), (timer_info_t*)NULL);
}

TEST(Widget, restack1) {
  widget_t* w = window_create(NULL, 0, 0, 400, 300);
  widget_t* b1 = button_create(w, 1, 0, 10, 20);

  ASSERT_EQ(widget_restack(b1, 0), RET_OK);
  ASSERT_EQ(widget_restack(b1, 1), RET_OK);
  ASSERT_EQ(widget_restack(b1, -1), RET_OK);

  widget_destroy(w);
}

TEST(Widget, restack2) {
  widget_t* w = window_create(NULL, 0, 0, 400, 300);
  widget_t* b1 = button_create(w, 1, 0, 10, 20);
  widget_t* b2 = button_create(w, 2, 0, 10, 20);

  ASSERT_EQ(widget_index_of(b1), 0);
  ASSERT_EQ(widget_index_of(b2), 1);

  ASSERT_EQ(widget_restack(b1, -1), RET_OK);
  ASSERT_EQ(widget_index_of(b1), 1);
  ASSERT_EQ(widget_index_of(b2), 0);

  ASSERT_EQ(widget_restack(b2, 1), RET_OK);
  ASSERT_EQ(widget_index_of(b2), 1);
  ASSERT_EQ(widget_index_of(b1), 0);

  widget_destroy(w);
}

TEST(Widget, restack3) {
  widget_t* w = window_create(NULL, 0, 0, 400, 300);
  widget_t* b1 = button_create(w, 1, 0, 10, 20);
  widget_t* b2 = button_create(w, 2, 0, 10, 20);
  widget_t* b3 = button_create(w, 3, 0, 10, 20);

  ASSERT_EQ(widget_index_of(b1), 0);
  ASSERT_EQ(widget_index_of(b2), 1);
  ASSERT_EQ(widget_index_of(b3), 2);

  ASSERT_EQ(widget_restack(b1, -1), RET_OK);
  ASSERT_EQ(widget_index_of(b2), 0);
  ASSERT_EQ(widget_index_of(b3), 1);
  ASSERT_EQ(widget_index_of(b1), 2);

  ASSERT_EQ(widget_restack(b1, 0), RET_OK);
  ASSERT_EQ(widget_index_of(b1), 0);
  ASSERT_EQ(widget_index_of(b2), 1);
  ASSERT_EQ(widget_index_of(b3), 2);

  ASSERT_EQ(widget_restack(b1, 1), RET_OK);
  ASSERT_EQ(widget_index_of(b1), 1);
  ASSERT_EQ(widget_index_of(b2), 0);
  ASSERT_EQ(widget_index_of(b3), 2);

  ASSERT_EQ(widget_restack(b1, 2), RET_OK);
  ASSERT_EQ(widget_index_of(b1), 2);
  ASSERT_EQ(widget_index_of(b2), 0);
  ASSERT_EQ(widget_index_of(b3), 1);

  ASSERT_EQ(widget_restack(b1, -1), RET_OK);
  ASSERT_EQ(widget_index_of(b1), 2);
  ASSERT_EQ(widget_index_of(b2), 0);
  ASSERT_EQ(widget_index_of(b3), 1);

  widget_destroy(w);
}

TEST(Widget, insert) {
  widget_t* w = window_create(NULL, 0, 0, 400, 300);
  widget_t* b1 = button_create(w, 1, 0, 10, 20);
  widget_t* b2 = button_create(w, 2, 0, 10, 20);
  widget_t* b3 = button_create(w, 3, 0, 10, 20);
  widget_t* b4 = button_create(NULL, 3, 0, 10, 20);
  widget_t* b5 = button_create(NULL, 3, 0, 10, 20);
  widget_t* b6 = button_create(NULL, 3, 0, 10, 20);

  ASSERT_EQ(widget_insert_child(w, 0, b4), RET_OK);
  ASSERT_EQ(widget_index_of(b4), 0);

  ASSERT_EQ(widget_insert_child(w, 1, b5), RET_OK);

  ASSERT_EQ(widget_index_of(b4), 0);
  ASSERT_EQ(widget_index_of(b5), 1);
  ASSERT_EQ(widget_index_of(b1), 2);
  ASSERT_EQ(widget_index_of(b2), 3);
  ASSERT_EQ(widget_index_of(b3), 4);

  ASSERT_EQ(widget_insert_child(w, 0, b6), RET_OK);
  ASSERT_EQ(widget_index_of(b6), 0);
  ASSERT_EQ(widget_index_of(b4), 1);
  ASSERT_EQ(widget_index_of(b5), 2);

  ASSERT_EQ(widget_insert_child(w, 1, b5), RET_FAIL);

  widget_destroy(w);
}

TEST(Widget, widget_get_state_for_style) {
  widget_t* w = window_create(NULL, 0, 0, 400, 300);
  widget_t* b = button_create(w, 1, 0, 10, 20);

  widget_set_state(b, WIDGET_STATE_NORMAL);
  ASSERT_EQ(string(WIDGET_STATE_NORMAL), widget_get_state_for_style(b, FALSE, FALSE));
  widget_set_state(b, WIDGET_STATE_PRESSED);
  ASSERT_EQ(string(WIDGET_STATE_PRESSED), widget_get_state_for_style(b, FALSE, FALSE));
  widget_set_state(b, WIDGET_STATE_OVER);
  ASSERT_EQ(string(WIDGET_STATE_OVER), widget_get_state_for_style(b, FALSE, FALSE));

  widget_set_state(b, WIDGET_STATE_NORMAL);
  ASSERT_EQ(string(WIDGET_STATE_NORMAL_OF_CHECKED), widget_get_state_for_style(b, FALSE, TRUE));
  widget_set_state(b, WIDGET_STATE_PRESSED);
  ASSERT_EQ(string(WIDGET_STATE_PRESSED_OF_CHECKED), widget_get_state_for_style(b, FALSE, TRUE));
  widget_set_state(b, WIDGET_STATE_OVER);
  ASSERT_EQ(string(WIDGET_STATE_OVER_OF_CHECKED), widget_get_state_for_style(b, FALSE, TRUE));

  widget_set_state(b, WIDGET_STATE_NORMAL);
  ASSERT_EQ(string(WIDGET_STATE_NORMAL_OF_ACTIVE), widget_get_state_for_style(b, TRUE, FALSE));
  widget_set_state(b, WIDGET_STATE_PRESSED);
  ASSERT_EQ(string(WIDGET_STATE_PRESSED_OF_ACTIVE), widget_get_state_for_style(b, TRUE, FALSE));
  widget_set_state(b, WIDGET_STATE_OVER);
  ASSERT_EQ(string(WIDGET_STATE_OVER_OF_ACTIVE), widget_get_state_for_style(b, TRUE, FALSE));

  widget_destroy(w);
}

TEST(Widget, update_style1) {
  widget_t* w = window_create(NULL, 0, 0, 400, 300);
  widget_t* b = button_create(NULL, 1, 0, 10, 20);
  style_const_t* style = (style_const_t*)(b->astyle);

  ASSERT_EQ(style->data, (const unsigned char*)NULL);
  widget_use_style(b, "edit_clear");
  ASSERT_EQ(style->data, (const unsigned char*)NULL);

  widget_add_child(w, b);
  ASSERT_NE(style->data, (const unsigned char*)NULL);

  widget_destroy(w);
}

TEST(Widget, update_style2) {
  widget_t* w = window_create(NULL, 0, 0, 400, 300);
  widget_t* g = group_box_create(NULL, 0, 0, 100, 200);
  widget_t* b = button_create(g, 1, 0, 10, 20);
  style_const_t* style = (style_const_t*)(b->astyle);

  ASSERT_EQ(style->data, (const unsigned char*)NULL);
  widget_use_style(b, "edit_clear");
  ASSERT_EQ(style->data, (const unsigned char*)NULL);

  widget_add_child(w, g);

  ASSERT_NE(style->data, (const unsigned char*)NULL);

  widget_destroy(w);
}

TEST(Widget, clone_custom_props) {
  widget_t* clone = NULL;
  widget_t* w = window_create(NULL, 0, 0, 400, 300);
  widget_t* b = button_create(w, 1, 0, 10, 20);

  ASSERT_EQ(widget_set_prop_int(b, "custom", 123), RET_OK);
  ASSERT_EQ(widget_get_prop_int(b, "custom", 0), 123);

  clone = widget_clone(b, b->parent);
  ASSERT_EQ(widget_get_prop_int(clone, "custom", 0), 123);

  widget_destroy(w);
}

TEST(Widget, is_keyboard) {
  widget_t* w = window_create(NULL, 0, 0, 400, 300);
  widget_t* group = group_box_create(w, 1, 0, 10, 20);

  ASSERT_EQ(widget_is_keyboard(group), FALSE);
  ASSERT_EQ(widget_set_prop_bool(group, WIDGET_PROP_IS_KEYBOARD, TRUE), RET_OK);
  ASSERT_EQ(widget_is_keyboard(group), TRUE);

  widget_destroy(w);
}

#include "base/ui_loader.h"

TEST(Widget, load_widget) {
  widget_t* w = ui_loader_load_widget("test_view");

  ASSERT_EQ(w != NULL, TRUE);
  ASSERT_STREQ(w->name, "test_view");

  widget_destroy(w);
}
