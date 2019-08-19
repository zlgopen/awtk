#include "tkc/mem.h"
#include "base/window.h"
#include "blend/image_g2d.h"
#include "mutable_image/mutable_image.h"
#include "gtest/gtest.h"

static ret_t dummy_prepare_image(void* ctx, bitmap_t* image) {
  color_t* c = (color_t*)ctx;

  rect_t r = rect_init(0, 0, image->w, image->h);
  image_fill(image, &r, *c);

  return RET_OK;
}

TEST(MutableImage, basic) {
  canvas_t c;
  uint32_t w = 100;
  uint32_t h = 100;
  uint32_t size = w * h * 4;
  bitmap_format_t format = BITMAP_FMT_RGBA8888;
  uint8_t* buff = (uint8_t*)TKMEM_ALLOC(size);
  color_t* p = (color_t*)buff;

  color_t color = color_init(1, 2, 3, 0xff);
  widget_t* img = mutable_image_create(NULL, 0, 0, w, h);
  mutable_image_set_prepare_image(img, dummy_prepare_image, &color);
  mutable_image_set_framebuffer(img, w, h, format, buff);

  memset(buff, 0xff, size);
  memset(&c, 0x00, sizeof(c));

  ASSERT_EQ(widget_on_paint_self(img, &c), RET_OK);

  ASSERT_EQ(p->color, color.color);

  TKMEM_FREE(buff);
  widget_destroy(img);
}

TEST(MutableImage, cast) {
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* img = mutable_image_create(w, 0, 0, 100, 100);

  ASSERT_EQ(img, mutable_image_cast(img));
  ASSERT_EQ(img, image_base_cast(img));

  widget_destroy(w);
}
