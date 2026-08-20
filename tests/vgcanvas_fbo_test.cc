#include <string.h>
#include "base/vgcanvas.h"
#include "gtest/gtest.h"

TEST(Vgcanvas, create_fbo_ex_bad_params) {
  framebuffer_object_t fbo;
  memset(&fbo, 0, sizeof(fbo));

  ASSERT_EQ(vgcanvas_create_fbo_ex(NULL, 10, 10, FALSE, TRUE, &fbo), RET_BAD_PARAMS);
  ASSERT_EQ(vgcanvas_create_fbo_ex(NULL, 10, 10, FALSE, TRUE, NULL), RET_BAD_PARAMS);
}
