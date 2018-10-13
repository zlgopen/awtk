#include <string.h>
#include <stdlib.h>
#include "nanovg_agg.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb/stb_image_write.h"
   
#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"

#include "draw_image.inc"

static int load_image(NVGcontext* vg, const char* filename, int32_t* comp, int32_t* iw, int32_t* ih) {

  uint8_t* data = stbi_load(filename, iw, ih, comp, 4);

  return nvgCreateImageRaw(vg, *iw, *ih, NVG_TEXTURE_RGBA, 0, data);
}


static void run_test(int32_t w, int32_t h, int32_t BPP, const char* filename) {
  int32_t size = w * h * BPP;
  uint8_t* data = (uint8_t*)malloc(size);
  NVGcontext* vg = nvgCreateAGG(w, h, BPP == 2 ? AGG_RGB565 : AGG_RGBA8888, data);

  memset(data, 0xff, size);
  do_draw(vg, w, h);

  nvgDeleteAGG(vg);
  
  if(filename != NULL) {
    stbi_write_png(filename, w, h, BPP, data, 0);
  }

  free(data);
}

int main() {
  run_test(400, 400, 4, "result32.png");
  run_test(400, 400, 2, "result16.png");

  return 0;
}
