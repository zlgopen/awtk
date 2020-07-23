
#include "base/canvas.h"
#include "base/widget.h"
#include "widgets/view.h"
#include "widgets/button.h"
#include "widgets/label.h"
#include "widgets/group_box.h"
#include "widgets/button_group.h"
#include "base/window.h"
#include "widgets/pages.h"
#include "base/style_const.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include <stdlib.h>
#include <string.h>
#include "gtest/gtest.h"
#include "base/ui_feedback.h"

using std::string;
#include "common.h"

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
  ASSERT_STREQ(w->state, WIDGET_STATE_PRESSED);

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

  ASSERT_EQ(widget_set_prop_str(w, WIDGET_PROP_POINTER_CURSOR, "cursor"), RET_OK);
  ASSERT_EQ(strcmp(w->pointer_cursor, "cursor"), 0);
  ASSERT_EQ(strcmp(widget_get_prop_str(w, WIDGET_PROP_POINTER_CURSOR, ""), "cursor"), 0);

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

TEST(Widget, move) {
  string event_log;
  widget_t* w = button_create(NULL, 0, 0, 0, 0);

  event_log = "";
  widget_on(w, EVT_MOVE, widget_log_events, &event_log);
  widget_on(w, EVT_WILL_MOVE, widget_log_events, &event_log);

  widget_move(w, 100, 200);
  ASSERT_EQ(w->x, 100);
  ASSERT_EQ(w->y, 200);
  ASSERT_EQ(event_log, string("will_move move "));
  ASSERT_EQ(w->ref_count, 1);

  widget_destroy(w);
}

TEST(Widget, resize) {
  string event_log;
  widget_t* w = button_create(NULL, 0, 0, 0, 0);

  event_log = "";
  widget_on(w, EVT_RESIZE, widget_log_events, &event_log);
  widget_on(w, EVT_WILL_RESIZE, widget_log_events, &event_log);

  widget_resize(w, 100, 200);
  ASSERT_EQ(w->w, 100);
  ASSERT_EQ(w->h, 200);
  ASSERT_EQ(event_log, string("will_resize resize "));
  ASSERT_EQ(w->ref_count, 1);

  widget_destroy(w);
}

TEST(Widget, move_resize) {
  string event_log;
  widget_t* w = button_create(NULL, 0, 0, 0, 0);

  event_log = "";
  widget_on(w, EVT_MOVE_RESIZE, widget_log_events, &event_log);
  widget_on(w, EVT_WILL_MOVE_RESIZE, widget_log_events, &event_log);

  widget_move_resize(w, 100, 200, 300, 400);
  ASSERT_EQ(w->x, 100);
  ASSERT_EQ(w->y, 200);
  ASSERT_EQ(w->w, 300);
  ASSERT_EQ(w->h, 400);
  ASSERT_EQ(event_log, string("will_move_resize move_resize "));
  ASSERT_EQ(w->ref_count, 1);

  widget_destroy(w);
}

