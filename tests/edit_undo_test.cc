#include "gtest/gtest.h"
#include "widgets/edit.h"
#include "mledit/mledit.h"
#include "base/text_edit.h"
#include "tkc/str.h"
#include "base/clip_board.h"
#include "clip_board/clip_board_default.h"

static void undo_key(text_edit_t* te, widget_t* w, uint32_t key) {
  key_event_t e;
  key_event_init(&e, EVT_KEY_DOWN, w, key);
#ifdef MACOS
  e.cmd = TRUE;
#else
  e.ctrl = TRUE;
#endif
  text_edit_key_down(te, &e);
}

/* 带 ctrl（macOS 下为 cmd）的按键，撤销/重做/删词等快捷键共用 */
static void press_ctrl_key(text_edit_t* te, widget_t* w, uint32_t key) {
  key_event_t e;
  key_event_init(&e, EVT_KEY_DOWN, w, key);
#ifdef MACOS
  e.cmd = TRUE;
#else
  e.ctrl = TRUE;
#endif
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

/* 撤销后恢复操作前的选区，重做后光标落在编辑结束的位置 */
TEST(Edit, undo_restore_selection) {
  widget_t* w = edit_create(NULL, 0, 0, 100, 30);
  text_edit_t* te = EDIT(w)->model;
  text_edit_state_t state;

  widget_set_text(w, L"AWTK");
  ASSERT_EQ(text_edit_set_cursor(te, 4), RET_OK);
  ASSERT_EQ(text_edit_set_select(te, 1, 4), RET_OK);

  widget_set_text(w, L"aa");
  ASSERT_STREQ(w->text.str, L"aa");

  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"AWTK");
  ASSERT_EQ(text_edit_get_state(te, &state), RET_OK);
  ASSERT_EQ(state.cursor, 4u);
  ASSERT_EQ(state.select_start, 1u);
  ASSERT_EQ(state.select_end, 4u);

  undo_key(te, w, TK_KEY_y);
  ASSERT_STREQ(w->text.str, L"aa");
  ASSERT_EQ(text_edit_get_state(te, &state), RET_OK);
  /* 重做后光标落在这次编辑结束的位置（"aa" 末尾），不是撤销那一刻的实时光标 */
  ASSERT_EQ(state.cursor, 2u);
  ASSERT_EQ(state.select_start, 2u);
  ASSERT_EQ(state.select_end, 2u);

  widget_destroy(w);
}

/* 编辑次数超过可保存的历史深度时，全部撤销后再全部重做，文本应完全恢复 */
TEST(Edit, undo_many) {
  uint32_t i = 0;
  widget_t* w = edit_create(NULL, 0, 0, 100, 30);
  text_edit_t* te = EDIT(w)->model;

  for (i = 0; i < 120; i++) {
    ASSERT_EQ(text_edit_insert_text(te, w->text.size, "x"), RET_OK);
  }
  ASSERT_EQ(w->text.size, 120u);

  for (i = 0; i < 120; i++) {
    undo_key(te, w, TK_KEY_z);
  }
  ASSERT_TRUE(w->text.size < 120u);

  for (i = 0; i < 120; i++) {
    undo_key(te, w, TK_KEY_y);
  }
  ASSERT_EQ(w->text.size, 120u);

  widget_destroy(w);
}

#if defined(WITH_SDL)
#define UNDO_TEST_CHAR_BUDGET 999
#else
#define UNDO_TEST_CHAR_BUDGET 32
#endif

