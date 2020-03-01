#include "gtest/gtest.h"
#include "tkc/data_reader_factory.h"
#include "tkc/data_reader_file.h"

TEST(DataReaderFactory, basic) {
  data_reader_t* reader = NULL;
  data_reader_factory_t* f = data_reader_factory_create();
  reader = data_reader_factory_create_reader(f, "file://./tests/testdata/main.xml");
  ASSERT_EQ(reader == NULL, true);

  ASSERT_EQ(data_reader_factory_register(f, "file", data_reader_file_create), RET_OK);

  reader = data_reader_factory_create_reader(f, "file://./tests/testdata/main.xml");
  ASSERT_EQ(reader != NULL, true);
  data_reader_destroy(reader);

  reader = data_reader_factory_create_reader(f, "./tests/testdata/main.xml");
  ASSERT_EQ(reader != NULL, true);

  data_reader_destroy(reader);
  data_reader_factory_destroy(f);
}
