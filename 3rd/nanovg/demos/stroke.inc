#include "nanovg.h"

static void do_stroke(NVGcontext* vg, int32_t w, int32_t h) {
  nvgBeginFrame(vg, w, h, 1);
  nvgBeginPath(vg);

  nvgStrokeWidth(vg, 2);
  nvgStrokeColor(vg, nvgRGBA(255, 0, 0, 255));

  nvgMoveTo(vg, 0, 0);
  nvgLineTo(vg, w/2, h/2);
  nvgLineTo(vg, w, 0);
  nvgStroke(vg);

  nvgStrokeWidth(vg, 4);
  nvgSave(vg);
  nvgBeginPath(vg);
  nvgTranslate(vg, 200, 20);
  nvgScale(vg, 2, 2);
  nvgRotate(vg, M_PI / 4);
  nvgMoveTo(vg, 10, 10);
  nvgLineTo(vg, 100, 10);
  nvgLineTo(vg, 100, 100);
  nvgClosePath(vg);
  nvgStroke(vg);
  nvgRestore(vg);

  nvgStrokeWidth(vg, 2);
  nvgBeginPath(vg);
  nvgRect(vg, 160, 160, 200, 20);
  nvgStroke(vg);

  nvgStrokeWidth(vg, 3);
  nvgTranslate(vg, 10, 200);
  nvgStrokeColor(vg, nvgRGBA(0, 255, 0, 255));
  nvgBeginPath(vg);
  nvgMoveTo(vg, 0, 0);
  nvgBezierTo(vg, 10, 10, 100, 10, 100, 100);
  nvgArc(vg, 100, 100, 50, 0, 3.14, 1);

  nvgStroke(vg);

  nvgEndFrame(vg);
}

