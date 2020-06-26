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
