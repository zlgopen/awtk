#include "base/keys.h"
#include "base/enums.h"
#include "gtest/gtest.h"
#include "base/assets_manager.h"

#define ENABLE_CUSTOM_KEYS TRUE
#include "base/custom_keys.inc"

TEST(Enums, basic) {
  ASSERT_EQ(asset_type_find("style"), asset_type_find_by_value(ASSET_TYPE_STYLE));
  ASSERT_EQ(asset_type_find("xml"), asset_type_find_by_value(ASSET_TYPE_XML));
}

TEST(Enums, keys) {
  ASSERT_EQ(keys_type_find("a"), keys_type_find_by_value(TK_KEY_a));
  ASSERT_EQ(keys_type_find("A"), keys_type_find_by_value(TK_KEY_A));
  ASSERT_EQ(keys_type_find("LEFT"), keys_type_find_by_value(TK_KEY_LEFT));
  ASSERT_EQ(keys_type_find("left"), keys_type_find_by_value(TK_KEY_LEFT));
  ASSERT_EQ(keys_type_find("RETURN"), keys_type_find_by_value(TK_KEY_RETURN));
  ASSERT_EQ(keys_type_find("return"), keys_type_find_by_value(TK_KEY_RETURN));
}

TEST(Enums, custom_keys) {
  static const key_type_value_t custom_keys[] = {
      {(char*)"CUSTOM1", 0, TK_KEY_RETURN},
  };
  ASSERT_EQ(keys_type_find("CUSTOM1") == NULL, TRUE);

  keys_type_set_custom_keys(custom_keys, ARRAY_SIZE(custom_keys));
  ASSERT_EQ(keys_type_find("CUSTOM1"), keys_type_find_by_value(TK_KEY_RETURN));

  keys_type_set_custom_keys(NULL, 0);
  ASSERT_EQ(keys_type_find("CUSTOM1") == NULL, TRUE);
}

TEST(Enums, custom_keys_init) {
  ASSERT_EQ(custom_keys() == NULL && custom_keys_nr() == 0, TRUE);

  custom_keys_init(TRUE);
  ASSERT_EQ(custom_keys() != NULL || custom_keys_nr() == 0, TRUE);

  custom_keys_deinit(TRUE);
  ASSERT_EQ(custom_keys() == NULL && custom_keys_nr() == 0, TRUE);
}
