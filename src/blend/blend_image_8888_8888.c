#include "base/rect.h"
#include "base/bitmap.h"
#include "base/pixel_pack_unpack.h"

static inline void blend_a(uint8_t* d, uint8_t* s, uint8_t alpha) {
  uint8_t r, g, b, sr, sg, sb, sa, dr, dg, db, da, a, minus_a;
  uint32_t s_pixel = *((uint32_t*)s);
  uint32_t d_pixel = *((uint32_t*)d);

  sa = a_from_image8888(s_pixel);
  a = alpha == 0xff ? sa : ((sa * alpha) >> 8);

  if (a == 0xff) {
    rgba_from_image8888(sr, sg, sb, sa, s_pixel);

    *((uint32_t*)d) = rgb_to_rgba8888(sr, sg, sb);
  } else if (a > 0) {
    minus_a = 0xff - a;
    rgba_from_image8888(sr, sg, sb, sa, s_pixel);
    rgba_from_rgba8888(dr, dg, db, da, d_pixel);
    BLEND_PIXEL();

    *((uint32_t*)d) = rgb_to_rgba8888(r, g, b);
  }
  (void)da;
}

#define blend(d, s) blend_a(d, s, 0xff)

#include "blend_image.inc"
ret_t blend_image_8888_8888(bitmap_t* dst, bitmap_t* src, rect_t* dst_r, rect_t* src_r, uint8_t a) {
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
