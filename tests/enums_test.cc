#include "base/enums.h"
#include "gtest/gtest.h"
#include "base/resource_manager.h"

TEST(Enums, basic) {
  ASSERT_EQ(resource_type_find("theme"), resource_type_find_by_value(RESOURCE_TYPE_THEME));
  ASSERT_EQ(resource_type_find("xml"), resource_type_find_by_value(RESOURCE_TYPE_XML));
}
