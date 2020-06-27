#include "base/glyph_cache.h"
#include "gtest/gtest.h"

TEST(GlyphCache, basic) {
  uint16_t i = 0;
  uint16_t size = 10;
  uint16_t nr = 10000;
  glyph_cache_t cache;
  glyph_t g;
  glyph_cache_t* c = glyph_cache_init(&cache, 64, (tk_destroy_t)glyph_destroy);

  memset(&g, 0x00, sizeof(g));
  for (i = 0; i < nr; i++) {
    ASSERT_EQ(glyph_cache_lookup(c, i, size, &g) == RET_NOT_FOUND, true);
    ASSERT_EQ(glyph_cache_add(c, i, size, glyph_clone(&g)) == RET_OK, true);
    ASSERT_EQ(glyph_cache_lookup(c, i, size, &g) == RET_OK, true);
  }

  glyph_cache_deinit(c);
}

TEST(GlyphCache, shrink) {
  uint16_t i = 0;
  uint16_t size = 10;
  uint16_t nr = 64;
  glyph_cache_t cache;
  glyph_t g;
  glyph_cache_t* c = glyph_cache_init(&cache, nr, (tk_destroy_t)glyph_destroy);

  memset(&g, 0x00, sizeof(g));
  for (i = 0; i < nr; i++) {
    ASSERT_EQ(glyph_cache_add(c, i, size, glyph_clone(&g)) == RET_OK, true);
  }

  for (i = 0; i < nr; i++) {
    glyph_cache_item_t* item = c->items + i;
    item->last_access_time = i;
  }

  ASSERT_EQ(glyph_cache_shrink(c, nr / 4), RET_OK);
  ASSERT_EQ(c->size, nr / 4);
  ASSERT_EQ(c->items[0].last_access_time, (nr - 1));
  ASSERT_EQ(c->items[1].last_access_time, (nr - 2));
  ASSERT_EQ(c->items[2].last_access_time, (nr - 3));

  ASSERT_EQ(glyph_cache_shrink(c, nr / 8), RET_OK);
  ASSERT_EQ(c->size, nr / 8);
  ASSERT_EQ(c->items[0].last_access_time, (nr - 1));

  ASSERT_EQ(glyph_cache_shrink(c, nr / 4), RET_OK);
  ASSERT_EQ(c->size, nr / 8);

  ASSERT_EQ(glyph_cache_shrink(c, 0), RET_OK);
  ASSERT_EQ(c->size, 0);

  glyph_cache_deinit(c);
}
