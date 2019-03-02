#include "color_picker/color_picker.h"
#include "gtest/gtest.h"

TEST(ColorPicker, cast) {
  widget_t* w = color_picker_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, color_picker_cast(w));

  widget_destroy(w);
}
