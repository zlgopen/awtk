#include <string>
#include "gtest/gtest.h"
#include "tkc/rect.h"
#include "tkc/utils.h"

using std::string;

#define pixel_t uint16_t

static string s_log;
static void set_window_func(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
  char buff[128];
  tk_snprintf(buff, sizeof(buff), "win(%u %u %u %u):", x, y, w, h);
  s_log += buff;
}

static void write_data_func(pixel_t pixel) {
  char buff[128];
  tk_snprintf(buff, sizeof(buff), "%02x ", pixel);
  s_log += buff;
}

typedef uint16_t pixel_t;
#define LCD_FORMAT BITMAP_FMT_BGR565
#define pixel_from_rgb(r, g, b) ((((r) >> 3) << 11) | (((g) >> 2) << 5) | ((b) >> 3))
#define pixel_to_rgba(p) \
  { (0xff & ((p >> 11) << 3)), (0xff & ((p >> 5) << 2)), (0xff & (p << 3)) }

#include "base/pixel.h"
#include "base/bitmap.h"
#include "lcd/fragment_frame_buffer.h"

TEST(FragmentFrameBuffer, basic) {
  pixel_t data[4 * 4];
  fragment_frame_buffer_t affb;
  fragment_frame_buffer_init(&affb, data, ARRAY_SIZE(data));

  rect_t r = rect_init(4, 4, 3, 2);

  s_log = "";
  fragment_frame_buffer_t* ffb = fragment_frame_buffer_begin_frame(&affb, &r);

  ASSERT_EQ(ffb->x, r.x);
  ASSERT_EQ(ffb->y, r.y);
  ASSERT_EQ(ffb->w, r.w);
  ASSERT_EQ(ffb->h, r.h);

  fragment_frame_buffer_set_window(ffb, 4, 4, 3, 2);
  ASSERT_EQ(ffb->win.x, 4);
  ASSERT_EQ(ffb->win.y, 4);
  ASSERT_EQ(ffb->win.w, 3);
  ASSERT_EQ(ffb->win.h, 2);

  fragment_frame_buffer_write_data(ffb, 1);
  fragment_frame_buffer_write_data(ffb, 2);
  fragment_frame_buffer_write_data(ffb, 3);
  fragment_frame_buffer_write_data(ffb, 4);
  fragment_frame_buffer_write_data(ffb, 5);
  fragment_frame_buffer_write_data(ffb, 6);
  fragment_frame_buffer_end_frame(ffb);

  ASSERT_EQ(s_log, "win(4 4 6 5):01 02 03 04 05 06 ");
}

TEST(FragmentFrameBuffer, not_large_enough) {
  pixel_t data[4 * 4];
  fragment_frame_buffer_t affb;
  fragment_frame_buffer_init(&affb, data, ARRAY_SIZE(data));

  rect_t r = rect_init(4, 4, 5, 5);
  fragment_frame_buffer_t* ffb = fragment_frame_buffer_begin_frame(&affb, &r);

  ASSERT_EQ(ffb, (fragment_frame_buffer_t*)NULL);
}

TEST(FragmentFrameBuffer, set) {
  pixel_t data[4 * 4];
  fragment_frame_buffer_t affb;
  fragment_frame_buffer_init(&affb, data, ARRAY_SIZE(data));

  rect_t r = rect_init(4, 4, 3, 2);

  s_log = "";
  fragment_frame_buffer_t* ffb = fragment_frame_buffer_begin_frame(&affb, &r);

  ASSERT_EQ(fragment_frame_buffer_set_pixel(ffb, 0, 0, 0), RET_BAD_PARAMS);
  ASSERT_EQ(fragment_frame_buffer_set_pixel(ffb, 3, 3, 0), RET_BAD_PARAMS);
  ASSERT_EQ(fragment_frame_buffer_set_pixel(ffb, 7, 3, 0), RET_BAD_PARAMS);
  ASSERT_EQ(fragment_frame_buffer_set_pixel(ffb, 0, 6, 0), RET_BAD_PARAMS);

  ASSERT_EQ(fragment_frame_buffer_set_pixel(ffb, 4, 4, 1), RET_OK);
  ASSERT_EQ(fragment_frame_buffer_get_pixel(ffb, 4, 4), 1);

  ASSERT_EQ(fragment_frame_buffer_set_pixel(ffb, 5, 4, 2), RET_OK);
  ASSERT_EQ(fragment_frame_buffer_get_pixel(ffb, 5, 4), 2);

  ASSERT_EQ(fragment_frame_buffer_set_pixel(ffb, 6, 4, 3), RET_OK);
  ASSERT_EQ(fragment_frame_buffer_get_pixel(ffb, 6, 4), 3);

  ASSERT_EQ(fragment_frame_buffer_set_pixel(ffb, 4, 5, 4), RET_OK);
  ASSERT_EQ(fragment_frame_buffer_get_pixel(ffb, 4, 5), 4);

  ASSERT_EQ(fragment_frame_buffer_set_pixel(ffb, 5, 5, 5), RET_OK);
  ASSERT_EQ(fragment_frame_buffer_get_pixel(ffb, 5, 5), 5);

  ASSERT_EQ(fragment_frame_buffer_set_pixel(ffb, 6, 5, 6), RET_OK);
  ASSERT_EQ(fragment_frame_buffer_get_pixel(ffb, 6, 5), 6);

  fragment_frame_buffer_end_frame(ffb);

  ASSERT_EQ(s_log, "win(4 4 6 5):01 02 03 04 05 06 ");
}