TEST(Widget, prop) {
  value_t v;
  string event_log;
  widget_t* w = button_create(NULL, 0, 0, 0, 0);

  widget_on(w, EVT_PROP_WILL_CHANGE, widget_log_events, &event_log);
  widget_on(w, EVT_PROP_CHANGED, widget_log_events, &event_log);

  event_log = "";
  value_set_str(&v, "123");
  ASSERT_EQ(widget_set_prop(w, "name", &v), RET_OK);
  ASSERT_EQ(event_log, string("prop_will_change name123 prop_changed name123 "));

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
  widget_ungrab(b1, l1);

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
  idle_dispatch();

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
  ASSERT_EQ(b->need_update_style, TRUE);
  ASSERT_EQ(widget_update_style(b), RET_OK);
  ASSERT_EQ(b->need_update_style, FALSE);
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

  ASSERT_EQ(b->need_update_style, TRUE);
  ASSERT_EQ(widget_update_style(b), RET_OK);
  ASSERT_EQ(b->need_update_style, FALSE);
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

TEST(Widget, clone_const_style) {
  widget_t* clone = NULL;
  widget_t* w = window_create(NULL, 0, 0, 400, 300);
  widget_t* b = button_create(w, 1, 0, 10, 20);

  clone = widget_clone(b, b->parent);
  ASSERT_EQ(style_is_mutable(b->astyle), FALSE);
  ASSERT_EQ(style_is_mutable(clone->astyle), FALSE);

  widget_destroy(w);
}

TEST(Widget, clone_mutable_style) {
  widget_t* clone = NULL;
  widget_t* w = window_create(NULL, 0, 0, 400, 300);
  widget_t* b = button_create(w, 1, 0, 10, 20);

  ASSERT_EQ(widget_set_style_int(b, "font_size", 123), RET_OK);
  ASSERT_EQ(widget_set_style_str(b, "icon", "icon1"), RET_OK);

  clone = widget_clone(b, b->parent);
  ASSERT_EQ(style_is_mutable(b->astyle), TRUE);
  ASSERT_EQ(style_is_mutable(clone->astyle), TRUE);

  ASSERT_EQ(style_get_int(clone->astyle, "font_size", 0), 123);
  ASSERT_STREQ(style_get_str(clone->astyle, "icon", NULL), "icon1");

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

TEST(Widget, is_window) {
  widget_t* w = window_create(NULL, 0, 0, 400, 300);
  widget_t* group = group_box_create(w, 1, 0, 10, 20);

  ASSERT_EQ(widget_is_window(w), TRUE);
  ASSERT_EQ(widget_is_window(group), FALSE);

  ASSERT_EQ(widget_is_window_manager(w->parent), TRUE);
  ASSERT_EQ(widget_is_designing_window(group), FALSE);

  widget_destroy(w);
}

TEST(Widget, is_window_manager) {
  widget_t* w = window_create(NULL, 0, 0, 400, 300);
  widget_t* group = group_box_create(w, 1, 0, 10, 20);

  ASSERT_EQ(widget_is_window_manager(w), FALSE);
  ASSERT_EQ(widget_is_window_manager(group), FALSE);
  ASSERT_EQ(widget_is_window_manager(w->parent), TRUE);

  widget_destroy(w);
}

TEST(Widget, get_window) {
  widget_t* w = window_create(NULL, 0, 0, 400, 300);
  widget_t* group = group_box_create(w, 1, 0, 10, 20);
  widget_t* group1 = group_box_create(group, 1, 0, 10, 20);

  ASSERT_EQ(widget_get_window(group), w);
  ASSERT_EQ(widget_get_window(group1), w);

  widget_destroy(w);
}

static ret_t on_button_events_stop(void* ctx, event_t* e) {
  string& event_log = *(string*)ctx;
  switch (e->type) {
    case EVT_POINTER_MOVE_BEFORE_CHILDREN: {
      event_log += "EVT_POINTER_MOVE_BEFORE_CHILDREN";
      break;
    }
    case EVT_POINTER_DOWN_BEFORE_CHILDREN: {
      event_log += "EVT_POINTER_DOWN_BEFORE_CHILDREN";
      break;
    }
    case EVT_POINTER_UP_BEFORE_CHILDREN: {
      event_log += "EVT_POINTER_UP_BEFORE_CHILDREN";
      break;
    }
    case EVT_KEY_DOWN_BEFORE_CHILDREN: {
      event_log += "EVT_KEY_DOWN_BEFORE_CHILDREN";
      break;
    }
    case EVT_KEY_UP_BEFORE_CHILDREN: {
      event_log += "EVT_KEY_UP_BEFORE_CHILDREN";
      break;
    }
    default:
      break;
  }

  return RET_STOP;
}

TEST(Widget, EVT_POINTER_DOWN_BEFORE_CHILDREN) {
  string event_log;
  pointer_event_t evt;
  widget_t* w = button_create(NULL, 0, 0, 0, 0);

  event_log = "";
  pointer_event_init(&evt, EVT_POINTER_DOWN, w, 1, 2);
  widget_on(w, EVT_POINTER_DOWN_BEFORE_CHILDREN, on_button_events_stop, &event_log);
  widget_on(w, EVT_POINTER_DOWN, widget_log_events, &event_log);
  widget_on_pointer_down(w, &evt);

  ASSERT_EQ(event_log, string("EVT_POINTER_DOWN_BEFORE_CHILDREN"));

  widget_destroy(w);
}

TEST(Widget, EVT_POINTER_UP_BEFORE_CHILDREN) {
  string event_log;
  pointer_event_t evt;
  widget_t* w = button_create(NULL, 0, 0, 0, 0);

  event_log = "";
  pointer_event_init(&evt, EVT_POINTER_UP, w, 1, 2);
  widget_on(w, EVT_POINTER_UP_BEFORE_CHILDREN, on_button_events_stop, &event_log);
  widget_on(w, EVT_POINTER_UP, widget_log_events, &event_log);
  widget_on_pointer_up(w, &evt);

  ASSERT_EQ(event_log, string("EVT_POINTER_UP_BEFORE_CHILDREN"));

  widget_destroy(w);
}

TEST(Widget, EVT_POINTER_MOVE_BEFORE_CHILDREN) {
  string event_log;
  pointer_event_t evt;
  widget_t* w = button_create(NULL, 0, 0, 0, 0);

  event_log = "";
  pointer_event_init(&evt, EVT_POINTER_MOVE, w, 1, 2);
  widget_on(w, EVT_POINTER_MOVE_BEFORE_CHILDREN, on_button_events_stop, &event_log);
  widget_on(w, EVT_POINTER_MOVE, widget_log_events, &event_log);
  widget_on_pointer_move(w, &evt);

  ASSERT_EQ(event_log, string("EVT_POINTER_MOVE_BEFORE_CHILDREN"));

  widget_destroy(w);
}

TEST(Widget, EVT_KEY_DOWN_BEFORE_CHILDREN) {
  key_event_t evt;
  string event_log;
  widget_t* w = button_create(NULL, 0, 0, 0, 0);

  event_log = "";
  key_event_init(&evt, EVT_KEY_DOWN, w, 1);
  widget_on(w, EVT_KEY_DOWN_BEFORE_CHILDREN, on_button_events_stop, &event_log);
  widget_on(w, EVT_KEY_DOWN, widget_log_events, &event_log);
  widget_on_keydown(w, &evt);

  ASSERT_EQ(event_log, string("EVT_KEY_DOWN_BEFORE_CHILDREN"));

  widget_destroy(w);
}

TEST(Widget, EVT_KEY_UP_BEFORE_CHILDREN) {
  key_event_t evt;
  string event_log;
  widget_t* w = button_create(NULL, 0, 0, 0, 0);

  event_log = "";
  key_event_init(&evt, EVT_KEY_UP, w, 1);
  widget_on(w, EVT_KEY_UP_BEFORE_CHILDREN, on_button_events_stop, &event_log);
  widget_on(w, EVT_KEY_UP, widget_log_events, &event_log);
  widget_on_keyup(w, &evt);

  ASSERT_EQ(event_log, string("EVT_KEY_UP_BEFORE_CHILDREN"));

  widget_destroy(w);
}

#include "base/ui_loader.h"

TEST(Widget, load_widget) {
  paint_event_t e;
  widget_t* win = window_create(NULL, 0, 0, 200, 200);
  widget_t* w = ui_loader_load_widget("test_view");
  widget_t* button = widget_child(w, "ok");

  ASSERT_EQ(w != NULL, TRUE);
  ASSERT_STREQ(w->name, "test_view");

  widget_add_child(win, w);
  widget_dispatch(button, paint_event_init(&e, EVT_BEFORE_PAINT, w, NULL));
  ASSERT_STREQ(button->text.str, L"确定");

  widget_destroy(win);
}

TEST(Widget, calc_icon_text_rect_icon) {
  rect_t ir = rect_init(10, 20, 200, 40);
  rect_t r_icon;

  widget_calc_icon_text_rect(&ir, 10, 10.0f, ICON_AT_TOP, 10, 10, 2, NULL, &r_icon);

  ASSERT_EQ(r_icon.x, ir.x);
  ASSERT_EQ(r_icon.y, ir.y);
  ASSERT_EQ(r_icon.w, ir.w);
  ASSERT_EQ(r_icon.h, ir.h);
}

TEST(Widget, calc_icon_text_rect_text) {
  rect_t ir = rect_init(10, 20, 200, 40);
  rect_t r_text;

  widget_calc_icon_text_rect(&ir, 10, 10.0f, ICON_AT_TOP, 10, 10, 2, &r_text, NULL);

  ASSERT_EQ(r_text.x, ir.x);
  ASSERT_EQ(r_text.y, ir.y);
  ASSERT_EQ(r_text.w, ir.w);
  ASSERT_EQ(r_text.h, ir.h);
}

TEST(Widget, calc_icon_text_rect_icon_top) {
  rect_t r_icon;
  rect_t r_text;
  int32_t spacer = 2;
  int32_t font_size = 20;
  rect_t ir = rect_init(10, 20, 200, 80);

  widget_calc_icon_text_rect(&ir, font_size, 10.0f, ICON_AT_TOP, 10, 10, spacer, &r_text, &r_icon);

  ASSERT_EQ(r_icon.x, ir.x);
  ASSERT_EQ(r_icon.y, ir.y);
  ASSERT_EQ(r_icon.w, ir.w);
  ASSERT_EQ(r_icon.h, ir.h - font_size - spacer);

  ASSERT_EQ(r_text.x, ir.x);
  ASSERT_EQ(r_text.y, r_icon.h + spacer);
  ASSERT_EQ(r_text.w, ir.w);
  ASSERT_EQ(r_text.h, font_size);
}

TEST(Widget, calc_icon_text_rect_icon_left) {
  rect_t r_icon;
  rect_t r_text;
  int32_t spacer = 2;
  int32_t font_size = 20;
  rect_t ir = rect_init(10, 20, 200, 80);

  widget_calc_icon_text_rect(&ir, font_size, 10.0f, ICON_AT_LEFT, 10, 10, spacer, &r_text, &r_icon);

  ASSERT_EQ(r_icon.x, ir.x);
  ASSERT_EQ(r_icon.y, ir.y);
  ASSERT_EQ(r_icon.w, ir.h);
  ASSERT_EQ(r_icon.h, ir.h);

  ASSERT_EQ(r_text.x, ir.x + ir.h + spacer);
  ASSERT_EQ(r_text.y, ir.y);
  ASSERT_EQ(r_text.w, ir.w - ir.h - spacer);
  ASSERT_EQ(r_text.h, ir.h);
}

TEST(Widget, calc_icon_text_rect_icon_right) {
  rect_t r_icon;
  rect_t r_text;
  int32_t spacer = 2;
  int32_t font_size = 20;
  rect_t ir = rect_init(10, 20, 200, 80);

  widget_calc_icon_text_rect(&ir, font_size, 10.0f, ICON_AT_RIGHT, 10, 10, spacer, &r_text,
                             &r_icon);

  ASSERT_EQ(r_icon.x, ir.x + ir.w - ir.h);
  ASSERT_EQ(r_icon.y, ir.y);
  ASSERT_EQ(r_icon.w, ir.h);
  ASSERT_EQ(r_icon.h, ir.h);

  ASSERT_EQ(r_text.x, ir.x);
  ASSERT_EQ(r_text.y, ir.y);
  ASSERT_EQ(r_text.w, ir.w - ir.h - spacer);
  ASSERT_EQ(r_text.h, ir.h);
}

TEST(Widget, calc_icon_text_rect_icon_centre) {
  rect_t r_icon;
  rect_t r_text;
  int32_t spacer = 2;
  int32_t font_size = 20;
  float_t text_size = 30;
  uint32_t icon_image_w = 20;
  uint32_t icon_image_h = 20;
  rect_t ir = rect_init(10, 20, 200, 80);

  int32_t icon_h = ir.h - icon_image_h;
  int32_t w = ir.w - spacer - text_size - icon_image_w;

  widget_calc_icon_text_rect(&ir, font_size, text_size, ICON_AT_CENTRE, icon_image_w, icon_image_h,
                             spacer, &r_text, &r_icon);

  ASSERT_EQ(r_icon.x, ir.x + w / 2);
  ASSERT_EQ(r_icon.y, ir.y + icon_h / 2);
  ASSERT_EQ(r_icon.w, icon_image_w);
  ASSERT_EQ(r_icon.h, icon_image_h);

  ASSERT_EQ(r_text.x, ir.x + icon_image_w + spacer + w / 2);
  ASSERT_EQ(r_text.y, ir.y);
  ASSERT_EQ(r_text.w, text_size);
  ASSERT_EQ(r_text.h, ir.h);
}

static ret_t on_visit_count(void* ctx, const void* data) {
  int32_t* count = (int32_t*)ctx;
  widget_t* widget = WIDGET(data);
  const char* type = widget->vt->type;

  if (tk_str_eq(type, WIDGET_TYPE_BUTTON_GROUP)) {
    return RET_SKIP;
  } else if (tk_str_eq(type, WIDGET_TYPE_BUTTON)) {
    *count = (*count) + 1;
    return RET_OK;
  } else if (tk_str_eq(type, WIDGET_TYPE_LABEL)) {
    return RET_STOP;
  }

  return RET_OK;
}

TEST(Widget, foreach_normal) {
  int32_t count = 0;
  widget_t* w = view_create(NULL, 0, 0, 400, 300);
  button_create(w, 0, 0, 0, 0);
  button_create(w, 0, 0, 0, 0);
  button_create(w, 0, 0, 0, 0);

  ASSERT_EQ(widget_foreach(w, on_visit_count, &count), RET_OK);
  ASSERT_EQ(count, 3);

  widget_destroy(w);
}

TEST(Widget, foreach_stop) {
  int32_t count = 0;
  widget_t* w = view_create(NULL, 0, 0, 400, 300);
  button_create(w, 0, 0, 0, 0);
  button_create(w, 0, 0, 0, 0);
  label_create(w, 0, 0, 0, 0);
  button_create(w, 0, 0, 0, 0);

  ASSERT_EQ(widget_foreach(w, on_visit_count, &count), RET_STOP);
  ASSERT_EQ(count, 2);

  widget_destroy(w);
}

TEST(Widget, foreach_skip) {
  int32_t count = 0;
  widget_t* w = view_create(NULL, 0, 0, 400, 300);
  button_create(w, 0, 0, 0, 0);
  button_create(w, 0, 0, 0, 0);

  widget_t* g = button_group_create(w, 0, 0, 400, 300);
  button_create(g, 0, 0, 0, 0);
  button_create(g, 0, 0, 0, 0);
  label_create(g, 0, 0, 0, 0);
  button_create(g, 0, 0, 0, 0);

  ASSERT_EQ(widget_foreach(w, on_visit_count, &count), RET_OK);
  ASSERT_EQ(count, 2);

  widget_destroy(w);
}

TEST(Widget, move_focus) {
  widget_t* w = window_create(NULL, 0, 0, 400, 300);
  widget_t* b1 = button_create(w, 0, 0, 0, 0);
  widget_t* b2 = button_create(w, 0, 0, 0, 0);
  widget_t* b3 = button_create(w, 0, 0, 0, 0);

  widget_set_prop_bool(b1, WIDGET_PROP_FOCUSABLE, TRUE);
  widget_set_prop_bool(b2, WIDGET_PROP_FOCUSABLE, TRUE);
  widget_set_prop_bool(b3, WIDGET_PROP_FOCUSABLE, TRUE);

  widget_set_focused(b1, TRUE);
  ASSERT_EQ(b1->focused, TRUE);
  ASSERT_EQ(widget_focus_next(b1), RET_OK);
  ASSERT_EQ(b1->focused, FALSE);
  ASSERT_EQ(b2->focused, TRUE);

  ASSERT_EQ(widget_focus_next(b2), RET_OK);
  ASSERT_EQ(b2->focused, FALSE);
  ASSERT_EQ(b3->focused, TRUE);

  ASSERT_EQ(widget_focus_prev(b3), RET_OK);
  ASSERT_EQ(b3->focused, FALSE);
  ASSERT_EQ(b2->focused, TRUE);

  widget_destroy(w);
}

TEST(Widget, move_focus_skip_invisible) {
  widget_t* w = window_create(NULL, 0, 0, 400, 300);
  widget_t* b1 = button_create(w, 0, 0, 0, 0);
  widget_t* b2 = button_create(w, 0, 0, 0, 0);
  widget_t* view = view_create(w, 0, 0, 0, 0);
  widget_t* b3 = button_create(w, 0, 0, 0, 0);
  widget_t* b4 = button_create(view, 0, 0, 0, 0);
  widget_t* b5 = button_create(view, 0, 0, 0, 0);

  widget_set_prop_bool(b1, WIDGET_PROP_FOCUSABLE, TRUE);
  widget_set_prop_bool(b2, WIDGET_PROP_FOCUSABLE, TRUE);
  widget_set_prop_bool(b3, WIDGET_PROP_FOCUSABLE, TRUE);
  widget_set_prop_bool(b4, WIDGET_PROP_FOCUSABLE, TRUE);
  widget_set_prop_bool(b5, WIDGET_PROP_FOCUSABLE, TRUE);

  widget_set_visible(view, FALSE, FALSE);

  widget_set_focused(b1, TRUE);
  ASSERT_EQ(b1->focused, TRUE);
  ASSERT_EQ(widget_focus_next(b1), RET_OK);
  ASSERT_EQ(b1->focused, FALSE);
  ASSERT_EQ(b2->focused, TRUE);

  ASSERT_EQ(widget_focus_next(b2), RET_OK);
  ASSERT_EQ(b2->focused, FALSE);
  ASSERT_EQ(b3->focused, TRUE);

  ASSERT_EQ(widget_focus_prev(b3), RET_OK);
  ASSERT_EQ(b3->focused, FALSE);
  ASSERT_EQ(b2->focused, TRUE);

  widget_destroy(w);
}

TEST(Widget, move_focus_first) {
  widget_t* w = window_create(NULL, 0, 0, 400, 300);
  widget_t* b1 = button_create(w, 0, 0, 0, 0);
  widget_t* b2 = button_create(w, 0, 0, 0, 0);
  widget_t* view = view_create(w, 0, 0, 0, 0);
  widget_t* b3 = button_create(w, 0, 0, 0, 0);
  widget_t* b4 = button_create(view, 0, 0, 0, 0);
  widget_t* b5 = button_create(view, 0, 0, 0, 0);

  widget_set_prop_bool(b1, WIDGET_PROP_FOCUSABLE, TRUE);
  widget_set_prop_bool(b2, WIDGET_PROP_FOCUSABLE, TRUE);
  widget_set_prop_bool(b3, WIDGET_PROP_FOCUSABLE, TRUE);
  widget_set_prop_bool(b4, WIDGET_PROP_FOCUSABLE, TRUE);
  widget_set_prop_bool(b5, WIDGET_PROP_FOCUSABLE, TRUE);

  widget_set_visible(view, FALSE, FALSE);

  ASSERT_EQ(widget_has_focused_widget_in_window(w), FALSE);
  widget_focus_first(w);
  ASSERT_EQ(widget_has_focused_widget_in_window(w), TRUE);
  ASSERT_EQ(b1->focused, TRUE);
  ASSERT_EQ(widget_focus_next(b1), RET_OK);
  ASSERT_EQ(b1->focused, FALSE);
  ASSERT_EQ(b2->focused, TRUE);

  widget_destroy(w);
}

TEST(Widget, move_focus_pages) {
  widget_t* w = window_create(NULL, 0, 0, 400, 300);
  widget_t* b0 = button_create(w, 0, 0, 0, 0);

  widget_t* pages = pages_create(w, 0, 0, 400, 300);

  widget_t* p1 = view_create(pages, 0, 0, 400, 300);
  widget_t* p2 = view_create(pages, 0, 0, 400, 300);
  widget_t* p3 = view_create(pages, 0, 0, 400, 300);

  widget_t* b1 = button_create(p1, 0, 0, 0, 0);
  widget_t* b2 = button_create(p2, 0, 0, 0, 0);
  widget_t* b3 = button_create(p3, 0, 0, 0, 0);

  widget_set_prop_bool(b0, WIDGET_PROP_FOCUSABLE, TRUE);
  widget_set_prop_bool(b1, WIDGET_PROP_FOCUSABLE, TRUE);
  widget_set_prop_bool(b2, WIDGET_PROP_FOCUSABLE, TRUE);
  widget_set_prop_bool(b3, WIDGET_PROP_FOCUSABLE, TRUE);

  widget_focus_first(w);
  ASSERT_EQ(b0->focused, TRUE);
  ASSERT_EQ(widget_focus_next(b0), RET_OK);
  ASSERT_EQ(b0->focused, FALSE);
  ASSERT_EQ(b1->focused, TRUE);

  ASSERT_EQ(widget_focus_next(b1), RET_OK);
  ASSERT_EQ(b1->focused, FALSE);
  ASSERT_EQ(b0->focused, TRUE);

  pages_set_active(pages, 1);
  widget_focus_first(w);
  ASSERT_EQ(b0->focused, TRUE);
  ASSERT_EQ(widget_focus_next(b0), RET_OK);
  ASSERT_EQ(b0->focused, FALSE);
  ASSERT_EQ(b2->focused, TRUE);

  ASSERT_EQ(widget_focus_next(b2), RET_OK);
  ASSERT_EQ(b2->focused, FALSE);
  ASSERT_EQ(b0->focused, TRUE);
  widget_destroy(w);
}

TEST(Widget, mutable_style) {
  value_t v;
  color_t c = color_init(0, 0, 0, 0);
  color_t black = color_init(0, 0, 0, 0xff);
  widget_t* w = window_create(NULL, 0, 0, 400, 300);
  widget_t* b = button_create(w, 0, 0, 0, 0);

  value_set_int(&v, 123);
  ASSERT_EQ(widget_set_style(b, "font_size", &v), RET_OK);
  ASSERT_EQ(style_get_int(b->astyle, "font_size", 0), value_int(&v));

  value_set_int(&v, 321);
  ASSERT_EQ(widget_set_style(b, "normal:font_size", &v), RET_OK);
  ASSERT_EQ(style_get_int(b->astyle, "font_size", 0), value_int(&v));

  value_set_str(&v, "black");
  ASSERT_EQ(widget_set_style(b, "text_color", &v), RET_OK);
  ASSERT_EQ(style_get_color(b->astyle, "text_color", c).color, black.color);

  value_set_str(&v, "black");
  ASSERT_EQ(widget_set_style(b, "normal:text_color", &v), RET_OK);
  ASSERT_EQ(style_get_color(b->astyle, "text_color", c).color, black.color);

  widget_destroy(w);
}

TEST(Widget, mutable_style1) {
  widget_t* w = window_create(NULL, 0, 0, 400, 300);
  widget_t* b = button_create(w, 0, 0, 0, 0);

  ASSERT_EQ(widget_set_style_int(b, "font_size", 123), RET_OK);
  ASSERT_EQ(style_get_int(b->astyle, "font_size", 0), 123);

  ASSERT_EQ(widget_set_style_int(b, "normal:font_size", 321), RET_OK);
  ASSERT_EQ(style_get_int(b->astyle, "font_size", 0), 321);

  ASSERT_EQ(widget_set_style_str(b, "icon", "icon1"), RET_OK);
  ASSERT_STREQ(style_get_str(b->astyle, "icon", NULL), "icon1");

  ASSERT_EQ(widget_set_style_str(b, "normal:icon", "icon2"), RET_OK);
  ASSERT_STREQ(style_get_str(b->astyle, "icon", NULL), "icon2");

  widget_destroy(w);
}

TEST(Widget, with_focus_state) {
  widget_t* w = window_create(NULL, 0, 0, 400, 300);

  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_WITH_FOCUS_STATE, FALSE), FALSE);
  ASSERT_EQ(widget_set_prop_str(w, WIDGET_PROP_WITH_FOCUS_STATE, "true"), RET_OK);
  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_WITH_FOCUS_STATE, FALSE), TRUE);

  widget_destroy(w);
}

