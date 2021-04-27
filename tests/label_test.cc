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
  widget_t* l = label_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_set_text(l, L"OK"), RET_OK);
  ASSERT_EQ(wcscmp(l->text.str, L"OK"), 0);
  ASSERT_EQ(widget_set_text(l, L"Cancel"), RET_OK);
  ASSERT_EQ(wcscmp(l->text.str, L"Cancel"), 0);

  value_set_wstr(&v1, L"label");
  ASSERT_EQ(widget_set_prop(l, WIDGET_PROP_TEXT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(l, WIDGET_PROP_TEXT, &v2), RET_OK);
  ASSERT_EQ(wcscmp(v1.value.wstr, v2.value.wstr), 0);

  widget_destroy(l);
}

TEST(Label, value) {
  widget_t* l = label_create(NULL, 10, 20, 30, 40);
  ASSERT_EQ(widget_set_value(l, 100), RET_OK);
  ASSERT_EQ(widget_get_value(l), 100);

  ASSERT_EQ(widget_add_value(l, 100), RET_OK);
  ASSERT_EQ(widget_get_value(l), 200);

  widget_destroy(l);
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
  widget_t* l = label_create(NULL, 10, 20, 30, 40);
  ASSERT_EQ(LABEL(l)->line_wrap, FALSE);
  ASSERT_EQ(widget_set_prop_bool(l, WIDGET_PROP_LINE_WRAP, TRUE), RET_OK);
  ASSERT_EQ(widget_get_prop_bool(l, WIDGET_PROP_LINE_WRAP, FALSE), TRUE);
  ASSERT_EQ(LABEL(l)->line_wrap, TRUE);

  widget_destroy(l);
}

TEST(Label, word_wrap) {
  widget_t* l = label_create(NULL, 10, 20, 30, 40);
  ASSERT_EQ(LABEL(l)->word_wrap, FALSE);
  ASSERT_EQ(widget_set_prop_bool(l, WIDGET_PROP_WORD_WRAP, TRUE), RET_OK);
  ASSERT_EQ(widget_get_prop_bool(l, WIDGET_PROP_WORD_WRAP, FALSE), TRUE);
  ASSERT_EQ(LABEL(l)->word_wrap, TRUE);

  widget_destroy(l);
}

TEST(Label, max_w) {
  widget_t* l = label_create(NULL, 10, 20, 30, 40);
  ASSERT_EQ(LABEL(l)->word_wrap, FALSE);
  ASSERT_EQ(widget_set_prop_int(l, WIDGET_PROP_MAX_W, 123), RET_OK);
  ASSERT_EQ(widget_get_prop_int(l, WIDGET_PROP_MAX_W, 0), 123);
  ASSERT_EQ(LABEL(l)->max_w, 123);

  widget_destroy(l);
}
