#include "tkc/url.h"
#include "gtest/gtest.h"

TEST(URL, basic) {
  url_t* url = url_create(NULL);
  ASSERT_EQ(url->schema == NULL, true);
  ASSERT_EQ(url->host == NULL, true);
  ASSERT_EQ(url->path == NULL, true);
  ASSERT_EQ(url->params == NULL, true);
  ASSERT_EQ(url->user_name == NULL, true);
  ASSERT_EQ(url->password == NULL, true);
  ASSERT_EQ(url->port, 0);
  url_destroy(url);
}

TEST(URL, noschema) {
  url_t* url = url_create("www.zlg.cn");
  ASSERT_STREQ(url->schema, "http");
  ASSERT_STREQ(url->host, "www.zlg.cn");
  ASSERT_EQ(url->path == NULL, true);
  ASSERT_EQ(url->params == NULL, true);
  ASSERT_EQ(url->user_name == NULL, true);
  ASSERT_EQ(url->password == NULL, true);
  ASSERT_EQ(url->port, 80);
  url_destroy(url);
}

TEST(URL, noschema2) {
  url_t* url = url_create("www.zlg.cn:8080");
  ASSERT_STREQ(url->schema, "http");
  ASSERT_STREQ(url->host, "www.zlg.cn");
  ASSERT_EQ(url->path == NULL, true);
  ASSERT_EQ(url->params == NULL, true);
  ASSERT_EQ(url->user_name == NULL, true);
  ASSERT_EQ(url->password == NULL, true);
  ASSERT_EQ(url->port, 8080);
  url_destroy(url);
}

TEST(URL, noschema3) {
  url_t* url = url_create("www.zlg.cn:8080/abc");
  ASSERT_STREQ(url->schema, "http");
  ASSERT_STREQ(url->host, "www.zlg.cn");
  ASSERT_STREQ(url->path, "abc");
  ASSERT_EQ(url->params == NULL, true);
  ASSERT_EQ(url->user_name == NULL, true);
  ASSERT_EQ(url->password == NULL, true);
  ASSERT_EQ(url->port, 8080);
  url_destroy(url);
}

TEST(URL, noschema4) {
  url_t* url = url_create("www.zlg.cn/abc");
  ASSERT_STREQ(url->schema, "http");
  ASSERT_STREQ(url->host, "www.zlg.cn");
  ASSERT_STREQ(url->path, "abc");
  ASSERT_EQ(url->params == NULL, true);
  ASSERT_EQ(url->user_name == NULL, true);
  ASSERT_EQ(url->password == NULL, true);
  ASSERT_EQ(url->port, 80);
  url_destroy(url);
}

TEST(URL, http) {
  url_t* url = url_create("http://www.zlg.cn");
  ASSERT_STREQ(url->schema, "http");
  ASSERT_STREQ(url->host, "www.zlg.cn");
  ASSERT_EQ(url->path == NULL, true);
  ASSERT_EQ(url->params == NULL, true);
  ASSERT_EQ(url->user_name == NULL, true);
  ASSERT_EQ(url->password == NULL, true);
  ASSERT_EQ(url->port, 80);
  url_destroy(url);
}

TEST(URL, http1) {
  url_t* url = url_create("http://www.zlg.cn:8080");
  ASSERT_STREQ(url->schema, "http");
  ASSERT_STREQ(url->host, "www.zlg.cn");
  ASSERT_EQ(url->path == NULL, true);
  ASSERT_EQ(url->params == NULL, true);
  ASSERT_EQ(url->user_name == NULL, true);
  ASSERT_EQ(url->password == NULL, true);
  ASSERT_EQ(url->port, 8080);
  url_destroy(url);
}

TEST(URL, http2) {
  url_t* url = url_create("http://www.zlg.cn/abc");
  ASSERT_STREQ(url->schema, "http");
  ASSERT_STREQ(url->host, "www.zlg.cn");
  ASSERT_STREQ(url->path, "/abc");
  ASSERT_EQ(url->params == NULL, true);
  ASSERT_EQ(url->user_name == NULL, true);
  ASSERT_EQ(url->password == NULL, true);
  ASSERT_EQ(url->port, 80);
  url_destroy(url);
}

