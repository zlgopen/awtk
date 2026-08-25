#include "base/window.h"
#include "gif_image/gif_image.h"
#include "gtest/gtest.h"
#include "tkc/fs.h"
#include "tkc/mem.h"
#include "image_loader/image_loader_stb.h"

TEST(GifImage, basic) {
  value_t v;
  value_t v1;
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* img = gif_image_create(w, 0, 0, 100, 100);

  value_set_str(&v, "earth");
  ASSERT_EQ(widget_set_prop(img, WIDGET_PROP_IMAGE, &v), RET_OK);
  ASSERT_EQ(widget_get_prop(img, WIDGET_PROP_IMAGE, &v1), RET_OK);
  ASSERT_EQ(strcmp(value_str(&v), value_str(&v1)), 0);
  ASSERT_EQ(widget_count_children(w), 1);

  ASSERT_EQ((uint32_t)widget_get_prop_int(img, WIDGET_PROP_LOOP, 0), 0xffffffff);
  ASSERT_EQ(widget_set_prop_int(img, WIDGET_PROP_LOOP, 1), RET_OK);
  ASSERT_EQ(widget_get_prop_int(img, WIDGET_PROP_LOOP, 0), 1);

  widget_destroy(w);
}

TEST(GifImage, cast) {
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* img = gif_image_create(w, 0, 0, 100, 100);

  ASSERT_EQ(img, gif_image_cast(img));
  ASSERT_EQ(img, image_base_cast(img));

  widget_destroy(w);
}

TEST(GifImage, state) {
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* img = gif_image_create(w, 0, 0, 100, 100);

  ASSERT_EQ(GIF_IMAGE(img)->running, TRUE);

  gif_image_stop(img);
  ASSERT_EQ(GIF_IMAGE(img)->running, FALSE);

  gif_image_play(img);
  ASSERT_EQ(GIF_IMAGE(img)->running, TRUE);

  gif_image_pause(img);
  ASSERT_EQ(GIF_IMAGE(img)->running, FALSE);

  widget_destroy(w);
}

TEST(GifImage, stb_next_frame_from_bee_gif) {
  uint32_t size = 0;
  bitmap_t bitmap;
  int delays = 0;
  int layers = 0;
  bool_t on_end = FALSE;
  void* gif_context = NULL;
  void* gif_msg = NULL;
  void* gif_cache = NULL;
  uint8_t* data = (uint8_t*)file_read(TK_ROOT "/design/default/images/x1/bee.gif", &size);

  ASSERT_TRUE(data != NULL);
  ASSERT_GT(size, 0u);
  memset(&bitmap, 0x00, sizeof(bitmap));
  ASSERT_EQ(stb_gif_frame_create(&gif_context, &gif_msg), RET_OK);

  ASSERT_EQ(stb_load_gif_next_frame(&bitmap, &gif_context, &gif_msg, &gif_cache, &delays, data,
                                    size, &layers, &on_end),
            RET_OK);
  ASSERT_EQ(bitmap.is_gif, TRUE);
  ASSERT_GT(bitmap.w, 0);
  ASSERT_GT(bitmap.gif_frame_h, 0u);
  ASSERT_EQ(layers, 1);
  ASSERT_EQ(on_end, FALSE);

  /* Keep bitmap.is_gif across frames: stb_load_gif_next_frame gates on it (gif_image does too). */
  ASSERT_EQ(stb_load_gif_next_frame(&bitmap, &gif_context, &gif_msg, &gif_cache, &delays, data,
                                    size, &layers, &on_end),
            RET_OK);
  ASSERT_GE(layers, 2);
  bitmap_deinit(&bitmap);

  stb_gif_frame_free(&gif_context, &gif_msg, &gif_cache);
  TKMEM_FREE(data);
}
