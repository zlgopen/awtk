#include "nanovg.h"

static void do_fill(NVGcontext* vg, int32_t w, int32_t h) {
  nvgBeginFrame(vg, w, h, 1);
  nvgBeginPath(vg);

  nvgFillColor(vg, nvgRGBA(255, 0, 0, 255));

  nvgSave(vg);
  nvgTranslate(vg, 20, 20);
  nvgScale(vg, 0.5f, 0.5f);
  nvgRotate(vg, M_PI / 4);
  nvgMoveTo(vg, 10, 10);
  nvgLineTo(vg, 100, 10);
  nvgLineTo(vg, 100, 100);
  nvgClosePath(vg);
  nvgFill(vg);
  nvgRestore(vg);

  nvgBeginPath(vg);
  nvgRect(vg, 128, 160, 100, 20);
  nvgFill(vg);

  nvgBeginPath(vg);
  nvgRect(vg, 128, 200, 100, 20);
  nvgFill(vg);

  nvgFillColor(vg, nvgRGBA(0, 255, 0, 255));
  nvgBeginPath(vg);

  nvgMoveTo(vg, 100, 100);
  nvgArc(vg, 100, 100, 50, 0, 3.14, 1);
  nvgClosePath(vg);

  nvgFill(vg);

  nvgEndFrame(vg);
}

