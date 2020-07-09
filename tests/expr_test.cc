#include "tkc/expr_eval.h"
#include "gtest/gtest.h"

TEST(ExprEval, basic) {
  ASSERT_EQ(0, tk_expr_eval("10%2"));
  ASSERT_EQ(1, tk_expr_eval("1 ? 1 : 2"));
  ASSERT_EQ(2, tk_expr_eval("0 ? 1 : 2"));
  ASSERT_EQ(1, tk_expr_eval("1 < 2 ? 1 : 2"));
  ASSERT_EQ(2, tk_expr_eval("1 > 2 ? 1 : 2"));
  ASSERT_EQ(11, tk_expr_eval("1 < 2 ? \"11\" : \"22\""));
  ASSERT_EQ(22, tk_expr_eval("1 > 2 ? \"11\" : \"22\""));
  ASSERT_EQ(0, tk_expr_eval("10%2"));
  ASSERT_EQ(1, tk_expr_eval("11%2"));
  ASSERT_EQ(1, tk_expr_eval("11%10"));
  ASSERT_EQ(0, tk_expr_eval("(2-2) && 3 + 2 * 2"));
  ASSERT_EQ(0, tk_expr_eval("(2-2) && (3 + 2 * 2)"));
  ASSERT_EQ(1, tk_expr_eval("(2-2) || 3 + 2 * 2"));
  ASSERT_EQ(1, tk_expr_eval("(2-2) || (3 + 2 * 2)"));
  ASSERT_EQ(0, tk_expr_eval("2-2 && 3 + 2 * 2"));
  ASSERT_EQ(0, tk_expr_eval("2-2 && (3 + 2 * 2)"));
  ASSERT_EQ(1, tk_expr_eval("2-2 || 3 + 2 * 2"));
  ASSERT_EQ(1, tk_expr_eval("2-2 || (3 + 2 * 2)"));
  ASSERT_EQ(1, tk_expr_eval("2*3 && 3 + 2 * 2"));
  ASSERT_EQ(1, tk_expr_eval("2*3 && (3 + 2 * 2)"));
  ASSERT_EQ(1, tk_expr_eval("2*3 || 3 + 2 * 2"));
  ASSERT_EQ(1, tk_expr_eval("2*3 || (3 + 2 * 2)"));
  ASSERT_EQ(0, tk_expr_eval("2*3 > 3 + 2 * 2"));
  ASSERT_EQ(0, tk_expr_eval("2-3 > 3 + 2 * 2"));
  ASSERT_EQ(1, tk_expr_eval("1"));
  ASSERT_EQ(2, tk_expr_eval("1 + 1"));
  ASSERT_EQ(2, tk_expr_eval("1 * 2"));
  ASSERT_EQ(2, tk_expr_eval("4/2"));
  ASSERT_EQ(4, tk_expr_eval("(1+1)*2"));
  ASSERT_EQ(1, tk_expr_eval("\"aa\"==\"aa\""));
  ASSERT_EQ(0, tk_expr_eval("\"aa\"==\"bb\""));
}

TEST(ExprEval, func) {
  ASSERT_EQ(11, tk_expr_eval("if(1, 11, 22)"));
  ASSERT_EQ(22, tk_expr_eval("if(0, 11, 22)"));
  ASSERT_EQ(11, tk_expr_eval("if(1, \"11\", \"22\")"));
  ASSERT_EQ(22, tk_expr_eval("if(0, \"11\", \"22\")"));
}

TEST(ExprEval, float) {
  ASSERT_EQ(1.1, tk_expr_eval("1.1"));
  ASSERT_EQ(22.2, tk_expr_eval("22.2"));
  ASSERT_EQ(333.3, tk_expr_eval("333.3"));
}

TEST(ExprEval, num) {
  ASSERT_EQ(1, tk_expr_eval("1"));
  ASSERT_EQ(22, tk_expr_eval("22"));
  ASSERT_EQ(333, tk_expr_eval("333"));
}

TEST(ExprEval, hex) {
  ASSERT_EQ(0x1, tk_expr_eval("0x1"));
  ASSERT_EQ(0x22, tk_expr_eval("0x22"));
  ASSERT_EQ(0x333, tk_expr_eval("0x333"));
  ASSERT_EQ(0xaaaa, tk_expr_eval("0xaaaa"));
  ASSERT_EQ(0xaaaa, tk_expr_eval("0xAAAA"));
  ASSERT_EQ(0xffff, tk_expr_eval("0xffff"));
  ASSERT_EQ(0xffff, tk_expr_eval("0xFFFF"));
  ASSERT_EQ(0xaaaaffff, tk_expr_eval("0xaaaaFFFF"));
  ASSERT_EQ(0xffffffff, tk_expr_eval("0xffffFFFF"));
  ASSERT_EQ(0xfffffffa, tk_expr_eval("0xffffFFFa"));
  ASSERT_EQ(0xfffafffa, tk_expr_eval("0xfffaFFFa"));
}

TEST(ExprEval, fformat) {
  char result[128];
  ASSERT_STREQ("1.23", tk_expr_eval_str("fformat(\"%2.2f\", 1.23)", result, sizeof(result)));
  ASSERT_STREQ("temp:1.20",
               tk_expr_eval_str("fformat(\"temp:%2.2f\", 1.2)", result, sizeof(result)));
}

TEST(ExprEval, iformat) {
  char result[128];
  ASSERT_STREQ("123", tk_expr_eval_str("iformat(\"%d\", 123)", result, sizeof(result)));
  ASSERT_STREQ("temp:123", tk_expr_eval_str("iformat(\"temp:%d\", 123)", result, sizeof(result)));
}
