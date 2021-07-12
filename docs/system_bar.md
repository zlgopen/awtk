# system_bar 使用方法

system\_bar 是一个位于屏幕上方的窗口，通常用来显示当前窗口的标题、时间、电池电量、网络信号和关闭当前窗口的按钮等。

system\_bar 是可选的，可以启用也可以不启用。如果要启用 system\_bar，可以按下列方法：

## 一、创建 system\_bar 窗口

system\_bar 和普通窗口类似，你可以用代码创建，也可以用 XML UI 描述文件创建，比如下面是 demoui 中的 system\_bar 的 XML UI 描述文件 (system\_bar.xml)：

```
<system_bar h="30">
  <column x="0" y="0" w="-40" h="100%">
    <label style="title" x="10" y="m" w="55%" h="100%" name="title"/>
    <digit_clock style="time" x="r" y="m" w="40%" h="100%" format="h:m"/>
  </column>
  <button style="close" x="r:5" y="m" w="26" h="26" name="close" text="x"/>
</system_bar>
```

在 system\_bar 窗口中，有两个控件比较特殊（均为可选）：

* 1. 名为 title 的 label 控件。它自动显示当前主窗口的标题或名称。

> 窗口的标题可以用窗口的 text 属性指定。

* 2. 名为 close 的 button 控件。点击它时，它向当前主窗口放送 EVT\_REQUEST\_CLOSE\_WINDOW 事件。

> 缺省情况下，窗口收到 EVT\_REQUEST\_CLOSE\_WINDOW 事件时，窗口会被关闭。
>
> 如果窗口不想接受 EVT\_REQUEST\_CLOSE\_WINDOW 事件，可以设置窗口的 closable 属性为"no"，此时 close 按钮自动禁用。如：

 
```
<window closable="no" anim_hint="htranslate">
```

> 如果窗口希望收到 EVT\_REQUEST\_CLOSE\_WINDOW 事件时，由用户决定是否关闭当前。可以设置窗口的 closable 属性为"confirm"，同时处理 EVT\_REQUEST\_CLOSE\_WINDOW 事件，根据当前的情况决定是否关闭窗口。

## 二、打开窗口

如果要启用 system\_bar 窗口，必须在打开应用程序的窗口之前打开 system\_bar 窗口。

> 可以在资源加载窗口之后打开 system\_bar 窗口，但是资源加载窗口顶部区域需要留白。

```
window_open("system_bar");
```

## 三、style

system\_bar 的 style 和其它窗口一样，可以在 styles 目录下放一个与窗口同名的 style 文件 (system\_bar.xml)。比如下面是 demoui 中的 system\_bar 的 style(system\_bar.xml)：

```
<system_bar>
  <style name="default">
    <normal bg_color="#404040"/>
  </style>
</system_bar>

<label>
  <style name="title" text_align_h="left">
    <normal text_color="white" />
  </style>
</label>

<digit_clock>
  <style name="time">
    <normal text_color="white" />
  </style>
</digit_clock>

<button>
  <style name="close" icon="earth">
    <normal     icon="close_n"/>
    <pressed    icon="close_p"/>
    <over       icon="close_n"/>
    <disable    icon="close_d"/>
  </style>
</button>
```

如果整个屏幕用一张大的背景图，可以把背景图片设置到窗口管理器上，这样就不需要为 system\_bar 和应用窗口分别指定背景图片了。如：

```
<window_manager>
  <style name="default">
    <normal bg_image="bg800x480"/>
  </style>
</window_manager>
```
