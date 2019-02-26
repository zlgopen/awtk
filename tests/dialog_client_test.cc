#include "widgets/dialog_client.h"
#include "gtest/gtest.h"

TEST(DialgClient, cast) {
  widget_t* w = dialog_client_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, dialog_client_cast(w));

  widget_destroy(w);
}
