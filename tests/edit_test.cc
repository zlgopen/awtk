#include "base/canvas.h"
#include "base/widget.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>
#include "widgets/edit.h"
#include "base/window.h"
#include "widgets/group_box.h"
#include "common.h"

TEST(Edit, int) {
  value_t v1;
  value_t v2;
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(edit_set_int_limit(b, 1, 100, 5), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(1, value_int(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(100, value_int(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_STEP, &v2), RET_OK);
  ASSERT_EQ(5, value_int(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_INPUT_TYPE, &v2), RET_OK);
  ASSERT_EQ(INPUT_INT, value_int(&v2));

  value_set_int(&v1, 10);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 100);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MAX, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 10);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_STEP, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_STEP, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  widget_destroy(b);
}

TEST(Edit, tips) {
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_set_prop_str(b, WIDGET_PROP_TIPS, "tips"), RET_OK);
  ASSERT_STREQ(EDIT(b)->tips, "tips");

  ASSERT_EQ(widget_set_prop_str(b, WIDGET_PROP_TR_TIPS, "tr_tips"), RET_OK);
  ASSERT_STREQ(EDIT(b)->tr_tips, "tr_tips");

  widget_destroy(b);
}

TEST(Edit, set_text) {
  text_edit_state_t state;
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);
  widget_set_text_utf8(b, "hello");

  ASSERT_EQ(text_edit_get_state(EDIT(b)->model, &state), RET_OK);
  ASSERT_EQ(state.cursor, 5);

  widget_destroy(b);
}

TEST(Edit, set_int) {
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);
  ASSERT_EQ(edit_set_int(b, -10), RET_OK);
  ASSERT_EQ(edit_get_int(b), -10);

  widget_destroy(b);
}

TEST(Edit, action_text) {
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);

  ASSERT_STREQ(EDIT(b)->action_text, "done");
  ASSERT_EQ(widget_set_prop_str(b, WIDGET_PROP_ACTION_TEXT, "next"), RET_OK);
  ASSERT_STREQ(EDIT(b)->action_text, "next");
  ASSERT_STREQ(widget_get_prop_str(b, WIDGET_PROP_ACTION_TEXT, ""), "next");

  widget_destroy(b);
}

TEST(Edit, inputable) {
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);
  ASSERT_EQ(b->vt->inputable, TRUE);
  widget_destroy(b);
}

TEST(Edit, focusable) {
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);
  ASSERT_EQ(b->vt->focusable, TRUE);
  widget_destroy(b);
}

TEST(Edit, text) {
  value_t v1;
  value_t v2;
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(edit_set_text_limit(b, 0, 100), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(0, value_int(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_INPUT_TYPE, &v2), RET_OK);
  ASSERT_EQ(INPUT_TEXT, value_int(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(100, value_int(&v2));

  value_set_int(&v1, 10);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 100);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MAX, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  widget_destroy(b);
}

TEST(Edit, float) {
  value_t v1;
  value_t v2;
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(edit_set_float_limit(b, 0, 100, 0.1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(0, value_float(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_INPUT_TYPE, &v2), RET_OK);
  ASSERT_EQ(INPUT_FLOAT, value_int(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(100, value_float(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_STEP, &v2), RET_OK);
  ASSERT_EQ(0.1 - value_float(&v2) < 0.001, true);

  value_set_int(&v1, 10);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(value_float(&v1), value_float(&v2));

  value_set_int(&v1, 100);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MAX, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(value_float(&v1), value_float(&v2));

  widget_destroy(b);
}

TEST(Edit, basic) {
  value_t v1;
  value_t v2;
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(edit_set_readonly(b, TRUE), RET_OK);
  ASSERT_EQ(EDIT(b)->readonly, TRUE);

  value_set_bool(&v1, FALSE);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_READONLY, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_READONLY, &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));

  ASSERT_EQ(edit_set_auto_fix(b, TRUE), RET_OK);
  ASSERT_EQ(EDIT(b)->auto_fix, TRUE);

  value_set_bool(&v1, FALSE);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_READONLY, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_READONLY, &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));

  value_set_int(&v1, 3);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_LEFT_MARGIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_LEFT_MARGIN, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 3);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_RIGHT_MARGIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_RIGHT_MARGIN, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 3);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_TOP_MARGIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_TOP_MARGIN, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 3);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_BOTTOM_MARGIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_BOTTOM_MARGIN, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  widget_destroy(b);
}

TEST(Edit, cast) {
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);
  ASSERT_EQ(b, edit_cast(b));
  ASSERT_EQ((edit_t*)NULL, EDIT(NULL));
  widget_destroy(b);
}

