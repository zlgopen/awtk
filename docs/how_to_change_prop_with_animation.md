# 如何在改变控件属性时附带动画

控件在修改属性时，如果这个属性是会体现在界面上的（如进度条的 value），这个属性值的跳变就会显得特别突兀，为此加入了`改变控件属性时附带动画`的功能。

## 如何使用该功能
在属性名前增加前缀 “`animate.`” 即可，如：想改变 value 值附带动画，将属性名从`value`改为`animate.value`。

在界面描述文件设置：
```xml
<window theme="main" name="main">
  <slider name="slider" x="c" y="m" w="200" h="16" animate.value="40"/>
</window>
```

或在代码中设置：
```c
widget_t* win = window_open("main");
widget_t* slider = widget_lookup(win, "slider", TRUE);
widget_set_prop_double(slider, "animate.value", 40.0);
```

## demo 演示
请打开 `demoui` 中 `Slider` 页面或 `demouiold` 中 `Animate Widget/Value Change Animate` 页面查看演示。