TEST(Widget, feedback) {
  widget_t* w = window_create(NULL, 0, 0, 400, 300);

  ASSERT_EQ(w->feedback, FALSE);
  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_FEEDBACK, TRUE), FALSE);
  ASSERT_EQ(widget_set_prop_str(w, WIDGET_PROP_FEEDBACK, "true"), RET_OK);
  ASSERT_EQ(widget_get_prop_bool(w, WIDGET_PROP_FEEDBACK, FALSE), TRUE);
  ASSERT_EQ(w->feedback, TRUE);

  widget_destroy(w);
}

static ret_t ui_on_feedback_test(void* ctx, widget_t* widget, event_t* evt) {
  int32_t* count = (int32_t*)ctx;

  if (evt->type == EVT_KEY_UP) {
    *count = *count + 1;
  }

  return RET_OK;
}

TEST(Widget, feedback1) {
  int32_t count = 0;
  widget_t* w = window_create(NULL, 0, 0, 400, 300);
  key_event_t e;

  ui_feedback_init(ui_on_feedback_test, &count);

  widget_dispatch(w, key_event_init(&e, EVT_KEY_DOWN, w, TK_KEY_z));
  ASSERT_EQ(count, 0);
  widget_dispatch(w, key_event_init(&e, EVT_KEY_UP, w, TK_KEY_z));
  ASSERT_EQ(count, 0);

  widget_set_feedback(w, TRUE);
  widget_on_keydown(w, (key_event_t*)key_event_init(&e, EVT_KEY_DOWN, w, TK_KEY_z));
  ASSERT_EQ(count, 0);
  widget_on_keyup(w, (key_event_t*)key_event_init(&e, EVT_KEY_UP, w, TK_KEY_z));
  ASSERT_EQ(count, 1);

  widget_destroy(w);
}

