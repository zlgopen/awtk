# 控件焦点相关问题

## 一、启用焦点停留 (tab stop)

除了 edit 控件外，其它控件如果需要焦点停留功能，可以指定控件的 focusable 属性为 true 来实现。

在 XML 中，可以这样指定：

```
 <button ... focusable="true"/>
```

在 C 中，可以这样指定：

```
 widget_get_prop_bool(widget, WIDGET_PROP_FOCUSABLE, TRUE);
```

**注意**： 如果指定了 fucusable 属性为 true，请确保控件的 style 中定义了 focused 状态的数据，否则会因为 focused 状态没有 style 数据而无法显示。如：

```
  <style name="default" border_color="#a0a0a0"  text_color="black">
    <normal     bg_color="#f0f0f0" />
    <pressed    bg_color="#c0c0c0" x_offset="1" y_offset="1"/>
    <over       bg_color="#e0e0e0" />
    <focused    bg_color="#e0e0e0" />
    <disable    bg_color="gray" text_color="#d0d0d0" />
  </style>    <focused    bg_color="#e0e0e0" />
```

## 二、切换焦点的按键

### 1. 缺省的向前和向后移动焦点的键值。

```
#ifndef TK_KEY_MOVE_FOCUS_NEXT
#define TK_KEY_MOVE_FOCUS_NEXT TK_KEY_TAB
#endif /*TK_KEY_MOVE_FOCUS_NEXT*/

#ifndef TK_KEY_MOVE_FOCUS_PREV
#define TK_KEY_MOVE_FOCUS_PREV TK_KEY_UNKNOWN
#endif /*TK_KEY_MOVE_FOCUS_PREV*/
```

> 用户可以重定义以上的宏来自定义这两个键值。

### 2. 窗口可以为当前窗口指定的向前和向后移动焦点的键值。

* move\_focus\_prev\_key 指定向前移动焦点的键值。 
* move\_focus\_next\_key 指定向后移动焦点的键值。

```
<window anim_hint="htranslate" move_focus_prev_key="up" move_focus_next_key="down">
```

> 在这个例子中，方向键 up 移动到前一个焦点控件，方向键 down 移动到下一个焦点控件。
> 
> 键名的定义请参考 [enums.c](https://github.com/zlgopen/awtk/blob/master/src/base/enums.c#L99)。

## 三、设置初始焦点

可以指定控件的 focus 属性为 true 将控件设置为初始焦点控件。

在 XML 中，可以这样指定：

```
 <button ... focus="true"/>
```

在 C 中，可以这样指定：

```
 widget_get_prop_bool(widget, WIDGET_PROP_FOCUS, TRUE);
```
