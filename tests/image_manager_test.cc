
#include <stdlib.h>
#include "gtest/gtest.h"
#include "base/image_manager.h"

TEST(ImageManager, basic) {
  bitmap_t bmp;
  ASSERT_EQ(image_manager_load(default_im(), "checked", &bmp), RET_OK);
  ASSERT_EQ(image_manager_load(default_im(), "not found", &bmp), RET_NOT_FOUND);
}
