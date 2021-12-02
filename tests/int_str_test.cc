#include "tkc/int_str.h"
#include "gtest/gtest.h"

#define EVT_CLICK 1
#define EVT_POINTER_DOWN 2
#define EVT_NONE 0

#include <string>

using std::string;

static int_str_t items[] = {
    {EVT_CLICK, "CLICK"}, {EVT_POINTER_DOWN, "POINTER_DOWN"}, {EVT_NONE, NULL}};

TEST(IntStr, name) {
  ASSERT_EQ(int_str_name(items, "CLICK", -1), EVT_CLICK);
  ASSERT_EQ(int_str_name(items, "POINTER_DOWN", -1), EVT_POINTER_DOWN);
  ASSERT_EQ(int_str_name(items, "NOT EXIST", -1), -1);
}

TEST(IntStr, value) {
  ASSERT_EQ(string(int_str_value(items, EVT_CLICK)), string("CLICK"));
  ASSERT_EQ(string(int_str_value(items, EVT_POINTER_DOWN)), string("POINTER_DOWN"));
  ASSERT_EQ(int_str_value(items, -1), (const char*)NULL);
}
