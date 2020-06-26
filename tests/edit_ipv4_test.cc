#include "tkc/utils.h"
#include "widgets/edit_ipv4.h"
#include "gtest/gtest.h"

TEST(Edit, ipv4_is_valid) {
  widget_t* w = edit_create(NULL, 10, 20, 30, 40);

  widget_set_text_utf8(w, "192.168.1.1");
  ASSERT_EQ(edit_ipv4_is_valid(w), TRUE);

  widget_set_text_utf8(w, "0.0.0.0");
  ASSERT_EQ(edit_ipv4_is_valid(w), TRUE);

  widget_set_text_utf8(w, "0.0");
  ASSERT_EQ(edit_ipv4_is_valid(w), FALSE);

  widget_set_text_utf8(w, "0");
  ASSERT_EQ(edit_ipv4_is_valid(w), FALSE);

  widget_set_text_utf8(w, "292.168.1.1");
  ASSERT_EQ(edit_ipv4_is_valid(w), FALSE);

  widget_set_text_utf8(w, "192.368.1.1");
  ASSERT_EQ(edit_ipv4_is_valid(w), FALSE);

  widget_set_text_utf8(w, "1.3.1.1223");
  ASSERT_EQ(edit_ipv4_is_valid(w), FALSE);

  widget_set_text_utf8(w, "0001.3.1.12");
  ASSERT_EQ(edit_ipv4_is_valid(w), FALSE);

  widget_destroy(w);
}

TEST(Edit, ipv4_fix) {
  widget_t* w = edit_create(NULL, 10, 20, 30, 40);

  widget_set_text_utf8(w, "1922.168.1.1");
  ASSERT_EQ(edit_ipv4_fix_ex(w, TRUE), RET_OK);
  ASSERT_EQ(wcscmp(w->text.str, L"192.168.1.1"), 0);

  widget_set_text_utf8(w, "1922.268.1.1");
  ASSERT_EQ(edit_ipv4_fix_ex(w, TRUE), RET_OK);
  ASSERT_EQ(wcscmp(w->text.str, L"192.26.1.1"), 0);

  widget_set_text_utf8(w, "1922.268.0.1");
  ASSERT_EQ(edit_ipv4_fix_ex(w, TRUE), RET_OK);
  ASSERT_EQ(wcscmp(w->text.str, L"192.26.0.1"), 0);

  widget_destroy(w);
}
