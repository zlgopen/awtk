#include "base/glyph_cache.h"
#include "gtest/gtest.h"

static uint32_t s_destroy_count = 0;

/* 记下销毁次数，再按glyph_destroy释放 */
static ret_t glyph_cache_test_destroy(glyph_t* g) {
  s_destroy_count++;

  return glyph_destroy(g);
}

/* 统计还能查到多少个，应该和容量/保留个数一致 */
static uint32_t glyph_cache_test_alive(glyph_cache_t* c, uint32_t nr, font_size_t size) {
  uint32_t i = 0;
  uint32_t n = 0;
  glyph_t g;

  memset(&g, 0x00, sizeof(g));
  for (i = 0; i < nr; i++) {
    if (glyph_cache_lookup(c, i, size, &g) == RET_OK) {
      n++;
    }
  }

  return n;
}

TEST(GlyphCache, basic) {
  uint16_t i = 0;
  uint16_t size = 10;
  uint16_t nr = 10000;
  glyph_t g;
  glyph_cache_t* c = glyph_cache_create(64, (tk_destroy_t)glyph_destroy);
  ASSERT_TRUE(c != NULL);

  memset(&g, 0x00, sizeof(g));
  for (i = 0; i < nr; i++) {
    ASSERT_EQ(glyph_cache_lookup(c, i, size, &g) == RET_NOT_FOUND, true);
    ASSERT_EQ(glyph_cache_add(c, i, size, glyph_clone(&g)) == RET_OK, true);
    ASSERT_EQ(glyph_cache_lookup(c, i, size, &g) == RET_OK, true);
  }

  /* 塞满后不再增长 */
  ASSERT_EQ(glyph_cache_test_alive(c, nr, size), 64u);

  ASSERT_EQ(glyph_cache_destroy(c), RET_OK);
}

TEST(GlyphCache, shrink) {
  uint16_t i = 0;
  uint16_t size = 10;
  uint16_t nr = 64;
  glyph_t g;
  glyph_cache_t* c = glyph_cache_create(nr, (tk_destroy_t)glyph_destroy);
  ASSERT_TRUE(c != NULL);

  memset(&g, 0x00, sizeof(g));
  for (i = 0; i < nr; i++) {
    ASSERT_EQ(glyph_cache_add(c, i, size, glyph_clone(&g)) == RET_OK, true);
  }
  ASSERT_EQ(glyph_cache_test_alive(c, nr, size), (uint32_t)nr);

  /* 缩到 nr/4：剩下的个数和能查到的一致 */
  ASSERT_EQ(glyph_cache_shrink(c, nr / 4), RET_OK);
  ASSERT_EQ(glyph_cache_test_alive(c, nr, size), (uint32_t)(nr / 4));

  /* 再缩到相同个数、以及比当前大的个数：都不变 */
  ASSERT_EQ(glyph_cache_shrink(c, nr / 4), RET_OK);
  ASSERT_EQ(glyph_cache_test_alive(c, nr, size), (uint32_t)(nr / 4));
  ASSERT_EQ(glyph_cache_shrink(c, nr), RET_OK);
  ASSERT_EQ(glyph_cache_test_alive(c, nr, size), (uint32_t)(nr / 4));

  ASSERT_EQ(glyph_cache_shrink(c, 0), RET_OK);
  ASSERT_EQ(glyph_cache_test_alive(c, nr, size), 0u);

  ASSERT_EQ(glyph_cache_destroy(c), RET_OK);
}

TEST(GlyphCache, error) {
  glyph_t g;
  glyph_cache_t* c = glyph_cache_create(0, NULL);
  ASSERT_TRUE(c == NULL);

  memset(&g, 0x00, sizeof(g));
  ASSERT_NE(glyph_cache_add(NULL, 0, 10, &g), RET_OK);
  ASSERT_NE(glyph_cache_lookup(NULL, 0, 10, &g), RET_OK);
  ASSERT_NE(glyph_cache_shrink(NULL, 0), RET_OK);
  ASSERT_NE(glyph_cache_destroy(NULL), RET_OK);
}

