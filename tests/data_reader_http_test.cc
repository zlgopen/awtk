#include "gtest/gtest.h"
#include "tkc/data_reader_factory.h"
#include "tkc/data_reader_http.h"

TEST(DataReaderHTTP, http) {
  data_reader_t* reader = data_reader_http_create("http://www.baidu.com");

  if (reader != NULL) {
    char buffer[1024];
    uint32_t size = data_reader_get_size(reader);
    int32_t ret = data_reader_read(reader, 0, buffer, sizeof(buffer));
    ASSERT_EQ(size > 0, true);
    ASSERT_EQ((size_t)ret, sizeof(buffer));
    ret = data_reader_read(reader, 0, buffer, sizeof(buffer));
    data_reader_destroy(reader);
  }
}

TEST(DataReaderHTTP, https) {
  data_reader_t* reader = data_reader_http_create("https://www.baidu.com");

  if (reader != NULL) {
    char buffer[1024];
    uint32_t size = data_reader_get_size(reader);
    int32_t ret = data_reader_read(reader, 0, buffer, sizeof(buffer));
    ASSERT_EQ(size > 0, true);
    ASSERT_EQ((size_t)ret, sizeof(buffer));

    ret = data_reader_read(reader, 0, buffer, sizeof(buffer));
    data_reader_destroy(reader);
  }
}
