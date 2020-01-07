#include "tkc/utils.h"
#include "tkc/func_call_parser.h"
#include "gtest/gtest.h"
#include <math.h>

typedef struct _parser_t {
  func_call_parser_t base;
  char name[TK_NAME_LEN + 1];
  int x;
  int y;
} parser_t;

static ret_t parser_on_name(func_call_parser_t* parser, const char* func_name) {
  parser_t* p = (parser_t*)parser;

  tk_strncpy(p->name, func_name, TK_NAME_LEN);

  return RET_OK;
}

static ret_t parser_on_param(func_call_parser_t* parser, const char* name, const char* value) {
  parser_t* p = (parser_t*)parser;
  if (tk_str_eq(name, "x")) {
    p->x = tk_atoi(value);
  } else if (tk_str_eq(name, "y")) {
    p->y = tk_atoi(value);
  }

  return RET_OK;
}

static ret_t parser_on_done(func_call_parser_t* parser) {
  return RET_OK;
}

static void parser_test(const char* str) {
  parser_t parser;

  memset(&parser, 0x00, sizeof(parser));
  func_call_parser_init(&(parser.base), str, strlen(str));

  parser.base.on_name = parser_on_name;
  parser.base.on_param = parser_on_param;
  parser.base.on_done = parser_on_done;

  func_call_parser_parse(&(parser.base));

  ASSERT_EQ(parser.x, 10);
  ASSERT_EQ(parser.y, 20);
  ASSERT_EQ(tk_str_eq(parser.name, "move"), true);

  func_call_parser_deinit(&parser.base);
}

TEST(FuncCallParser, basic) {
  parser_test("move(x=10, y=20)");
  parser_test("move   (x=10, y=20)");
  parser_test("move(x = 10, y = 20 )");
  parser_test("  move(x = 10, y = 20 )");
}

static void parser_test_object(const char* str) {
  object_t* obj = func_call_parse(str, strlen(str));

  ASSERT_STREQ(obj->name, "move");
  ASSERT_EQ(object_get_prop_int(obj, "x", 0), 10);
  ASSERT_EQ(object_get_prop_int(obj, "y", 0), 20);

  object_unref(obj);
}

TEST(FuncCallParser, object) {
  parser_test_object("move(x=10, y=20)");
  parser_test_object("move   (x=10, y=20)");
  parser_test_object("move(x = 10, y = 20 )");
  parser_test_object("  move(x = 10, y = 20 )");
  parser_test_object("  move(x = 10, \r\ny \t= 20 )");
}
