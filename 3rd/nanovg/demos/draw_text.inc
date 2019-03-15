#include "nanovg.h"

static void do_draw_text(NVGcontext* vg, int32_t w, int32_t h) {

  const char* text = "Hello AWTK";
  nvgBeginFrame(vg, w, h, 1);
  nvgFillColor(vg, nvgRGBA(255, 0, 0, 255));
  nvgSave(vg);
  nvgFontSize(vg, 20);
  nvgTextAlign(vg, NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
  //nvgFontBlur(vg,2);
  nvgFillColor(vg, nvgRGBA(0, 255, 0, 255));
  nvgText(vg, 20, 20, text, NULL);
  nvgRestore(vg);

  nvgEndFrame(vg);
}