/* 重做字符缓冲不足时会丢弃最老的重做记录；剩余记录的重做必须仍然正确 */
TEST(Edit, undo_redo_chars) {
  static char big[UNDO_TEST_CHAR_BUDGET + 1];
  const uint32_t chunk = UNDO_TEST_CHAR_BUDGET / 4;
  uint32_t i = 0;
  widget_t* w = edit_create(NULL, 0, 0, 100, 30);
  text_edit_t* te = EDIT(w)->model;

  for (i = 0; i < 5; i++) {
    memset(big, (int)('a' + i), chunk);
    big[chunk] = 0;
    ASSERT_EQ(text_edit_insert_text(te, w->text.size, big), RET_OK);
  }
  ASSERT_EQ(w->text.size, 5 * chunk);

  for (i = 0; i < 5; i++) {
    undo_key(te, w, TK_KEY_z);
  }
  ASSERT_EQ(w->text.size, 0u);

  /* 第 5 次撤销时字符缓冲不够，最老的一条重做记录被丢弃 */
  undo_key(te, w, TK_KEY_y);
  ASSERT_EQ(w->text.size, chunk);
  ASSERT_EQ(w->text.str[0], L'a');

  undo_key(te, w, TK_KEY_y);
  ASSERT_EQ(w->text.size, 2 * chunk);

  undo_key(te, w, TK_KEY_y);
  ASSERT_EQ(w->text.size, 3 * chunk);

  undo_key(te, w, TK_KEY_y);
  ASSERT_EQ(w->text.size, 4 * chunk);
  ASSERT_EQ(w->text.str[chunk - 1], L'a');
  ASSERT_EQ(w->text.str[chunk], L'b');
  ASSERT_EQ(w->text.str[4 * chunk - 1], L'd');

  /* 被丢弃的记录不再重做 */
  undo_key(te, w, TK_KEY_y);
  ASSERT_EQ(w->text.size, 4 * chunk);

  widget_destroy(w);
}

static void type_text(text_edit_t* te, widget_t* w, const wchar_t* s) {
  key_event_t e;
  uint32_t i = 0;
  for (i = 0; s[i]; i++) {
    key_event_init(&e, EVT_KEY_DOWN, w, (int32_t)s[i]);
    text_edit_key_down(te, &e);
  }
}

static void press_key(text_edit_t* te, widget_t* w, uint32_t key) {
  key_event_t e;
  key_event_init(&e, EVT_KEY_DOWN, w, key);
  text_edit_key_down(te, &e);
}

/* 连续输入合并成一条记录；重做后光标回到输入结束处 */
TEST(Edit, undo_merge_typing) {
  widget_t* w = edit_create(NULL, 0, 0, 100, 30);
  text_edit_t* te = EDIT(w)->model;
  text_edit_state_t st;

  type_text(te, w, L"hello");
  ASSERT_STREQ(w->text.str, L"hello");

  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"");
  undo_key(te, w, TK_KEY_y);
  ASSERT_STREQ(w->text.str, L"hello");
  ASSERT_EQ(text_edit_get_state(te, &st), RET_OK);
  ASSERT_EQ(st.cursor, 5u);
  ASSERT_EQ(st.select_start, st.select_end);

  widget_destroy(w);
}

/* 只有半角空格断组：空格归到后面那一段，标点不断组（对齐 VS Code） */
TEST(Edit, undo_group_by_space) {
  widget_t* w = edit_create(NULL, 0, 0, 100, 30);
  text_edit_t* te = EDIT(w)->model;

  type_text(te, w, L"hello world");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"hello");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"");

  widget_destroy(w);

  /* 标点不是分组边界 */
  w = edit_create(NULL, 0, 0, 100, 30);
  te = EDIT(w)->model;
  type_text(te, w, L"a,b.c");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"");

  widget_destroy(w);

  /* 连续空格：第一个空格开始新分组，连续的空格单独一段 */
  w = edit_create(NULL, 0, 0, 100, 30);
  te = EDIT(w)->model;
  type_text(te, w, L"abc  d");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"abc  ");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"abc");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"");

  widget_destroy(w);
}

/* 换行是独立的一步，之后的输入另起一组（mledit） */
TEST(Mledit, undo_enter_group) {
  widget_t* w = mledit_create(NULL, 0, 0, 100, 100);
  text_edit_t* te = MLEDIT(w)->model;

  type_text(te, w, L"a");
  press_key(te, w, TK_KEY_RETURN);
  type_text(te, w, L"b");
  ASSERT_STREQ(w->text.str, L"a\nb");

  /* 第一次只退掉换行之后的 b，第二次才退掉换行 */
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"a\n");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"a");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"");

  widget_destroy(w);
}

