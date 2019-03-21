# 如何启用屏保

屏保是一个普通窗口，定时切换不同的图片或者动态绘制一些图形，以到达保护屏幕的效果。

## 一、设置屏保定时器时间

用函数window\_manager\_set\_screen\_saver\_time设置屏保时间。

```
/**
 * @method window_manager_set_screen_saver_time
 * 设置屏保时间。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 窗口管理器对象。
 * @param {uint32_t}  screen_saver_time 屏保时间(单位毫秒)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_manager_set_screen_saver_time(widget_t* widget, uint32_t screen_saver_time);
```

```
window_manager_set_screen_saver_time(window_manager(), 180 * 1000);
```

## 二、注册事件并打开屏保窗口

示例：

```
static ret_t close_window_on_event(void* ctx, event_t* e) {
  window_close(WIDGET(ctx));

  return RET_REMOVE;
}

static ret_t on_screen_saver(void* ctx, event_t* e) {
  widget_t* win = NULL;
  const char* screen_saver_win = "image_animation";

  if (widget_child(window_manager(), screen_saver_win) != NULL) {
    log_debug("screen saver exist.\n");
    return RET_OK;
  }

  win = window_open(screen_saver_win);
  widget_on(win, EVT_POINTER_MOVE, close_window_on_event, win);
  widget_on(win, EVT_POINTER_UP, close_window_on_event, win);
  widget_on(win, EVT_KEY_UP, close_window_on_event, win);

  return RET_OK;
}

ret_t application_init() {
  tk_ext_widgets_init();

  /*enable screen saver*/
  window_manager_set_screen_saver_time(window_manager(), 180 * 1000);
  widget_on(window_manager(), EVT_SCREEN_SAVER, on_screen_saver, NULL);

  return show_preload_res_window();
}

```

> 可以在XML设置window的fullscreen属性，让屏保窗口进入全屏状态。
