#ifndef NANOVG_AGGE_H
#define NANOVG_AGGE_H

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "nanovg.h"

#ifndef M_PI
#define M_PI 3.1415926f
#endif /*M_PI*/

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _agge_bitmap_format_t {
  AGGE_RGBA8888 = 0,
  AGGE_BGRA8888,
  AGGE_RGB888,
  AGGE_RGB565
} agge_bitmap_format_t;

NVGcontext* nvgCreateAGGE(int32_t w, int32_t h, agge_bitmap_format_t format, uint8_t* data);
void nvgReinitAgge(NVGcontext* ctx, int32_t w, int32_t h, agge_bitmap_format_t format,
                   uint8_t* data);
void nvgDeleteAGGE(NVGcontext* ctx);

#ifdef __cplusplus
}
#endif

#endif /* NANOVG_AGGE_H */