TEST(GlyphCache, key_packing) {
  uint32_t i = 0;
  glyph_t g;
  glyph_cache_t* c = glyph_cache_create(16, (tk_destroy_t)glyph_destroy);

  ASSERT_TRUE(c != NULL);
  memset(&g, 0x00, sizeof(g));
  g.chr = 0x41;

  /* 同一个码点、不同字号：互不干扰 */
  ASSERT_EQ(glyph_cache_add(c, 0x41, 12, glyph_clone(&g)), RET_OK);
  g.chr = 0x42;
  ASSERT_EQ(glyph_cache_add(c, 0x41, 24, glyph_clone(&g)), RET_OK);
  g.chr = 0x43;
  ASSERT_EQ(glyph_cache_add(c, 0x41, 48, glyph_clone(&g)), RET_OK);

  ASSERT_EQ(glyph_cache_lookup(c, 0x41, 12, &g), RET_OK);
  ASSERT_EQ(g.chr, 0x41);
  ASSERT_EQ(glyph_cache_lookup(c, 0x41, 24, &g), RET_OK);
  ASSERT_EQ(g.chr, 0x42);
  ASSERT_EQ(glyph_cache_lookup(c, 0x41, 48, &g), RET_OK);
  ASSERT_EQ(g.chr, 0x43);

  /* 同一个字号、不同码点(含BMP和扩展区)：互不干扰 */
  for (i = 0; i < 3; i++) {
    g.chr = (wchar_t)(0x4e00 + i);
    ASSERT_EQ(glyph_cache_add(c, 0x4e00 + i, 24, glyph_clone(&g)), RET_OK);
  }
  /* 扩展区的码点(wchar_t在位宽16的平台上装不下，只验证键能区分) */
  ASSERT_EQ(glyph_cache_add(c, 0x1F600, 24, glyph_clone(&g)), RET_OK);

  for (i = 0; i < 3; i++) {
    ASSERT_EQ(glyph_cache_lookup(c, 0x4e00 + i, 24, &g), RET_OK);
    ASSERT_EQ((uint32_t)g.chr, 0x4e00 + i);
  }
  ASSERT_EQ(glyph_cache_lookup(c, 0x1F600, 24, &g), RET_OK);
  ASSERT_EQ(glyph_cache_lookup(c, 0x1F600, 12, &g), RET_NOT_FOUND);

  /* 不存在的组合要miss */
  ASSERT_EQ(glyph_cache_lookup(c, 0x4e00, 12, &g), RET_NOT_FOUND);
  ASSERT_EQ(glyph_cache_lookup(c, 0x41, 32, &g), RET_NOT_FOUND);

  ASSERT_EQ(glyph_cache_destroy(c), RET_OK);
}

TEST(GlyphCache, duplicate_key) {
  glyph_t g;
  glyph_cache_t* c = glyph_cache_create(16, (tk_destroy_t)glyph_cache_test_destroy);

  ASSERT_TRUE(c != NULL);
  s_destroy_count = 0;
  memset(&g, 0x00, sizeof(g));

  /* 同一个(码点,字号)写两次：只留最新的那个，旧的当场销毁 */
  g.chr = 0x41;
  ASSERT_EQ(glyph_cache_add(c, 0x41, 24, glyph_clone(&g)), RET_OK);
  ASSERT_EQ(s_destroy_count, 0u);
  g.chr = 0x42;
  ASSERT_EQ(glyph_cache_add(c, 0x41, 24, glyph_clone(&g)), RET_OK);
  ASSERT_EQ(s_destroy_count, 1u);

  /* 查到的必须是最新的那个 */
  ASSERT_EQ(glyph_cache_lookup(c, 0x41, 24, &g), RET_OK);
  ASSERT_EQ(g.chr, 0x42);

  ASSERT_EQ(glyph_cache_destroy(c), RET_OK);
  ASSERT_EQ(s_destroy_count, 2u);
}

TEST(GlyphCache, add_same_glyph) {
  glyph_t g;
  glyph_t* g1 = NULL;
  glyph_cache_t* c = glyph_cache_create(16, (tk_destroy_t)glyph_cache_test_destroy);

  ASSERT_TRUE(c != NULL);
  s_destroy_count = 0;
  memset(&g, 0x00, sizeof(g));
  g.chr = 0x41;
  g1 = glyph_clone(&g);
  ASSERT_TRUE(g1 != NULL);

  /* 把表里已有的同一个指针再写回去：不能销毁它，否则查出来是悬空指针 */
  ASSERT_EQ(glyph_cache_add(c, 0x41, 24, g1), RET_OK);
  ASSERT_EQ(glyph_cache_add(c, 0x41, 24, g1), RET_OK);
  ASSERT_EQ(s_destroy_count, 0u);

  ASSERT_EQ(glyph_cache_lookup(c, 0x41, 24, &g), RET_OK);
  ASSERT_EQ(g.chr, 0x41);

  ASSERT_EQ(glyph_cache_destroy(c), RET_OK);
  ASSERT_EQ(s_destroy_count, 1u);
}