TEST(Widget, off) {
  widget_t* w = button_create(NULL, 0, 0, 0, 0);

  ASSERT_EQ(widget_on(w, EVT_MOVE, widget_log_events, NULL) > 0, TRUE);
  ASSERT_EQ(emitter_size(w->emitter), 1);
  ASSERT_EQ(widget_off_by_func(w, EVT_MOVE, widget_log_events, NULL), RET_OK);

  widget_destroy(w);
}

TEST(Widget, off_by_tag) {
  widget_t* w = button_create(NULL, 0, 0, 0, 0);

  ASSERT_EQ(widget_on_with_tag(w, EVT_MOVE, widget_log_events, NULL, 1) > 0, TRUE);
  ASSERT_EQ(widget_on_with_tag(w, EVT_MOVE, widget_log_events, NULL, 1) > 0, TRUE);
  ASSERT_EQ(widget_on_with_tag(w, EVT_MOVE, widget_log_events, NULL, 1) > 0, TRUE);
  ASSERT_EQ(widget_on_with_tag(w, EVT_MOVE, widget_log_events, NULL, 2) > 0, TRUE);
  ASSERT_EQ(widget_on_with_tag(w, EVT_MOVE, widget_log_events, NULL, 2) > 0, TRUE);

  ASSERT_EQ(emitter_size(w->emitter), 5);

  ASSERT_EQ(widget_off_by_tag(w, 1), RET_OK);
  ASSERT_EQ(emitter_size(w->emitter), 2);

  widget_destroy(w);
}

