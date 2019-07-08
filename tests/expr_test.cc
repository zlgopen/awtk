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
