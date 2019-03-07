# 如何用"返回"的实体键去关闭任何一个窗口

有时需要用一个"返回"的实体键去关闭任何一个窗口，在AWTK中，可以这样实现：

基本原理：注册窗口管理器的按键事件，在该事件处理函数中，如果检查到指定的键按下，则请求窗口管理器关闭顶层窗口。具体做法如下：

1.编写事件处理函数。

```
static ret_t wm_on_key_down(void* ctx, event_t* e) {
  key_event_t* evt = (key_event_t*)e;
  if (evt->key == TK_KEY_ESCAPE) {
    window_manager_request_close_top_window(WIDGET(ctx));
  }

  return RET_OK;
}
```

2.注册窗口的按键事件处理函数：

```
widget_on(window_manager(), EVT_KEY_DOWN, wm_on_key_down, window_manager());
```

3.把主窗口设置为不可关闭：

```
widget_set_prop_str(win, WIDGET_PROP_CLOSABLE, "no");
```

请参考完整示例: [multi_windows.c](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/multi_windows.c)

如果在关闭窗口前，需要用户确认，请参考：[system_bar.md](https://github.com/zlgopen/awtk/blob/master/docs/system_bar.md)


