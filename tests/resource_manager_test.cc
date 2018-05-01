#include "base/resource_manager.h"
#include "gtest/gtest.h"

TEST(ResourceManager, basic) {
  const resource_info_t* null_res = NULL;
  resource_manager_t* rm = resource_manager_create(10);

  resource_info_t img1 = {RESOURCE_TYPE_IMAGE, RESOURCE_TYPE_IMAGE_BMP, TRUE, 100, "img1"};
  resource_info_t img2 = {RESOURCE_TYPE_IMAGE, RESOURCE_TYPE_IMAGE_PNG, TRUE, 101, "img2"};
  resource_info_t ui1 = {RESOURCE_TYPE_UI, RESOURCE_TYPE_UI_BIN, TRUE, 102, "ui1"};
  resource_info_t ui2 = {RESOURCE_TYPE_UI, RESOURCE_TYPE_UI_XML, TRUE, 103, "ui2"};

  ASSERT_EQ(resource_manager_add(rm, &img1), RET_OK);
  ASSERT_EQ(resource_manager_add(rm, &img2), RET_OK);
  ASSERT_EQ(resource_manager_add(rm, &ui1), RET_OK);
  ASSERT_EQ(resource_manager_add(rm, &ui2), RET_OK);

  ASSERT_EQ(resource_manager_ref(rm, RESOURCE_TYPE_IMAGE, "img1"), &img1);
  ASSERT_EQ(resource_manager_ref(rm, RESOURCE_TYPE_IMAGE, "img2"), &img2);
  ASSERT_EQ(resource_manager_ref(rm, RESOURCE_TYPE_IMAGE, "img3"), null_res);

  ASSERT_EQ(resource_manager_ref(rm, RESOURCE_TYPE_UI, "ui1"), &ui1);
  ASSERT_EQ(resource_manager_ref(rm, RESOURCE_TYPE_UI, "ui2"), &ui2);

  resource_manager_destroy(rm);
}

TEST(ResourceManager, file) {
  const resource_info_t* r = NULL;
  resource_manager_t* rm = resource_manager_create(10);

  r = resource_manager_ref(rm, RESOURCE_TYPE_IMAGE, "earth");
  ASSERT_EQ(r != NULL, true);
  ASSERT_EQ(resource_manager_unref(rm, r), RET_OK);

  resource_manager_destroy(rm);
}
