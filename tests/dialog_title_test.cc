#include "widgets/dialog_title.h"
#include "gtest/gtest.h"

TEST(DialgTitle, cast) {
  widget_t* w = dialog_title_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, dialog_title_cast(w));

  widget_destroy(w);
}
