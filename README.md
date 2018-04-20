# LFTK = Light Funny Toolkit

[FTK](https://github.com/xianjimli/ftk)是一个嵌入式GUI库，而[LFTK](https://github.com/xianjimli/lftk)是吸取了[FTK](https://github.com/xianjimli/ftk)和[CanTK](https://github.com/drawapp8/cantk)的精华，针对低端嵌入式设备，重新开发的嵌入式GUI库。其主要特色有：

1. 小巧。在精简配置下，不依赖第三方软件包，仅需要8K RAM+32K FLASH即可开发一些简单的图形应用程序。

2. 高效。采用脏矩形裁剪算法，每次只绘制和更新变化的部分，极大提高运行效率和能源利用率。

3. 稳定。通过良好的架构设计和编程风格、单元测试、动态(valgrind)检查和Code Review保证其运行的稳定性。

4. 丰富的GUI组件。提供窗口、对话框和各种常用的组件(用户可以配置自己需要的组件，降低对运行环境的要求)。

5. 支持多种字体格式。内置位图字体(并提供转换工具)，也可以使用stb\_truetype或freetype加载ttf字体。

6. 支持多种图片格式。内置位图图片(并提供转换工具)，也可以使用stb\_image加载png/jpg等格式的图片。

7. 紧凑的二进制界面描述格式。可以手工编辑的XML格式的界面描述文件，可以使用Qt Designer设计界面，也可以使用Visual Studio设计界面，然后转换成紧凑的二进制界面描述格式，提高运行效率，减小内存开销。

8. 支持主题并采用紧凑的二进制格式。开发时使用XML格式描述主题，然后转换成紧凑的二进制格式，提高运行效率，减小内存开销。

9. 支持裸系统，无需OS和文件系统。字体、图片、主题和界面描述数据都编译到代码中，以常量数据的形式存放，运行时无需加载到内存。

10. 可移植。支持移植到各种RTOS和嵌入式Linux系统，并通过SDL在各种流行的PC系统上运行。

11. 脚本化。从API注释中提取API的描述信息，通过这些信息可以自动生成各种脚本的绑定代码。

12. 采用LGPL协议发布，在商业软件中使用时无需付费。

目前核心功能已经完成(还不具备应用到产品的水平)，欢迎有兴趣的朋友移植到各个RTOS，如果在移植时需要开发板支持或有其它疑问，可以找我(QQ:302003333)

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

[stm32f103ze](https://github.com/xianjimli/lftk-stm32f103ze-raw.git)

## 文档

* [LFTK脚本绑定的实现原理 - lua绑定](docs/binding_lua.md)
* [LFTK控件的布局参数介绍](docs/layout.md)
* [LFTK界面描述文件介绍](docs/ui_desc.md)
* [LFTK移植到STM32裸系统](docs/porting_to_stm32.md)
* [LFTK中的图片显示方式](docs/image_draw_type.md)
* [LFTK中的矢量图绘图函数](docs/vgcanvas.md)
* [LFTK中的事件处理函数](docs/event_listener.md)
* [LFTK中的主题](docs/theme.md)

## 任务完成情况
[TODO.md](TODO.md)

## 最新动态

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
    * [Luncher](https://github.com/Luncher) 加入LFTK核心团队。
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
    * 支持将Qt的UI文件转成LFTK的UI文件。
    * 支持将VC的RC文件转成LFTK的UI文件。
    * 编写LFTK UI界面描述的文档。
    * 增加事件队列，解决在触屏中断中分发事件的问题。
    * 编写STM32移植文档。

* 2018/03/11
    * 增加XML界面描述文件预览工具。
    * 支持极速模式(定义FAST\_MODE启用)，只绘制顶层的窗口中变化的控件，绘制效率非常高，但不支持透明背景。
    * 支持基本的layout功能。

* 2018/03/10 
    * lua绑定及相关示例完成。
    * 支持从UI描述文件创建界面。
