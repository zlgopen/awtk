# 如何访问控件的属性

## 1.修改控件的属性

控件所有的公开属性，都提供了相应的 set 函数，用它可以修改对应的属性。如：

```c
/**
 * @method check_button_set_value
 * 设置控件的值。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget check_button 对象。
 * @param {bool_t}  value 值
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t check_button_set_value(widget_t* widget, bool_t value);
```

## 2.读取控件的属性

控件都提供了一个宏，将其由 widget 类型转换成实际的类型，然后可以直接读取它的属性。如：

```c
bool_t checked = CHECK_BUTTON(button)->value;
```

## 3.通过 widget 的函数访问控件的属性

widget 提供了 widget\_get\_prop/widget\_set\_prop 系列函数，也可以用来访问控件的属性。

```c
/**
 * @method widget_get_prop
 * 获取控件指定属性的值。
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 属性的名称。
 * @param {value_t*} v 返回属性的值。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t widget_get_prop(widget_t* widget, const char* name, value_t* v);

/**
 * @method widget_set_prop
 * 设置控件指定属性的值。
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 属性的名称。
 * @param {value_t*} v 属性的值。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t widget_set_prop(widget_t* widget, const char* name, const value_t* v);
```

## 4.访问 text 属性

```
/**
 * @method widget_get_text
 * 获取控件的文本。
 * 只是对 widget\_get\_prop 的包装，文本的意义由子类控件决定。
 *
 * 如果希望获取 UTF8 格式的文本，可以参考下面的代码：
 *
 * ```c
 *  str_t str;
 *  str_init(&str, 0);
 *  str_from_wstr(&str, widget_get_text(target));
 *  log_debug("%s: %s\n", target->name, str.str);
 *  str_reset(&str);
 * ```
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {const wchar_t*} 返回文本。
 */
const wchar_t* widget_get_text(widget_t* widget);

/**
 * @method widget_set_text
 * 设置控件的文本。
 * 只是对 widget\_set\_prop 的包装，文本的意义由子类控件决定。
 * @param {widget_t*} widget 控件对象。
 * @param {const wchar_t*}  text 文本。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t widget_set_text(widget_t* widget, const wchar_t* text)

/**
 * @method widget_set_text_utf8
 * 设置控件的文本。
 * 只是对 widget\_set\_prop 的包装，文本的意义由子类控件决定。
 * @annotation ["scriptable"]
 * @alias set_text
 * @param {widget_t*} widget 控件对象。
 * @param {const char*}  text 文本。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t widget_set_text_utf8(widget_t* widget, const char* text);
```

## 5.访问 value 属性

> 仅使用与 int/bool 类型的属性

```
/**
 * @method widget_set_value
 * 设置控件的值。
 * 只是对 widget\_set\_prop 的包装，值的意义由子类控件决定。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t}  value 值。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t widget_set_value(widget_t* widget, int32_t value)

/**
 * @method widget_get_value
 * 获取控件的值。只是对 widget\_get\_prop 的包装，值的意义由子类控件决定。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 *
 * @return {int32_t} 返回值。
 */
int32_t widget_get_value(widget_t* widget);

```
