#include "base/enums.h"
#include "gtest/gtest.h"
#include "base/resource_manager.h"

TEST(Enums, basic) {
  ASSERT_EQ(resouce_type_name_find("theme"), resouce_type_name_find_by_value(RESOURCE_TYPE_THEME));
  ASSERT_EQ(resouce_type_name_find("xml"), resouce_type_name_find_by_value(RESOURCE_TYPE_XML));
}
