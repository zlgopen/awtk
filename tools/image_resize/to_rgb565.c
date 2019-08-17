#include <stdio.h>
#include <assert.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION

#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

static void image_shrink(unsigned char* idata, int iw, int ih, int n) {
  int x = 0;
  int y = 0;
  unsigned char* p = idata;

  for (y = 0; y < ih; y++) {
    for (x = 0; x < iw; x++) {
      p[0] = (p[0] >> 3) << 3;
      p[1] = (p[1] >> 2) << 2;
      p[2] = (p[2] >> 3) << 3;

      p += n;
    }
  }
}

static int image_to_rgb565(const char* ifilename, const char* ofilename) {
  int n = 0;
  int iw = 0;
  int ih = 0;
  int ret = 0;
  unsigned char* idata = stbi_load(ifilename, &iw, &ih, &n, 0);

  if (idata != NULL) {
    image_shrink(idata, iw, ih, n);

    if (strstr(ofilename, ".png") != NULL) {
      ret = stbi_write_png(ofilename, iw, ih, n, idata, iw * n);
    } else if (strstr(ofilename, ".jpg") != NULL) {
      ret = stbi_write_jpg(ofilename, iw, ih, n, idata, 90);
    } else {
      ret = 0;
      printf("not supported file format.\n");
    }

    stbi_image_free(idata);
  } else {
    printf("load %s failed.\n", ifilename);
  }

  return ret;
}

int main(int argc, char* argv[]) {
  const char* ifilename = argv[1];
  const char* ofilename = argv[2];

  if (argc < 3) {
    printf("Usage: %s ifilename ofilename \n", argv[0]);

    return 0;
  }

  image_to_rgb565(ifilename, ofilename);

  return 0;
}