static ret_t on_key_event(void* ctx, event_t* e) {
  uint32_t* key = (uint32_t*)ctx;

  *key = ((key_event_t*)e)->key;

  return RET_OK;
}

TEST(Widget, map_key) {
  key_event_t e;
  uint32_t key = 0;
  widget_t* w = button_create(NULL, 0, 0, 0, 0);

  key_event_init(&e, EVT_KEY_DOWN, w, TK_KEY_s);
  ASSERT_EQ(widget_on(w, EVT_KEY_DOWN, on_key_event, &key) > 0, TRUE);

  widget_on_keydown(w, &e);
  ASSERT_EQ(key, TK_KEY_s);

  widget_set_prop_str(w, "map_key:s", "left");
  widget_on_keydown(w, &e);
  ASSERT_EQ(key, TK_KEY_LEFT);

  widget_destroy(w);
}

TEST(Widget, map_key1) {
  key_event_t e;
  uint32_t key = 0;
  widget_t* w = button_create(NULL, 0, 0, 0, 0);

  ASSERT_EQ(widget_on(w, EVT_KEY_DOWN, on_key_event, &key) > 0, TRUE);

  widget_set_prop_str(w, "map_key:left", "pageup");
  widget_set_prop_str(w, "map_key:RIGHT", "PAGEDOWN");

  key_event_init(&e, EVT_KEY_DOWN, w, TK_KEY_LEFT);
  widget_on_keydown(w, &e);
  ASSERT_EQ(key, TK_KEY_PAGEUP);

  key_event_init(&e, EVT_KEY_DOWN, w, TK_KEY_RIGHT);
  widget_on_keydown(w, &e);
  ASSERT_EQ(key, TK_KEY_PAGEDOWN);

  widget_destroy(w);
}