/* 光标移动（含移走又移回）都结束当前分组 */
TEST(Edit, undo_group_break_on_move) {
  widget_t* w = edit_create(NULL, 0, 0, 100, 30);
  text_edit_t* te = EDIT(w)->model;

  type_text(te, w, L"ab");
  ASSERT_EQ(text_edit_set_cursor(te, 0), RET_OK);
  ASSERT_EQ(text_edit_set_cursor(te, 2), RET_OK);
  type_text(te, w, L"c");
  ASSERT_STREQ(w->text.str, L"abc");

  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"ab");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"");

  /* 方向键同样打断分组 */
  type_text(te, w, L"ab");
  press_key(te, w, TK_KEY_LEFT);
  press_key(te, w, TK_KEY_RIGHT);
  type_text(te, w, L"c");
  ASSERT_STREQ(w->text.str, L"abc");

  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"ab");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"");

  widget_destroy(w);
}

/* 连续删除一律合并，不按词边界断开（对齐 VS Code） */
TEST(Edit, undo_merge_delete) {
  widget_t* w = edit_create(NULL, 0, 0, 100, 30);
  text_edit_t* te = EDIT(w)->model;
  uint32_t i = 0;

  /* 退格连删跨过空格，仍然是一条记录 */
  widget_set_text(w, L"hello world");
  ASSERT_EQ(text_edit_set_cursor(te, 11), RET_OK);
  for (i = 0; i < 7; i++) {
    press_key(te, w, TK_KEY_BACKSPACE);
  }
  ASSERT_STREQ(w->text.str, L"hell");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"hello world");
  undo_key(te, w, TK_KEY_y);
  ASSERT_STREQ(w->text.str, L"hell");

  widget_destroy(w);

  /* Delete 向后连删，跨过空格同样是一条记录 */
  w = edit_create(NULL, 0, 0, 100, 30);
  te = EDIT(w)->model;
  widget_set_text(w, L"ab cd");
  ASSERT_EQ(text_edit_set_cursor(te, 0), RET_OK);
  for (i = 0; i < 5; i++) {
    press_key(te, w, TK_KEY_DELETE);
  }
  ASSERT_STREQ(w->text.str, L"");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"ab cd");

  widget_destroy(w);

  /* 退格和 Delete 是不同命令，交替使用要分两步撤销 */
  w = edit_create(NULL, 0, 0, 100, 30);
  te = EDIT(w)->model;
  widget_set_text(w, L"abcdef");
  ASSERT_EQ(text_edit_set_cursor(te, 3), RET_OK);
  press_key(te, w, TK_KEY_BACKSPACE);
  press_key(te, w, TK_KEY_DELETE);
  ASSERT_STREQ(w->text.str, L"abef");

  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"abdef");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"abcdef");

  widget_destroy(w);
}

/* 删除选区是一步完整操作，之后再输入另起一组；撤销恢复操作前的选区 */
TEST(Edit, undo_delete_selection) {
  widget_t* w = edit_create(NULL, 0, 0, 100, 30);
  text_edit_t* te = EDIT(w)->model;
  text_edit_state_t st;

  widget_set_text(w, L"abcdef");
  ASSERT_EQ(text_edit_set_cursor(te, 4), RET_OK);
  ASSERT_EQ(text_edit_set_select(te, 2, 4), RET_OK);
  press_key(te, w, TK_KEY_BACKSPACE);
  ASSERT_STREQ(w->text.str, L"abef");

  type_text(te, w, L"x");
  ASSERT_STREQ(w->text.str, L"abxef");

  /* 第一步只撤销刚输入的 x，第二步才恢复被删掉的选区 */
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"abef");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"abcdef");
  ASSERT_EQ(text_edit_get_state(te, &st), RET_OK);
  ASSERT_EQ(st.cursor, 4u);
  ASSERT_EQ(st.select_start, 2u);
  ASSERT_EQ(st.select_end, 4u);

  widget_destroy(w);
}

