#include "blend/soft_g2d.h"
#include "gtest/gtest.h"

#define FB_W 8
#define FB_H 6
#define IMG_W FB_H
#define IMG_H FB_W

static uint16_t s_fb_buff[FB_W * FB_H * 2];
static uint16_t s_img_buff[IMG_W * IMG_H * 2];

static void init_image(bitmap_t* fb, bitmap_t* img, bitmap_format_t format) {
  uint8_t* fb_buff = (uint8_t*)s_fb_buff;
  uint8_t* img_buff = (uint8_t*)s_img_buff;

  bitmap_init(fb, FB_W, FB_H, format, fb_buff);
  bitmap_init(img, IMG_W, IMG_H, format, img_buff);
  fb->should_free_data = TRUE;
  img->should_free_data = TRUE;

  memset(s_fb_buff, 0x00, sizeof(s_fb_buff));
  memset(s_img_buff, 0xfe, sizeof(s_img_buff));
}

static void init_image16(bitmap_t* fb, bitmap_t* img) {
  init_image(fb, img, BITMAP_FMT_BGR565);
}

static void init_image32(bitmap_t* fb, bitmap_t* img) {
  init_image(fb, img, BITMAP_FMT_BGRA8888);
}

TEST(Soft2d, rotate16) {
  rect_t r;
  bitmap_t fb;
  bitmap_t img;
  uint16_t* fb_buff = (uint16_t*)s_fb_buff;

  memset(&fb, 0x00, sizeof(fb));
  memset(&img, 0x00, sizeof(img));

  init_image16(&fb, &img);
  r = rect_init(0, 0, 1, 3);
  soft_rotate_image(&fb, &img, &r, LCD_ORIENTATION_90);
  ASSERT_EQ(fb_buff[(FB_H - 1) * FB_W], (uint16_t)0xfefe);
  ASSERT_EQ(fb_buff[(FB_H - 1) * FB_W + 1], (uint16_t)0xfefe);
  ASSERT_EQ(fb_buff[(FB_H - 1) * FB_W + 2], (uint16_t)0xfefe);
  bitmap_destroy(&fb);
  bitmap_destroy(&img);

  init_image16(&fb, &img);
  r = rect_init(1, 1, 1, 3);
  soft_rotate_image(&fb, &img, &r, LCD_ORIENTATION_90);
  ASSERT_EQ(fb_buff[(FB_H - 2) * FB_W + 1], (uint16_t)0xfefe);
  ASSERT_EQ(fb_buff[(FB_H - 2) * FB_W + 2], (uint16_t)0xfefe);
  ASSERT_EQ(fb_buff[(FB_H - 2) * FB_W + 3], (uint16_t)0xfefe);
  bitmap_destroy(&fb);
  bitmap_destroy(&img);

  init_image16(&fb, &img);
  r = rect_init(1, 1, 3, 1);
  soft_rotate_image(&fb, &img, &r, LCD_ORIENTATION_90);
  ASSERT_EQ(fb_buff[(FB_H - 2) * FB_W + 1], (uint16_t)0xfefe);
  ASSERT_EQ(fb_buff[(FB_H - 3) * FB_W + 1], (uint16_t)0xfefe);
  ASSERT_EQ(fb_buff[(FB_H - 4) * FB_W + 1], (uint16_t)0xfefe);
  bitmap_destroy(&fb);
  bitmap_destroy(&img);

  init_image16(&fb, &img);
  r = rect_init(1, 1, 2, 2);
  soft_rotate_image(&fb, &img, &r, LCD_ORIENTATION_90);
  ASSERT_EQ(fb_buff[(FB_H - 2) * FB_W + 1], (uint16_t)0xfefe);
  ASSERT_EQ(fb_buff[(FB_H - 3) * FB_W + 1], (uint16_t)0xfefe);
  ASSERT_EQ(fb_buff[(FB_H - 2) * FB_W + 2], (uint16_t)0xfefe);
  ASSERT_EQ(fb_buff[(FB_H - 3) * FB_W + 2], (uint16_t)0xfefe);
  bitmap_destroy(&fb);
  bitmap_destroy(&img);
}

TEST(Soft2d, rotate32) {
  rect_t r;
  bitmap_t fb;
  bitmap_t img;
  uint32_t* fb_buff = (uint32_t*)s_fb_buff;

  memset(&fb, 0x00, sizeof(fb));
  memset(&img, 0x00, sizeof(img));
  init_image32(&fb, &img);
  r = rect_init(0, 0, 1, 3);
  soft_rotate_image(&fb, &img, &r, LCD_ORIENTATION_90);
  ASSERT_EQ(fb_buff[(FB_H - 1) * FB_W], (uint32_t)0xfefefefe);
  ASSERT_EQ(fb_buff[(FB_H - 1) * FB_W + 1], (uint32_t)0xfefefefe);
  ASSERT_EQ(fb_buff[(FB_H - 1) * FB_W + 2], (uint32_t)0xfefefefe);
  bitmap_destroy(&fb);
  bitmap_destroy(&img);

  init_image32(&fb, &img);
  r = rect_init(1, 1, 1, 3);
  soft_rotate_image(&fb, &img, &r, LCD_ORIENTATION_90);
  ASSERT_EQ(fb_buff[(FB_H - 2) * FB_W + 1], (uint32_t)0xfefefefe);
  ASSERT_EQ(fb_buff[(FB_H - 2) * FB_W + 2], (uint32_t)0xfefefefe);
  ASSERT_EQ(fb_buff[(FB_H - 2) * FB_W + 3], (uint32_t)0xfefefefe);
  bitmap_destroy(&fb);
  bitmap_destroy(&img);

  init_image32(&fb, &img);
  r = rect_init(1, 1, 3, 1);
  soft_rotate_image(&fb, &img, &r, LCD_ORIENTATION_90);
  ASSERT_EQ(fb_buff[(FB_H - 2) * FB_W + 1], (uint32_t)0xfefefefe);
  ASSERT_EQ(fb_buff[(FB_H - 3) * FB_W + 1], (uint32_t)0xfefefefe);
  ASSERT_EQ(fb_buff[(FB_H - 4) * FB_W + 1], (uint32_t)0xfefefefe);
  bitmap_destroy(&fb);
  bitmap_destroy(&img);

  init_image32(&fb, &img);
  r = rect_init(1, 1, 2, 2);
  soft_rotate_image(&fb, &img, &r, LCD_ORIENTATION_90);
  ASSERT_EQ(fb_buff[(FB_H - 2) * FB_W + 1], (uint32_t)0xfefefefe);
  ASSERT_EQ(fb_buff[(FB_H - 3) * FB_W + 1], (uint32_t)0xfefefefe);
  ASSERT_EQ(fb_buff[(FB_H - 2) * FB_W + 2], (uint32_t)0xfefefefe);
  ASSERT_EQ(fb_buff[(FB_H - 3) * FB_W + 2], (uint32_t)0xfefefefe);
  bitmap_destroy(&fb);
  bitmap_destroy(&img);
}
