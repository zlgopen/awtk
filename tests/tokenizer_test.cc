#include "tkc/tokenizer.h"
#include "gtest/gtest.h"
#include <string>

using std::string;

TEST(Tokenizer, basic) {
  tokenizer_t tokenizer;

  tokenizer_t* t = tokenizer_init(&tokenizer, "w10 h10 10 3 123.1", 0xffffff, " ");
  ASSERT_EQ(t, &tokenizer);

  ASSERT_EQ(tokenizer_has_more(t), TRUE);
  ASSERT_EQ(string("w10"), tokenizer_next(t));

  ASSERT_EQ(tokenizer_has_more(t), TRUE);
  ASSERT_EQ(string("h10"), tokenizer_next(t));

  ASSERT_EQ(tokenizer_has_more(t), TRUE);
  ASSERT_EQ(10, tokenizer_next_int(t, 0));

  ASSERT_EQ(tokenizer_has_more(t), TRUE);
  ASSERT_EQ(3, tokenizer_next_int(t, 0));

  ASSERT_EQ(tokenizer_has_more(t), TRUE);
  ASSERT_EQ(123.1f, tokenizer_next_float(t, 0));

  ASSERT_EQ(tokenizer_has_more(t), FALSE);
  ASSERT_EQ(1.0f, tokenizer_next_float(t, 1));

  tokenizer_deinit(t);
}

TEST(Tokenizer, separators) {
  tokenizer_t tokenizer;

  tokenizer_t* t = tokenizer_init(&tokenizer, "w10,   h10, 10 ,   3, 123.1", 0xffffff, " ,");
  ASSERT_EQ(t, &tokenizer);

  ASSERT_EQ(tokenizer_has_more(t), TRUE);
  ASSERT_EQ(string("w10"), tokenizer_next(t));

  ASSERT_EQ(tokenizer_has_more(t), TRUE);
  ASSERT_EQ(string("h10"), tokenizer_next(t));

  ASSERT_EQ(tokenizer_has_more(t), TRUE);
  ASSERT_EQ(10, tokenizer_next_int(t, 0));

  ASSERT_EQ(tokenizer_has_more(t), TRUE);
  ASSERT_EQ(3, tokenizer_next_int(t, 0));

  ASSERT_EQ(tokenizer_has_more(t), TRUE);
  ASSERT_EQ(123.1f, tokenizer_next_float(t, 0));

  tokenizer_deinit(t);
}

TEST(Tokenizer, single_char_token) {
  tokenizer_t tokenizer;

  tokenizer_t* t = tokenizer_init_ex(&tokenizer, "{a, b=2}", 100, "{ }", "=,");
  ASSERT_EQ(t, &tokenizer);

  ASSERT_EQ(tokenizer_has_more(t), TRUE);
  ASSERT_EQ(string("a"), tokenizer_next(t));

  ASSERT_EQ(tokenizer_has_more(t), TRUE);
  ASSERT_EQ(string(","), tokenizer_next(t));

  ASSERT_EQ(tokenizer_has_more(t), TRUE);
  ASSERT_EQ(string("b"), tokenizer_next(t));

  ASSERT_EQ(tokenizer_has_more(t), TRUE);
  ASSERT_EQ(string("="), tokenizer_next(t));

  ASSERT_EQ(tokenizer_has_more(t), TRUE);
  ASSERT_EQ(string("2"), tokenizer_next(t));

  tokenizer_deinit(t);
}

TEST(Tokenizer, until1) {
  tokenizer_t tokenizer;

  tokenizer_t* t = tokenizer_init_ex(&tokenizer, "{a==1}", 100, "{ }", "=,");
  ASSERT_EQ(t, &tokenizer);

  ASSERT_EQ(string(tokenizer_next_until(t, ",}")), string("a==1"));

  tokenizer_deinit(t);
}