/* 选区被输入/粘贴替换：删除选区与第一个内容合成一步，之后按输入规则继续分组 */
TEST(Edit, undo_replace_selection) {
  widget_t* w = edit_create(NULL, 0, 0, 100, 30);
  text_edit_t* te = EDIT(w)->model;
  text_edit_state_t st;

  widget_set_text(w, L"abcdef");
  ASSERT_EQ(text_edit_set_cursor(te, 4), RET_OK);
  ASSERT_EQ(text_edit_set_select(te, 2, 4), RET_OK);
  type_text(te, w, L"xy");
  ASSERT_STREQ(w->text.str, L"abxyef");

  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"abcdef");
  ASSERT_EQ(text_edit_get_state(te, &st), RET_OK);
  ASSERT_EQ(st.select_start, 2u);
  ASSERT_EQ(st.select_end, 4u);

  /* 重做后光标落在替换结束处 */
  undo_key(te, w, TK_KEY_y);
  ASSERT_STREQ(w->text.str, L"abxyef");
  ASSERT_EQ(text_edit_get_state(te, &st), RET_OK);
  ASSERT_EQ(st.cursor, 4u);
  ASSERT_EQ(st.select_start, st.select_end);

  widget_destroy(w);

  /* 粘贴替换选区同样一步撤销 */
  w = edit_create(NULL, 0, 0, 100, 30);
  te = EDIT(w)->model;
  widget_set_text(w, L"abcdef");
  ASSERT_EQ(text_edit_set_select(te, 2, 4), RET_OK);
  ASSERT_EQ(text_edit_paste(te, L"xy", 2), RET_OK);
  ASSERT_STREQ(w->text.str, L"abxyef");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"abcdef");

  widget_destroy(w);

  /* 替换之后输入空格：空格开始新的一段（与 VS Code 一致） */
  w = edit_create(NULL, 0, 0, 100, 30);
  te = EDIT(w)->model;
  widget_set_text(w, L"abcdef");
  ASSERT_EQ(text_edit_set_cursor(te, 4), RET_OK);
  ASSERT_EQ(text_edit_set_select(te, 2, 4), RET_OK);
  type_text(te, w, L"xy z");
  ASSERT_STREQ(w->text.str, L"abxy zef");

  /* 第一次只退掉 " z" */
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"abxyef");
  /* 第二次才恢复被替换的选区 */
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"abcdef");
  ASSERT_EQ(text_edit_get_state(te, &st), RET_OK);
  ASSERT_EQ(st.select_start, 2u);
  ASSERT_EQ(st.select_end, 4u);

  /* 重做同样分两步 */
  undo_key(te, w, TK_KEY_y);
  ASSERT_STREQ(w->text.str, L"abxyef");
  undo_key(te, w, TK_KEY_y);
  ASSERT_STREQ(w->text.str, L"abxy zef");

  widget_destroy(w);

  /* 替换后的第一个字符就是空格：和删除选区算同一步 */
  w = edit_create(NULL, 0, 0, 100, 30);
  te = EDIT(w)->model;
  widget_set_text(w, L"abcdef");
  ASSERT_EQ(text_edit_set_cursor(te, 4), RET_OK);
  ASSERT_EQ(text_edit_set_select(te, 2, 4), RET_OK);
  type_text(te, w, L" xy");
  ASSERT_STREQ(w->text.str, L"ab xyef");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"abcdef");
  ASSERT_EQ(text_edit_get_state(te, &st), RET_OK);
  ASSERT_EQ(st.select_start, 2u);
  ASSERT_EQ(st.select_end, 4u);

  widget_destroy(w);

  /* Ctrl+A 全选后输入：删除与输入合成一步 */
  w = edit_create(NULL, 0, 0, 100, 30);
  te = EDIT(w)->model;
  type_text(te, w, L"ab");
  press_ctrl_key(te, w, TK_KEY_A);
  type_text(te, w, L"x");
  ASSERT_STREQ(w->text.str, L"x");

  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"ab");
  ASSERT_EQ(text_edit_get_state(te, &st), RET_OK);
  ASSERT_EQ(st.select_start, 0u);
  ASSERT_EQ(st.select_end, 2u);

  widget_destroy(w);
}

