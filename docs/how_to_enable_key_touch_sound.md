# 如何开启按键音

在有的应用程序中，按键和触屏时，需要给用户提供某种反馈：

- 反馈的方式通常是声音或震动。
- 声音的类型可能与当前的按键有关。比如拨号界面，按下不同的数字会播放不同的声音。
- 在同一个应用，不同场景的需求也可能不一样，所以需要全局开启或关闭反馈。

由于 AWTK 本身没有提供震动和声音的接口，所以 AWTK 提供了一个 ui_feedback 接口，开发者可以设置回调函数，去播放声音或震动。

## 1 反馈回调函数

反馈回调函数的原型和初始化函数如下：

```c
/**
 * @method ui_on_feedback_t
 * 反馈回调函数。
 *
 * @param {void*} ctx 回调函数的上下文，即ui_feedback_init中传递的ctx。
 * @param {widget_t*} widget 设置feedback属性的控件对象。
 * @param {event_t*} evt 事件对象。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
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

通常只是部分控件需要提供反馈，因此控件缺省情况下不提供反馈功能。需要把控件的 feedback 属性设置为 TRUE，当有按键(按下和弹起)和触屏事件(按下和弹起)发生在该控件上时，才会调用开发者设置的回调函数。

在回调函数中，开发者可以根据事件的类型、当前设置和控件的其它属性，来决定如何反馈。

## 2 示例

由于 AWTK 没有提供震动和播放声音的接口，所以这里通过修改文本的方式展现该功能，要实现上面的功能点很简单，其步骤如下：

(1) 在 UI 文件中设置控件的"feedback"属性为 true，在这里需要设置"button"按钮的 feedback="true"，代码如下：

```xml
<window>
  <button name="button" x="60%" y="60%" w="20%" h="40" text="inc" feedback="true"/>
  <label name="label" x="c" y="60%" w="20%" h="40" text=""/>
</window>
```

(2) 在C代码中调用 ui_feedback_init 接口初始化，代码如下：

```c
static ret_t ui_on_btn_feedback(void* ctx, widget_t* widget, event_t* evt) {
  widget_t* win = (widget_t*)ctx;
  widget_t* label = widget_lookup(win, "label", TRUE);
  const char* name = widget_get_prop_str(widget, WIDGET_PROP_NAME, "");

  if (tk_str_eq(name, "button")) {
    if (evt->type == EVT_POINTER_DOWN) {
      widget_set_text_utf8(label, "point down");
    } else if (evt->type == EVT_POINTER_UP) {
      widget_set_text_utf8(label, "point up");
    }
  }
  return RET_OK;
}

/* 初始化 */
ret_t home_page_init(widget_t* win, void* ctx) {
  (void)ctx;
  return_value_if_fail(win != NULL, RET_BAD_PARAMS);

  widget_foreach(win, visit_init_child, win);
  ui_feedback_init(ui_on_btn_feedback, win);
    
  return RET_OK;
}
```

