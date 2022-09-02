# ListView 子控件的布局器

## 一、语法

子控件布局器统一使用 children_layout 属性指定，其语法为：

```bash
ListView 子控件布局器 => list_view '(' PARAM_LIST ')'
PARAM_LIST => PARAM | PARAM ',' PARAM_LIST
```

> ListView 子控件布局器只适用于 ListView 下的 ScrollView 控件，除非需要指定边距、间距或删除item的动画时间，否则一般无需指定该参数。

示例：

```xml
<list_view x="0"  y="30" w="100%" h="-80" item_height="60">
  <scroll_view name="view" x="0"  y="0" w="100%" h="100%" children_layout="list_view(m=10,s=15)">
```

## 二、参数

ListView 子控件的布局器提供了下列参数：

| 参数                  | 简写  | 说明                                                                          |
| --------------------- | :---: | :---------------------------------------------------------------------------- |
| item\_height          |   i   | 行高（作用与 listview 的 item\_height 属性相同）                              |
| default\_item\_height |   d   | 缺省行高（作用与 listview 的 default\_item\_height 属性相同）                 |
| margin                |   m   | 边距                                                                          |
| x\_margin             |  xm   | 水平方向的边距                                                                |
| y\_margin             |  ym   | 垂直方向的边距                                                                |
| spacing               |   s   | 子控件之间的间距                                                              |
| keep\_invisible       |  ki   | 是否给不可见的控件留位置(缺省否)                                              |
| keep\_disable         |  kd   | 是否给不用的控件留位置(缺省是)                                                |
| animating\_time       |   a   | 添加和删除item时附带动画的播放时间(缺省为0，单位为毫秒)，仅仅适用于单列的情况 |

> 注意事项：设置 animating\_time 属性后，添加 item 后动画效果会让这个 item 从原来的坐标缓动到 layout 后的坐标，所以在添加 item 之前请确保其坐标是我们所期望的。比如，想在 itemA 和 itemB 之间插入一个 itemC，那么添加前 itemC 坐标应该与 itemA 一致，让 itemC 从 itemA 的位置缓动到中间。

在代码中，可以通过 widget\_set\_children\_layout 函数启用子控件布局器：

```c
/**
 * @method widget_set_children_layout
 * 设置子控件的布局参数。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} params 布局参数。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t widget_set_children_layout(widget_t* widget, const char* params);
```

示例：

```c
widget_set_children_layout(w, "list_view(m=10,s=15)");
```

在 XML 中，可以通过 children\_layout 属性设置：

```xml
<list_view x="0"  y="30" w="100%" h="-80" item_height="60">
  <scroll_view name="view" x="0"  y="0" w="100%" h="100%" children_layout="list_view(m=10,s=15)">
```