/* 一次粘贴单独成一条记录；两次剪贴板粘贴不合并 */
TEST(Edit, undo_paste) {
  clip_board_t* save_cl = clip_board();
  clip_board_t* cl = clip_board_default_create();
  widget_t* w = edit_create(NULL, 0, 0, 100, 30);
  text_edit_t* te = EDIT(w)->model;

  /* 粘贴内容里的空格、中文标点都不是分组边界 */
  widget_set_text(w, L"AWTK");
  ASSERT_EQ(text_edit_set_cursor(te, 4), RET_OK);
  ASSERT_EQ(text_edit_paste(te, L"中文 测试，OK", 8), RET_OK);
  assert_text(w, "AWTK中文 测试，OK");
  undo_key(te, w, TK_KEY_z);
  assert_text(w, "AWTK");

  widget_destroy(w);

  /* 测试环境的默认剪贴板不支持读取，这里装一个真实实现 */
  ASSERT_EQ(clip_board_set(cl), RET_OK);
  w = edit_create(NULL, 0, 0, 100, 30);
  te = EDIT(w)->model;

  ASSERT_EQ(clip_board_set_text("ab"), RET_OK);
  undo_key(te, w, TK_KEY_v);
  ASSERT_STREQ(w->text.str, L"ab");

  ASSERT_EQ(clip_board_set_text("cd"), RET_OK);
  undo_key(te, w, TK_KEY_v);
  ASSERT_STREQ(w->text.str, L"abcd");

  /* 第一次撤销只回退第二次粘贴 */
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"ab");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"");

  widget_destroy(w);

  /* 粘贴覆盖选区是一步，之后再输入另起一步 */
  ASSERT_EQ(clip_board_set_text("xy"), RET_OK);
  w = edit_create(NULL, 0, 0, 100, 30);
  te = EDIT(w)->model;
  widget_set_text(w, L"abcdef");
  ASSERT_EQ(text_edit_set_cursor(te, 4), RET_OK);
  ASSERT_EQ(text_edit_set_select(te, 2, 4), RET_OK);
  undo_key(te, w, TK_KEY_v);
  ASSERT_STREQ(w->text.str, L"abxyef");

  type_text(te, w, L"z");
  ASSERT_STREQ(w->text.str, L"abxyzef");

  /* 先退掉 z，再退掉粘贴 */
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"abxyef");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"abcdef");

  widget_destroy(w);
  ASSERT_EQ(clip_board_set(save_cl), RET_OK);
  clip_board_destroy(cl);
}

/* 输入法提交按“输入”处理：与相邻输入合并 */
TEST(Edit, undo_ime_commit) {
  widget_t* w = edit_create(NULL, 0, 0, 100, 30);
  text_edit_t* te = EDIT(w)->model;

  /* 打字后提交 "cd"，一步撤销把整串退掉 */
  type_text(te, w, L"ab");
  ASSERT_EQ(text_edit_paste(te, L"cd", 2), RET_OK);
  ASSERT_STREQ(w->text.str, L"abcd");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"");
  undo_key(te, w, TK_KEY_y);
  ASSERT_STREQ(w->text.str, L"abcd");

  widget_destroy(w);

  /* 提交之后继续打字，仍是同一组 */
  w = edit_create(NULL, 0, 0, 100, 30);
  te = EDIT(w)->model;
  ASSERT_EQ(text_edit_paste(te, L"中文", 2), RET_OK);
  type_text(te, w, L"ab");
  ASSERT_STREQ(w->text.str, L"中文ab");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"");

  widget_destroy(w);

  /* 逐字提交且中间没有空格，合并成一条 */
  w = edit_create(NULL, 0, 0, 100, 30);
  te = EDIT(w)->model;
  ASSERT_EQ(text_edit_paste(te, L"中", 1), RET_OK);
  ASSERT_EQ(text_edit_paste(te, L"，", 1), RET_OK);
  ASSERT_EQ(text_edit_paste(te, L"文", 1), RET_OK);
  assert_text(w, "中，文");
  undo_key(te, w, TK_KEY_z);
  assert_text(w, "");

  widget_destroy(w);

  /* 空格依旧断组：“中 文”分两次撤销 */
  w = edit_create(NULL, 0, 0, 100, 30);
  te = EDIT(w)->model;
  ASSERT_EQ(text_edit_paste(te, L"中", 1), RET_OK);
  ASSERT_EQ(text_edit_paste(te, L" ", 1), RET_OK);
  ASSERT_EQ(text_edit_paste(te, L"文", 1), RET_OK);
  assert_text(w, "中 文");
  undo_key(te, w, TK_KEY_z);
  assert_text(w, "中");
  undo_key(te, w, TK_KEY_z);
  assert_text(w, "");

  widget_destroy(w);
}

