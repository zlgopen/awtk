#pragma once

#include "pixel.h"

namespace agge {
template <typename PixelT>
class blender_solid_color_rgb {
 public:
  typedef PixelT pixel;
  typedef uint8_t cover_type;

 public:
  blender_solid_color_rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);

  void operator()(pixel* pixels, int x, int y, count_t n) const;
  void operator()(pixel* pixels, int x, int y, count_t n, const cover_type* covers) const;

 private:
  uint8_t _r, _g, _b, _a;
};

template <typename PixelT>
inline blender_solid_color_rgb<PixelT>::blender_solid_color_rgb(uint8_t r, uint8_t g, uint8_t b,
                                                                uint8_t a)
    : _r(r), _g(g), _b(b), _a(a) {
}

template <typename PixelT>
inline void blender_solid_color_rgb<PixelT>::operator()(pixel* pixels, int /*x*/, int /*y*/,
                                                        count_t n) const {
  pixel32_rgba p(_r, _g, _b, _a);
  for (; n; --n, ++pixels) {
    pixel_blend<PixelT, pixel32_rgba>(*pixels, p, p.a);
  }
}

template <typename PixelT>
inline void blender_solid_color_rgb<PixelT>::operator()(pixel* pixels, int /*x*/, int /*y*/,
                                                        count_t n, const cover_type* covers) const {
  pixel32_rgba p(_r, _g, _b, _a);
  for (; n; --n, ++pixels, ++covers) {
    uint8_t a = (_a * covers[0]) >> 8;

    pixel_blend<PixelT, pixel32_rgba>(*pixels, p, a);
  }
}
}  // namespace agge
