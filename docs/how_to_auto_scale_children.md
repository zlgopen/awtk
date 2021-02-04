# 如何根据实际分辨率自动调整窗口中子控件的位置大小

## 1. 介绍

一般来说，我们希望设计的界面在不同分辨率的设备上都能正常显示，此时应该使用 [layout 参数](layout.md)，而不要使用固定坐标和大小。

在有的情况下，设计时的分辨率与运行时的分辨率差不大，或者宽高之比的比例相近。比如手机，用 AWTK 开发的应用程序在手机上运行时，可以使用一种简便的方法，让 AWTK 自动调整子控件的坐标和大小（或者辅以 layout 参数进行微调）。

通过指定窗口下面这些属性，可以实现自动调整控件的坐标和大小。

```c
/**
   * @property {uint16_t} design_w
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 设计时宽度。
   */
  uint16_t design_w;
  /**
   * @property {uint16_t} design_h
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 设计时高度。
   */
  uint16_t design_h;
  /**
   * @property {bool_t} auto_scale_children_x
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 窗口大小与设计时大小不同时，是否自动调整子控件的 x 坐标。
   */
  uint16_t auto_scale_children_x : 1;
  /**
   * @property {bool_t} auto_scale_children_y
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 窗口大小与设计时大小不同时，是否自动调整子控件的 y 坐标。
   */
  uint16_t auto_scale_children_y : 1;
  /**
   * @property {bool_t} auto_scale_children_w
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 窗口大小与设计时大小不同时，是否自动调整子控件的宽度。
   */
  uint16_t auto_scale_children_w : 1;
  /**
   * @property {bool_t} auto_scale_children_h
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 窗口大小与设计时大小不同时，是否自动调整子控件的高度。
   */
  uint16_t auto_scale_children_h : 1;
```

> 如果子控件具有 self\_layout 参数或者所在容器具有 children\_layout 参数，则不使用自动调整功能。


## 2. 通过 XML 指定参数

示例：

```xml
<window 
  design_w="320" design_h="480" 
  auto_scale_children_x="true" auto_scale_children_y="true" 
  auto_scale_children_w="true" auto_scale_children_h="true" >
  <button text="Hello" x="120" w="80" y="220" h="40" />
</window>
```

## 3. 通过代码指定参数

在代码中，可以使用下面这个函数自动调整控件的坐标和大小：

```c
/**
 * @method window_set_auto_scale_children
 * 当设计分辨率和实际分辨率不一致时，自动调整子控件的位置和大小。
 *
 * > 当子控件有 self_layout 参数或者子控件的父控件有 children_layout 参数时，不会自动调整。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget window 对象。
 * @param {uint32_t} design_w 设计时宽度。
 * @param {uint32_t} design_h 设计时高度。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t window_set_auto_scale_children(widget_t* widget, uint32_t design_w, uint32_t design_h);
```

示例：

```c
widget_t* win = window_open("foobar");
window_set_auto_scale_children(win, 320, 480);
```