TEST(URL, http3) {
  url_t* url = url_create("http://www.zlg.cn?name=jim");
  ASSERT_STREQ(url->schema, "http");
  ASSERT_STREQ(url->host, "www.zlg.cn");
  ASSERT_EQ(url->params != NULL, true);
  ASSERT_EQ(url->user_name == NULL, true);
  ASSERT_EQ(url->password == NULL, true);
  ASSERT_EQ(url->port, 80);
  ASSERT_STREQ(url_get_param(url, "name"), "jim");
  url_destroy(url);
}

TEST(URL, http4) {
  url_t* url = url_create("http://www.zlg.cn/abc?name=jim");
  ASSERT_STREQ(url->schema, "http");
  ASSERT_STREQ(url->host, "www.zlg.cn");
  ASSERT_STREQ(url->path, "/abc");
  ASSERT_EQ(url->params != NULL, true);
  ASSERT_EQ(url->user_name == NULL, true);
  ASSERT_EQ(url->password == NULL, true);
  ASSERT_EQ(url->port, 80);
  ASSERT_STREQ(url_get_param(url, "name"), "jim");
  url_destroy(url);
}

TEST(URL, http5) {
  url_t* url = url_create("http://www.zlg.cn/abc?name=jim&");
  ASSERT_STREQ(url->schema, "http");
  ASSERT_STREQ(url->host, "www.zlg.cn");
  ASSERT_STREQ(url->path, "/abc");
  ASSERT_EQ(url->params != NULL, true);
  ASSERT_EQ(url->user_name == NULL, true);
  ASSERT_EQ(url->password == NULL, true);
  ASSERT_EQ(url->port, 80);
  ASSERT_STREQ(url_get_param(url, "name"), "jim");
  url_destroy(url);
}

TEST(URL, http6) {
  url_t* url = url_create("http://www.zlg.cn/abc?name=jim&age=100");
  ASSERT_STREQ(url->schema, "http");
  ASSERT_STREQ(url->host, "www.zlg.cn");
  ASSERT_STREQ(url->path, "/abc");
  ASSERT_EQ(url->params != NULL, true);
  ASSERT_EQ(url->user_name == NULL, true);
  ASSERT_EQ(url->password == NULL, true);
  ASSERT_EQ(url->port, 80);
  ASSERT_STREQ(url_get_param(url, "name"), "jim");
  ASSERT_STREQ(url_get_param(url, "age"), "100");
  url_destroy(url);
}

TEST(URL, http7) {
  url_t* url = url_create("http://www.zlg.cn/abc?name=jim&age=100&");
  ASSERT_STREQ(url->schema, "http");
  ASSERT_STREQ(url->host, "www.zlg.cn");
  ASSERT_STREQ(url->path, "/abc");
  ASSERT_EQ(url->params != NULL, true);
  ASSERT_EQ(url->user_name == NULL, true);
  ASSERT_EQ(url->password == NULL, true);
  ASSERT_EQ(url->port, 80);
  ASSERT_STREQ(url_get_param(url, "name"), "jim");
  ASSERT_STREQ(url_get_param(url, "age"), "100");
  url_destroy(url);
}

TEST(URL, http8) {
  url_t* url = url_create("http://jim:1234@www.zlg.cn/abc?name=jim&age=100&");
  ASSERT_STREQ(url->schema, "http");
  ASSERT_STREQ(url->host, "www.zlg.cn");
  ASSERT_STREQ(url->path, "/abc");
  ASSERT_EQ(url->params != NULL, true);
  ASSERT_STREQ(url->user_name, "jim");
  ASSERT_STREQ(url->password, "1234");
  ASSERT_EQ(url->port, 80);
  ASSERT_STREQ(url_get_param(url, "name"), "jim");
  ASSERT_STREQ(url_get_param(url, "age"), "100");
  url_destroy(url);
}

