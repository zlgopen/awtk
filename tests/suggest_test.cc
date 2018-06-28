#include "gtest/gtest.h"
#include "base/suggest_words.h"
#include "tests/suggest_test.inc"

TEST(SuggestWords, basic) {
  suggest_words_t* sw = suggest_words_create((const resource_info_t*)data_suggest_test);

  ASSERT_EQ(suggest_words_find(sw, L"故"[0]), RET_OK);
  ASSERT_EQ(sw->words_nr, 3);
  ASSERT_EQ(strcmp(sw->words, "事"), 0);

  ASSERT_EQ(suggest_words_find(sw, L"飞"[0]), RET_OK);
  ASSERT_EQ(sw->words_nr, 3);
  ASSERT_EQ(strcmp(sw->words, "刀"), 0);

  ASSERT_EQ(suggest_words_find(sw, L"几"[0]), RET_OK);
  ASSERT_EQ(sw->words_nr, 6);
  ASSERT_EQ(strcmp(sw->words, "个"), 0);

  suggest_words_destroy(sw);
}
