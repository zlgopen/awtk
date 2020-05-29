
# 如何使用对话框

对话框有模态和非模态两种，具体用法如下：

## 非模态对话框

非模态对话框的使用方法与普通窗口完全一样，只是对话框的大小和位置可以改变。通常情况下，尽量使用非模态对话框。

示例：

>完整代码请参考： [awtk-c-demos/demos/normal_dialog.c](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/normal_dialog.c)

```c
static ret_t on_yes(void* ctx, event_t* e) {
  tk_quit();

  return RET_OK;
}

static ret_t on_no(void* ctx, event_t* e) {
  window_close(widget_get_window(WIDGET(e->target)));

  return RET_OK;
}

static ret_t on_open_dialog(void* ctx, event_t* e) {
  widget_t* dlg = dialog_create_simple(NULL, 0, 0, 240, 160);

  ...

  return RET_OK;
}
```

## 模态对话框

对于模态对话框，需要调用函数 dialog\_modal 等待对话框退出，dialog\_modal 函数会阻塞直到 dialog\_quit 被调用。

在点击某个按钮时，如果要关闭对话框，一定要通过 dialog\_quit 函数，不能直接关闭对话框！！！

示例：

>完整代码请参考： [awtk-c-demos/demos/modal_dialog.c](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/modal_dialog.c)

```c
static ret_t on_yes(void* ctx, event_t* e) {
  dialog_quit(widget_get_window(WIDGET(e->target)), DIALOG_QUIT_YES);

  return RET_OK;
}

static ret_t on_no(void* ctx, event_t* e) {
  dialog_quit(widget_get_window(WIDGET(e->target)), DIALOG_QUIT_NO);

  return RET_OK;
}

static ret_t on_open_dialog(void* ctx, event_t* e) {
  int code = 0;
  
  ...

  code = dialog_modal(dlg);
  if(code == DIALOG_QUIT_YES) {
    tk_quit();
  }

  return RET_OK;
}
```

在上面的代码中，下面的代码不会被执行，直到按钮被点击，然后调用 dialog_quit。

```c
if(code == DIALOG_QUIT_YES)
```

对于模态对话框来说，在确认按钮的事件处理函数中，除了调用 dialog_quit 函数，什么都别做！处理逻辑放到 dialog\_modal 之后，根据 dialog\_modal 的返回值决定下一步如何做。
