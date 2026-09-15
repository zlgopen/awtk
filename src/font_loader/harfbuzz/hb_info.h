#ifndef HB_INFO_H
#define HB_INFO_H

#include "base/types_def.h"

typedef struct _hb_info_t {
  uint32_t glyphid;
  uint32_t cluster;
  int32_t x_offset;
  int32_t y_offset;
  int32_t x_advance;
  int32_t y_advance;
  int32_t bidi_type;
} hb_info_t;

#endif