TEST(Tokenizer, until2) {
  tokenizer_t tokenizer;

  tokenizer_t* t = tokenizer_init_ex(&tokenizer, "{a==1, a}", 100, "{ }", "=,");
  ASSERT_EQ(t, &tokenizer);

  ASSERT_EQ(string(tokenizer_next_until(t, ",}")), string("a==1"));

  tokenizer_deinit(t);
}

TEST(Tokenizer, expr1) {
  tokenizer_t tokenizer;

  tokenizer_t* t = tokenizer_init_ex(&tokenizer, "{\"a,a\"}", 100, "{}", "=,");
  ASSERT_EQ(string(tokenizer_next_expr_until(t, ",}")), string("\"a,a\""));

  tokenizer_deinit(t);
}

TEST(Tokenizer, expr2) {
  tokenizer_t tokenizer;

  tokenizer_t* t = tokenizer_init_ex(&tokenizer, "{\"{\\\"}\"}", 100, "{}", "=,");
  ASSERT_EQ(string(tokenizer_next_expr_until(t, ",}")), string("\"{\\\"}\""));

  tokenizer_deinit(t);
}

TEST(Tokenizer, expr3) {
  tokenizer_t tokenizer;

  tokenizer_t* t = tokenizer_init_ex(&tokenizer, "{(1+2)+\"aaa\"}", 100, "{}", "=,");
  ASSERT_EQ(string(tokenizer_next_expr_until(t, ",}")), string("(1+2)+\"aaa\""));

  tokenizer_deinit(t);
}

TEST(Tokenizer, expr4) {
  tokenizer_t tokenizer;

  tokenizer_t* t = tokenizer_init_ex(&tokenizer, "{format(\"%d\", 100)}", 100, "{}", "=,");
  ASSERT_EQ(string(tokenizer_next_expr_until(t, ",}")), string("format(\"%d\", 100)"));

  tokenizer_deinit(t);
}

TEST(Tokenizer, expr5) {
  tokenizer_t tokenizer;

  tokenizer_t* t = tokenizer_init_ex(&tokenizer, "{a, name=aaa}", 100, "{}", "=,");
  ASSERT_EQ(string(tokenizer_next_expr_until(t, ",}")), string("a"));

  tokenizer_deinit(t);
}

TEST(Tokenizer, expr6) {
  tokenizer_t tokenizer;

  tokenizer_t* t = tokenizer_init_ex(&tokenizer, "{(a+b), name=aaa}", 100, "{}", "=,");
  ASSERT_EQ(string(tokenizer_next_expr_until(t, ",}")), string("(a+b)"));

  tokenizer_deinit(t);
}

TEST(Tokenizer, expr7) {
  tokenizer_t tokenizer;

  tokenizer_t* t = tokenizer_init_ex(
      &tokenizer, "{fformat(tr(\"current temperature %f\"), $value)+\"c\"}", 100, "{}", "=,");
  ASSERT_EQ(string(tokenizer_next_expr_until(t, ",}")),
            string("fformat(tr(\"current temperature %f\"), $value)+\"c\""));

  tokenizer_deinit(t);
}

TEST(Tokenizer, expr8) {
  tokenizer_t tokenizer;

  tokenizer_t* t =
      tokenizer_init_ex(&tokenizer, "{a(b(1,2),c(2,3),d(5,6), e(\"()\"))),}", 100, "{}", "=,");
  ASSERT_EQ(string(tokenizer_next_expr_until(t, ",}")),
            string("a(b(1,2),c(2,3),d(5,6), e(\"()\")))"));

  tokenizer_deinit(t);
}

TEST(Tokenizer, expr9) {
  tokenizer_t tokenizer;

  tokenizer_t* t = tokenizer_init_ex(
      &tokenizer, "{fformat(\"%2.2lf F\", round($value * 1.8 + 32))}", 100, "{}", "=,");
  ASSERT_EQ(string(tokenizer_next_expr_until(t, ",}")),
            string("fformat(\"%2.2lf F\", round($value * 1.8 + 32))"));

  tokenizer_deinit(t);
}
