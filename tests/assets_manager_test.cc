#include "base/assets_manager.h"
#include "gtest/gtest.h"
#include "tkc/str.h"

TEST(AssetsManager, basic) {
  const asset_info_t* null_res = NULL;
  assets_manager_t* rm = assets_manager_create(10);

  asset_info_t img1 = {ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_BMP, TRUE, 100, 0, "img1"};
  asset_info_t img2 = {ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_PNG, TRUE, 101, 0, "img2"};
  asset_info_t ui1 = {ASSET_TYPE_UI, ASSET_TYPE_UI_BIN, TRUE, 102, 0, "ui1"};
  asset_info_t ui2 = {ASSET_TYPE_UI, ASSET_TYPE_UI_XML, TRUE, 103, 0, "ui2"};

  ASSERT_EQ(assets_manager_add(rm, &img1), RET_OK);
  ASSERT_EQ(assets_manager_add(rm, &img2), RET_OK);
  ASSERT_EQ(assets_manager_add(rm, &ui1), RET_OK);
  ASSERT_EQ(assets_manager_add(rm, &ui2), RET_OK);

  ASSERT_EQ(assets_manager_ref(rm, ASSET_TYPE_IMAGE, "img1"), &img1);
  ASSERT_EQ(assets_manager_ref(rm, ASSET_TYPE_IMAGE, "img2"), &img2);
  ASSERT_EQ(assets_manager_ref(rm, ASSET_TYPE_IMAGE, "img3"), null_res);

  ASSERT_EQ(assets_manager_ref(rm, ASSET_TYPE_UI, "ui1"), &ui1);
  ASSERT_EQ(assets_manager_ref(rm, ASSET_TYPE_UI, "ui2"), &ui2);

  assets_manager_destroy(rm);
}

TEST(AssetsManager, file_image) {
  const asset_info_t* r = NULL;
  assets_manager_t* rm = assets_manager_create(10);

  r = assets_manager_ref(rm, ASSET_TYPE_IMAGE, "earth");
  ASSERT_EQ(r != NULL, true);
  ASSERT_EQ(r->refcount, 1);
  ASSERT_EQ(assets_manager_unref(rm, r), RET_OK);

  assets_manager_destroy(rm);
}

TEST(AssetsManager, file_script) {
  const asset_info_t* r = NULL;
  assets_manager_t* rm = assets_manager_create(10);

  r = assets_manager_ref(rm, ASSET_TYPE_SCRIPT, "dummy");
  ASSERT_EQ(r != NULL, true);
  ASSERT_EQ(r->refcount, 2);

  r = assets_manager_find_in_cache(rm, ASSET_TYPE_SCRIPT, "dummy");
  ASSERT_EQ(r != NULL, true);

  ASSERT_EQ(assets_manager_unref(rm, r), RET_OK);
  r = assets_manager_find_in_cache(rm, ASSET_TYPE_SCRIPT, "dummy");
  ASSERT_EQ(r->refcount, 1);

  assets_manager_destroy(rm);
}

TEST(AssetsManager, clearCache) {
  const asset_info_t* r = NULL;
  assets_manager_t* rm = assets_manager_create(10);

  r = assets_manager_find_in_cache(rm, ASSET_TYPE_IMAGE, "earth");
  ASSERT_EQ(r == NULL, true);

  r = assets_manager_ref(rm, ASSET_TYPE_IMAGE, "earth");
  ASSERT_EQ(r != NULL, true);

  ASSERT_EQ(assets_manager_unref(rm, r), RET_OK);
  r = assets_manager_find_in_cache(rm, ASSET_TYPE_IMAGE, "earth");
  ASSERT_EQ(r == NULL, true);

  r = assets_manager_ref(rm, ASSET_TYPE_IMAGE, "earth");
  ASSERT_EQ(r != NULL, true);
  ASSERT_EQ(assets_manager_unref(rm, r), RET_OK);

  ASSERT_EQ(assets_manager_clear_cache(rm, ASSET_TYPE_IMAGE), RET_OK);

  r = assets_manager_find_in_cache(rm, ASSET_TYPE_IMAGE, "earth");
  ASSERT_EQ(r == NULL, true);

  assets_manager_destroy(rm);
}

