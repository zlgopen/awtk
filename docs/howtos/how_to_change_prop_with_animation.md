# 如何在改变控件属性时附带动画

控件在修改属性时，如果这个属性是会体现在界面上的（如进度条的 value），这个属性值发生跳变时就会显得特别突兀，因此，AWTK 增加了`改变控件属性时附带动画`的功能。

## 如何使用该功能

在属性名前增加前缀 `animate:` 即可，如：想改变 value 值附带动画，将属性名从`value`改为`animate:value`。

- 在界面描述文件设置：

```xml
<window theme="main" name="main">
  <slider name="slider" x="c" y="m" w="200" h="16" animate:value="40"/>
</window>
```

- 或在代码中设置：

```c
widget_t* win = window_open("main");
widget_t* slider = widget_lookup(win, "slider", TRUE);
widget_set_prop_double(slider, "animate:value", 40.0);
```

## 修改动画播放时间

附带动画的默认播放时间是 500 ms，可以通过设置控件的 `animate:animating_time` 属性修改动画播放时间，该属性单位为毫秒。

> 注意：在设置 `animate:prop_name` 属性添加附加动画之前，设置控件的 `animate:animating_time` 属性才能生效。

- 在界面描述文件设置：

```xml
<window theme="main" name="main">
  <!-- 先设置控件的 animate:animating_time 属性，让其生效 -->
  <slider name="slider" x="c" y="m" w="200" h="16" animate:animating_time="200" animate:value="40"/>
</window>
```

- 或在代码中设置：

```c
widget_t* win = window_open("main");
widget_t* slider = widget_lookup(win, "slider", TRUE);
/* 先设置控件的 animate:animating_time 属性，让其生效 */
widget_set_prop_int(slider, "animate:animating_time", 200);
widget_set_prop_double(slider, "animate:value", 40.0);
```

## demo 演示

请打开 `demoui` 中 `Slider` 页面或 `demouiold` 中 `Animate Widget/Value Change Animate` 页面查看演示。
