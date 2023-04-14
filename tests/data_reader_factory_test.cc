#include "gtest/gtest.h"
#include "tkc/path.h"
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

#ifdef WIN32
  char cwd[MAX_PATH + 1] = {0};
  char path[MAX_PATH + 1] = {0};

  path_cwd(cwd);
  path_build(path, sizeof(path) - 1, cwd, "tests\\testdata\\main.xml", NULL);
  log_debug("%s\n", path);
  reader = data_reader_factory_create_reader(f, path);
  ASSERT_EQ(reader != NULL, true);
  data_reader_destroy(reader);
#endif

  data_reader_factory_destroy(f);
}
