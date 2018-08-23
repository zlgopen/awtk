#include "base/enums.h"
#include "gtest/gtest.h"
#include "base/assets_manager.h"

TEST(Enums, basic) {
  ASSERT_EQ(asset_type_find("style"), asset_type_find_by_value(ASSET_TYPE_STYLE));
  ASSERT_EQ(asset_type_find("xml"), asset_type_find_by_value(ASSET_TYPE_XML));
}
