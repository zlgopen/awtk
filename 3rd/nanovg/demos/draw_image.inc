#include "nanovg.h"

static int load_image(NVGcontext* vg, const char* filename, int32_t* comp, int32_t* iw, int32_t* ih) {

  uint8_t* data = stbi_load(filename, iw, ih, comp, 4);

  return nvgCreateImageRaw(vg, *iw, *ih, NVG_TEXTURE_RGBA, 0, data);
}

static void draw_image(NVGcontext* vg, NVGpaint imgPaint, float sx, float sy, float sw, float sh, float dx,
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

static void do_draw(NVGcontext* vg, int32_t w, int32_t h) {
  int32_t iw = 0;
  int32_t ih = 0;
  int32_t comp = 4;
  int img = load_image(vg, "../demos/assets/raw/images/x2/rgb.png", &comp, &iw, &ih);

  nvgBeginFrame(vg, w, h, 1);

  NVGpaint imgPaint = nvgImagePattern(vg, 0, 0, iw, ih, 0, img, 1);
  draw_image(vg, imgPaint, 0, 0, iw, ih, 10, 10, iw, ih);
  
  draw_image(vg, imgPaint, 0, 0, iw, ih, 10, 100, iw*2, ih*2);
 

  int img2 = load_image(vg, "../demos/assets/raw/images/x2/rgba.png", &comp, &iw, &ih);
  NVGpaint imgPaint2 = nvgImagePattern(vg, 0, 0, iw, ih, 0, img2, 1);

  nvgTranslate(vg, 100, 100);
  nvgTranslate(vg, iw / 2, ih / 2);
  nvgRotate(vg, 3.14 / 8);
  nvgTranslate(vg, -iw / 2, -ih / 2);
  draw_image(vg, imgPaint2, 0, 0, iw, ih, 0, 0, iw, ih);

  nvgEndFrame(vg);
}
