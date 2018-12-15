#include "widgets/color_tile.h"
#include "base/canvas.h"
#include "base/widget.h"
#include "base/layout.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <string.h>

using std::string;

TEST(ColorTile, basic) {
  value_t v1;
  value_t v2;
  widget_t* w = color_tile_create(NULL, 10, 20, 30, 40);

  value_set_str(&v1, "#112233");
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_BG_COLOR, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_BG_COLOR, &v2), RET_OK);
  ASSERT_EQ(string(v1.value.str), string(v2.value.str));

  value_set_str(&v1, "#882233");
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_BORDER_COLOR, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_BORDER_COLOR, &v2), RET_OK);
  ASSERT_EQ(string(v1.value.str), string(v2.value.str));

  widget_destroy(w);
}

TEST(ColorTile, clone) {
  value_t v1;
  widget_t* w1 = color_tile_create(NULL, 10, 20, 30, 40);

  value_set_str(&v1, "#112233");
  ASSERT_EQ(widget_set_prop(w1, WIDGET_PROP_BG_COLOR, &v1), RET_OK);

  value_set_str(&v1, "#882233");
  ASSERT_EQ(widget_set_prop(w1, WIDGET_PROP_BORDER_COLOR, &v1), RET_OK);

  widget_t* w2 = widget_clone(w1, NULL);
  ASSERT_EQ(widget_equal(w1, w2), TRUE);
  widget_destroy(w1);
  widget_destroy(w2);
}
