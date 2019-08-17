/**
 * File:   fragment_frame_buffer
 * Author: AWTK Develop Team
 * Brief:  fragment_frame_buffer
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * this program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * license file for more details.
 *
 */

/**
 * history:
 * ================================================================
 * 2019-08-16 li xianjing <xianjimli@hotmail.com> created
 *
 */

typedef struct _fragment_frame_buffer_t {
  uint32_t x;
  uint32_t y;
  uint32_t w;
  uint32_t h;
  pixel_t* data;
  uint32_t pixels_nr;

  rect_t win;
  int32_t cursor_x;
  int32_t cursor_y;
} fragment_frame_buffer_t;

static fragment_frame_buffer_t* fragment_frame_buffer_init(fragment_frame_buffer_t* ffb,
                                                           pixel_t* data, uint32_t pixels_nr) {
  return_value_if_fail(ffb != NULL && data != NULL, NULL);

  memset(ffb, 0x00, sizeof(fragment_frame_buffer_t));

  ffb->data = data;
  ffb->pixels_nr = pixels_nr;

  return ffb;
}

static fragment_frame_buffer_t* fragment_frame_buffer_begin_frame(fragment_frame_buffer_t* ffb,
                                                                  rect_t* r) {
  uint32_t nr = 0;
  return_value_if_fail(ffb != NULL && r != NULL, NULL);

  nr = r->w * r->h;

  if (nr > ffb->pixels_nr) {
    return NULL;
  }

  ffb->x = r->x;
  ffb->y = r->y;
  ffb->w = r->w;
  ffb->h = r->h;

  return ffb;
}

static inline ret_t fragment_frame_buffer_set_pixel(fragment_frame_buffer_t* ffb, uint32_t x,
                                                    uint32_t y, pixel_t pixel) {
  uint32_t dx = x - ffb->x;
  uint32_t dy = y - ffb->y;
  pixel_t* p = ffb->data + dy * ffb->w + dx;

  return_value_if_fail((x >= ffb->x && y >= ffb->y), RET_BAD_PARAMS);
  return_value_if_fail((x < (ffb->x + ffb->w) && y < (ffb->y + ffb->h)), RET_BAD_PARAMS);

  *p = pixel;

  return RET_OK;
}

static inline pixel_t fragment_frame_buffer_get_pixel(fragment_frame_buffer_t* ffb, uint32_t x,
                                                      uint32_t y) {
  uint32_t dx = x - ffb->x;
  uint32_t dy = y - ffb->y;
  pixel_t* p = ffb->data + dy * ffb->w + dx;

  assert(x >= ffb->x && y >= ffb->y && x < (ffb->x + ffb->w) && y < (ffb->y + ffb->h));

  return *p;
}

static inline ret_t fragment_frame_buffer_end_frame(fragment_frame_buffer_t* ffb) {
  uint32_t i = 0;
  pixel_t* p = ffb->data;
  uint32_t nr = ffb->w * ffb->h;

  set_window_func(ffb->x, ffb->y, ffb->x + ffb->w - 1, ffb->y + ffb->h - 1);
  for (i = 0; i < nr; i++, p++) {
    write_data_func(*p);
  }

  return RET_OK;
}

static inline ret_t fragment_frame_buffer_set_window(fragment_frame_buffer_t* ffb, uint32_t x,
                                                     uint32_t y, uint32_t w, uint32_t h) {
  ffb->win.x = x;
  ffb->win.y = y;
  ffb->win.w = w;
  ffb->win.h = h;
  ffb->cursor_x = 0;
  ffb->cursor_y = 0;

  return RET_OK;
}

static inline ret_t fragment_frame_buffer_write_data(fragment_frame_buffer_t* ffb, pixel_t pixel) {
  uint32_t x = ffb->win.x + ffb->cursor_x;
  uint32_t y = ffb->win.y + ffb->cursor_y;
  ret_t ret = fragment_frame_buffer_set_pixel(ffb, x, y, pixel);

  ffb->cursor_x++;
  assert(ffb->cursor_y < ffb->win.h);
  if (ffb->cursor_x >= ffb->win.w) {
    ffb->cursor_x = 0;
    ffb->cursor_y++;
  }

  return ret;
}
