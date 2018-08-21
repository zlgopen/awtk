#include "base/rect.h"
#include "base/bitmap.h"
#include "base/pixel_pack_unpack.h"

#define blend(d, s) *((uint16_t*)d) = *((uint16_t*)s)

static inline void blend_a(uint8_t* d, uint8_t* s, uint8_t a) {
  uint8_t dr, dg, db, sr, sg, sb, r, g, b;
  uint16_t d_pixel = *((uint16_t*)d);
  uint16_t s_pixel = *((uint16_t*)s);
  uint16_t minus_a = 0xff - a;

  rgb_from_565(dr, dg, db, d_pixel);
  rgb_from_565(sr, sg, sb, s_pixel);
  BLEND_PIXEL();

  *((uint16_t*)d) = rgb_to_565(r, g, b);
}

#include "blend_image.inc"
ret_t blend_image_565_565(bitmap_t* dst, bitmap_t* src, rect_t* dst_r, rect_t* src_r, uint8_t a) {
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
