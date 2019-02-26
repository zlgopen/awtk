#include "widgets/popup.h"
#include "gtest/gtest.h"

TEST(Popup, cast) {
  widget_t* w = popup_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, popup_cast(w));
  ASSERT_EQ(w, window_base_cast(w));

  widget_destroy(w);
}
