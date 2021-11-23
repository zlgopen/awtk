#include "base/canvas.h"
#include "widgets/label.h"
#include "base/widget.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>

TEST(Label, basic) {
  value_t v1;
  value_t v2;
  widget_t* label = label_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_set_text(label, L"OK"), RET_OK);
  ASSERT_EQ(wcscmp(label->text.str, L"OK"), 0);
  ASSERT_EQ(widget_set_text(label, L"Cancel"), RET_OK);
  ASSERT_EQ(wcscmp(label->text.str, L"Cancel"), 0);

  value_set_wstr(&v1, L"label");
  ASSERT_EQ(widget_set_prop(label, WIDGET_PROP_TEXT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(label, WIDGET_PROP_TEXT, &v2), RET_OK);
  ASSERT_EQ(wcscmp(v1.value.wstr, v2.value.wstr), 0);

  widget_destroy(label);
}

TEST(Label, value) {
  widget_t* label = label_create(NULL, 10, 20, 30, 40);
  ASSERT_EQ(widget_set_value(label, 100), RET_OK);
  ASSERT_EQ(widget_get_value(label), 100);

  ASSERT_EQ(widget_add_value(label, 100), RET_OK);
  ASSERT_EQ(widget_get_value(label), 200);

  widget_destroy(label);
}

TEST(Label, clone) {
  value_t v1;
  widget_t* w1 = label_create(NULL, 10, 20, 30, 40);

  widget_set_name(w1, "label");
  value_set_wstr(&v1, L"label");
  ASSERT_EQ(widget_set_prop(w1, WIDGET_PROP_TEXT, &v1), RET_OK);

  widget_t* w2 = widget_clone(w1, NULL);

  ASSERT_EQ(widget_equal(w1, w2), TRUE);

  widget_destroy(w1);
  widget_destroy(w2);
}

TEST(Label, line_wrap) {
  widget_t* label = label_create(NULL, 10, 20, 30, 40);
  ASSERT_EQ(LABEL(label)->line_wrap, FALSE);
  ASSERT_EQ(widget_set_prop_bool(label, WIDGET_PROP_LINE_WRAP, TRUE), RET_OK);
  ASSERT_EQ(widget_get_prop_bool(label, WIDGET_PROP_LINE_WRAP, FALSE), TRUE);
  ASSERT_EQ(LABEL(label)->line_wrap, TRUE);

  widget_destroy(label);
}

TEST(Label, word_wrap) {
  widget_t* label = label_create(NULL, 10, 20, 30, 40);
  ASSERT_EQ(LABEL(label)->word_wrap, FALSE);
  ASSERT_EQ(widget_set_prop_bool(label, WIDGET_PROP_WORD_WRAP, TRUE), RET_OK);
  ASSERT_EQ(widget_get_prop_bool(label, WIDGET_PROP_WORD_WRAP, FALSE), TRUE);
  ASSERT_EQ(LABEL(label)->word_wrap, TRUE);

  widget_destroy(label);
}

TEST(Label, max_w) {
  widget_t* label = label_create(NULL, 10, 20, 30, 40);
  ASSERT_EQ(LABEL(label)->word_wrap, FALSE);
  ASSERT_EQ(widget_set_prop_int(label, WIDGET_PROP_MAX_W, 123), RET_OK);
  ASSERT_EQ(widget_get_prop_int(label, WIDGET_PROP_MAX_W, 0), 123);
  ASSERT_EQ(LABEL(label)->max_w, 123);

  widget_destroy(label);
}

TEST(Label, add_value) {
  widget_t* label = label_create(NULL, 10, 20, 30, 40);

  widget_set_text_utf8(label, "10");
  ASSERT_EQ((int)widget_get_value(label), 10);
  ASSERT_EQ(widget_add_value(label, -1), RET_OK);
  ASSERT_EQ((int)widget_get_value(label), 9);

  ASSERT_EQ(widget_set_value(label, 100), RET_OK);
  ASSERT_EQ((int)widget_get_value(label), 100);

  widget_destroy(label);
}

TEST(Label, add_value_int) {
  widget_t* label = label_create(NULL, 10, 20, 30, 40);

  widget_set_text_utf8(label, "10");
  ASSERT_EQ(widget_get_value_int(label), 10);
  ASSERT_EQ(widget_add_value_int(label, -1), RET_OK);
  ASSERT_EQ(widget_get_value_int(label), 9);

  ASSERT_EQ(widget_set_value_int(label, 100), RET_OK);
  ASSERT_EQ(widget_get_value_int(label), 100);

  widget_destroy(label);
}