TEST(Widget, exec) {
  widget_t* w = button_create(NULL, 0, 0, 0, 0);

  ASSERT_EQ(
      widget_set_prop_str(w, WIDGET_PROP_ANIMATION,
                          "move(y_from=0, y_to=128, yoyo_times=1000, duration=1000, delay=3000)"),
      RET_OK);
  ASSERT_EQ(widget_set_prop_str(w, WIDGET_PROP_EXEC, "start_animator:move"), RET_OK);
  ASSERT_EQ(widget_set_prop_str(w, WIDGET_PROP_EXEC, "pause_animator:move"), RET_OK);
  ASSERT_EQ(widget_set_prop_str(w, WIDGET_PROP_EXEC, "stop_animator:move"), RET_OK);
  ASSERT_EQ(widget_set_prop_str(w, WIDGET_PROP_EXEC, "start_animator:move"), RET_OK);
  ASSERT_EQ(widget_set_prop_str(w, WIDGET_PROP_EXEC, "destroy_animator:move"), RET_OK);
  ASSERT_EQ(widget_set_prop_str(w, WIDGET_PROP_EXEC, "not found"), RET_NOT_FOUND);
  ASSERT_EQ(widget_set_prop_str(w, WIDGET_PROP_EXEC, NULL), RET_NOT_FOUND);

  widget_destroy(w);
}

