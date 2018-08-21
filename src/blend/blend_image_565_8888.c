#include "base/rect.h"
#include "base/bitmap.h"
#include "base/pixel_pack_unpack.h"

static inline void blend_a(uint8_t* d, uint8_t* s, uint8_t alpha) {
  uint8_t r, g, b, sr, sg, sb, sa, dr, dg, db, a, minus_a;
  uint16_t d_pixel = *((uint16_t*)d);
  uint32_t s_pixel = *((uint32_t*)s);

  sa = a_from_image8888(s_pixel);
  a = alpha != 0xff ? ((sa * alpha) >> 8) : sa;

  if (a > 0xf0) {
    rgba_from_image8888(sr, sg, sb, sa, s_pixel);

    *((uint16_t*)d) = rgb_to_565(sr, sg, sb);
  } else if (a > 8) {
    minus_a = 0xff - a;
    rgba_from_image8888(sr, sg, sb, sa, s_pixel);
    rgb_from_565(dr, dg, db, d_pixel);
    BLEND_PIXEL();

    *((uint16_t*)d) = rgb_to_565(r, g, b);
  }
}

#define blend(d, s) blend_a(d, s, 0xff)

#include "blend_image.inc"
ret_t blend_image_565_8888(bitmap_t* dst, bitmap_t* src, rect_t* dst_r, rect_t* src_r, uint8_t a) {
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
