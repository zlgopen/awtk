#include "tkc/utils.h"
#include "widgets/edit_time_full.h"
#include "gtest/gtest.h"

TEST(Edit, time_full_is_valid) {
  widget_t* w = edit_create(NULL, 10, 20, 30, 40);

  widget_set_text_utf8(w, "18:12:21");
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

  widget_destroy(w);
}
