# 如何实现"back"键和"home"键

* **back键** 关闭当前窗口，回到上一级窗口。
* **home键** 回到第一个主窗口，并关闭之上的全部窗口。

1.编写事件处理函数。

```
static ret_t on_key_back_or_back_to_home(void* ctx, event_t* e) {
  key_event_t* evt = (key_event_t*)e;
  if (evt->key == TK_KEY_F2) {
    window_manager_back(WIDGET(ctx));
  } else if (evt->key == TK_KEY_F3) {
    window_manager_back_to_home(WIDGET(ctx));
  }

  return RET_OK;
}
```

2.注册窗口管理器的按键事件处理函数：

```
  widget_t* wm = window_manager();
  widget_on(wm, EVT_KEY_DOWN, on_key_back_or_back_to_home, wm);
```

3.把主窗口设置为不可关闭：

```
widget_set_prop_str(win, WIDGET_PROP_CLOSABLE, "no");
```

请参考完整示例: [multi_windows.c](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/multi_windows.c)

如果在关闭窗口前，需要用户确认，请参考：[system_bar.md](https://github.com/zlgopen/awtk/blob/master/docs/system_bar.md)


