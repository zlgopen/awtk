#include "gtest/gtest.h"
#include "tkc/data_reader_file.h"
#include "base/asset_loader_zip.h"

TEST(AssetLoaderZip, basic) {
  asset_loader_t* loader = asset_loader_zip_create("tests/testdata/assets.zip");
  asset_info_t* info = asset_loader_load(loader, ASSET_TYPE_UI, ASSET_TYPE_UI_BIN,
                                         "assets/default/raw/ui/basic.bin", "basic");
  ASSERT_EQ(info != NULL, true);
  asset_info_destroy(info);

  info = asset_loader_load(loader, ASSET_TYPE_UI, ASSET_TYPE_UI_BIN,
                           "assets/default/raw/ui/main.bin", "main");
  ASSERT_EQ(info != NULL, true);
  asset_info_destroy(info);

  asset_loader_destroy(loader);
}

TEST(AssetLoaderZip, reader) {
  data_reader_t* reader = data_reader_file_create("tests/testdata/assets.zip");
  asset_loader_t* loader = asset_loader_zip_create_with_reader(reader, TRUE);
  asset_info_t* info = asset_loader_load(loader, ASSET_TYPE_UI, ASSET_TYPE_UI_BIN,
                                         "assets/default/raw/ui/basic.bin", "basic");
  ASSERT_EQ(info != NULL, true);
  asset_info_destroy(info);

  info = asset_loader_load(loader, ASSET_TYPE_UI, ASSET_TYPE_UI_BIN,
                           "assets/default/raw/ui/main.bin", "main");
  ASSERT_EQ(info != NULL, true);
  asset_info_destroy(info);

  asset_loader_destroy(loader);
}
