
static ret_t blend_image_without_alpha(bitmap_t* dst, bitmap_t* src, rect_t* dst_r, rect_t* src_r) {
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
  uint8_t* srcp = (uint8_t*)(src->data);
  uint8_t* dstp = (uint8_t*)(dst->data);
  uint8_t src_bpp = src->format == BITMAP_FMT_RGB565 ? 2 : 4;
  uint8_t dst_bpp = dst->format == BITMAP_FMT_RGB565 ? 2 : 4;
  ;
  uint32_t src_line_offset = src_bpp * (src_iw - sw);
  uint32_t dst_line_offset = dst_bpp * (dst_iw - dw);

  return_value_if_fail(sx >= 0 && sy >= 0 && (sx + sw) <= src_iw && (sy + sh) <= src_ih,
                       RET_BAD_PARAMS);
  return_value_if_fail(dx >= 0 && dy >= 0 && (dx + dw) <= dst_iw && (dy + dh) <= dst_ih,
                       RET_BAD_PARAMS);
  return_value_if_fail(dst->format == BITMAP_FMT_RGBA || dst->format == BITMAP_FMT_BGRA ||
                           dst->format == BITMAP_FMT_RGB565,
                       RET_BAD_PARAMS);
  return_value_if_fail(src->format == BITMAP_FMT_RGBA || src->format == BITMAP_FMT_BGRA ||
                           src->format == BITMAP_FMT_RGB565,
                       RET_BAD_PARAMS);

  if (sw == dw && sh == dh) {
    srcp += src_bpp * (sy * src_iw + sx);
    dstp += dst_bpp * (dy * dst_iw + dx);

    for (j = 0; j < dh; j++) {
      for (i = 0; i < dw; i++) {
        blend(dstp, srcp);
        dstp += dst_bpp;
        srcp += src_bpp;
      }
      dstp += dst_line_offset;
      srcp += src_line_offset;
    }
  } else {
    uint32_t src_bpp_shift = src_bpp >> 1;
    uint32_t scale_x = (((sw + 1) << 16) / dw);
    uint32_t scale_y = (((sh + 1) << 16) / dh);

    dstp += dst_bpp * (dy * dst_iw + dx);
    for (j = 0; j < dh; j++) {
      uint8_t* srcp_start =
          (uint8_t*)(src->data) + ((src_iw * (sy + ((j * scale_y) >> 16)) + sx) << src_bpp_shift);

      for (i = 0; i < dw; i++) {
        srcp = srcp_start + (((i * scale_x) >> 16) << src_bpp_shift);
        blend(dstp, srcp);
        dstp += dst_bpp;
      }
      dstp += dst_line_offset;
    }
  }

  return RET_OK;
}

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
  uint8_t* srcp = (uint8_t*)(src->data);
  uint8_t* dstp = (uint8_t*)(dst->data);
  uint8_t src_bpp = src->format == BITMAP_FMT_RGB565 ? 2 : 4;
  uint8_t dst_bpp = dst->format == BITMAP_FMT_RGB565 ? 2 : 4;
  ;
  uint32_t src_line_offset = src_bpp * (src_iw - sw);
  uint32_t dst_line_offset = dst_bpp * (dst_iw - dw);

  return_value_if_fail(sx >= 0 && sy >= 0 && (sx + sw) <= src_iw && (sy + sh) <= src_ih,
                       RET_BAD_PARAMS);
  return_value_if_fail(dx >= 0 && dy >= 0 && (dx + dw) <= dst_iw && (dy + dh) <= dst_ih,
                       RET_BAD_PARAMS);
  return_value_if_fail(dst->format == BITMAP_FMT_RGBA || dst->format == BITMAP_FMT_BGRA ||
                           dst->format == BITMAP_FMT_RGB565,
                       RET_BAD_PARAMS);
  return_value_if_fail(src->format == BITMAP_FMT_RGBA || src->format == BITMAP_FMT_BGRA ||
                           src->format == BITMAP_FMT_RGB565,
                       RET_BAD_PARAMS);

  if (sw == dw && sh == dh) {
    srcp += src_bpp * (sy * src_iw + sx);
    dstp += dst_bpp * (dy * dst_iw + dx);

    for (j = 0; j < dh; j++) {
      for (i = 0; i < dw; i++) {
        blend_a(dstp, srcp, a);
        dstp += dst_bpp;
        srcp += src_bpp;
      }
      dstp += dst_line_offset;
      srcp += src_line_offset;
    }
  } else {
    uint32_t src_bpp_shift = src_bpp >> 1;
    uint32_t scale_x = (((sw + 1) << 16) / dw);
    uint32_t scale_y = (((sh + 1) << 16) / dh);

    dstp += dst_bpp * (dy * dst_iw + dx);
    for (j = 0; j < dh; j++) {
      uint8_t* srcp_start =
          (uint8_t*)(src->data) + ((src_iw * (sy + ((j * scale_y) >> 16)) + sx) << src_bpp_shift);

      for (i = 0; i < dw; i++) {
        srcp = srcp_start + (((i * scale_x) >> 16) << src_bpp_shift);
        blend_a(dstp, srcp, a);
        dstp += dst_bpp;
      }
      dstp += dst_line_offset;
    }
  }

  return RET_OK;
}
