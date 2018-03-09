#include "base/resource_manager.h"
#include "gtest/gtest.h"

TEST(ResourceManager, basic) {
  const resource_info_t* null_res = NULL;
  resource_info_t img1 = {RESOURCE_TYPE_IMAGE_BMP, 100, "img1"};
  resource_info_t img2 = {RESOURCE_TYPE_IMAGE_PNG, 101, "img2"};
  resource_info_t ui1 = {RESOURCE_TYPE_UI, 102, "ui1"};
  resource_info_t ui2 = {RESOURCE_TYPE_UI, 103, "ui2"};

  ASSERT_EQ(resource_manager_init(10), RET_OK);
  ASSERT_EQ(resource_manager_add(&img1), RET_OK);
  ASSERT_EQ(resource_manager_add(&img2), RET_OK);
  ASSERT_EQ(resource_manager_add(&ui1), RET_OK);
  ASSERT_EQ(resource_manager_add(&ui2), RET_OK);

  ASSERT_EQ(resource_manager_ref(RESOURCE_TYPE_IMAGE_BMP, "img1"), &img1);
  ASSERT_EQ(resource_manager_ref(RESOURCE_TYPE_IMAGE_PNG, "img2"), &img2);
  ASSERT_EQ(resource_manager_ref(RESOURCE_TYPE_IMAGE_BMP, "img2"), null_res);
  ASSERT_EQ(resource_manager_ref(RESOURCE_TYPE_IMAGE_PNG, "img3"), null_res);

  ASSERT_EQ(resource_manager_ref(RESOURCE_TYPE_UI, "ui1"), &ui1);
  ASSERT_EQ(resource_manager_ref(RESOURCE_TYPE_UI, "ui2"), &ui2);
}
