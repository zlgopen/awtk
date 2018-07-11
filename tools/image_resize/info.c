#include <stdio.h>
#include <assert.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb/stb_image.h"

static int image_info(const char* filename) {
  int n = 0;
  int iw = 0;
  int ih = 0;
  unsigned char* idata = stbi_load(filename, &iw, &ih, &n, 0);
  if (idata != NULL) {
    printf("image info: %s(%dx%dx%d)\n", filename, iw, ih, n);
    stbi_image_free(idata);
  } else {
    printf("load %s failed.\n", filename);
  }

  return 0;
}

int main(int argc, char* argv[]) {
  const char* filename = argv[1];

  if (argc < 2) {
    printf("Usage: %s filename\n", argv[0]);

    return 0;
  }

  image_info(filename);

  return 0;
}
