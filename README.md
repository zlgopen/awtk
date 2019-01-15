# AWTK = Toolkit AnyWhere

[AWTK](https://github.com/zlgopen/awtk)是吸取了[FTK](https://github.com/xianjimli/ftk)和[CanTK](https://github.com/drawapp8/cantk)的精华，重新开发的GUI，计划分以下几个阶段实施：

* 第一阶段专注于嵌入式系统，到达并超越TouchGfx/Embedded Wizard的功能。计划在2018/9底完成(欢迎评测)。
* 第二阶段用AWTK本身开发界面编辑器(或组态软件IDE)，从而验证AWTK支持PC软件开发的能力。计划在2019/6完成。
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

2. 高效。采用脏矩形裁剪算法，每次只绘制和更新变化的部分，极大提高运行效率和能源利用率。

3. 稳定。通过良好的架构设计和编程风格、单元测试、动态(valgrind)检查和Code Review保证其运行的稳定性。

4. 丰富的GUI组件。提供窗口、对话框和各种常用的组件(用户可以配置自己需要的组件，降低对运行环境的要求)。

5. 支持多种字体格式。内置位图字体(并提供转换工具)，也可以使用stb\_truetype或freetype加载ttf字体。

6. 支持多种图片格式。内置位图图片(并提供转换工具)，也可以使用stb\_image加载png/jpg等格式的图片。

7. 紧凑的二进制界面描述格式。可以手工编辑的XML格式的界面描述文件，也可以使用Qt Designer设计界面，然后转换成紧凑的二进制界面描述格式，提高运行效率，减小内存开销。

8. 支持主题并采用紧凑的二进制格式。开发时使用XML格式描述主题，然后转换成紧凑的二进制格式，提高运行效率，减小内存开销。

9. 支持裸系统，无需OS和文件系统。字体、图片、主题和界面描述数据都编译到代码中，以常量数据的形式存放，运行时无需加载到内存。

10. 内置nanovg实现高质量的矢量动画，并支持SVG矢量图。

11. 支持窗口动画、控件动画、滑动动画和高清LCD等现代GUI常见特性。

12. 支持国际化(Unicode、字符串翻译和输入法等)。

13. 可移植。支持移植到各种RTOS和嵌入式Linux系统，并通过SDL在各种流行的PC/手机系统上运行。

14. 脚本化。从API注释中提取API的描述信息，通过这些信息可以自动生成各种脚本的绑定代码。

15. 支持硬件2D加速(目前支持STM32的DMA2D和NXP的PXP)和GPU加速(OpenGL/OpenGLES/DirectX/Metal)，充分挖掘硬件潜能。

16. 采用LGPL协议开源发布，在商业软件中使用时无需付费。

目前核心功能已经完成，内部开始在实际项目中使用了，欢迎有兴趣的朋友评估和尝试，期待您的反馈。

## 模拟运行：

### Mac

如果没有安装scons和sdl2，请在终端运行下面的命令(假定已安装brew)：

```
brew install scons sdl2
```

编译运行(在终端下，进入awtk所在的目录，并运行下列命令)：

```
scons
./bin/demoui
```

### Ubuntu (版本>=16)

如果没有安装scons和依赖的软件包，请在终端运行下面的命令：

```
sudo apt-get install scons libsndio-dev libgtk-3-dev libglu1-mesa libglu1-mesa-dev libgl1-mesa-glx libgl1-mesa-dev
```

编译运行(在终端下，进入awtk所在的目录，并运行下列命令)：

```
scons
./bin/demoui
```

### Windows

(请先安装python2.7，scons和Visual Studio C++(版本>=2017)

编译运行(在命令行模式下，进入awtk所在的目录，并运行下列命令)：

```
scons
bin\demoui
```

## 演示视频

1.[基本控件演示](https://v.qq.com/txp/iframe/player.html?vid=n07811pocfx)

## 各个平台的移植

#### 官方支持平台 

* [awtk-aworks-rt1052](https://github.com/zlgopen/awtk-aworks-rt1052)

#### 移植示例(不定期更新)

* [awtk-sylixos](https://github.com/zlgopen/awtk-sylixos)

* [stm32f103ze](https://github.com/zlgopen/awtk-stm32f103ze-raw)

* [stm32f429igtx](https://github.com/zlgopen/awtk-stm32f429igtx-raw)

* [stm32f767igtx](https://github.com/zlgopen/awtk-stm32f767igtx-raw)

* [linux-framebuffer(嵌入式Linux请用本项目)](https://github.com/zlgopen/awtk-linux-fb)

## 各语言的绑定

* [awtk-lua](https://github.com/zlgopen/awtk-lua)

* [awtk-js](https://github.com/zlgopen/awtk-js)

## 相关工程

* [模板工程](https://github.com/zlgopen/awtk-hello)

* [C示例代码](https://github.com/zlgopen/awtk-c-demos)

* [扩展控件实例: 二维码](https://github.com/CUITzhaoqi/awtk-qr)

## 文档

[文档](docs/index.md)

## 任务完成情况

[任务完成情况](TODO.md)

## 最新动态

[最新动态](docs/changes.md)

