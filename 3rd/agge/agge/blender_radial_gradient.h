#pragma once

#include "pixel.h"

namespace agge {
template <typename PixelT>
class blender_radial_gradient {
 public:
  typedef PixelT pixel;
  typedef uint8_t cover_type;

 public:
  blender_radial_gradient(float cx, float cy, float inr, float outr, pixel32_rgba sc, pixel32_rgba ec);
  void operator()(pixel* pixels, int x, int y, count_t n, const cover_type* covers) const;
  bool gradient(float factor, pixel32_rgba& c) const;
  bool get_color(int x, int y, pixel32_rgba& c) const;

 private:
  float _cx;
  float _cy;
  float _inr;
  float _outr;
  pixel32_rgba _sc;
  pixel32_rgba _ec;
};

template <typename PixelT>
inline blender_radial_gradient<PixelT>::blender_radial_gradient(float cx, float cy, float inr, float outr, 
    pixel32_rgba sc, pixel32_rgba ec) : _cx(cx), _cy(cy), _inr(inr), _outr(outr), _sc(sc), _ec(ec) {
}

template <typename PixelT>
inline bool blender_radial_gradient<PixelT>::gradient(float factor, pixel32_rgba& c) const {
  if(factor <= 0) {
    c = _sc;
  } else if(factor >= 1.0f) {
    c = _ec;
  } else {
    c.r = _sc.r + (_ec.r - _sc.r) * factor;
    c.g = _sc.g + (_ec.g - _sc.g) * factor;
    c.b = _sc.b + (_ec.b - _sc.b) * factor;
    c.a = _sc.a + (_ec.a - _sc.a) * factor;
  }

  return true;
}

template <typename PixelT>
inline bool blender_radial_gradient<PixelT>::get_color(int x, int y, pixel32_rgba& c) const {
  float dx = x - _cx;
  float dy = y - _cy;
  float r = sqrt(dx * dx + dy * dy);

  if(r <= _inr) {
    c = _sc;
  } else if(r >= _outr) {
    c = _ec;
  } else {
    float factor = (r - _inr)/(_outr - _inr);
    this->gradient(factor, c);
  }

  return true;
}

template <typename PixelT>
inline void blender_radial_gradient<PixelT>::operator()(pixel* pixels, int x, int y,
                                                        count_t n, const cover_type* covers) const {
  pixel32_rgba p(0, 0, 0, 0);

  for (; n; --n, ++pixels, ++covers) {
    if(this->get_color(x++, y, p)) {
      uint8_t a = (*covers * p.a) >> 8;
      pixel_blend<PixelT, pixel32_rgba>(*pixels, p, a);
    }
  }
}
}  // namespace agge
