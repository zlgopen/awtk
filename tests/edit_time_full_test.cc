#include "tkc/utils.h"
#include "widgets/edit_time_full.h"
#include "gtest/gtest.h"

TEST(Edit, time_full_is_valid) {
  widget_t* w = edit_create(NULL, 10, 20, 30, 40);

  widget_set_text_utf8(w, "18:12:21");
  ASSERT_EQ(edit_time_full_is_valid(w), TRUE);

  widget_set_text_utf8(w, "18/12/21");
  ASSERT_EQ(edit_time_full_is_valid(w), FALSE);

  widget_set_text_utf8(w, "00:00");
  ASSERT_EQ(edit_time_full_is_valid(w), FALSE);

  widget_set_text_utf8(w, "00:00:00");
  ASSERT_EQ(edit_time_full_is_valid(w), TRUE);

  widget_set_text_utf8(w, "00:00:00:00");
  ASSERT_EQ(edit_time_full_is_valid(w), FALSE);

  widget_set_text_utf8(w, "23:59:59");
  ASSERT_EQ(edit_time_full_is_valid(w), TRUE);

  widget_set_text_utf8(w, "0:0:0");
  ASSERT_EQ(edit_time_full_is_valid(w), FALSE);

  widget_set_text_utf8(w, "0:0");
  ASSERT_EQ(edit_time_full_is_valid(w), FALSE);

  widget_set_text_utf8(w, "0");
  ASSERT_EQ(edit_time_full_is_valid(w), FALSE);

  widget_set_text_utf8(w, "18:13:60");
  ASSERT_EQ(edit_time_full_is_valid(w), FALSE);

  widget_set_text_utf8(w, "18:60:40");
  ASSERT_EQ(edit_time_full_is_valid(w), FALSE);

  widget_set_text_utf8(w, "018:12:40");
  ASSERT_EQ(edit_time_full_is_valid(w), FALSE);

  widget_set_text_utf8(w, "18:12:");
  ASSERT_EQ(edit_time_full_is_valid(w), FALSE);

  widget_set_text_utf8(w, "24:12:00");
  ASSERT_EQ(edit_time_full_is_valid(w), FALSE);

  widget_set_text_utf8(w, "25:00:01");
  ASSERT_EQ(edit_time_full_is_valid(w), FALSE);

  widget_destroy(w);
}

TEST(Edit, time_full_fix) {
  widget_t* w = edit_create(NULL, 10, 20, 30, 40);

  widget_set_text_utf8(w, "180:12:01");
  ASSERT_EQ(edit_time_full_fix_ex(w, TRUE), RET_OK);
  ASSERT_EQ(wcscmp(w->text.str, L"18:12:01"), 0);

  widget_set_text_utf8(w, "180:12:60");
  ASSERT_EQ(edit_time_full_fix_ex(w, TRUE), RET_OK);
  ASSERT_EQ(wcscmp(w->text.str, L"18:12:00"), 0);

  widget_set_text_utf8(w, "180:60:03");
  ASSERT_EQ(edit_time_full_fix_ex(w, TRUE), RET_OK);
  ASSERT_EQ(wcscmp(w->text.str, L"18:00:03"), 0);

  widget_set_text_utf8(w, "180:60:03");
  ASSERT_EQ(edit_time_full_fix(w), RET_OK);
  ASSERT_EQ(wcscmp(w->text.str, L"18:00:03"), 0);

  widget_set_text_utf8(w, "233:456:78");
  ASSERT_EQ(edit_time_full_fix(w), RET_OK);
  ASSERT_EQ(wcscmp(w->text.str, L"23:45:00"), 0);

  widget_set_text_utf8(w, "156:59:789");
  ASSERT_EQ(edit_time_full_fix(w), RET_OK);
  ASSERT_EQ(wcscmp(w->text.str, L"15:59:00"), 0);

  // 位数不足两位时补前导 0
  widget_set_text_utf8(w, "0:0:0");
  ASSERT_EQ(edit_time_full_fix(w), RET_OK);
  ASSERT_EQ(wcscmp(w->text.str, L"00:00:00"), 0);

  widget_set_text_utf8(w, "1:2:3");
  ASSERT_EQ(edit_time_full_fix(w), RET_OK);
  ASSERT_EQ(wcscmp(w->text.str, L"01:02:03"), 0);

  widget_destroy(w);
}

TEST(Edit, time_full_inc_dec) {
  widget_t* w = edit_create(NULL, 10, 20, 30, 40);

  widget_set_text_utf8(w, "18:12:01");
  edit_set_cursor(w, w->text.size);
  ASSERT_EQ(edit_time_full_inc_value(w), RET_OK);
  ASSERT_EQ(wcscmp(w->text.str, L"18:12:02"), 0);

  ASSERT_EQ(edit_time_full_dec_value(w), RET_OK);
  ASSERT_EQ(wcscmp(w->text.str, L"18:12:01"), 0);

  widget_destroy(w);
}

TEST(Edit, time_full_is_valid_char) {
  widget_t* w = edit_create(NULL, 10, 20, 30, 40);

  widget_set_text_utf8(w, "18:12:0");
  ASSERT_EQ(edit_time_full_is_valid_char(w, '0'), TRUE);
  ASSERT_EQ(edit_time_full_pre_input(w, '0'), RET_OK);

  widget_set_text_utf8(w, "18:12:01");
  ASSERT_EQ(edit_time_full_is_valid_char(w, '0'), TRUE);
  ASSERT_EQ(edit_time_full_pre_input(w, '0'), RET_OK);

  widget_set_text_utf8(w, "18:12:0");
  ASSERT_EQ(edit_time_full_is_valid_char(w, 'a'), FALSE);

  widget_destroy(w);
}
