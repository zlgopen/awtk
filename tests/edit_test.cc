#include "widgets/edit.h"
#include "base/canvas.h"
#include "base/widget.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>

TEST(Edit, int) {
  value_t v1;
  value_t v2;
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(edit_set_int_limit(b, 1, 100, 5), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(1, value_int(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(100, value_int(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_STEP, &v2), RET_OK);
  ASSERT_EQ(5, value_int(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_INPUT_TYPE, &v2), RET_OK);
  ASSERT_EQ(INPUT_INT, value_int(&v2));

  value_set_int(&v1, 10);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 100);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MAX, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 10);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_STEP, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_STEP, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  widget_destroy(b);
}

TEST(Edit, text) {
  value_t v1;
  value_t v2;
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(edit_set_text_limit(b, 0, 100), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(0, value_int(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_INPUT_TYPE, &v2), RET_OK);
  ASSERT_EQ(INPUT_TEXT, value_int(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(100, value_int(&v2));

  value_set_int(&v1, 10);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 100);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MAX, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  widget_destroy(b);
}

TEST(Edit, float) {
  value_t v1;
  value_t v2;
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(edit_set_float_limit(b, 0, 100, 0.1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(0, value_float(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_INPUT_TYPE, &v2), RET_OK);
  ASSERT_EQ(INPUT_FLOAT, value_int(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(100, value_float(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_STEP, &v2), RET_OK);
  ASSERT_EQ(0.1 - value_float(&v2) < 0.001, true);

  value_set_int(&v1, 10);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MIN, &v2), RET_OK);
  ASSERT_EQ(value_float(&v1), value_float(&v2));

  value_set_int(&v1, 100);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_MAX, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_MAX, &v2), RET_OK);
  ASSERT_EQ(value_float(&v1), value_float(&v2));

  widget_destroy(b);
}

TEST(Edit, basic) {
  value_t v1;
  value_t v2;
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(edit_set_readonly(b, TRUE), RET_OK);
  ASSERT_EQ(EDIT(b)->readonly, TRUE);

  value_set_bool(&v1, FALSE);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_READONLY, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_READONLY, &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));

  ASSERT_EQ(edit_set_auto_fix(b, TRUE), RET_OK);
  ASSERT_EQ(EDIT(b)->auto_fix, TRUE);

  value_set_bool(&v1, FALSE);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_READONLY, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_READONLY, &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));

  value_set_int(&v1, 3);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_LEFT_MARGIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_LEFT_MARGIN, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 3);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_RIGHT_MARGIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_RIGHT_MARGIN, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 3);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_TOP_MARGIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_TOP_MARGIN, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 3);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_BOTTOM_MARGIN, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_BOTTOM_MARGIN, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  widget_destroy(b);
}

TEST(Edit, cast) {
  widget_t* b = edit_create(NULL, 10, 20, 30, 40);
  ASSERT_EQ(b, edit_cast(b));
  widget_destroy(b);
}
