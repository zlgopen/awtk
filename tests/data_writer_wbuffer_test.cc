#include "gtest/gtest.h"
#include "tkc/data_writer_factory.h"
#include "tkc/data_writer_wbuffer.h"

TEST(DataWriterWbuffer, wbuffer) {
  wbuffer_t wb;
  char url[MAX_PATH + 1];
  data_writer_t* writer = NULL;
  data_writer_factory_t* f = data_writer_factory_create();

  wbuffer_init_extendable(&wb);
  data_writer_wbuffer_build_url(&wb, url);
  writer = data_writer_factory_create_writer(f, url);
  ASSERT_EQ(writer == NULL, true);

  ASSERT_EQ(data_writer_factory_register(f, "wbuffer", data_writer_wbuffer_create), RET_OK);

  writer = data_writer_factory_create_writer(f, url);
  ASSERT_EQ(writer != NULL, true);
  data_writer_destroy(writer);

  writer = data_writer_factory_create_writer(f, url);
  ASSERT_EQ(writer != NULL, true);
  data_writer_write(writer, 0, "hello ", 6);
  ASSERT_EQ(memcmp(wb.data, "hello ", 6), 0);
  ASSERT_EQ(wb.cursor, 6);

  data_writer_write(writer, 6, "world", 5);
  ASSERT_EQ(wb.cursor, 11);
  ASSERT_EQ(memcmp(wb.data, "hello world", 11), 0);

  data_writer_destroy(writer);

  data_writer_factory_destroy(f);
  wbuffer_deinit(&wb);
}
