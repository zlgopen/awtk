#include "base/canvas.h"
#include "base/widget.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>
#include "mledit/mledit.h"
#include "base/window.h"
#include "common.h"
#include "lcd/lcd_mem_rgba8888.h"

TEST(MLEdit, basic) {
  widget_t* b = mledit_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(MLEDIT(b)->readonly, FALSE);
  ASSERT_EQ(widget_set_prop_bool(b, WIDGET_PROP_READONLY, TRUE), RET_OK);
  ASSERT_EQ(MLEDIT(b)->readonly, TRUE);
  ASSERT_EQ(widget_get_prop_bool(b, WIDGET_PROP_READONLY, FALSE), TRUE);

  widget_destroy(b);
}

TEST(MLEdit, int) {
  value_t v1;
  value_t v2;
  widget_t* b = mledit_create(NULL, 10, 20, 30, 40);

  value_set_int(&v1, 20);
  ASSERT_EQ(widget_set_prop(b, MLEDIT_PROP_MAX_LINES, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, MLEDIT_PROP_MAX_LINES, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 12);
  ASSERT_EQ(widget_set_prop(b, MLEDIT_PROP_SCROLL_LINE, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, MLEDIT_PROP_SCROLL_LINE, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_bool(&v1, true);
  ASSERT_EQ(widget_set_prop(b, MLEDIT_PROP_WRAP_WORD, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, MLEDIT_PROP_WRAP_WORD, &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));

  value_set_bool(&v1, false);
  ASSERT_EQ(widget_set_prop(b, MLEDIT_PROP_WRAP_WORD, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, MLEDIT_PROP_WRAP_WORD, &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));

  widget_destroy(b);
}

TEST(MLEdit, tips) {
  widget_t* b = mledit_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_set_prop_str(b, WIDGET_PROP_TIPS, "tips"), RET_OK);
  ASSERT_STREQ(MLEDIT(b)->tips, "tips");

  ASSERT_EQ(widget_set_prop_str(b, WIDGET_PROP_TR_TIPS, "tr_tips"), RET_OK);
  ASSERT_STREQ(MLEDIT(b)->tr_tips, "tr_tips");

  widget_destroy(b);
}

TEST(MLEdit, events) {
  string event_log;
  widget_t* b = mledit_create(NULL, 10, 20, 30, 40);

  widget_on(b, EVT_VALUE_CHANGING, widget_log_events, &event_log);
  widget_on(b, EVT_VALUE_CHANGED, widget_log_events, &event_log);

  event_log = "";
  widget_set_text(b, L"a");
  ASSERT_EQ(wcscmp(widget_get_text(b), L"a"), 0);
  ASSERT_EQ(event_log, "value_changed");
  ASSERT_EQ(wcscmp(b->text.str, L"a"), 0);

  event_log = "";
  widget_set_text(b, L"a");
  ASSERT_EQ(event_log, "");
  ASSERT_EQ(wcscmp(b->text.str, L"a"), 0);

  event_log = "";
  widget_set_text(b, L"1");
  ASSERT_EQ(event_log, "value_changed");
  ASSERT_EQ(wcscmp(b->text.str, L"1"), 0);

  widget_destroy(b);
}

TEST(MLEdit, intputing1) {
  pointer_event_t evt;
  widget_t* w1 = mledit_create(NULL, 10, 20, 30, 40);
  pointer_event_init(&evt, EVT_POINTER_DOWN, w1, 0, 0);
  ASSERT_EQ(widget_get_prop_bool(w1, WIDGET_PROP_INPUTING, TRUE), FALSE);

  widget_set_focused(w1, TRUE);
  widget_dispatch(w1, (event_t*)&evt);
  ASSERT_EQ(widget_get_prop_bool(w1, WIDGET_PROP_INPUTING, TRUE), TRUE);

  widget_destroy(w1);
}

TEST(MLEdit, intputing2) {
  key_event_t evt;
  widget_t* w1 = mledit_create(NULL, 10, 20, 30, 40);
  key_event_init(&evt, EVT_KEY_DOWN, w1, 0);
  ASSERT_EQ(widget_get_prop_bool(w1, WIDGET_PROP_INPUTING, TRUE), FALSE);

  widget_set_focused(w1, TRUE);
  widget_dispatch(w1, (event_t*)&evt);
  ASSERT_EQ(widget_get_prop_bool(w1, WIDGET_PROP_INPUTING, TRUE), TRUE);

  widget_destroy(w1);
}

TEST(MLEdit, intputing3) {
  wheel_event_t evt;
  widget_t* w1 = mledit_create(NULL, 10, 20, 30, 40);
  wheel_event_init(&evt, EVT_WHEEL, w1, 0);
  ASSERT_EQ(widget_get_prop_bool(w1, WIDGET_PROP_INPUTING, TRUE), FALSE);

  widget_set_focused(w1, TRUE);
  widget_dispatch(w1, (event_t*)&evt);
  ASSERT_EQ(widget_get_prop_bool(w1, WIDGET_PROP_INPUTING, TRUE), TRUE);

  widget_destroy(w1);
}

TEST(MLEdit, open_im_when_focused) {
  widget_t* w1 = mledit_create(NULL, 10, 20, 30, 40);
  ASSERT_EQ(widget_get_prop_bool(w1, WIDGET_PROP_OPEN_IM_WHEN_FOCUSED, FALSE), TRUE);

  mledit_set_open_im_when_focused(w1, FALSE);
  ASSERT_EQ(widget_get_prop_bool(w1, WIDGET_PROP_OPEN_IM_WHEN_FOCUSED, TRUE), FALSE);

  widget_destroy(w1);
}

TEST(MLEdit, close_im_when_blured) {
  widget_t* w1 = mledit_create(NULL, 10, 20, 30, 40);
  ASSERT_EQ(widget_get_prop_bool(w1, WIDGET_PROP_CLOSE_IM_WHEN_BLURED, FALSE), TRUE);

  mledit_set_close_im_when_blured(w1, FALSE);
  ASSERT_EQ(widget_get_prop_bool(w1, WIDGET_PROP_CLOSE_IM_WHEN_BLURED, TRUE), FALSE);

  widget_destroy(w1);
}

TEST(MLEdit, keys) {
  key_event_t key;
  widget_t* e = mledit_create(NULL, 10, 20, 30, 40);

  key_event_init(&key, EVT_KEY_DOWN, e, TK_KEY_ESCAPE);
  ASSERT_EQ(widget_dispatch(e, (event_t*)&key), RET_OK);

  key_event_init(&key, EVT_KEY_DOWN, e, TK_KEY_F1);
  ASSERT_EQ(widget_dispatch(e, (event_t*)&key), RET_OK);

  key_event_init(&key, EVT_KEY_DOWN, e, TK_KEY_F10);
  ASSERT_EQ(widget_dispatch(e, (event_t*)&key), RET_OK);

  widget_destroy(e);
}

TEST(MLEdit, insert_text_overwrite) {
  char get_text[32] = {0};
  const char* str = "1\n2\n3\n4\n5";
  const char* str2 = "1\r\n2\r\n3\r\n4\r\n5";
  canvas_t c;
  lcd_t* lcd = lcd_mem_rgba8888_create(150, 150, TRUE);
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* e = mledit_create(win, 10, 20, 30, 40);
  mledit_t* mledit = MLEDIT(e);
  text_edit_state_t state;
  text_edit_get_state(mledit->model, &state);

  canvas_init(&c, lcd, font_manager());
  widget_set_prop_pointer(win, WIDGET_PROP_CANVAS, &c);
  mledit_set_max_lines(e, 5);
  mledit_set_overwrite(e, TRUE);

  widget_set_text_utf8(e, str);
  mledit_insert_text(e, e->text.size - 1, "6\n7");
  widget_get_text_utf8(e, get_text, sizeof(get_text));
  ASSERT_STREQ(get_text, "2\n3\n4\n6\n7");

  memset(get_text, 0, sizeof(get_text));
  widget_set_text_utf8(e, str2);
  mledit_insert_text(e, e->text.size - 1, "6\r\n7");
  widget_get_text_utf8(e, get_text, sizeof(get_text));
  ASSERT_STREQ(get_text, "2\r\n3\r\n4\r\n6\r\n7");

  memset(get_text, 0, sizeof(get_text));
  widget_set_text_utf8(e, str);
  mledit_insert_text(e, e->text.size - 3, "6\n");
  widget_get_text_utf8(e, get_text, sizeof(get_text));
  ASSERT_STREQ(get_text, "1\n2\n3\n6\n4");

  memset(get_text, 0, sizeof(get_text));
  widget_set_text_utf8(e, str);
  mledit_insert_text(e, -1, "\n6\n7");
  widget_get_text_utf8(e, get_text, sizeof(get_text));
  ASSERT_STREQ(get_text, "3\n4\n5\n6\n7");

  memset(get_text, 0, sizeof(get_text));
  widget_set_text_utf8(e, str);
  mledit_insert_text(e, 0, "0\n");
  widget_get_text_utf8(e, get_text, sizeof(get_text));
  ASSERT_STREQ(get_text, "0\n1\n2\n3\n4");

  memset(get_text, 0, sizeof(get_text));
  mledit_set_max_lines(e, 7);
  widget_set_text_utf8(e, str);
  mledit_insert_text(e, 0, "0\n");
  widget_get_text_utf8(e, get_text, sizeof(get_text));
  ASSERT_STREQ(get_text, "0\n1\n2\n3\n4\n5");
  mledit_set_max_lines(e, 5);

  memset(get_text, 0, sizeof(get_text));
  mledit_set_max_lines(e, 7);
  widget_set_text_utf8(e, str);
  mledit_insert_text(e, e->text.size - 1, "4.5\n");
  widget_get_text_utf8(e, get_text, sizeof(get_text));
  ASSERT_STREQ(get_text, "1\n2\n3\n4\n4.5\n5");
  mledit_set_max_lines(e, 5);

  memset(get_text, 0, sizeof(get_text));
  mledit_set_max_lines(e, 7);
  widget_set_text_utf8(e, str);
  mledit_insert_text(e, -1, "\n6\n7");
  widget_get_text_utf8(e, get_text, sizeof(get_text));
  ASSERT_STREQ(get_text, "1\n2\n3\n4\n5\n6\n7");
  mledit_set_max_lines(e, 5);

  memset(get_text, 0, sizeof(get_text));
  mledit_set_max_lines(e, 1);
  widget_set_text_utf8(e, "1");
  mledit_insert_text(e, -1, "6\n7");
  widget_get_text_utf8(e, get_text, sizeof(get_text));
  ASSERT_STREQ(get_text, "7");
  mledit_set_max_lines(e, 5);

  memset(get_text, 0, sizeof(get_text));
  mledit_set_max_lines(e, 2);
  widget_set_text_utf8(e, "1");
  mledit_insert_text(e, -1, "6\n7\n8");
  widget_get_text_utf8(e, get_text, sizeof(get_text));
  ASSERT_STREQ(get_text, "7\n8");
  mledit_set_max_lines(e, 5);

  memset(get_text, 0, sizeof(get_text));
  widget_set_text_utf8(e, str);
  mledit_set_cursor(e, 1);
  mledit_insert_text(e, e->text.size - 1, "6\n7");
  widget_get_text_utf8(e, get_text, sizeof(get_text));
  ASSERT_STREQ(get_text, "2\n3\n4\n6\n7");
  ASSERT_EQ(mledit_get_cursor(e), e->text.size);

  widget_set_text_utf8(e, str);
  mledit_set_select(e, 2, 4);
  mledit_insert_text(e, -1, "\n6");
  text_edit_get_state(mledit->model, &state);
  ASSERT_EQ(state.select_start, 0);
  ASSERT_EQ(state.select_end, 2);

  widget_set_text_utf8(e, str);
  mledit_set_select(e, 2, 4);
  mledit_insert_text(e, -1, "\n6\n7");
  text_edit_get_state(mledit->model, &state);
  ASSERT_EQ(state.select_start, state.select_end);

  widget_set_text_utf8(e, str);
  mledit_set_select(e, e->text.size - 3, e->text.size);
  mledit_insert_text(e, e->text.size - 1, "\n6\n7");
  text_edit_get_state(mledit->model, &state);
  ASSERT_EQ(state.select_start, state.select_end);

  widget_set_prop_pointer(win, WIDGET_PROP_CANVAS, NULL);
  widget_destroy(win);
  canvas_reset(&c);
  lcd_destroy(lcd);
}
