## 如何在非 GUI 线程操作 GUI 控件

GUI 控件只能在 GUI 线程进行操作，非 GUI 线程想操作 GUI 控件，必须用以下函数进行串行化。

### 1. idle\_queue

idle\_queue 向主循环的事件队列提交一个增加 idle 的请求，GUI 线程的主循环在处理事件队列时，会把该 idle 函数放到 idle 管理器中，在分发 idle 时，该 idle 函数在 GUI 线程执行。

```c
/**
 * @method idle_queue
 * 用于非 GUI 线程增加一个 idle，本函数向主循环的事件队列中发送一个增加 idle 的请求。
 * @annotation ["static"]
 * @param {idle_func_t} on_idle idle 回调函数。
 * @param {void*} ctx idle 回调函数的上下文。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t idle_queue(idle_func_t on_idle, void* ctx);
```

> on\_idle 函数返回 RET\_REPEAT 时，将重复执行。

### 2. timer\_queue

timer\_queue 向主循环的事件队列提交一个增加 timer 的请求，GUI 线程的主循环在处理事件队列时，会把该 timer 函数放到 timer 管理器中，在分发 timer 时，该 timer 函数在 GUI 线程执行。

```c
/**
 * @method timer_queue
 * 用于非 GUI 线程增加一个 timer，本函数向主循环的事件队列中发送一个增加 timer 的请求。
 * @annotation ["static"]
 * @param {timer_func_t} on_timer
 * timer 回调函数，回调函数返回 RET_REPEAT，则下次继续执行，否则自动移出。
 * @param {void*} ctx timer 回调函数的上下文。
 * @param {uint32_t} duration 时间。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t timer_queue(timer_func_t on_timer, void* ctx, uint32_t duration);
```

> on\_timer 函数返回 RET\_REPEAT 时，将重复执行。

### 3. tk\_run\_in\_ui\_thread

tk\_run\_in\_ui\_thread 让后台线程在 UI 线程执行指定的函数，它是对 idle\_queue 的包装，支持等待调用完成。

```c
/**
 * @method tk_run_in_ui_thread
 * 后台线程在 UI 线程执行指定的函数。
 *
 * @param {tk_callback_t} func 函数。
 * @param {void*} ctx  回调函数的上下文。
 * @param {bool_t} wait_until_done 是否等待完成。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t tk_run_in_ui_thread(tk_callback_t func, void* ctx, bool_t wait_until_done)
```

> 如果 wait\_until\_done 为 FALSE，func 函数返回 RET\_REPEAT 时，将重复执行。

> 注意：以上是少数几个可以在非 GUI 线程安全调用的函数，请不要在非 GUI 线程调用其它 widget 相关的函数。

### 示例：

```c
void* test_thread1(void* args) {
  int nr = 500000;
  while ((nr-- > 0) && (!s_app_quit)) {
    tk_run_in_ui_thread((tk_callback_t)update_progress_bar, args, TRUE);
    sleep_ms(30);
  }

  return NULL;
}

void* test_thread2(void* args) {
  int nr = 500000;
  while ((nr-- > 0) && (!s_app_quit)) {
    tk_run_in_ui_thread((tk_callback_t)update_progress_bar, args, FALSE);
    sleep_ms(30);
  }

  return NULL;
}

static ret_t on_idle(const idle_info_t* idle) {
  return update_progress_bar(WIDGET(idle->ctx));
}

void* test_thread3(void* args) {
  int nr = 500000;
  while ((nr-- > 0) && (!s_app_quit)) {
    idle_queue(on_idle, args);
    sleep_ms(30);
  }

  return NULL;
}

static ret_t on_timer(const timer_info_t* timer) {
  return update_progress_bar(WIDGET(timer->ctx));
}

void* test_thread4(void* args) {
  int nr = 500000;
  while ((nr-- > 0) && (!s_app_quit)) {
    timer_queue(on_timer, args, 30);
    sleep_ms(30);
  }

  return NULL;
}
```

### 参考

> demos/demo\_thread\_app.c

### 注意事项

* 在 idle 函数执行的时候，窗口可能已经被关闭，控件已经处于无效状态。为了避免出现也指针的问题，在 idle 函数中，应该检查目标窗口和控件是否存在。

