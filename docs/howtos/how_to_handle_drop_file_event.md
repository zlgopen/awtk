# 如何处理将文件拖入到窗口的事件。

在 PC 上，如果将系统文件浏览器中的文件拖入到 AWTK 的窗口上，会触发 EVT\_DROP\_FILE 事件。该事件会分发到窗口管理器对象和当前窗口对象上，可以根据需要选择注册事件的目标。

## 使用方法

* 注册事件。

```c
widget_on(win, EVT_DROP_FILE, window_on_drop_file, win);
```

* 实现事件处理函数。

```c
static ret_t window_on_drop_file(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  widget_t* label = widget_lookup(win, "filename", TRUE);
  drop_file_event_t* drop = drop_file_event_cast(e);

  widget_set_text_utf8(label, drop->filename);

  return RET_OK;
}
```

> 可以从 drop\_file\_event\_t 事件的 filename 中获取文件名，如果拖入的是多个文件，该事件会触发多次。

完整使用方法请参考: demos/demo\_ui\_old\_app.c
