## 如何在非 GUI 线程操作 GUI 控件

GUI 控件只能在 GUI 线程进行操作，非 GUI 线程想操作 GUI 控件，必须用 idle\_queue 或 timer\_queue 进行串行化。

* idle\_queue 向主循环的事件队列提交一个增加 idle 的请求，GUI 线程的主循环在处理事件队列时，会把该 idle 函数放到 idle 管理器中，在分发 idle 时，该 idle 函数在 GUI 线程执行。

* timer\_queue 向主循环的事件队列提交一个增加 timer 的请求，GUI 线程的主循环在处理事件队列时，会把该 timer 函数放到 timer 管理器中，在分发 timer 时，该 timer 函数在 GUI 线程执行。

**注意：idle\_queue 和 timer\_queue 是少数几个可以在非 GUI 线程安全调用的函数。**

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

### 注意事项

在 idle 函数执行的时候，窗口可能已经被关闭，控件已经处于无效状态。为了避免出现也指针的问题，在 idle 函数中，应该检查目标窗口和控件是否存在。
