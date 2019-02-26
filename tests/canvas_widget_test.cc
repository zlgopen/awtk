#include "canvas_widget/canvas_widget.h"
#include "gtest/gtest.h"

TEST(CanvasWidget, cast) {
  widget_t* w = canvas_widget_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, canvas_widget_cast(w));

  widget_destroy(w);
}