TEST(URL, http9) {
  url_t* url = url_create("jim:1234@www.zlg.cn/abc?name=jim&age=100&");
  ASSERT_STREQ(url->schema, "http");
  ASSERT_STREQ(url->host, "www.zlg.cn");
  ASSERT_STREQ(url->path, "/abc");
  ASSERT_EQ(url->params != NULL, true);
  ASSERT_STREQ(url->user_name, "jim");
  ASSERT_STREQ(url->password, "1234");
  ASSERT_EQ(url->port, 80);
  ASSERT_STREQ(url_get_param(url, "name"), "jim");
  ASSERT_STREQ(url_get_param(url, "age"), "100");
  url_destroy(url);
}

TEST(URL, file0) {
  url_t* url = url_create("file:///abc");
  ASSERT_STREQ(url->schema, "file");
  ASSERT_EQ(url->host == NULL, true);
  ASSERT_EQ(url->params == NULL, true);
  ASSERT_STREQ(url->path, "/abc");

  url_destroy(url);
}

TEST(URL, file1) {
  url_t* url = url_create("file://./abc");
  ASSERT_STREQ(url->schema, "file");
  ASSERT_EQ(url->host == NULL, true);
  ASSERT_EQ(url->params == NULL, true);
  ASSERT_STREQ(url->path, "./abc");

  url_destroy(url);
}

TEST(URL, file2) {
  url_t* url = url_create("file://c:/abc");
  ASSERT_STREQ(url->schema, "file");
  ASSERT_EQ(url->host == NULL, true);
  ASSERT_EQ(url->params == NULL, true);
  ASSERT_STREQ(url->path, "c:/abc");

  url_destroy(url);
}

TEST(URL, file3) {
  url_t* url = url_create("file://c:\\abc");
  ASSERT_STREQ(url->schema, "file");
  ASSERT_EQ(url->host == NULL, true);
  ASSERT_EQ(url->params == NULL, true);
  ASSERT_STREQ(url->path, "c:\\abc");

  url_destroy(url);
}

TEST(URL, file4) {
  url_t* url = url_create("c:\\abc");
  ASSERT_STREQ(url->schema, "file");
  ASSERT_EQ(url->host == NULL, true);
  ASSERT_EQ(url->params == NULL, true);
  ASSERT_STREQ(url->path, "c:\\abc");

  url_destroy(url);
}

TEST(URL, file5) {
  url_t* url = url_create("/abc");
  ASSERT_STREQ(url->schema, "file");
  ASSERT_EQ(url->host == NULL, true);
  ASSERT_EQ(url->params == NULL, true);
  ASSERT_STREQ(url->path, "/abc");

  url_destroy(url);
}

TEST(URL, file6) {
  url_t* url = url_create("./abc");
  ASSERT_STREQ(url->schema, "file");
  ASSERT_EQ(url->host == NULL, true);
  ASSERT_EQ(url->params == NULL, true);
  ASSERT_STREQ(url->path, "./abc");

  url_destroy(url);
}

TEST(URL, file7) {
  url_t* url = url_create("/abc");
  ASSERT_STREQ(url->schema, "file");
  ASSERT_EQ(url->host == NULL, true);
  ASSERT_EQ(url->params == NULL, true);
  ASSERT_STREQ(url->path, "/abc");

  url_destroy(url);
}

TEST(URL, mem) {
  url_t* url = url_create("mem://0xaabbccdd");
  ASSERT_STREQ(url->schema, "mem");
  ASSERT_EQ(url->path == NULL, true);
  ASSERT_EQ(url->params == NULL, true);
  ASSERT_STREQ(url->host, "0xaabbccdd");

  url_destroy(url);
}

TEST(URL, asset) {
  url_t* url = url_create("asset://ui/main");
  ASSERT_STREQ(url->schema, "asset");
  ASSERT_STREQ(url->host, "ui");
  ASSERT_EQ(url->params == NULL, true);
  ASSERT_STREQ(url->path, "/main");

  url_destroy(url);
}

TEST(URL, serial) {
  url_t* url = url_create("serial:///dev/cu.usbserial-1410");
  ASSERT_STREQ(url->schema, "serial");
  ASSERT_STREQ(url->host, "");
  ASSERT_EQ(url->params == NULL, true);
  ASSERT_STREQ(url->path, "/dev/cu.usbserial-1410");

  url_destroy(url);
}
