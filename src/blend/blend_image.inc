#define pixel_t pixel_dst_t
#define pixel_from_rgb pixel_dst_from_rgb

#ifndef blend_a

static inline void blend_a(uint8_t* dst, uint8_t* src, uint8_t alpha, bool_t premulti_alpha) {
  pixel_dst_t* d = (pixel_dst_t*)dst;
  pixel_src_t* s = (pixel_src_t*)src;
  rgba_t srgba = pixel_src_to_rgba((*s));
  uint8_t a = alpha == 0xff ? srgba.a : ((srgba.a * alpha) >> 8);

  if (a > 0xf8) {
    pixel_dst_t p = pixel_dst_from_rgb(srgba.r, srgba.g, srgba.b);
    *d = p;
  } else if (a > 8) {
    rgba_t drgba = pixel_dst_to_rgba((*d));

    if (premulti_alpha) {
      if(alpha <= 0xf8) {
        srgba.r = (srgba.r * alpha) >> 8;
        srgba.g = (srgba.g * alpha) >> 8;
        srgba.b = (srgba.b * alpha) >> 8;
      }
      *d = blend_rgba_premulti(drgba, srgba, 0xff - a);
    } else  {
      *d = blend_rgba(drgba, srgba, a);
    }

  }
}
#endif /*blend_a*/

static ret_t blend_image_with_alpha(bitmap_t* dst, bitmap_t* src, rect_t* dst_r, rect_t* src_r,
                                    uint8_t a) {
  wh_t i = 0;
  wh_t j = 0;
  xy_t sx = src_r->x;
  xy_t sy = src_r->y;
  wh_t sw = src_r->w;
  wh_t sh = src_r->h;
  xy_t dx = dst_r->x;
  xy_t dy = dst_r->y;
  wh_t dw = dst_r->w;
  wh_t dh = dst_r->h;
  wh_t src_iw = src->w;
  wh_t src_ih = src->h;
  wh_t dst_iw = dst->w;
  wh_t dst_ih = dst->h;
  uint8_t* srcp = NULL;
  uint8_t* dstp = NULL;
  uint8_t* src_data = NULL;
  uint8_t* dst_data = NULL;
  uint8_t src_bpp = bitmap_get_bpp(src);
  uint8_t dst_bpp = bitmap_get_bpp(dst);
  uint32_t src_line_length = bitmap_get_line_length(src);
  uint32_t dst_line_length = bitmap_get_line_length(dst);
  uint32_t src_line_offset = src_line_length - sw * src_bpp;
  uint32_t dst_line_offset = dst_line_length - dw * dst_bpp;
  bool_t premulti_alpha = src->flags & BITMAP_FLAG_PREMULTI_ALPHA;

  return_value_if_fail(sx >= 0 && sy >= 0 && (sx + sw) <= src_iw && (sy + sh) <= src_ih,
                       RET_BAD_PARAMS);
  return_value_if_fail(dx >= 0 && dy >= 0 && (dx + dw) <= dst_iw && (dy + dh) <= dst_ih,
                       RET_BAD_PARAMS);

  src_data = bitmap_lock_buffer_for_read(src);
  dst_data = bitmap_lock_buffer_for_write(dst);
  return_value_if_fail(src_data != NULL && dst_data != NULL, RET_BAD_PARAMS);

  srcp = src_data;
  dstp = dst_data;
  if (sw == dw && sh == dh) {
    srcp += (sy * src_line_length + sx * src_bpp);
    dstp += (dy * dst_line_length + dx * dst_bpp);

    for (j = 0; j < dh; j++) {
      for (i = 0; i < dw; i++) {
        blend_a(dstp, srcp, a, premulti_alpha);
        dstp += dst_bpp;
        srcp += src_bpp;
      }
      dstp += dst_line_offset;
      srcp += src_line_offset;
    }
  } else {
    uint32_t right = dw - 1;
    uint32_t bottom = dh - 1;

    uint32_t scale_x = (sw << 8) / dw;
    uint32_t scale_y = (sh << 8) / dh;

    dstp += (dy * dst_line_length + dx * dst_bpp);
    for (j = 0; j < dh; j++) {
      uint32_t y = sy + ((j == bottom) ? (sh - 1) : ((j * scale_y) >> 8));
      uint8_t* row_data = ((uint8_t*)(src_data)) + y * src_line_length + sx * src_bpp;

      for (i = 0; i < dw; i++) {
        uint32_t x = (i == right) ? (sw - 1) : ((i * scale_x) >> 8);

        srcp = row_data + x * src_bpp;
        blend_a(dstp, srcp, a, premulti_alpha);
        dstp += dst_bpp;
      }
      dstp += dst_line_offset;
    }
  }
  bitmap_unlock_buffer(src);
  bitmap_unlock_buffer(dst);

  return RET_OK;
}

static ret_t blend_image_without_alpha(bitmap_t* dst, bitmap_t* src, rect_t* dst_r, rect_t* src_r) {
  return blend_image_with_alpha(dst, src, dst_r, src_r, 0xff);
}
