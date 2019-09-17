# 如何开启按键音

在有的应用程序中，按键和触屏时，需要给用户提供某种反馈：

* 反馈的方式通常是声音或震动。

* 声音的类型可能与当前的按键有关。比如拨号界面，按下不同的数字会播放不同的声音。

* 在同一个应用，不同场景的需求也可能不一样，所以需要全局开启或关闭反馈。

由于AWTK 本身没有提供震动和声音的接口，所以 AWTK 提供了一个 ui_feedback 接口，开发者可以设置回调函数，去播放声音或震动。

回调函数的原型和初始化函数如下：

```
typedef ret_t (*ui_on_feedback_t)(void* ctx, widget_t* widget, event_t* evt);
/**
 * @method ui_feedback_init
 * 初始化。
 * 设置实际的处理函数。
 *
 * @annotation ["static"]
 * @param {ui_on_feedback_t} on_feedback 实际的处理函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ui_feedback_init(ui_on_feedback_t on_feedback, void* ctx);
```

通常只是部分控件需要提供反馈，因此控件缺省情况下不提供反馈功能。需要把控件的feedback属性设置为TRUE，当有按键(按下和弹起)和触屏事件(按下和弹起)发生在该控件上时，才会调用开发者设置的回调函数。

在回调函数中，开发者可以根据事件的类型、当前设置和控件的其它属性，来决定如何反馈。

