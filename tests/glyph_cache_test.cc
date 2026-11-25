#include "base/glyph_cache.h"
#include "gtest/gtest.h"

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

  glyph_cache_destroy(c);
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
