## 如何在非GUI线程操作GUI控件

GUI控件只能在GUI线程进行操作，非GUI线程想操作GUI控件，必须用idle\_queue或timer\_queue进行串行化。

* idle\_queue向主循环的事件队列提交一个增加idle的请求，GUI线程的主循环在处理事件队列时，会把该idle函数放到idle管理器中，在分发idle时，该idle函数在GUI线程执行。

* timer\_queue向主循环的事件队列提交一个增加timer的请求，GUI线程的主循环在处理事件队列时，会把该timer函数放到timer管理器中，在分发timer时，该timer函数在GUI线程执行。

**注意：idle\_queue和timer\_queue是少数几个可以在非GUI线程安全调用的函数。**

### 示例：

```
static ret_t on_timer(const timer_info_t* timer) {
  return update_progress_bar(WIDGET(timer->ctx));
}

static ret_t on_idle(const idle_info_t* idle) {
  return update_progress_bar(WIDGET(idle->ctx));
}

void* thread_entry(void* args) {
  int nr = 500;
  while(nr-- > 0) {
    idle_queue(on_idle, args);
    timer_queue(on_timer, args, 30);
    sleep_ms(30);
  }

  return NULL;
}

ret_t application_init() {
  thread_t* thread = NULL;
  widget_t* progress_bar = NULL;
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* label = label_create(win, 10, 10, 300, 20);

  widget_set_text(label, L"Update progressbar in non GUI thread");
  progress_bar = progress_bar_create(win, 10, 80, 300, 20);

  thread = thread_create(thread_entry, progress_bar);
  thread_start(thread);

  return RET_OK;
}
```

### 参考

> demos/demo\_thread\_app.c