/* 撤销/重做之后再输入，必须另起一组 */
TEST(Edit, undo_after_undo_redo) {
  widget_t* w = edit_create(NULL, 0, 0, 100, 30);
  text_edit_t* te = EDIT(w)->model;

  type_text(te, w, L"ab");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"");
  undo_key(te, w, TK_KEY_y);
  ASSERT_STREQ(w->text.str, L"ab");

  type_text(te, w, L"c");
  ASSERT_STREQ(w->text.str, L"abc");

  /* 只撤销刚输入的 c，不能连 ab 一起退掉 */
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"ab");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"");

  widget_destroy(w);

  /* 空历史时撤销/重做没有副作用 */
  w = edit_create(NULL, 0, 0, 100, 30);
  te = EDIT(w)->model;
  undo_key(te, w, TK_KEY_z);
  undo_key(te, w, TK_KEY_y);
  ASSERT_STREQ(w->text.str, L"");

  widget_destroy(w);
}

/* Ctrl+Backspace 每次单独成一步；撤销恢复文本但不选中 */
TEST(Edit, undo_delete_word) {
  widget_t* w = edit_create(NULL, 0, 0, 100, 30);
  text_edit_t* te = EDIT(w)->model;
  text_edit_state_t st;

  widget_set_text(w, L"hello world");
  ASSERT_EQ(text_edit_set_cursor(te, 11), RET_OK);
  press_ctrl_key(te, w, TK_KEY_BACKSPACE);
  ASSERT_STREQ(w->text.str, L"hello ");
  press_ctrl_key(te, w, TK_KEY_BACKSPACE);
  ASSERT_STREQ(w->text.str, L"");

  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"hello ");
  ASSERT_EQ(text_edit_get_state(te, &st), RET_OK);
  ASSERT_EQ(st.cursor, 6u);
  ASSERT_EQ(st.select_start, st.select_end);

  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"hello world");

  widget_destroy(w);

  /* 删词之后紧跟退格：两条记录，分开撤销 */
  w = edit_create(NULL, 0, 0, 100, 30);
  te = EDIT(w)->model;
  widget_set_text(w, L"hello world");
  ASSERT_EQ(text_edit_set_cursor(te, 11), RET_OK);
  press_ctrl_key(te, w, TK_KEY_BACKSPACE);
  ASSERT_STREQ(w->text.str, L"hello ");
  press_key(te, w, TK_KEY_BACKSPACE);
  ASSERT_STREQ(w->text.str, L"hello");

  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"hello ");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"hello world");

  widget_destroy(w);

  /* Ctrl+Delete 之后紧跟 Delete 也一样 */
  w = edit_create(NULL, 0, 0, 100, 30);
  te = EDIT(w)->model;
  widget_set_text(w, L"hello world");
  ASSERT_EQ(text_edit_set_cursor(te, 0), RET_OK);
  press_ctrl_key(te, w, TK_KEY_DELETE);
  ASSERT_STREQ(w->text.str, L" world");
  press_key(te, w, TK_KEY_DELETE);
  ASSERT_STREQ(w->text.str, L"world");

  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L" world");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"hello world");

  widget_destroy(w);
}

/* 剪切是独立的一步，后面的删除不能并进这条记录 */
TEST(Edit, undo_cut) {
  widget_t* w = edit_create(NULL, 0, 0, 100, 30);
  text_edit_t* te = EDIT(w)->model;

  widget_set_text(w, L"abcdef");
  ASSERT_EQ(text_edit_set_select(te, 2, 4), RET_OK);
  ASSERT_EQ(text_edit_cut(te), RET_OK);
  ASSERT_STREQ(w->text.str, L"abef");

  press_key(te, w, TK_KEY_BACKSPACE);
  ASSERT_STREQ(w->text.str, L"aef");

  /* 第一次撤销只恢复刚删掉的 b */
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"abef");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"abcdef");

  widget_destroy(w);
}

