#include "base/tokenizer.h"
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
}

