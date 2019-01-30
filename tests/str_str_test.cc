#include "tkc/str_str.h"
#include "gtest/gtest.h"

#include <string>

using std::string;

static str_str_t items[] = {{"click", "CLICK"}, {"pointer", "POINTER"}, {NULL, NULL}};

TEST(StrStr, name) {
  ASSERT_EQ(string(str_str_name(items, "CLICK")), string("click"));
  ASSERT_EQ(string(str_str_name(items, "POINTER")), string("pointer"));
  ASSERT_EQ(str_str_name(items, "NOT EXIST"), (const char*)NULL);
}

TEST(StrStr, value) {
  ASSERT_EQ(string(str_str_value(items, "click")), string("CLICK"));
  ASSERT_EQ(string(str_str_value(items, "pointer")), string("POINTER"));
  ASSERT_EQ(str_str_value(items, "NOT EXIST"), (const char*)NULL);
}
