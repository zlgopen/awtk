# 如何启用屏保

屏保是一个普通窗口，定时切换不同的图片或者动态绘制一些图形，以到达保护屏幕的效果。

## 一、设置屏保定时器时间

AWTK对屏保提供了一定支持，只要定义宏WITH\_SCREEN\_SAVER\_TIME，为之指定一个时间(毫秒)，在指定一个时间内没有用户操作，即通过窗口管理器触发一个EVT\_SCREEN\_SAVER事件。

```
/**
 * 如果启用屏保定时器，请定义本宏
 * 如果启用，在指定时间内没有用户操作时，通过窗口管理器触EVT_SCREEN_SAVER事件。
 * 时间为毫秒。
 *
 * #define WITH_SCREEN_SAVER_TIME 180 * 1000
 */
```

> 参考：src/base/awtk\_config\_sample.h

## 二、注册事件并打开屏保窗口

```
static ret_t on_screen_saver(void* ctx, event_t* e) {
  window_open(SCREEN_SAVER_WINDOW);

  return RET_OK;
}

widget_on(window_manager(), EVT_SCREEN_SAVER, on_screen_saver, NULL);
```

> 屏保窗口在收到用户事件后应关闭自己。
