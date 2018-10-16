#include "base/bitmap.h"
#include "gtest/gtest.h"

TEST(Bitmap, basic) {
  uint32_t n = 100;
  uint32_t i = 0;
  for (i = 0; i < n; i++) {
    bitmap_t* b = bitmap_create_ex(i + 1, i + 1, BITMAP_FMT_BGRA8888);
    ASSERT_EQ(((uint64_t)(b->data)) % BITMAP_ALIGN_SIZE, 0);
    bitmap_destroy(b);
  }

  for (i = 0; i < n; i++) {
    bitmap_t* b = bitmap_create_ex(i + 1, i + 1, BITMAP_FMT_BGR565);
    ASSERT_EQ(((uint64_t)(b->data)) % BITMAP_ALIGN_SIZE, 0);
    bitmap_destroy(b);
  }
}
