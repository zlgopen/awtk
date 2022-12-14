# 如何启用鼠标指针

## 1 启用鼠标指针

启用鼠标指针，需要定义宏 ENABLE\_CURSOR。

## 2 在应用程序启用鼠标指针

如果要在自己的应用程序中启用鼠标指针，可以按下面步骤：

(1) 添加鼠标指针图片，如"cursor.png"，将其放到类似design/default/images/xx的目录下。

(2) 调用 window_manager_set_cursor 设置鼠标指针，如：

```c
window_manager_set_cursor(window_manager(), "cursor");
```

> 上面函数中的"cursor"，对应鼠标图片的名称，两者名称需要一致

## 3 指针图片的要求

指针的形状有多种，比如普通指针、十字、文本选择和等待等等。为了简化处理，在绘制指针时，AWTK会把图片的中心定位到鼠标的当前位置，在制作指针图片时，请考虑这一点。

## 4 控件的缺省鼠标指针

控件的缺省鼠标指针由 vtable 的 pointer_cursor 属性指定。如 edit 的鼠标指针：

```c
TK_DECL_VTABLE(edit) = {.size = sizeof(edit_t),
                        .type = WIDGET_TYPE_EDIT,
                        .focusable = TRUE,
                        .inputable = TRUE,
                        .pointer_cursor = WIDGET_CURSOR_EDIT,
                        .clone_properties = s_edit_properties,
                        .persistent_properties = s_edit_properties,
                        .get_parent_vt = TK_GET_PARENT_VTABLE(widget),
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

## 5 为单个控件指定鼠标指针

为单个控件指定鼠标指针，只需要指定控件的 pointer_cursor 属性即可。如：

```xml
 <label x="0" y="0" w="100%" h="100%" text="up/down key change focus" pointer_cursor="cursor_hand"/>
```


## 6 等待状态的鼠标指针

下列两个函数用于开始和结束等待鼠标指针。

开始等待鼠标指针后：

- 如果 ignore\_user\_input 为 TRUE，窗口忽略所有输入事件。
- 鼠标移动时不再自动切换鼠标指针。

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
