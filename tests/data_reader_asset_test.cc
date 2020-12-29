#include "gtest/gtest.h"
#include "tkc/data_reader_factory.h"
#include "base/data_reader_asset.h"

TEST(DataReaderAsset, basic) {
  char data[256];
  uint32_t size = 0;
  data_reader_t* reader = NULL;
  data_reader_factory_t* f = data_reader_factory_create();

  ASSERT_EQ(data_reader_factory_register(f, "asset", data_reader_asset_create), RET_OK);

  reader = data_reader_factory_create_reader(f, "asset://test.json");
  ASSERT_EQ(reader != NULL, true);

  memset(data, 0x00, sizeof(data));
  size = data_reader_get_size(reader);
  ASSERT_EQ(size >= 24, true);
  ASSERT_EQ(data_reader_read(reader, 0, data, sizeof(data)), size);

  data_reader_destroy(reader);
  data_reader_factory_destroy(f);
}

TEST(DataReaderAsset, data) {
  char data[256];
  uint32_t size = 0;
  char url[MAX_PATH + 1];
  data_reader_t* reader = NULL;
  data_reader_factory_t* f = data_reader_factory_create();

  ASSERT_EQ(data_reader_factory_register(f, "asset", data_reader_asset_create), RET_OK);

  data_reader_asset_build_url("test.json", ASSET_TYPE_DATA, url);
  reader = data_reader_factory_create_reader(f, url);
  ASSERT_EQ(reader != NULL, true);

  memset(data, 0x00, sizeof(data));
  size = data_reader_get_size(reader);
  ASSERT_EQ(size >= 24, true);
  ASSERT_EQ(data_reader_read(reader, 0, data, sizeof(data)), size);

  data_reader_destroy(reader);
  data_reader_factory_destroy(f);
}

TEST(DataReaderAsset, ui) {
  char data[32];
  uint32_t size = 0;
  char url[MAX_PATH + 1];
  data_reader_t* reader = NULL;
  data_reader_factory_t* f = data_reader_factory_create();

  ASSERT_EQ(data_reader_factory_register(f, "asset", data_reader_asset_create), RET_OK);

  data_reader_asset_build_url("main", ASSET_TYPE_UI, url);
  reader = data_reader_factory_create_reader(f, url);
  ASSERT_EQ(reader != NULL, true);

  memset(data, 0x00, sizeof(data));
  size = data_reader_get_size(reader);
  ASSERT_EQ(size >= 24, true);
  ASSERT_EQ(data_reader_read(reader, 0, data, sizeof(data)), sizeof(data));

  data_reader_destroy(reader);
  data_reader_factory_destroy(f);
}

TEST(DataReaderAsset, style) {
  char data[32];
  uint32_t size = 0;
  char url[MAX_PATH + 1];
  data_reader_t* reader = NULL;
  data_reader_factory_t* f = data_reader_factory_create();

  ASSERT_EQ(data_reader_factory_register(f, "asset", data_reader_asset_create), RET_OK);

  data_reader_asset_build_url("default", ASSET_TYPE_STYLE, url);
  reader = data_reader_factory_create_reader(f, url);
  ASSERT_EQ(reader != NULL, true);

  memset(data, 0x00, sizeof(data));
  size = data_reader_get_size(reader);
  ASSERT_EQ(size >= 24, true);
  ASSERT_EQ(data_reader_read(reader, 0, data, sizeof(data)), sizeof(data));

  data_reader_destroy(reader);
  data_reader_factory_destroy(f);
}
