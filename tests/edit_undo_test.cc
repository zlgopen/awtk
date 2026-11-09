#include "gtest/gtest.h"
#include "widgets/edit.h"
#include "mledit/mledit.h"
#include "base/text_edit.h"
#include "tkc/str.h"

static void undo_key(text_edit_t* te, widget_t* w, uint32_t key) {
  key_event_t e;
  key_event_init(&e, EVT_KEY_DOWN, w, key);
  e.ctrl = TRUE;
  text_edit_key_down(te, &e);
}

static void assert_text(widget_t* w, const char* expected) {
  str_t s;
  str_init(&s, 0);
  str_from_wstr(&s, w->text.str);
  ASSERT_STREQ(s.str == NULL ? "" : s.str, expected);
  str_reset(&s);
}

/* 设置文本 -> 撤销恢复旧文本 -> 重做恢复新文本 */
TEST(Edit, undo_set_text) {
  widget_t* w = edit_create(NULL, 0, 0, 100, 30);
  text_edit_t* te = EDIT(w)->model;

  widget_set_text(w, L"AWTK");
  ASSERT_STREQ(w->text.str, L"AWTK");

  widget_set_text(w, L"aa");
  ASSERT_STREQ(w->text.str, L"aa");

  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"AWTK");

  undo_key(te, w, TK_KEY_y);
  ASSERT_STREQ(w->text.str, L"aa");

  widget_destroy(w);
}

/* 清空文本可以撤销 */
TEST(Edit, undo_clear) {
  widget_t* w = edit_create(NULL, 0, 0, 100, 30);
  text_edit_t* te = EDIT(w)->model;

  widget_set_text(w, L"AWTK");
  ASSERT_EQ(edit_clear(EDIT(w)), RET_OK);
  ASSERT_STREQ(w->text.str, L"");

  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"AWTK");

  widget_destroy(w);
}

/* 内容不变时不再产生撤销记录 */
TEST(Edit, undo_set_same_text) {
  widget_t* w = edit_create(NULL, 0, 0, 100, 30);
  text_edit_t* te = EDIT(w)->model;

  widget_set_text(w, L"AWTK");
  widget_set_text(w, L"AWTK");

  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"");

  widget_destroy(w);
}

/* 替换指定区间：AWTK 把 [1,3) "WT" 换成 "XY" -> AXYK */
TEST(Edit, undo_replace_range) {
  widget_t* w = edit_create(NULL, 0, 0, 100, 30);
  text_edit_t* te = EDIT(w)->model;

  widget_set_text(w, L"AWTK");

  ASSERT_EQ(text_edit_replace_text(te, 1, 2, L"XY", 2), RET_OK);
  assert_text(w, "AXYK");

  undo_key(te, w, TK_KEY_z);
  assert_text(w, "AWTK");

  undo_key(te, w, TK_KEY_y);
  assert_text(w, "AXYK");

  widget_destroy(w);
}

/* 程序插入的文本也可以撤销 */
TEST(Edit, undo_insert_text) {
  widget_t* w = edit_create(NULL, 0, 0, 100, 30);
  text_edit_t* te = EDIT(w)->model;

  widget_set_text(w, L"AWTK");

  ASSERT_EQ(text_edit_insert_text(te, 2, "XY"), RET_OK);
  assert_text(w, "AWXYTK");

  undo_key(te, w, TK_KEY_z);
  assert_text(w, "AWTK");

  widget_destroy(w);
}

/* mledit 设置文本同样可以撤销 */
TEST(Mledit, undo_set_text) {
  widget_t* w = mledit_create(NULL, 0, 0, 100, 100);
  text_edit_t* te = MLEDIT(w)->model;

  widget_set_text(w, L"AWTK");
  widget_set_text(w, L"aa");
  ASSERT_STREQ(w->text.str, L"aa");

  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"AWTK");

  widget_destroy(w);
}

/* 受 max_chars 限制被截断时，撤销仍然恢复旧文本（记录长度按实际插入长度修正） */
TEST(Mledit, undo_set_text_exceed_max_chars) {
  widget_t* w = mledit_create(NULL, 0, 0, 100, 100);
  text_edit_t* te = MLEDIT(w)->model;

  ASSERT_EQ(mledit_set_max_chars(w, 4), RET_OK);
  widget_set_text(w, L"AWTK");
  assert_text(w, "AWTK");

  widget_set_text(w, L"1234567890");
  assert_text(w, "1234");

  undo_key(te, w, TK_KEY_z);
  assert_text(w, "AWTK");

  widget_destroy(w);
}
