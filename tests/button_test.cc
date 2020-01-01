#include "widgets/button.h"
#include "base/canvas.h"
#include "base/idle.h"
#include "base/keys.h"
#include "base/layout.h"
#include "base/widget.h"
#include "base/window_manager.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "widgets/view.h"
#include "base/window.h"
#include "gtest/gtest.h"
#include <stdlib.h>
#include "ui_loader/ui_serializer.h"

TEST(Button, basic) {
  value_t v1;
  value_t v2;
  widget_t* w = button_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_set_text(w, L"OK"), RET_OK);
  ASSERT_EQ(wcscmp(w->text.str, L"OK"), 0);
  ASSERT_EQ(widget_set_text(w, L"Cancel"), RET_OK);
  ASSERT_EQ(wcscmp(w->text.str, L"Cancel"), 0);

  value_set_wstr(&v1, L"button");
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_TEXT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_TEXT, &v2), RET_OK);
  ASSERT_EQ(wcscmp(v1.value.wstr, v2.value.wstr), 0);

  value_set_int(&v1, 200);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_REPEAT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_REPEAT, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_bool(&v1, TRUE);
  ASSERT_EQ(widget_set_prop_str(w, WIDGET_PROP_ENABLE_LONG_PRESS, "true"), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_ENABLE_LONG_PRESS, &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));

  widget_destroy(w);
}

TEST(Button, clone) {
  value_t v1;
  str_t str;
  widget_t* w1 = button_create(NULL, 10, 20, 30, 40);

  str_init(&str, 0);
  value_set_int(&v1, 200);
  ASSERT_EQ(widget_set_prop(w1, WIDGET_PROP_REPEAT, &v1), RET_OK);
  widget_set_self_layout_params(w1, "1", "2", "3", "4");
  widget_set_children_layout(w1, "default(r=0,c=0,x=10,y=10,s=10)");
  widget_set_sensitive(w1, FALSE);
  widget_set_floating(w1, TRUE);
  ASSERT_EQ(button_cast(w1), w1);

  widget_t* w2 = widget_clone(w1, NULL);

  log_debug("==================================\n");
  widget_to_xml(w1, &str);
  log_debug("w1:%s\n", str.str);

  str_set(&str, "");
  widget_to_xml(w2, &str);
  log_debug("w2:%s\n", str.str);
  log_debug("==================================\n");

  ASSERT_EQ(widget_equal(w1, w2), TRUE);
  widget_destroy(w1);
  widget_destroy(w2);
  str_reset(&str);
}

static ret_t button_on_click_to_remove_parent(void* ctx, event_t* e) {
  widget_t* target = WIDGET(e->target);
  widget_destroy(target->parent);

  return RET_OK;
}

TEST(Button, remove_parent) {
  pointer_event_t e;
  widget_t* w = window_create(NULL, 0, 0, 320, 240);
  widget_t* group = view_create(w, 20, 20, 200, 200);
  widget_t* b = button_create(group, 10, 10, 30, 40);

  widget_resize(w, 320, 240);
  widget_on(b, EVT_CLICK, button_on_click_to_remove_parent, NULL);
  e.e = event_init(EVT_POINTER_DOWN, w);
  e.x = 35;
  e.y = 35;
  window_manager_dispatch_input_event(w->parent, (event_t*)(&e));

  e.e = event_init(EVT_POINTER_UP, w);
  window_manager_dispatch_input_event(w->parent, (event_t*)(&e));

  widget_destroy(w);
  idle_dispatch();
}

TEST(Button, cast) {
  widget_t* w = window_create(NULL, 0, 0, 320, 240);
  widget_t* b = button_create(w, 10, 10, 30, 40);

  ASSERT_EQ(button_cast(b), b);
  ASSERT_EQ(button_cast(w), WIDGET(NULL));

  widget_destroy(w);
}

static ret_t on_click1(void* ctx, event_t* e) {
  return RET_OK;
}

static ret_t on_click2(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  widget_on(widget, EVT_CLICK, on_click2, widget);

  return RET_REMOVE;
}

TEST(Button, event) {
  widget_t* w = window_create(NULL, 0, 0, 320, 240);
  event_t e = event_init(EVT_CLICK, w);

  widget_on(w, EVT_CLICK, on_click1, w);
  widget_on(w, EVT_CLICK, on_click2, w);

  ASSERT_EQ(widget_dispatch(w, &e), RET_OK);
  ASSERT_EQ(widget_dispatch(w, &e), RET_OK);
  ASSERT_EQ(widget_dispatch(w, &e), RET_OK);

  widget_destroy(w);
}

static ret_t on_click_count(void* ctx, event_t* e) {
  int32_t* count = (int32_t*)ctx;
  *count = *count + 1;

  return RET_OK;
}

TEST(Button, activate) {
  key_event_t e;
  int32_t count = 0;
  widget_t* w = button_create(NULL, 10, 20, 30, 40);

  widget_on(w, EVT_CLICK, on_click_count, &count);

  widget_on_keydown(w, (key_event_t*)key_event_init(&e, EVT_KEY_DOWN, w, TK_KEY_SPACE));
  widget_on_keyup(w, (key_event_t*)key_event_init(&e, EVT_KEY_UP, w, TK_KEY_SPACE));
  ASSERT_EQ(count, 1);

  widget_on_keydown(w, (key_event_t*)key_event_init(&e, EVT_KEY_DOWN, w, TK_KEY_SPACE));
  widget_on_keyup(w, (key_event_t*)key_event_init(&e, EVT_KEY_UP, w, TK_KEY_SPACE));
  ASSERT_EQ(count, 2);

  widget_on_keydown(w, (key_event_t*)key_event_init(&e, EVT_KEY_DOWN, w, TK_KEY_RETURN));
  widget_on_keyup(w, (key_event_t*)key_event_init(&e, EVT_KEY_UP, w, TK_KEY_RETURN));
  ASSERT_EQ(count, 3);

  widget_on_keydown(w, (key_event_t*)key_event_init(&e, EVT_KEY_DOWN, w, TK_KEY_RETURN));
  widget_on_keyup(w, (key_event_t*)key_event_init(&e, EVT_KEY_UP, w, TK_KEY_RETURN));
  ASSERT_EQ(count, 4);

  widget_destroy(w);
}
