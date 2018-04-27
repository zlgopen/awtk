
#include <stdlib.h>
#include "gtest/gtest.h"
#include "base/image_manager.h"

TEST(ImageManager, basic) {
  bitmap_t bmp;
  ASSERT_EQ(image_manager_load(image_manager(), "checked", &bmp), RET_OK);
  ASSERT_EQ(image_manager_load(image_manager(), "not found", &bmp), RET_NOT_FOUND);
}