TEST(Edit, focus) {
  widget_t* w = window_create(NULL, 0, 0, 400, 400);
  widget_t* g = group_box_create(w, 0, 0, 400, 400);
  widget_t* e = edit_create(g, 10, 20, 30, 40);

  ASSERT_EQ(edit_set_focus(e, TRUE), RET_OK);
  ASSERT_EQ(w->key_target, g);
  ASSERT_EQ(g->key_target, e);

  widget_destroy(w);
}

TEST(Edit, focus_next) {
  widget_t* w = window_create(NULL, 0, 0, 400, 400);
  widget_t* g = group_box_create(w, 0, 0, 400, 400);
  widget_t* e1 = edit_create(g, 10, 20, 30, 40);
  widget_t* e2 = edit_create(g, 10, 60, 30, 40);
  widget_t* e3 = edit_create(g, 10, 90, 30, 40);

  ASSERT_EQ(edit_set_focus(e1, TRUE), RET_OK);
  ASSERT_EQ(g->key_target, e1);
  ASSERT_EQ(widget_focus_next(e1), RET_OK);
  ASSERT_EQ(g->key_target, e2);
  ASSERT_EQ(widget_focus_next(e1), RET_FAIL);

  ASSERT_EQ(widget_focus_next(e2), RET_OK);
  ASSERT_EQ(g->key_target, e3);

  ASSERT_EQ(widget_focus_next(e3), RET_OK);
  ASSERT_EQ(g->key_target, e1);

  widget_destroy(w);
}

TEST(Edit, focus_prev) {
  widget_t* w = window_create(NULL, 0, 0, 400, 400);
  widget_t* g = group_box_create(w, 0, 0, 400, 400);
  widget_t* e1 = edit_create(g, 10, 20, 30, 40);
  widget_t* e2 = edit_create(g, 10, 60, 30, 40);
  widget_t* e3 = edit_create(g, 10, 90, 30, 40);

  ASSERT_EQ(edit_set_focus(e1, TRUE), RET_OK);
  ASSERT_EQ(g->key_target, e1);

  ASSERT_EQ(widget_focus_prev(e1), RET_OK);
  ASSERT_EQ(g->key_target, e3);
  ASSERT_EQ(widget_focus_prev(e1), RET_FAIL);

  ASSERT_EQ(widget_focus_prev(e3), RET_OK);
  ASSERT_EQ(g->key_target, e2);

  ASSERT_EQ(widget_focus_prev(e2), RET_OK);
  ASSERT_EQ(g->key_target, e1);

  widget_destroy(w);
}

TEST(Edit, is_valid_chr_4_str) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_text_limit(e, 0, 3);
  ASSERT_EQ(edit_input_char(e, L'a'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'b'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'c'), RET_OK);
  ASSERT_NE(edit_input_char(e, L'd'), RET_OK);
  ASSERT_EQ(e->text.size, 3);

  widget_destroy(e);
}

TEST(Edit, is_valid_chr_4_int) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_input_type(e, INPUT_INT);
  edit_set_int_limit(e, -1000, 1000, 10);
  ASSERT_NE(edit_input_char(e, L'd'), RET_OK);

  ASSERT_EQ(edit_input_char(e, L'-'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'1'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);

  ASSERT_NE(edit_input_char(e, L'd'), RET_OK);

  ASSERT_EQ(e->text.size, 5);

  widget_destroy(e);
}

TEST(Edit, is_valid_chr_4_uint) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_input_type(e, INPUT_UINT);
  edit_set_int_limit(e, 0, 1000, 10);
  ASSERT_NE(edit_input_char(e, L'd'), RET_OK);

  ASSERT_NE(edit_input_char(e, L'-'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'1'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);

  ASSERT_NE(edit_input_char(e, L'd'), RET_OK);

  ASSERT_EQ(e->text.size, 4);

  widget_destroy(e);
}

TEST(Edit, is_valid_chr_4_float) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_input_type(e, INPUT_FLOAT);
  edit_set_float_limit(e, -1000, 1000, 10);
  ASSERT_NE(edit_input_char(e, L'd'), RET_OK);

  ASSERT_EQ(edit_input_char(e, L'-'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'1'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'.'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);

  ASSERT_NE(edit_input_char(e, L'd'), RET_OK);
  ASSERT_NE(edit_input_char(e, L'.'), RET_OK);

  widget_destroy(e);
}

TEST(Edit, is_valid_chr_4_ufloat) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_input_type(e, INPUT_UFLOAT);
  edit_set_float_limit(e, 0, 1000, 10);
  ASSERT_NE(edit_input_char(e, L'd'), RET_OK);

  ASSERT_NE(edit_input_char(e, L'-'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'+'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'1'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'.'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);

  ASSERT_NE(edit_input_char(e, L'.'), RET_OK);
  ASSERT_NE(edit_input_char(e, L'd'), RET_OK);

  widget_destroy(e);
}

