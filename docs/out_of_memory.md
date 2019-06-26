# 内存耗尽处理流程

当 AWTK 应用程序内存分配失败时，会做以下尝试：

* 1. 释放最近 10 秒内没有用过的图片缓存，然后内存管理器再尝试分配内存。
* 2. 释放全部图片缓存，然后内存管理器再尝试分配内存。
* 3. 发出 EVT\_LOW\_MEMORY 事件，应用程序此时可以释放不必要的内存，然后内存管理器再尝试分配内存。
* 4. 发出 EVT\_OUT\_OF\_MEMORY 事件，应用程序此时可以释放紧急备用内存，保存重要数据，重启应用程序。
* 5. 如果第 4 步应用程序没有重启，内存管理器返回 NULL，后面发生的事情自求多福。

> 为了避免内存泄漏导致内存耗尽，建议应用程序编译awtk-linux-fb版本，用valgrind检查是否存在内存泄漏的问题。

注册事件的示例：

```c
static ret_t wm_on_low_memory(void* ctx, event_t* evt) {
  log_debug("low memory\n");
  return RET_OK;
}

static ret_t wm_on_out_of_memory(void* ctx, event_t* evt) {
  log_debug("out of memory\n");
  return RET_OK;
}

widget_t* wm = window_manager();
widget_on(wm, EVT_LOW_MEMORY, wm_on_low_memory, wm);
widget_on(wm, EVT_OUT_OF_MEMORY, wm_on_out_of_memory, wm);
```
