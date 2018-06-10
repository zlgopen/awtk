# AWTK = Toolkit AnyWhere

[AWTK](https://github.com/zlgopen/awtk)是吸取了[FTK](https://github.com/xianjimli/ftk)和[CanTK](https://github.com/drawapp8/cantk)的精华，重新开发的GUI，计划分以下几个阶段实施：

* 第一阶段专注于嵌入式系统，到达并超越TouchGfx/Embedded Wizard的功能。计划在2018/9底完成。
* 第二阶段用AWTK本身开发界面编辑器(或组态软件IDE)，从而验证AWTK支持PC软件开发的能力。计划在2018年底完成。
* 第三阶段支持Mobile应用程序开发，这一步比较容易，因为在第一阶段就是要把手机上的现代GUI推广到传统嵌入式系统中去。计划在2019/3完成。
* 第四阶段支持2D游戏开发，把以前开发游戏引擎的经验融合到AWTK中，方便开发交互式的多媒体应用。计划在2019/6完成。 

最终目标：

* 支持开发嵌入式软件。
* 支持开发Linux应用程序。
* 支持开发MacOS应用程序。
* 支持开发Windows应用程序。
* 支持开发Android应用程序。
* 支持开发iOS应用程序。
* 支持开发2D游戏。

其主要特色有：

1. 小巧。在精简配置下，不依赖第三方软件包，仅需要8K RAM+32K FLASH即可开发一些简单的图形应用程序。

2. 高效。采用脏矩形裁剪算法，每次只绘制和更新变化的部分，极大提高运行效率和能源利用率

3. 稳定。通过良好的架构设计和编程风格、单元测试、动态(valgrind)检查和Code Review保证其运行的稳定性。

4. 丰富的GUI组件。提供窗口、对话框和各种常用的组件(用户可以配置自己需要的组件，降低对运行环境的要求)。

5. 支持多种字体格式。内置位图字体(并提供转换工具)，也可以使用stb\_truetype或freetype加载ttf字体。

6. 支持多种图片格式。内置位图图片(并提供转换工具)，也可以使用stb\_image加载png/jpg等格式的图片。

7. 紧凑的二进制界面描述格式。可以手工编辑的XML格式的界面描述文件，可以使用Qt Designer设计界面，也可以使用Visual Studio设计界面，然后转换成紧凑的二进制界面描述格式，提高运行效率，减小内存开销。

8. 支持主题并采用紧凑的二进制格式。开发时使用XML格式描述主题，然后转换成紧凑的二进制格式，提高运行效率，减小内存开销。

9. 支持裸系统，无需OS和文件系统。字体、图片、主题和界面描述数据都编译到代码中，以常量数据的形式存放，运行时无需加载到内存。

10. 内置agg/picasso/nanovg实现高质量的矢量动画。

11. 支持窗口动画、控件动画和滑动动画等现代GUI常见特性。

12. 可移植。支持移植到各种RTOS和嵌入式Linux系统，并通过SDL在各种流行的PC/手机系统上运行。

13. 脚本化。从API注释中提取API的描述信息，通过这些信息可以自动生成各种脚本的绑定代码。

14. 采用LGPL协议发布，在商业软件中使用时无需付费。

目前核心功能已经完成(还不具备应用到产品的水平)，欢迎有兴趣的朋友移植到各个RTOS。

## 模拟运行：

### Mac

如果没有安装scons和sdl2，请在终端运行下面的命令：

```
brew install scons sdl2
```

编译运行：

```
scons
./bin/demo1
```

### Ubuntu

如果没有安装scons和libsdl2-dev，请在终端运行下面的命令：

```
sudo apt-get install scons libsdl2-dev
```

编译运行：

```
scons
./bin/demo1
```

### Windows

(请先安装scons和Visual Studio C++)

编译运行：

```
scons
bin\demo1
```

## 各个平台的移植

* [stm32f103ze](https://github.com/zlgopen/awtk-stm32f103ze-raw)

* [stm32f429igtx](https://github.com/zlgopen/awtk-stm32f429igtx-raw)

* [awtk-aworks-rt1052](https://github.com/zlgopen/awtk-aworks-rt1052)

## 文档

### 1.使用文档
* [AWTK控件的布局参数介绍](docs/layout.md)
* [AWTK界面描述文件介绍](docs/ui_desc.md)
* [AWTK中的图片显示方式](docs/image_draw_type.md)
* [AWTK中的矢量图绘图函数](docs/vgcanvas.md)
* [AWTK中的事件处理函数](docs/event_listener.md)
* [AWTK中的国际化之字符串翻译](docs/locale.md)
* [AWTK中的主题](docs/theme.md)
* [AWTK中的资源管理器](docs/resource_manager.md)
* [AWTK中的窗口动画](docs/window_animator.md)
* [AWTK中的控件动画](docs/widget_animator.md)

### 2.HowTos
* [如何在非GUI线程操作GUI控件](docs/how_to_non_gui_thread_operate_widget.md)

### 3.内部原理
* [AWTK脚本绑定的实现原理 - lua绑定](docs/binding_lua.md)

### 4.移植文档
* [嵌入式平台移植注意事项](docs/porting_common.md)
* [将AWTK移植到STM32f103ze裸系统](docs/porting_to_stm32f103ze.md)
* [将AWTK移植到STM32f429igtx裸系统](docs/porting_to_stm32f429igtx.md)
* [将AWTK移植到AWorks平台](docs/porting_to_awtk_aworks_rt1052.md)

### 5.技术笔记
* [FrameBuffer的几种使用方式](docs/framebuffer.md)
* [LCD接口的三种实现方式](docs/lcd.md)


## 任务完成情况
[TODO.md](TODO.md)

## 最新动态

* 2018/06/10
    * 优化窗口动画。
    * 完善lcd的接口。
    * 完善资源管理。

* 2018/06/09
    * 修改stb\_image/stb\_truetype在开发板不工作的问题(内存分配/stack大小)。
    * 图片加载支持bgra/rgb565格式。
    * 修stm32\_g2d中blend\_image的BUG。

* 2018/06/08
    * 修改几个BUG。
    * 完善demos。

* 2018/06/07
    * slide\_view实现基本功能。

* 2018/06/06
    * 增加memset/memcpy速度测试。
    * 增加widget\_animator\_scroll实现滚动的动画。
    * 增加滑动速度计算。

* 2018/06/05
    * event结果中增加时间戳。
    * 完善layout参数，支持指定子控件的宽度和高度。

* 2018/06/04
    * 移植到正点原子号令者开发板。

* 2018/06/03
    * 把systick作为公共代码放到platform/raw下。
    * 去掉对stdio中部分函数的依赖。
    * 修改匿名union的问题。

* 2018/06/02
    * 控件动画文档。
    * misc/new实现C++的内存分配。

* 2018/06/01
    * 修改STM F103ZE JTAG无法下载的问题。
    * 优化lcd\_reg.inc
    * STM F103ZE实现获取时间的函数。

* 2018/05/31
    * 控件支持缩放动画(目前需要OpenGL)。
    * 控件支持旋转动画(目前需要OpenGL)。

* 2018/05/30
    * 控件支持淡入淡出动画。

* 2018/05/28
    * aworks平台上的memcpy有问题，暂时使用自定义的memcpy。

* 2018/05/27
    * 改进lcd\_mem的实现，编写部分文档。

* 2018/05/26
    * 优化图片缩放算法。

* 2018/05/25
    * 优化填充颜色算法。

* 2018/05/24
    * 优化soft\_g2d。

* 2018/05/23
    * 移植到aworks平台。

* 2018/05/18
    * 编写framebuffer的文档。

* 2018/05/17
    * 支持非GUI线程添加idle/timer到GUI线程执行。

* 2018/05/16
    * 实现控件动画框架。
    * 实现控件移动位置的动画。
    * 实现控件修改值的动画。

* 2018/05/15
    * 重新实现timer/idle，支持timer/idle处理函数内部操作timer/idle。

* 2018/05/14
    * 修改内存管理器的BUG，完善内存管理器的测试程序。

* 2018/05/13
    * 编写stm32f429igtx移植文档。

* 2018/05/12
    * lcd 支持double framebuffer，移植到stm32429igtx裸系统。

* 2018/05/10
    * 为了方便代码管理，把硬件平台相关的代码移到独立的项目中。

* 2018/05/09
    * 重新整理了基于framebuffer的LCD，并支持硬件加速功能。

* 2018/05/07
    * 实现STM32的dma2d加速的矩形填充和图片合成。

* 2018/05/05
    * lftk改名为awtk。

* 2018/05/03
    * 增加locale，支持多国语言字符串翻译。

* 2018/05/01
    * 更新文档。

* 2018/04/30
    * 窗口动画的anim\_hint用字符串表示，方便附加参数。 

* 2018/04/29
    * 增加str\_t，widget的名称用str\_t。

* 2018/04/28
    * 图片管理器支持缓存。
    * 图片管理器支持根据不同的DPI加载不同的图片。

* 2018/04/27
    * 资源管理器支持从文件中加载资源。

* 2018/04/26
    * 增加picasso实现的vgcanvas。
    * 增加system\_info

* 2018/04/21
    * 对话框支持从下往上弹出、从上往下弹出、中心缩放和淡入淡出几种动画。
    * 窗口支持左右平移动画。

* 2018/04/20
    * 实现窗口动画框架。

* 2018/04/15
    * 增加nanovg实现的vgcanvas。
    * 增加基于vgcanvas实现的lcd。

* 2018/04/14
    * PC环境缺省使用SDL2+nanovg渲染，解决在MacOS上文字模糊的问题。
    * 调整lcd的接口，使之具有更强的适应性。 

* 2018/04/8
    * 重新实现dialog的主题。

* 2018/04/7
    * 增加按键定义。
    * 增加edit控件(未完成)

* 2018/04/5
    * Slider控件支持step和事件。
    * Slider和ProgressBar支持在主题中设置图片。
    * 因为性能原因，agg中使用float代替double。

* 2018/04/2
    * 增加Slider控件。

* 2018/04/1
    * 字体管理器支持TTF字体。 
    * 增加工具resgen。
    * 增加主题文档。

* 2018/03/31
    * 重新设计XML主题的格式。
    * 重构window/button/label/check\_buton，使用同一个绘制函数。

* 2018/03/27
    * [Luncher](https://github.com/Luncher) 加入AWTK核心团队。
    * 完善文档。

* 2018/03/26
    * 主题支持设置背景图片和背景图片的显示方式。

* 2018/03/25
    * 增加矢量图绘图函数接口，并用agg实现部分功能。

* 2018/03/24
    * 支持图片居中、缩放、9宫格和平铺等10余种显示方式。

* 2018/03/18
    * 支持anti-aliasing直线。
    * 重新设计资源管理器。

* 2018/03/17
    * 支持将Qt的UI文件转成AWTK的UI文件。
    * 支持将VC的RC文件转成AWTK的UI文件。
    * 编写AWTK UI界面描述的文档。
    * 增加事件队列，解决在触屏中断中分发事件的问题。
    * 编写STM32移植文档。

* 2018/03/11
    * 增加XML界面描述文件预览工具。
    * 支持极速模式(定义FAST\_MODE启用)，只绘制顶层的窗口中变化的控件，绘制效率非常高，但不支持透明背景。
    * 支持基本的layout功能。

* 2018/03/10 
    * lua绑定及相关示例完成。
    * 支持从UI描述文件创建界面。
