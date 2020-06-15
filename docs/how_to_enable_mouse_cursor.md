# 如何启用鼠标指针

## 1. 启用鼠标指针

启用鼠标指针，需要定义宏 ENABLE\_CURSOR。

## 2. 控件的缺省鼠标指针

控件的缺省鼠标指针由 vtable 的 pointer_cursor 属性指定。如 edit 的鼠标指针：

```c
TK_DECL_VTABLE(edit) = {.size = sizeof(edit_t),
                        .type = WIDGET_TYPE_EDIT,
                        .focusable = TRUE,
                        .inputable = TRUE,
                        .pointer_cursor = WIDGET_CURSOR_EDIT,
                        .clone_properties = s_edit_properties,
                        .persistent_properties = s_edit_properties,
                        .parent = TK_PARENT_VTABLE(widget),
                        .create = edit_create,
                        .on_re_translate = edit_on_re_translate,
                        .on_paint_self = edit_on_paint_self,
                        .set_prop = edit_set_prop,
                        .get_prop = edit_get_prop,
                        .on_destroy = edit_on_destroy,
                        .on_copy = edit_on_copy,
                        .on_event = edit_on_event}
```

> 控件的缺省鼠标指针在 AWTK 内部写定了，开发者可以换图片来改变鼠标指针的形状。

## 3. 为单个控件指定鼠标指针。

为单个控件指定鼠标指针，只需要指定控件的 pointer_cursor 属性即可。如：

```xml
 <label x="0" y="0" w="100%" h="100%" text="up/down key change focus" pointer_cursor="cursor_hand"/>
 ```
 

## 4. 等待状态的鼠标指针

下列两个函数用于开始和结束等待鼠标指针。

开始等待鼠标指针后：

* 如果 ignore\_user\_input 为 TRUE，窗口忽略所有输入事件。

* 鼠标移动时不再自动切换鼠标指针。

```c
/**
 * @method widget_begin_wait_pointer_cursor
 * 开始等待鼠标指针。
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} ignore_user_input 是否忽略用户输入。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t widget_begin_wait_pointer_cursor(widget_t* widget, bool_t ignore_user_input);

/**
 * @method widget_end_wait_pointer_cursor
 * 结束等待鼠标指针。
 * @param {widget_t*} widget 控件对象。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t widget_begin_end_pointer_cursor(widget_t* widget);
```

[完整实例](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/wait_cursor.c)

