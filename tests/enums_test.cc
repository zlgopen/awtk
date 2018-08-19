#include "base/enums.h"
#include "gtest/gtest.h"
#include "base/resource_manager.h"

TEST(Enums, basic) {
  ASSERT_EQ(resource_type_find("style"), resource_type_find_by_value(RESOURCE_TYPE_STYLE));
  ASSERT_EQ(resource_type_find("xml"), resource_type_find_by_value(RESOURCE_TYPE_XML));
}
