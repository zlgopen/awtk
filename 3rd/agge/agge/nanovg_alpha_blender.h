#pragma once

#include "agge/math.h"
#include "agge/pixel.h"
#include "nanovg.h"

namespace agge {
template <typename PixelT, typename BitmapT>
class nanovg_alpha_blender {
 public:
  typedef PixelT pixel;
  typedef uint8_t cover_type;

 public:
  nanovg_alpha_blender(BitmapT* bitmap, float* matrix, NVGcolor color);

  bool get_alpha(float x, float y, uint8_t& alpha) const;
  void operator()(pixel* pixels, int x, int y, count_t n) const;
  void operator()(pixel* pixels, int x, int y, count_t n, const cover_type* covers) const;

 private:
  void get_physical_image_point(int& x, int& y) const;
  static inline uint8_t to_u8(float value);

 private:
  BitmapT* _bitmap;
  float* _matrix;
  pixel32_rgba _color;
  int w;
  int h;
};

template <typename PixelT, typename BitmapT>
inline nanovg_alpha_blender<PixelT, BitmapT>::nanovg_alpha_blender(BitmapT* bitmap, float* matrix,
                                                                    NVGcolor color)
    : _bitmap(bitmap), _matrix(matrix) {
  this->w = bitmap->width();
  this->h = bitmap->height();
  _color.r = to_u8(color.r);
  _color.g = to_u8(color.g);
  _color.b = to_u8(color.b);
  _color.a = to_u8(color.a);
}

template <typename PixelT, typename BitmapT>
inline uint8_t nanovg_alpha_blender<PixelT, BitmapT>::to_u8(float value) {
  int32_t v = (int32_t)(value * 255.0f + 0.5f);
  if (v < 0) {
    v = 0;
  } else if (v > 255) {
    v = 255;
  }
  return (uint8_t)v;
}

template <typename PixelT, typename BitmapT>
inline void nanovg_alpha_blender<PixelT, BitmapT>::get_physical_image_point(int& x, int& y) const {
  int tmp_x = x;
  int tmp_y = y;
  unsigned int width = _bitmap->width();
  unsigned int height = _bitmap->height();
  switch (_bitmap->orientation()) {
    case 90:
      x = tmp_y;
      y = width - tmp_x - 1;
      break;
    case 180:
      x = width - tmp_x - 1;
      y = height - tmp_y - 1;
      break;
    case 270:
      y = tmp_x;
      x = height - tmp_y - 1;
      break;
    default:
      break;
  }
}

template <typename PixelT, typename BitmapT>
inline bool nanovg_alpha_blender<PixelT, BitmapT>::get_alpha(float x, float y, uint8_t& alpha) const {
  float ox = 0;
  float oy = 0;
  float p[4];
  int x1 = 0;
  int x2 = 0;
  int y1 = 0;
  int y2 = 0;

  nvgTransformPoint(&ox, &oy, _matrix, x, y);

  p[0] = 0;
  p[1] = 0;
  p[2] = 0;
  p[3] = 0;
  x1 = (int)floorf(ox);
  x2 = (int)ceilf(ox);
  y1 = (int)floorf(oy);
  y2 = (int)ceilf(oy);
  if (x1 >= 0 && x1 < this->w) {
    if (y1 >= 0 && y1 < this->h) {
      int px = x1;
      int py = y1;
      get_physical_image_point(px, py);
      p[0] = _bitmap->row_ptr(py)[px].a;
    }
    if (y2 >= 0 && y2 < this->h) {
      int px = x1;
      int py = y2;
      get_physical_image_point(px, py);
      p[2] = _bitmap->row_ptr(py)[px].a;
    }
  }
  if (x2 >= 0 && x2 < this->w) {
    if (y1 >= 0 && y1 < this->h) {
      int px = x2;
      int py = y1;
      get_physical_image_point(px, py);
      p[1] = _bitmap->row_ptr(py)[px].a;
    }
    if (y2 >= 0 && y2 < this->h) {
      int px = x2;
      int py = y2;
      get_physical_image_point(px, py);
      p[3] = _bitmap->row_ptr(py)[px].a;
    }
  }

  p[0] = p[0] + (p[1] - p[0]) * (ox - x1);
  p[2] = p[2] + (p[3] - p[2]) * (ox - x1);
  p[0] = p[0] + (p[2] - p[0]) * (oy - y1);
  if (p[0] < 0) {
    p[0] = 0;
  } else if (p[0] > 255) {
    p[0] = 255;
  }
  alpha = (uint8_t)p[0];

  return true;
}

template <typename PixelT, typename BitmapT>
inline void nanovg_alpha_blender<PixelT, BitmapT>::operator()(pixel* pixels, int x, int y,
                                                              count_t n) const {
  pixel32_rgba src = _color;
  uint8_t alpha = 0;
  for (count_t i = 0; i < n; i++, ++pixels) {
    if (!this->get_alpha((float)x + i, (float)y, alpha)) {
      continue;
    }
    src.a = pixel_a(_color, alpha);
    pixel_blend<PixelT, pixel32_rgba>(*pixels, src, src.a);
  }
}

template <typename PixelT, typename BitmapT>
inline void nanovg_alpha_blender<PixelT, BitmapT>::operator()(pixel* pixels, int x, int y, count_t n,
                                                              const cover_type* covers) const {
  pixel32_rgba src = _color;
  uint8_t alpha = 0;
  uint8_t final_alpha = 0;
  for (count_t i = 0; i < n; i++, ++pixels, ++covers) {
    if (!this->get_alpha((float)x + i, (float)y, alpha)) {
      continue;
    }
    src.a = pixel_a(_color, alpha);
    final_alpha = pixel_a(src, covers[0]);
    pixel_blend<PixelT, pixel32_rgba>(*pixels, src, final_alpha);
  }
}
}  // namespace agge
