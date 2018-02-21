
#include "base/image_manager.h"
#include <stdlib.h>
#include "base/array.h"
#include "gtest/gtest.h"
#include "lcd_log.h"

static ret_t dummy_loader_load1(image_loader_t* loader, const char* name, bitmap_t* image) {
  return_value_if_fail(strcmp(name, "11") == 0, RET_FAIL);

  image->w = 1;
  image->h = 1;

  return RET_OK;
}

static ret_t dummy_loader_load2(image_loader_t* loader, const char* name, bitmap_t* image) {
  return_value_if_fail(strcmp(name, "22") == 0, RET_FAIL);

  image->w = 2;
  image->h = 2;

  return RET_OK;
}

static ret_t dummy_loader_load3(image_loader_t* loader, const char* name, bitmap_t* image) {
  return_value_if_fail(strcmp(name, "33") == 0, RET_FAIL);

  image->w = 3;
  image->h = 3;

  return RET_OK;
}

TEST(ImageManager, basic) {
  bitmap_t image1;
  bitmap_t image2;
  bitmap_t image3;
  image_loader_t loader1;
  image_loader_t loader2;
  image_loader_t loader3;

  image_manager_t im;
  image_manager_init(&im);

  ASSERT_EQ(image_manager_add_image(&im, "1", &image1), RET_OK);
  ASSERT_EQ(image_manager_add_image(&im, "2", &image2), RET_OK);
  ASSERT_EQ(image_manager_add_image(&im, "3", &image3), RET_OK);
  ASSERT_EQ(image_manager_find_image(&im, "1"), &image1);
  ASSERT_EQ(image_manager_find_image(&im, "2"), &image2);
  ASSERT_EQ(image_manager_find_image(&im, "3"), &image3);

  loader1.load = dummy_loader_load1;
  loader2.load = dummy_loader_load2;
  loader3.load = dummy_loader_load3;
  ASSERT_EQ(image_manager_add_loader(&im, &loader1), RET_OK);
  ASSERT_EQ(image_manager_add_loader(&im, &loader2), RET_OK);
  ASSERT_EQ(image_manager_add_loader(&im, &loader3), RET_OK);

  bitmap_t image;
  ASSERT_EQ(image_manager_load(&im, "11", &image), RET_OK);
  ASSERT_EQ(image.w, 1);

  ASSERT_EQ(image_manager_load(&im, "22", &image), RET_OK);
  ASSERT_EQ(image.w, 2);

  ASSERT_EQ(image_manager_load(&im, "33", &image), RET_OK);
  ASSERT_EQ(image.w, 3);

  ASSERT_EQ(image_manager_load(&im, "44", &image), RET_FAIL);

  image_manager_deinit(&im);
}
