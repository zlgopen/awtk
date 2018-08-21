#include "base/rect.h"
#include "base/bitmap.h"
#include "base/pixel_pack_unpack.h"

#define blend(d, s) *((uint32_t*)d) = rgb565_to_8888(*((uint16_t*)s))

static inline void blend_a(uint8_t* d, uint8_t* s, uint8_t alpha) {
  uint8_t r, g, b, sr, sg, sb, dr, dg, db, da, a, minus_a;
  uint16_t s_pixel = *((uint16_t*)s);
  uint32_t d_pixel = *((uint32_t*)d);

  a = alpha;

  if (a == 0xff) {
    rgb_from_565(sr, sg, sb, s_pixel);

    *((uint32_t*)d) = rgb_to_rgba8888(sr, sg, sb);
  } else if (a > 0) {
    minus_a = 0xff - a;
    rgb_from_565(sr, sg, sb, s_pixel);
    rgba_from_rgba8888(dr, dg, db, da, d_pixel);
    BLEND_PIXEL();

    *((uint32_t*)d) = rgb_to_rgba8888(r, g, b);
  }
  (void)da;
}

#include "blend_image.inc"
ret_t blend_image_8888_565(bitmap_t* dst, bitmap_t* src, rect_t* dst_r, rect_t* src_r, uint8_t a) {
  return_value_if_fail(dst != NULL && src != NULL && src_r != NULL && dst_r != NULL,
                       RET_BAD_PARAMS);
  if (a > 0xf0) {
    return blend_image_without_alpha(dst, src, dst_r, src_r);
  } else if (a > 8) {
    return blend_image_with_alpha(dst, src, dst_r, src_r, a);
  } else {
    return RET_OK;
  }
}
