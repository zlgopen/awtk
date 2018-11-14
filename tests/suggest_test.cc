#include "common.h"
#include "gtest/gtest.h"
#include "base/suggest_words.h"
#include "tests/suggest_test.inc"

TEST(SuggestWords, basic) {
  suggest_words_t* sw = suggest_words_create((const asset_info_t*)data_suggest_test);

  ASSERT_EQ(suggest_words_find(sw, L"故"[0]), RET_OK);
  ASSERT_EQ(sw->words_nr, 8);
  assert_str_eq(L"事", sw->words);

  ASSERT_EQ(suggest_words_find(sw, L"飞"[0]), RET_OK);
  ASSERT_EQ(sw->words_nr, 15);
  assert_str_eq(L"机", sw->words);

  ASSERT_EQ(suggest_words_find(sw, L"几"[0]), RET_OK);
  ASSERT_EQ(sw->words_nr, 15);
  assert_str_eq(L"个", sw->words);

  suggest_words_destroy(sw);
}
