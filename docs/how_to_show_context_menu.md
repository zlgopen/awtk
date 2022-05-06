# 如何显示上下文菜单（俗称右键菜单）

* 1.注册 EVT\_CONTEXT\_MENU 事件

```c
widget_on(win, EVT_CONTEXT_MENU, on_context_menu, win);
```

* 2.实现事件处理函数，在其中打开菜单窗口（建议用 popup 窗口）

```c
static ret_t on_context_menu(void* ctx, event_t* e) {
  pointer_event_t* evt = pointer_event_cast(e);
  widget_t* menu = popup_create(NULL, evt->x, evt->y, 128, 100);
  widget_t* b1 = button_create(menu, 2, 2, 124, 30);
  widget_t* b2 = button_create(menu, 2, 34, 124, 30);
  widget_t* b3 = button_create(menu, 2, 66, 124, 30);

  widget_set_text_utf8(b1, "Copy");
  widget_set_text_utf8(b2, "Paste");
  widget_set_text_utf8(b3, "Cut");

  popup_set_close_when_click(menu, TRUE);

  return RET_OK;
}
```

* [完整例子](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/context_menu.c)