TEST(Widget, tr_text) {
  widget_t* w = button_create(NULL, 0, 0, 0, 0);
  widget_set_tr_text(w, "hello");
  widget_set_text(w, L"ok");

  ASSERT_STREQ(w->tr_text, "hello");

  widget_destroy(w);
}

TEST(Widget, dirty_rect_tolerance) {
  widget_t* w = button_create(NULL, 0, 0, 0, 0);

  ASSERT_EQ(widget_set_dirty_rect_tolerance(w, 16), RET_OK);
  ASSERT_EQ(w->dirty_rect_tolerance, 16);
  ASSERT_EQ(w->dirty_rect_tolerance, widget_get_prop_int(w, WIDGET_PROP_DIRTY_RECT_TOLERANCE, 0));

  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_DIRTY_RECT_TOLERANCE, 21), RET_OK);
  ASSERT_EQ(w->dirty_rect_tolerance, 21);
  ASSERT_EQ(w->dirty_rect_tolerance, widget_get_prop_int(w, WIDGET_PROP_DIRTY_RECT_TOLERANCE, 0));

  widget_destroy(w);
}

TEST(Widget, set_child_text) {
  widget_t* view = view_create(NULL, 0, 0, 0, 0);
  widget_t* label = label_create(view, 0, 0, 0, 0);
  widget_set_name(label, "aaa");

  ASSERT_EQ(widget_set_child_text_utf8(view, "aaa", "awtk"), RET_OK);
  ASSERT_EQ(wcs_cmp(label->text.str, L"awtk"), 0);

  widget_destroy(view);
}