TEST(AssetsManager, xml) {
  const asset_info_t* r = NULL;
  assets_manager_t* rm = assets_manager();
#ifdef WITH_FS_RES
  r = assets_manager_ref(rm, ASSET_TYPE_XML, "test");
  ASSERT_EQ(r != NULL, true);
  assets_manager_unref(rm, r);
#else
  r = assets_manager_find_in_cache(rm, ASSET_TYPE_XML, "test");
  ASSERT_EQ(r != NULL, true);
#endif /*WITH_FS_RES*/
}

TEST(AssetsManager, data) {
  const asset_info_t* r = NULL;
  assets_manager_t* rm = assets_manager();

#ifdef WITH_FS_RES
  r = assets_manager_ref(rm, ASSET_TYPE_DATA, "test.dat");
  ASSERT_EQ(r != NULL, true);
  assets_manager_unref(rm, r);
#else
  r = assets_manager_find_in_cache(rm, ASSET_TYPE_DATA, "test.dat");
  ASSERT_EQ(r != NULL, true);
#endif /*WITH_FS_RES*/
}

TEST(AssetsManager, json) {
  const asset_info_t* r = NULL;
  assets_manager_t* rm = assets_manager();
#ifdef WITH_FS_RES
  str_t s;
  str_init(&s, 0);
  r = assets_manager_ref(rm, ASSET_TYPE_DATA, "com.zlg.app.json");
  ASSERT_EQ(r != NULL, true);
  str_set_with_len(&s, (const char*)(r->data), r->size);
  str_replace(&s, "\r\n", "\n");
  ASSERT_STREQ(s.str, "{}\n");
  str_reset(&s);
  assets_manager_unref(rm, r);
#else
  r = assets_manager_find_in_cache(rm, ASSET_TYPE_DATA, "com.zlg.app.json");
  ASSERT_EQ(r != NULL, true);
  ASSERT_EQ(strncmp((const char*)(r->data), "{}\n", 3), 0);
#endif /*WITH_FS_RES*/
}

TEST(AssetsManager, json_dupname) {
  const asset_info_t* r = NULL;
  assets_manager_t* rm = assets_manager();
#ifdef WITH_FS_RES
  r = assets_manager_ref(rm, ASSET_TYPE_DATA, "test.json");
  ASSERT_EQ(r != NULL, true);
  assets_manager_unref(rm, r);
#else
  r = assets_manager_find_in_cache(rm, ASSET_TYPE_DATA, "test.json");
  ASSERT_EQ(r != NULL, true);
#endif /*WITH_FS_RES*/
}

TEST(AssetsManager, any) {
  const asset_info_t* r = NULL;
  assets_manager_t* rm = assets_manager();
#ifdef WITH_FS_RES
  str_t s;
  str_init(&s, 0);
  r = assets_manager_ref(rm, ASSET_TYPE_DATA, "a-b-c.any");
  ASSERT_EQ(r != NULL, true);
  str_set_with_len(&s, (const char*)(r->data), r->size);
  str_replace(&s, "\r\n", "\n");
  ASSERT_STREQ(s.str, "abc\n");
  str_reset(&s);
  assets_manager_unref(rm, r);
#else
  r = assets_manager_find_in_cache(rm, ASSET_TYPE_DATA, "a-b-c.any");
  ASSERT_EQ(r != NULL, true);
  ASSERT_EQ(strncmp((const char*)(r->data), "abc\n", 4), 0);
#endif /*WITH_FS_RES*/
}

static asset_info_t* custom_load_asset(assets_manager_t* am, asset_type_t type, const char* name) {
  asset_info_t* info = asset_info_create(ASSET_TYPE_DATA, 0, "test.any", 5);
  memcpy(info->data, "abcd", 5);

  return info;
}

TEST(AssetsManager, custom_load_asset) {
  const asset_info_t* r = NULL;
  assets_manager_t* rm = assets_manager();

  assets_manager_set_custom_load_asset(rm, custom_load_asset, NULL);
  r = assets_manager_ref(rm, ASSET_TYPE_DATA, "test.any");

  ASSERT_EQ(r != NULL, true);
  ASSERT_STREQ((const char*)(r->data), "abcd");
  assets_manager_set_custom_load_asset(rm, NULL, NULL);

  assets_manager_unref(rm, r);
}
