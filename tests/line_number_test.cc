#include "ext_widgets/mledit/line_number.h"

#include "gtest/gtest.h"

TEST(View, basic) {
  widget_t* w = line_number_create(NULL, 10, 20, 30, 40);
  ASSERT_EQ(line_number_add_highlight_line(w, 1), RET_OK);
  ASSERT_EQ(line_number_is_highlight_line(w, 1), TRUE);
  ASSERT_EQ(line_number_add_highlight_line(w, 2), RET_OK);
  ASSERT_EQ(line_number_is_highlight_line(w, 2), TRUE);
  ASSERT_EQ(line_number_add_highlight_line(w, 3), RET_OK);
  ASSERT_EQ(line_number_is_highlight_line(w, 3), TRUE);
  ASSERT_EQ(line_number_clear_highlight(w), RET_OK);
  ASSERT_EQ(line_number_is_highlight_line(w, 3), FALSE);
  ASSERT_EQ(line_number_set_active_line(w, 5), RET_OK);
  ASSERT_EQ(LINE_NUMBER(w)->active_line, 5);

  widget_destroy(w);
}