TEST(Widget, set_child_text_with_double) {
  widget_t* view = view_create(NULL, 0, 0, 0, 0);
  widget_t* label = label_create(view, 0, 0, 0, 0);
  widget_set_name(label, "aaa");

  ASSERT_EQ(widget_set_child_text_with_double(view, "aaa", "%2.2lf", 12.3), RET_OK);
  ASSERT_EQ(wcs_cmp(label->text.str, L"12.30"), 0);

  ASSERT_EQ(widget_set_child_text_with_double(view, "aaa", "%2.4lfkg", 12.3), RET_OK);
  ASSERT_EQ(wcs_cmp(label->text.str, L"12.3000kg"), 0);

  widget_destroy(view);
}

TEST(Widget, is_parent_of) {
  widget_t* view = view_create(NULL, 0, 0, 0, 0);
  widget_t* label = label_create(view, 0, 0, 0, 0);
  widget_t* button = button_create(label, 0, 0, 0, 0);
  widget_set_name(label, "aaa");

  ASSERT_EQ(widget_is_parent_of(view, label), TRUE);
  ASSERT_EQ(widget_is_direct_parent_of(view, label), TRUE);

  ASSERT_EQ(widget_is_parent_of(view, button), TRUE);
  ASSERT_EQ(widget_is_direct_parent_of(view, button), FALSE);

  ASSERT_EQ(widget_is_parent_of(label, view), FALSE);
  ASSERT_EQ(widget_is_direct_parent_of(label, view), FALSE);

  widget_destroy(view);
}
