#include "gtest/gtest.h"
#include "tkc/data_reader_factory.h"
#include "tkc/data_reader_mem.h"

TEST(DataReaderMem, basic) {
  char buffer[32];
  char url[MAX_PATH + 1];
  const char* data = "hello world";
  data_reader_t* reader = NULL;
  data_reader_factory_t* f = data_reader_factory_create();

  data_reader_mem_build_url(data, strlen(data), url);
  reader = data_reader_factory_create_reader(f, url);
  ASSERT_EQ(reader == NULL, true);

  ASSERT_EQ(data_reader_factory_register(f, "mem", data_reader_mem_create), RET_OK);

  reader = data_reader_factory_create_reader(f, url);
  ASSERT_EQ(reader != NULL, true);
  data_reader_destroy(reader);

  reader = data_reader_factory_create_reader(f, url);
  ASSERT_EQ(reader != NULL, true);

  ASSERT_EQ(data_reader_read(reader, 0, buffer, 5), 5);
  ASSERT_EQ(memcmp(buffer, "hello", 5), 0);

  ASSERT_EQ(data_reader_read(reader, 1, buffer, 5), 5);
  ASSERT_EQ(memcmp(buffer, "ello ", 5), 0);
  data_reader_destroy(reader);

  data_reader_factory_destroy(f);
}
