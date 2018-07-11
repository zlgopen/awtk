#include <stdio.h>
#include <assert.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION

#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "stb/stb_image_resize.h"

static int image_resize(const char* ifilename, const char* ofilename, int ow, int oh) {
  int n = 0;
  int iw = 0;
  int ih = 0;
  int ret = 0;
  unsigned char* odata = NULL;
  unsigned char* idata = stbi_load(ifilename, &iw, &ih, &n, 0);
  if (idata != NULL) {
    if (ow == 0) {
      ow = (oh * iw) / ih;
    }
    if (oh == 0) {
      oh = (ow * ih) / iw;
    }

    odata = (unsigned char*)malloc(ow * oh * n);
    if (odata != NULL) {
      ret = stbir_resize_uint8(idata, iw, ih, iw * n, odata, ow, oh, ow * n, n);
      if (ret != 0) {
        if (strstr(ofilename, ".png") != NULL) {
          ret = stbi_write_png(ofilename, ow, oh, n, odata, ow * n);
        } else if (strstr(ofilename, ".jpg") != NULL) {
          ret = stbi_write_jpg(ofilename, ow, oh, n, odata, 90);
        } else {
          ret = 0;
          printf("not supported file format.\n");
        }

        if (ret != 0) {
          int w = 0;
          int h = 0;
          unsigned char* data = stbi_load(ofilename, &w, &h, &n, 0);
          assert(w == ow && h == oh && data != NULL);
          if (w == ow && h == oh) {
            printf("image resize success:%s(%dx%d) => %s(%dx%dx%d)\n", ifilename, iw, ih, ofilename,
                   ow, oh, n);
          }
          stbi_image_free(data);
        } else {
          printf("write failed\n");
        }
      } else {
        printf("resize failed\n");
      }

      free(odata);
    }
    stbi_image_free(idata);
  } else {
    printf("load %s failed.\n", ifilename);
  }

  return 0;
}

int main(int argc, char* argv[]) {
  int ow = 0;
  int oh = 0;
  const char* ifilename = argv[1];
  const char* ofilename = argv[2];

  if (argc < 5) {
    printf("Usage: %s ifilename ofilename width height\n", argv[0]);

    return 0;
  }

  ow = atoi(argv[3]);
  oh = atoi(argv[4]);

  image_resize(ifilename, ofilename, ow, oh);

  return 0;
}
