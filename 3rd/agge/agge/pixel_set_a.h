#pragma once

template <typename PixelT>
inline void pixel_set_a(PixelT& p, uint8_t a) {
}

template <>
inline void pixel_set_a(pixel32_rgba& p, uint8_t a) {
  p.a = a;
}

template <>
inline void pixel_set_a(pixel32_bgra& p, uint8_t a) {
  p.a = a;
}

template <>
inline void pixel_set_a(pixel32_abgr& p, uint8_t a) {
  p.a = a;
}

template <>
inline void pixel_set_a(pixel32_argb& p, uint8_t a) {
  p.a = a;
}

template <>
inline void pixel_set_a(pixel8& p, uint8_t a) {
  p.a = a;
}
