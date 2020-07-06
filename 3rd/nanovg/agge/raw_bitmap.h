#pragma once

#include <string.h>
#include <stdint.h>
#include <agge/config.h>
#include <agge/pixel.h>

typedef uint8_t* image_handle;

namespace agge {
class raw_bitmap : noncopyable {
 public:  // General
  raw_bitmap(count_t width, count_t height, count_t stride, count_t flags, bits_per_pixel bpp, uint8_t* data);
  ~raw_bitmap() {
  }

  count_t flags() const;
  count_t width() const;
  count_t height() const;

  void* row_ptr(count_t y);
  const void* row_ptr(count_t y) const;

 public:
  image_handle native() const;
  void blit(int x, int y, count_t width, count_t height) const;

 private:
  uint8_t* _memory;
  count_t _width, _height;
  count_t _stride;
  count_t _flags;
  const bits_per_pixel _bpp;
  image_handle _native;
};

inline raw_bitmap::raw_bitmap(count_t width, count_t height, count_t stride, count_t flags, bits_per_pixel bpp, uint8_t* data)
    : _memory(data), _width(width), _height(height), _stride(stride), _flags(flags), _bpp(bpp), _native(0) {
  
}

inline count_t raw_bitmap::flags() const {
  return _flags;
}

inline count_t raw_bitmap::width() const {
  return _width;
}

inline count_t raw_bitmap::height() const {
  return _height;
}

inline void* raw_bitmap::row_ptr(count_t y) {
  return _memory + y * _stride;
}

inline const void* raw_bitmap::row_ptr(count_t y) const {
  return _memory + y * _stride;
}

inline image_handle raw_bitmap::native() const {
  return _native;
}

}  // namespace agge
