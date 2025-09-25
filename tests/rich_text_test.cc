#include "base/window.h"
#include "rich_text/rich_text.h"
#include "gtest/gtest.h"

TEST(RichText, basic) {
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* rich_text = rich_text_create(w, 0, 0, 100, 100);

  ASSERT_EQ(rich_text, rich_text_cast(rich_text));
  ASSERT_EQ(RICH_TEXT(rich_text)->word_wrap, TRUE);

  rich_text_set_word_wrap(w, FALSE);
  ASSERT_EQ(!RICH_TEXT(rich_text)->word_wrap, FALSE);

  widget_destroy(w);
}

TEST(RichText, cast) {
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* rich_text = rich_text_create(w, 0, 0, 100, 100);

  ASSERT_EQ(rich_text, rich_text_cast(rich_text));

  widget_destroy(w);
}
