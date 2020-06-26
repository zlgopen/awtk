#include "tkc/utils.h"
#include "widgets/edit_date.h"
#include "gtest/gtest.h"

TEST(Edit, date_is_valid) {
  widget_t* w = edit_create(NULL, 10, 20, 30, 40);

  widget_set_text_utf8(w, "2020/12/21");
  ASSERT_EQ(edit_date_is_valid(w), TRUE);

  widget_set_text_utf8(w, "0/0/0");
  ASSERT_EQ(edit_date_is_valid(w), FALSE);

  widget_set_text_utf8(w, "0/0");
  ASSERT_EQ(edit_date_is_valid(w), FALSE);

  widget_set_text_utf8(w, "0");
  ASSERT_EQ(edit_date_is_valid(w), FALSE);

  widget_set_text_utf8(w, "2020/13/20");
  ASSERT_EQ(edit_date_is_valid(w), FALSE);

  widget_set_text_utf8(w, "2020/12/40");
  ASSERT_EQ(edit_date_is_valid(w), FALSE);

  widget_set_text_utf8(w, "02020/12/40");
  ASSERT_EQ(edit_date_is_valid(w), FALSE);

  widget_set_text_utf8(w, "2020/12/");
  ASSERT_EQ(edit_date_is_valid(w), FALSE);

  widget_set_text_utf8(w, "2020/12/00");
  ASSERT_EQ(edit_date_is_valid(w), FALSE);

  widget_set_text_utf8(w, "2020/00/01");
  ASSERT_EQ(edit_date_is_valid(w), FALSE);

  widget_destroy(w);
}

TEST(Edit, date_fix) {
  widget_t* w = edit_create(NULL, 10, 20, 30, 40);

  widget_set_text_utf8(w, "20200/12/01");
  ASSERT_EQ(edit_date_fix_ex(w, TRUE), RET_OK);
  ASSERT_EQ(wcscmp(w->text.str, L"2020/12/01"), 0);

  widget_set_text_utf8(w, "20200/12/33");
  ASSERT_EQ(edit_date_fix_ex(w, TRUE), RET_OK);
  ASSERT_EQ(wcscmp(w->text.str, L"2020/12/01"), 0);

  widget_set_text_utf8(w, "20200/21/03");
  ASSERT_EQ(edit_date_fix_ex(w, TRUE), RET_OK);
  ASSERT_EQ(wcscmp(w->text.str, L"2020/01/03"), 0);

  widget_destroy(w);
}
