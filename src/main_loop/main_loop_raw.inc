#include "native_window/native_window_raw.h"

static ret_t main_loop_raw_dispatch(main_loop_simple_t* loop) {
  platform_disaptch_input((main_loop_t*)loop);

  return RET_OK;
}

static ret_t main_loop_raw_destroy(main_loop_t* l) {
  main_loop_simple_t* loop = (main_loop_simple_t*)l;
  main_loop_simple_reset(loop);
  native_window_raw_deinit();

  return RET_OK;
}

static ret_t main_loop_raw_init_canvas(uint32_t w, uint32_t h) {
  lcd_t* lcd = platform_create_lcd(w, h);

  return_value_if_fail(lcd != NULL, RET_OOM);
  native_window_raw_init(lcd);

  return RET_OK;
}

#ifndef TK_QUEUE_EVENT
#define TK_QUEUE_EVENT NULL
#endif /*TK_QUEUE_EVENT*/

#ifndef TK_RECV_EVENT
#define TK_RECV_EVENT NULL
#endif /*TK_RECV_EVENT*/

main_loop_t* main_loop_init(int w, int h) {
  main_loop_simple_t* loop = NULL;
  return_value_if_fail(main_loop_raw_init_canvas(w, h) == RET_OK, NULL);

  loop = main_loop_simple_init(w, h, TK_QUEUE_EVENT, TK_RECV_EVENT);
  return_value_if_fail(loop != NULL, NULL);

  loop->base.destroy = main_loop_raw_destroy;
  loop->dispatch_input = main_loop_raw_dispatch;

  return (main_loop_t*)loop;
}
