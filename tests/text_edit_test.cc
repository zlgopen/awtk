#include "tkc/str.h"
#include "gtest/gtest.h"
#include "mledit/mledit.h"
#include "base/text_edit.h"
#include "base/clip_board.h"
#include "base/window_manager.h"

TEST(TextEdit, basic) {
  str_t str;
  key_event_t keye;
  text_edit_state_t state;
  widget_t* w = mledit_create(NULL, 10, 20, 30, 40);
  text_edit_t* text_edit = text_edit_create(w, FALSE);
  text_edit_set_canvas(text_edit, widget_get_canvas(w));

  str_init(&str, 0);

  widget_set_text(w, L"hello");
  ASSERT_EQ(text_edit_set_select(text_edit, 1, 2), RET_OK);
  ASSERT_EQ(text_edit_set_cursor(text_edit, -1), RET_OK);

  ASSERT_EQ(text_edit_get_state(text_edit, &state), RET_OK);
  ASSERT_EQ(state.select_start, 1);
  ASSERT_EQ(state.select_end, 2);
  ASSERT_EQ(state.cursor, 5);
  ASSERT_EQ(state.mask, FALSE);
  ASSERT_EQ(state.mask_char, FALSE);
  ASSERT_EQ(state.single_line, FALSE);

  ASSERT_EQ(text_edit_paste(text_edit, L"123", 3), RET_OK);
  str_from_wstr(&str, w->text.str);
  ASSERT_STREQ(str.str, "h123llo");

  ASSERT_EQ(text_edit_set_select(text_edit, 0, 100), RET_OK);
  ASSERT_EQ(text_edit_copy(text_edit), RET_OK);

  clip_board_clear(clip_board());
  ASSERT_EQ(text_edit_cut(text_edit), RET_OK);
  ASSERT_EQ(text_edit_get_state(text_edit, &state), RET_OK);
  ASSERT_EQ(state.select_start, 0);
  ASSERT_EQ(state.select_end, 0);
  ASSERT_EQ(state.cursor, 0);

  key_event_init(&keye, EVT_KEY_DOWN, w, TK_KEY_0);
  ASSERT_EQ(text_edit_key_down(text_edit, &keye), RET_OK);
  ASSERT_EQ(text_edit_key_down(text_edit, &keye), RET_OK);

  str_from_wstr(&str, w->text.str);
  ASSERT_STREQ(str.str, "00");

  str_reset(&str);
  widget_destroy(w);
  text_edit_destroy(text_edit);
}
