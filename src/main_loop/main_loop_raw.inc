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

static ret_t main_loop_raw_init_canvas(main_loop_simple_t* loop) {
  lcd_t* lcd = platform_create_lcd(loop->w, loop->h);

  return_value_if_fail(lcd != NULL, RET_OOM);
  native_window_raw_init(lcd);

  return RET_OK;
}

main_loop_t* main_loop_init(int w, int h) {
  main_loop_simple_t* loop = main_loop_simple_init(w, h);

  loop->base.destroy = main_loop_raw_destroy;
  loop->dispatch_input = main_loop_raw_dispatch;
  main_loop_raw_init_canvas(loop);

  return (main_loop_t*)loop;
}
