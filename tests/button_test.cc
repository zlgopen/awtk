#include "base/button.h"
#include "base/canvas.h"
#include "base/widget.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>

TEST(Button, basic) {
  value_t v1;
  value_t v2;
  widget_t* b = button_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_set_text(b, L"OK"), RET_OK);
  ASSERT_EQ(wcscmp(b->text.str, L"OK"), 0);
  ASSERT_EQ(widget_set_text(b, L"Cancel"), RET_OK);
  ASSERT_EQ(wcscmp(b->text.str, L"Cancel"), 0);

  value_set_wstr(&v1, L"button");
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_TEXT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_TEXT, &v2), RET_OK);
  ASSERT_EQ(wcscmp(v1.value.wstr, v2.value.wstr), 0);

  widget_destroy(b);
}
