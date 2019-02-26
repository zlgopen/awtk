#include "color_picker/color_component.h"
#include "gtest/gtest.h"

TEST(ColorComponent, cast) {
  widget_t* w = color_component_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, color_component_cast(w));

  widget_destroy(w);
}
