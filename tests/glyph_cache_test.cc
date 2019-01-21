#include "base/glyph_cache.h"
#include "gtest/gtest.h"

TEST(GlyphCache, basic) {
  uint16_t i = 0;
  uint16_t size = 10;
  uint16_t nr = 10000;
  glyph_cache_t cache;
  glyph_t g;
  glyph_cache_t* c = glyph_cache_init(&cache, 64, (tk_destroy_t)glyph_destroy);

  for (i = 0; i < nr; i++) {
    ASSERT_EQ(glyph_cache_lookup(c, i, size, &g) == RET_NOT_FOUND, true);
    ASSERT_EQ(glyph_cache_add(c, i, size, glyph_clone(&g)) == RET_OK, true);
    ASSERT_EQ(glyph_cache_lookup(c, i, size, &g) == RET_OK, true);
  }

  glyph_cache_deinit(c);
}