TEST(Edit, is_valid_chr_4_email) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_input_type(e, INPUT_EMAIL);
  ASSERT_EQ(edit_input_char(e, L'a'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'a'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'a'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'@'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);

  ASSERT_NE(edit_input_char(e, L'@'), RET_OK);

  widget_destroy(e);
}

TEST(Edit, is_valid_chr_4_hex) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_input_type(e, INPUT_HEX);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'x'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'1'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'2'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'3'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'4'), RET_OK);

  ASSERT_NE(edit_input_char(e, L'x'), RET_OK);

  widget_destroy(e);
}

TEST(Edit, is_valid_chr_4_phone) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_input_type(e, INPUT_PHONE);
  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'1'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'1'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'2'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'3'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'4'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'-'), RET_OK);

  ASSERT_NE(edit_input_char(e, L'x'), RET_OK);
  ASSERT_NE(edit_input_char(e, L'a'), RET_OK);

  widget_destroy(e);
}

static bool_t any_char_is_valid(widget_t* widget, wchar_t c) {
  return TRUE;
}

TEST(Edit, is_valid_chr_4_custom) {
  widget_t* e = edit_create(NULL, 10, 20, 30, 40);

  edit_set_input_type(e, INPUT_PHONE);
  edit_set_is_valid_char(e, any_char_is_valid);

  ASSERT_EQ(edit_input_char(e, L'0'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'1'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'1'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'2'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'3'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'4'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'-'), RET_OK);

  ASSERT_EQ(edit_input_char(e, L'x'), RET_OK);
  ASSERT_EQ(edit_input_char(e, L'a'), RET_OK);

  widget_destroy(e);
}

TEST(Edit, events) {
  string event_log;
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);

  widget_on(b, EVT_VALUE_CHANGING, widget_log_events, &event_log);
  widget_on(b, EVT_VALUE_CHANGED, widget_log_events, &event_log);

  event_log = "";
  widget_set_text(b, L"a");
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

  event_log = "";
  edit_input_char(b, 'a');
  ASSERT_EQ(event_log, "value_changing");
  ASSERT_EQ(wcscmp(b->text.str, L"1a"), 0);

  widget_destroy(b);
}

TEST(Edit, clone) {
  widget_t* w1 = edit_create(NULL, 10, 20, 30, 40);
  widget_t* w2 = widget_clone(w1, NULL);

  ASSERT_EQ(widget_equal(w1, w2), TRUE);

  widget_destroy(w1);
  widget_destroy(w2);
}

TEST(Edit, value) {
  value_t v;
  widget_t* w1 = edit_create(NULL, 10, 20, 30, 40);
  widget_set_text(w1, L"123");
  edit_set_input_type(w1, INPUT_INT);
  ASSERT_EQ(widget_get_prop(w1, WIDGET_PROP_VALUE, &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_INT32);
  ASSERT_EQ(value_int(&v), 123);

  edit_set_input_type(w1, INPUT_UINT);
  ASSERT_EQ(widget_get_prop(w1, WIDGET_PROP_VALUE, &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_UINT32);
  ASSERT_EQ(value_int(&v), 123);

  edit_set_input_type(w1, INPUT_FLOAT);
  ASSERT_EQ(widget_get_prop(w1, WIDGET_PROP_VALUE, &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_DOUBLE);
  ASSERT_EQ(value_int(&v), 123);

  edit_set_input_type(w1, INPUT_UFLOAT);
  ASSERT_EQ(widget_get_prop(w1, WIDGET_PROP_VALUE, &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_DOUBLE);
  ASSERT_EQ(value_int(&v), 123);

  widget_destroy(w1);
}

TEST(Edit, intputing2) {
  value_t v;
  key_event_t evt;
  widget_t* w1 = edit_create(NULL, 10, 20, 30, 40);
  key_event_init(&evt, EVT_KEY_DOWN, w1, 0);
  ASSERT_EQ(widget_get_prop_bool(w1, WIDGET_PROP_INPUTING, TRUE), FALSE);

  widget_dispatch(w1, (event_t*)&evt);
  widget_set_focused(w1, TRUE);
  ASSERT_EQ(widget_get_prop_bool(w1, WIDGET_PROP_INPUTING, TRUE), TRUE);

  widget_destroy(w1);
}

TEST(Edit, intputing3) {
  value_t v;
  wheel_event_t evt;
  widget_t* w1 = edit_create(NULL, 10, 20, 30, 40);
  wheel_event_init(&evt, EVT_WHEEL, w1, 0);
  ASSERT_EQ(widget_get_prop_bool(w1, WIDGET_PROP_INPUTING, TRUE), FALSE);

  widget_set_focused(w1, TRUE);
  widget_dispatch(w1, (event_t*)&evt);
  ASSERT_EQ(widget_get_prop_bool(w1, WIDGET_PROP_INPUTING, TRUE), TRUE);

  widget_destroy(w1);
}
