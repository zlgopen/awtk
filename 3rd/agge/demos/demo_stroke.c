#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "agge/nanovg_agge.h"

static void runTest(int32_t w, int32_t h, int32_t bpp) {
  int32_t size = w * h * bpp;
  uint8_t* data = (uint8_t*)malloc(size);

  memset(data, 0xff, size);
  NVGcontext* vg = nvgCreateAGGE(w, h, bpp == 2 ? AGGE_BGR565 : AGGE_RGBA8888, data);
  nvgBeginFrame(vg, w, h, 1);
  nvgBeginPath(vg);

  nvgStrokeWidth(vg, 8);
  nvgStrokeColor(vg, nvgRGBA(255, 0, 0, 255));

  nvgScissor(vg, 50, 60, 100, 120);
  nvgMoveTo(vg, 0, 0);
  nvgLineTo(vg, 200, 200);

  nvgSave(vg);
  nvgTranslate(vg, 20, 20);
  nvgScale(vg, 0.5f, 0.5f);
  nvgRotate(vg, M_PI / 4);
  nvgMoveTo(vg, 10, 10);
  nvgLineTo(vg, 100, 10);
  nvgLineTo(vg, 100, 100);
  nvgClosePath(vg);
  nvgStroke(vg);
  nvgRestore(vg);

  nvgStrokeWidth(vg, 2);
  nvgBeginPath(vg);
  nvgRect(vg, 160, 160, 20, 20);
  nvgStroke(vg);

  nvgStrokeWidth(vg, 1);
  nvgBeginPath(vg);
  nvgRect(vg, 120, 160, 20, 20);
  nvgStroke(vg);

  nvgStrokeWidth(vg, 3);
  nvgStrokeColor(vg, nvgRGBA(0, 255, 0, 255));
  nvgBeginPath(vg);
  nvgMoveTo(vg, 0, 0);
  nvgBezierTo(vg, 10, 10, 100, 10, 100, 100);
  nvgArc(vg, 100, 100, 50, 0, 3.14, 1);

  nvgStroke(vg);

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
