#include "base/bidi.h"
#include "tkc/wstr.h"
#include "gtest/gtest.h"

TEST(Bidi, basic) {
  bidi_t bidi;
  bidi_t* b = bidi_init(&bidi, FALSE, FALSE, BIDI_TYPE_LTR);
  ASSERT_EQ(bidi_log2vis(b, L"abc", 3), RET_OK);
  bidi_deinit(b);
}

TEST(Bidi, type) {
  ASSERT_EQ(bidi_type_from_name("ltr"), BIDI_TYPE_LTR);
  ASSERT_EQ(bidi_type_from_name("rtl"), BIDI_TYPE_RTL);
  ASSERT_EQ(bidi_type_from_name("wltr"), BIDI_TYPE_WLTR);
  ASSERT_EQ(bidi_type_from_name("wrtl"), BIDI_TYPE_WRTL);
  ASSERT_EQ(bidi_type_from_name("lro"), BIDI_TYPE_LRO);
  ASSERT_EQ(bidi_type_from_name("rlo"), BIDI_TYPE_RLO);
}

TEST(Bidi, type_null) {
  ASSERT_EQ(bidi_type_from_name(NULL), BIDI_TYPE_AUTO);
}

TEST(Bidi, type_empty) {
  ASSERT_EQ(bidi_type_from_name(""), BIDI_TYPE_AUTO);
}

TEST(Bidi, type_unknown) {
  ASSERT_EQ(bidi_type_from_name("unknown"), BIDI_TYPE_AUTO);
}

TEST(Bidi, log2vis_bad_params) {
  bidi_t bidi;
  ASSERT_EQ(bidi_log2vis(NULL, L"a", 1), RET_BAD_PARAMS);
  bidi_t* b = bidi_init(&bidi, FALSE, FALSE, BIDI_TYPE_LTR);
#ifdef WITH_TEXT_BIDI
  ASSERT_EQ(bidi_log2vis(b, NULL, 1), RET_BAD_PARAMS);
  ASSERT_EQ(bidi_log2vis(b, L"a", 0), RET_BAD_PARAMS);
#endif
  bidi_deinit(b);
}

TEST(Bidi, log2vis_contract_ltr) {
  bidi_t bidi;
  bidi_t* b = bidi_init(&bidi, FALSE, FALSE, BIDI_TYPE_LTR);
  ASSERT_EQ(bidi_log2vis(b, L"abc", 3), RET_OK);
  ASSERT_NE(b->vis_str, nullptr);
  ASSERT_EQ(b->vis_str_size, 3u);
  ASSERT_EQ(b->vis_str[0], L'a');
  ASSERT_EQ(b->vis_str[1], L'b');
  ASSERT_EQ(b->vis_str[2], L'c');
  ASSERT_EQ(b->positions_L_to_V, nullptr);
  ASSERT_EQ(b->positions_V_to_L, nullptr);
  bidi_deinit(b);
}

#ifdef WITH_TEXT_BIDI
#if defined(WITH_BIDI_SHEEN) && defined(WITH_BIDI_FRIBIDI)
#error "WITH_BIDI_SHEEN and WITH_BIDI_FRIBIDI cannot both be defined"
#elif !defined(WITH_BIDI_SHEEN) && !defined(WITH_BIDI_FRIBIDI)
#error "WITH_TEXT_BIDI requires WITH_BIDI_SHEEN or WITH_BIDI_FRIBIDI"
#endif

TEST(Bidi, log2vis_alloc_l2v_ltr) {
  bidi_t bidi;
  bidi_t* b = bidi_init(&bidi, TRUE, FALSE, BIDI_TYPE_LTR);
  ASSERT_EQ(bidi_log2vis(b, L"ab", 2), RET_OK);
  ASSERT_NE(b->positions_L_to_V, nullptr);
  ASSERT_EQ(b->positions_V_to_L, nullptr);
  ASSERT_EQ(b->positions_L_to_V[0], 0);
  ASSERT_EQ(b->positions_L_to_V[1], 1);
  bidi_deinit(b);
}

TEST(Bidi, log2vis_alloc_v2l_ltr) {
  bidi_t bidi;
  bidi_t* b = bidi_init(&bidi, FALSE, TRUE, BIDI_TYPE_LTR);
  ASSERT_EQ(bidi_log2vis(b, L"ab", 2), RET_OK);
  ASSERT_EQ(b->positions_L_to_V, nullptr);
  ASSERT_NE(b->positions_V_to_L, nullptr);
  ASSERT_EQ(b->positions_V_to_L[0], 0);
  ASSERT_EQ(b->positions_V_to_L[1], 1);
  bidi_deinit(b);
}

TEST(Bidi, log2vis_alloc_both_ltr) {
  bidi_t bidi;
  bidi_t* b = bidi_init(&bidi, TRUE, TRUE, BIDI_TYPE_LTR);
  ASSERT_EQ(bidi_log2vis(b, L"xy", 2), RET_OK);
  ASSERT_NE(b->positions_L_to_V, nullptr);
  ASSERT_NE(b->positions_V_to_L, nullptr);
  for (int i = 0; i < 2; i++) {
    ASSERT_EQ(b->positions_V_to_L[b->positions_L_to_V[i]], i);
  }
  bidi_deinit(b);
}

#if defined(WITH_BIDI_SHEEN)
TEST(Bidi, backend_macro_sheenbidi) {
#if !defined(WITH_BIDI_FRIBIDI)
  ASSERT_TRUE(true);
#else
  FAIL() << "WITH_BIDI_FRIBIDI should not be set with SheenBidi backend";
#endif
}
#elif defined(WITH_BIDI_FRIBIDI)
TEST(Bidi, backend_macro_fribidi) {
#if !defined(WITH_BIDI_SHEEN)
  ASSERT_TRUE(true);
#else
  FAIL() << "WITH_BIDI_SHEEN should not be set with FriBidi backend";
#endif
}
#endif

#endif /* WITH_TEXT_BIDI */

TEST(Bidi, auto1) {
  bidi_t bidi;
  wstr_t str;
  wstr_init(&str, 10);
  wstr_append(&str, L"م");
  wstr_append(&str, L"ص");
  wstr_append(&str, L"ر");

  bidi_t* b = bidi_init(&bidi, FALSE, FALSE, BIDI_TYPE_RTL);
  ASSERT_EQ(bidi_log2vis(b, str.str, str.size), RET_OK);

  wstr_reset(&str);
  bidi_deinit(b);
}
