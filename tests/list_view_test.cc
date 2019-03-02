#include "scroll_view/list_view.h"
#include "base/canvas.h"
#include "base/widget.h"
#include "base/layout.h"
#include "widgets/button.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>

TEST(ListView, basic) {
  value_t v1;
  widget_t* widget = list_view_create(NULL, 0, 0, 200, 400);
  list_view_t* list_view = LIST_VIEW(widget);

  ASSERT_EQ(list_view_set_item_height(widget, 40), RET_OK);
  ASSERT_EQ(list_view->item_height, 40);

  value_set_int(&v1, 600);
  ASSERT_EQ(widget_set_prop(widget, WIDGET_PROP_ITEM_HEIGHT, &v1), RET_OK);
  ASSERT_EQ(list_view->item_height, value_int(&v1));

  ASSERT_EQ(list_view_set_default_item_height(widget, 40), RET_OK);
  ASSERT_EQ(list_view->default_item_height, 40);

  value_set_int(&v1, 600);
  ASSERT_EQ(widget_set_prop(widget, WIDGET_PROP_DEFAULT_ITEM_HEIGHT, &v1), RET_OK);
  ASSERT_EQ(list_view->default_item_height, value_int(&v1));

  widget_destroy(widget);
}

TEST(ListView, cast) {
  widget_t* w = list_view_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, list_view_cast(w));

  widget_destroy(w);
}
