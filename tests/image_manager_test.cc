#include <stdlib.h>
#include "gtest/gtest.h"
#include "base/image_manager.h"

TEST(ImageManager, basic) {
  bitmap_t bmp;
  memset(&bmp, 0x00, sizeof(bmp));
  ASSERT_EQ(image_manager_load(image_manager(), "checked", &bmp), RET_OK);
  ASSERT_EQ(image_manager_lookup(image_manager(), "checked", &bmp), RET_OK);
  ASSERT_EQ(image_manager_load(image_manager(), "not found", &bmp), RET_NOT_FOUND);
  ASSERT_EQ(image_manager_lookup(image_manager(), "unchecked", &bmp), RET_NOT_FOUND);

  ASSERT_EQ(image_manager_unload_unused(image_manager(), 0), RET_OK);
}

TEST(ImageManager, unload) {
  bitmap_t bmp;
  memset(&bmp, 0x00, sizeof(bmp));

  ASSERT_EQ(image_manager_load(image_manager(), "checked", &bmp), RET_OK);
  ASSERT_EQ(image_manager_lookup(image_manager(), "checked", &bmp), RET_OK);
  ASSERT_EQ(image_manager_unload_unused(image_manager(), 0), RET_OK);
  ASSERT_EQ(image_manager_lookup(image_manager(), "checked", &bmp), RET_NOT_FOUND);

  ASSERT_EQ(image_manager_unload_unused(image_manager(), 0), RET_OK);
}

TEST(ImageManager, add) {
  bitmap_t bmp;
  memset(&bmp, 0x00, sizeof(bmp));

  bmp.w = 10;
  bmp.h = 10;
  bmp.data = NULL;
  bmp.destroy = NULL;

  ASSERT_EQ(image_manager_add(image_manager(), "checked", &bmp), RET_OK);
  ASSERT_EQ(image_manager_lookup(image_manager(), "checked", &bmp), RET_OK);
  ASSERT_EQ(image_manager_unload_unused(image_manager(), 0), RET_OK);
}
