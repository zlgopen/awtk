#include "gtest/gtest.h"
#include "tkc/data_reader_file.h"
#include "base/asset_loader_zip.h"
#include "base/assets_manager.h"

extern "C" ret_t assets_manager_build_asset_filename(assets_manager_t* am, char* path,
                                                     uint32_t size, const char* theme,
                                                     bool_t ratio_sensitive, const char* subpath,
                                                     const char* name, const char* extname);

TEST(AssetLoaderZip, basic) {
  char path[MAX_PATH + 1];
  assets_manager_t* am = assets_manager();
  asset_loader_t* loader = asset_loader_zip_create("tests/testdata/assets.zip");
  asset_info_t* info;
  ret_t ret;

  ret = assets_manager_build_asset_filename(am, path, MAX_PATH, "default", FALSE, "ui", "basic",
                                            ".bin");
  ASSERT_EQ(ret == RET_OK, true);

  info = asset_loader_load(loader, ASSET_TYPE_UI, ASSET_TYPE_UI_BIN, path, "basic");
  ASSERT_EQ(info != NULL, true);
  asset_info_destroy(info);

  ret = assets_manager_build_asset_filename(am, path, MAX_PATH, "default", FALSE, "ui", "main",
                                            ".bin");
  ASSERT_EQ(ret == RET_OK, true);

  info = asset_loader_load(loader, ASSET_TYPE_UI, ASSET_TYPE_UI_BIN, path, "main");
  ASSERT_EQ(info != NULL, true);
  asset_info_destroy(info);

  asset_loader_destroy(loader);
}

TEST(AssetLoaderZip, reader) {
  char path[MAX_PATH + 1];
  assets_manager_t* am = assets_manager();
  data_reader_t* reader = data_reader_file_create("tests/testdata/assets.zip");
  asset_loader_t* loader = asset_loader_zip_create_with_reader(reader, TRUE);
  asset_info_t* info;
  ret_t ret;

  ret = assets_manager_build_asset_filename(am, path, MAX_PATH, "default", FALSE, "ui", "basic",
                                            ".bin");
  ASSERT_EQ(ret == RET_OK, true);

  info = asset_loader_load(loader, ASSET_TYPE_UI, ASSET_TYPE_UI_BIN, path, "basic");
  ASSERT_EQ(info != NULL, true);
  asset_info_destroy(info);

  ret = assets_manager_build_asset_filename(am, path, MAX_PATH, "default", FALSE, "ui", "main",
                                            ".bin");
  ASSERT_EQ(ret == RET_OK, true);

  info = asset_loader_load(loader, ASSET_TYPE_UI, ASSET_TYPE_UI_BIN, path, "main");
  ASSERT_EQ(info != NULL, true);
  asset_info_destroy(info);

  asset_loader_destroy(loader);
}
