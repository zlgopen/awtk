#include <string.h>
#include <stdlib.h>
#include "agge/nanovg_agge.h"

void draw_image(NVGcontext* vg, NVGpaint imgPaint, float sx, float sy, float sw, float sh, float dx,
                float dy, float dw, float dh) {
  float scaleX = (float)dw / sw;
  float scaleY = (float)dh / sh;

  nvgSave(vg);
  nvgBeginPath(vg);
  nvgTranslate(vg, dx - (sx * scaleX), dy - (sy * scaleY));
  nvgScale(vg, scaleX, scaleY);
  nvgRect(vg, sx, sy, sw, sh);
  nvgFillPaint(vg, imgPaint);
  nvgClosePath(vg);
  nvgFill(vg);
  nvgBeginPath(vg);
  nvgRestore(vg);
}

static void runTest(int32_t w, int32_t h, int32_t bpp) {
  int32_t iw = 0;
  int32_t ih = 0;
  int32_t size = w * h * bpp;
  uint8_t* data = (uint8_t*)malloc(size);
  memset(data, 0xff, size);
  NVGcontext* vg = nvgCreateAGGE(w, h, bpp == 2 ? AGGE_RGB565 : AGGE_RGBA8888, data);
  int img = nvgCreateImage(vg, "demos/rgb.png", 0);

  nvgBeginFrame(vg, w, h, 1);

  nvgImageSize(vg, img, &iw, &ih);
  NVGpaint imgPaint = nvgImagePattern(vg, 0, 0, iw, ih, 0, img, 1);
  draw_image(vg, imgPaint, 0, 0, iw, ih, 10, 20, iw, ih);

  int img2 = nvgCreateImage(vg, "demos/rgba.png", 0);
  nvgImageSize(vg, img2, &iw, &ih);
  NVGpaint imgPaint2 = nvgImagePattern(vg, 0, 0, iw, ih, 0, img2, 1);

  nvgTranslate(vg, 100, 100);
  nvgTranslate(vg, iw / 2, ih / 2);
  nvgRotate(vg, 3.14 / 8);
  nvgTranslate(vg, -iw / 2, -ih / 2);
  draw_image(vg, imgPaint2, 0, 0, iw, ih, 0, 0, iw, ih);

  nvgEndFrame(vg);
  nvgDeleteAGGE(vg);

  free(data);
}

int main() {
  int i = 0;
  for (i = 0; i < 100; i++) {
    runTest(400, 400, 2);
    runTest(400, 400, 4);
  }

  return 0;
}