/* 编辑后再移动光标：撤销回编辑之前，重做回编辑结束处 */
TEST(Edit, undo_redo_caret) {
  widget_t* w = edit_create(NULL, 0, 0, 100, 30);
  text_edit_t* te = EDIT(w)->model;
  text_edit_state_t st;

  type_text(te, w, L"abc");
  ASSERT_EQ(text_edit_set_cursor(te, 0), RET_OK);
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"");
  undo_key(te, w, TK_KEY_y);
  ASSERT_STREQ(w->text.str, L"abc");
  ASSERT_EQ(text_edit_get_state(te, &st), RET_OK);
  ASSERT_EQ(st.cursor, 3u);

  widget_destroy(w);

  /* 粘贴后撤销，再把光标挪走，重做仍然回到粘贴结束处 */
  w = edit_create(NULL, 0, 0, 100, 30);
  te = EDIT(w)->model;
  widget_set_text(w, L"AWTK");
  ASSERT_EQ(text_edit_set_cursor(te, 4), RET_OK);
  ASSERT_EQ(text_edit_paste(te, L"XYZ", 3), RET_OK);

  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"AWTK");
  ASSERT_EQ(text_edit_set_cursor(te, 0), RET_OK);

  undo_key(te, w, TK_KEY_y);
  ASSERT_STREQ(w->text.str, L"AWTKXYZ");
  ASSERT_EQ(text_edit_get_state(te, &st), RET_OK);
  ASSERT_EQ(st.cursor, 7u);

  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"AWTK");
  ASSERT_EQ(text_edit_get_state(te, &st), RET_OK);
  ASSERT_EQ(st.cursor, 4u);

  widget_destroy(w);
}

/* 粘贴失败（空串、超出上限）不能动已有的撤销记录 */
TEST(Edit, undo_paste_keeps_history) {
  widget_t* w = edit_create(NULL, 0, 0, 100, 30);
  text_edit_t* te = EDIT(w)->model;

  /* 清掉字符过滤回调，走直接调用 stb_textedit_paste 的路径 */
  ASSERT_EQ(text_edit_set_on_char_will_input(te, NULL, NULL), RET_OK);
  type_text(te, w, L"abc");
  ASSERT_EQ(text_edit_paste(te, L"", 0), RET_SKIP);
  ASSERT_STREQ(w->text.str, L"abc");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"");

  widget_destroy(w);

  /* 达到 max_chars 上限后粘贴同样不能动历史 */
  w = edit_create(NULL, 0, 0, 100, 30);
  te = EDIT(w)->model;
  ASSERT_EQ(text_edit_set_on_char_will_input(te, NULL, NULL), RET_OK);
  ASSERT_EQ(text_edit_set_max_chars(te, 3), RET_OK);
  type_text(te, w, L"abc");
  ASSERT_STREQ(w->text.str, L"abc");
  text_edit_paste(te, L"XYZ", 3);
  ASSERT_STREQ(w->text.str, L"abc");
  undo_key(te, w, TK_KEY_z);
  ASSERT_STREQ(w->text.str, L"");

  widget_destroy(w);
}

/* 分组长度超过撤销字符缓冲时另起一条记录，否则撤销后就无法重做 */
TEST(Edit, undo_merge_group_cap) {
  static wchar_t buf[UNDO_TEST_CHAR_BUDGET + 16];
  const uint32_t total = UNDO_TEST_CHAR_BUDGET + 8;
  uint32_t i = 0;
  widget_t* w = edit_create(NULL, 0, 0, 100, 30);
  text_edit_t* te = EDIT(w)->model;

  for (i = 0; i < total; i++) {
    buf[i] = L'a';
  }
  buf[total] = 0;
  type_text(te, w, buf);
  ASSERT_EQ(w->text.size, total);

  /* 一个分组最多 UNDO_TEST_CHAR_BUDGET - 1 个字符 */
  undo_key(te, w, TK_KEY_z);
  ASSERT_EQ(w->text.size, (uint32_t)(UNDO_TEST_CHAR_BUDGET - 1));

  /* 这一组撤销之后必须还能重做 */
  undo_key(te, w, TK_KEY_y);
  ASSERT_EQ(w->text.size, total);

  widget_destroy(w);
}

/* 选中区域后粘贴，max_chars 要按删掉选区之后的长度计算 */
TEST(Edit, paste_over_selection_max_chars) {
  widget_t* w = edit_create(NULL, 0, 0, 100, 30);
  text_edit_t* te = EDIT(w)->model;

  ASSERT_EQ(text_edit_set_max_chars(te, 20), RET_OK);
  widget_set_text(w, L"abcdefghij");
  ASSERT_EQ(text_edit_set_select(te, 4, 10), RET_OK);

  /* 选区占 6 个字符，删掉后还剩 4 个，因此还能放 16 个 */
  ASSERT_EQ(text_edit_paste(te, L"zzzzzzzzzzzzzzzzzzzz", 20), RET_OK);
  ASSERT_STREQ(w->text.str, L"abcdzzzzzzzzzzzzzzzz");

  widget_destroy(w);
}
