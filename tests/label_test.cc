#include "base/label.h"
#include <stdlib.h>
#include "base/canvas.h"
#include "base/widget.h"
#include "font_dummy.h"
#include "gtest/gtest.h"
#include "lcd_log.h"

TEST(Label, basic) {
  value_t v1;
  value_t v2;
  widget_t* b = label_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(label_set_text(b, L"OK"), RET_OK);
  ASSERT_EQ(wcscmp(LABEL(b)->text.str, L"OK"), 0);
  ASSERT_EQ(label_set_text(b, L"Cancel"), RET_OK);
  ASSERT_EQ(wcscmp(LABEL(b)->text.str, L"Cancel"), 0);

  value_set_wstr(&v1, L"label");
  ASSERT_EQ(widget_set_prop(b, "text", &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, "text", &v2), RET_OK);
  ASSERT_EQ(wcscmp(v1.value.wstr, v2.value.wstr), 0);

  value_set_uint8(&v1, 1);
  ASSERT_EQ(widget_set_prop(b, "text-align-h", &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, "text-align-h", &v2), RET_OK);
  ASSERT_EQ(value_int(v1), value_int(v2));

  value_set_uint8(&v1, 2);
  ASSERT_EQ(widget_set_prop(b, "text-align-v", &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, "text-align-v", &v2), RET_OK);
  ASSERT_EQ(value_int(v1), value_int(v2));

  value_set_uint8(&v1, 3);
  ASSERT_EQ(widget_set_prop(b, "border", &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, "border", &v2), RET_OK);
  ASSERT_EQ(value_int(v1), value_int(v2));

  widget_destroy(b);
}
