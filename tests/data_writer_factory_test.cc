#include "gtest/gtest.h"
#include "tkc/data_writer_factory.h"
#include "tkc/data_writer_file.h"

TEST(DataWriterFactory, basic) {
  data_writer_t* writer = NULL;
  data_writer_factory_t* f = data_writer_factory_create();
  writer = data_writer_factory_create_writer(f, "file://./tests/testdata/test.xml");
  ASSERT_EQ(writer == NULL, true);

  ASSERT_EQ(data_writer_factory_register(f, "file", data_writer_file_create), RET_OK);

  writer = data_writer_factory_create_writer(f, "file://./tests/testdata/test.xml");
  ASSERT_EQ(writer != NULL, true);
  data_writer_destroy(writer);

  writer = data_writer_factory_create_writer(f, "./tests/testdata/test.xml");
  ASSERT_EQ(writer != NULL, true);
  data_writer_write(writer, 0, "hello ", 6);
  data_writer_write(writer, 7, "world", 5);
  data_writer_destroy(writer);

  data_writer_factory_destroy(f);
}
