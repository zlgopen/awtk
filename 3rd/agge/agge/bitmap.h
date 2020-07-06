#pragma once

#include "pixel.h"
#include "tools.h"

namespace agge {
template <typename PixelT>
struct pixel_bpp {};

template <typename PixelT, typename RawBitmapT>
class bitmap : public RawBitmapT {
 public:
  typedef PixelT pixel;

 public:
  bitmap(count_t width, count_t height, count_t stride, uint8_t* data);
  bitmap(count_t width, count_t height, count_t stride, count_t flags, uint8_t* data);

  pixel* row_ptr(count_t y);
  const pixel* row_ptr(count_t y) const;
};

template <>
struct pixel_bpp<pixel32_rgba> {
  static const bits_per_pixel bpp = bpp32;
};

template <>
struct pixel_bpp<pixel32_abgr> {
  static const bits_per_pixel bpp = bpp32;
};

template <>
struct pixel_bpp<pixel32_bgra> {
  static const bits_per_pixel bpp = bpp32;
};

template <>
struct pixel_bpp<pixel32_argb> {
  static const bits_per_pixel bpp = bpp32;
};

template <>
struct pixel_bpp<pixel24_rgb> {
  static const bits_per_pixel bpp = bpp24;
};

template <>
struct pixel_bpp<pixel24_bgr> {
  static const bits_per_pixel bpp = bpp24;
};

template <>
struct pixel_bpp<pixel16_rgb565> {
  static const bits_per_pixel bpp = bpp16;
};

template <>
struct pixel_bpp<pixel16_bgr565> {
  static const bits_per_pixel bpp = bpp16;
};

template <>
struct pixel_bpp<uint8_t> {
  static const bits_per_pixel bpp = bpp8;
};

template <>
struct pixel_bpp<pixel8> {
  static const bits_per_pixel bpp = bpp8;
};

template <typename PixelT, typename RawBitmapT>
inline bitmap<PixelT, RawBitmapT>::bitmap(count_t width, count_t height, count_t stride, uint8_t* data)
    : RawBitmapT(width, height, stride, 0, pixel_bpp<PixelT>::bpp, data) {
}

template <typename PixelT, typename RawBitmapT>
inline bitmap<PixelT, RawBitmapT>::bitmap(count_t width, count_t height, count_t stride, count_t flags, uint8_t* data)
    : RawBitmapT(width, height, stride, flags, pixel_bpp<PixelT>::bpp, data) {
}

template <typename PixelT, typename RawBitmapT>
inline typename bitmap<PixelT, RawBitmapT>::pixel* bitmap<PixelT, RawBitmapT>::row_ptr(count_t y) {
  return static_cast<pixel*>(RawBitmapT::row_ptr(y));
}

template <typename PixelT, typename RawBitmapT>
inline const typename bitmap<PixelT, RawBitmapT>::pixel* bitmap<PixelT, RawBitmapT>::row_ptr(
    count_t y) const {
  return static_cast<const pixel*>(RawBitmapT::row_ptr(y));
}

template <typename SrcBitmapT, typename DestBitmapT>
inline void copy(const SrcBitmapT& src, int src_x, int src_y, DestBitmapT& dest, int dest_x,
                 int dest_y, count_t width, count_t height) {
  if (src_x < 0) width += src_x, dest_x -= src_x, src_x = 0;
  if (src_y < 0) height += src_y, dest_y -= src_y, src_y = 0;

  if (dest_x < 0) width += dest_x, src_x -= dest_x, dest_x = 0;
  if (dest_y < 0) height += dest_y, src_y -= dest_y, dest_y = 0;

  width = agge_min(width, agge_min(src.width() - src_x, dest.width() - dest_x));
  height = agge_min(height, agge_min(src.height() - src_y, dest.height() - dest_y));

  for (count_t y = 0; y < height; ++y) {
    const typename SrcBitmapT::pixel* src_pixel = src.row_ptr(y + src_y) + src_x;
    typename DestBitmapT::pixel* dest_pixel = dest.row_ptr(y + dest_y) + dest_x;

    for (count_t i = 0; i < width; ++i, ++src_pixel, ++dest_pixel) *dest_pixel = *src_pixel;
  }
}
}  // namespace agge
