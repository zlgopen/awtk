
# AWTK = Toolkit AnyWhere

![zlg](docs/images/zlg_adv.jpeg)

## 一、介绍

[AWTK](https://github.com/zlgopen/awtk)全称Toolkit AnyWhere，是[ZLG](http://www.zlg.cn/)开发的开源GUI引擎，旨在为嵌入式系统、WEB、各种小程序、手机和PC打造的通用GUI引擎，为用户提供一个功能强大、高效可靠、简单易用、可轻松做出炫酷效果的GUI引擎。

#### [AWTK](https://github.com/zlgopen/awtk)寓意有两个方面：

* Toolkit AnyWhere。 
* ZLG物联网操作系统AWorks内置GUI。

#### [AWTK](https://github.com/zlgopen/awtk)源码仓库：

* 主源码仓库：[https://github.com/zlgopen/awtk](https://github.com/zlgopen/awtk)
* 镜像源码仓库：[https://gitee.com/zlgopen/awtk](https://gitee.com/zlgopen/awtk)


#### 运行效果截图：

![CleanAir-Demo](https://raw.githubusercontent.com/zlgopen/awtk-examples/master/docs/images/cleanair_main.png)

![Meter-Demo](https://raw.githubusercontent.com/zlgopen/awtk-examples/master/docs/images/meter_main.png)

![Chart-Demo](https://raw.githubusercontent.com/zlgopen/awtk-examples/master/docs/images/chart_main.png)

![MusicPlayer-Demo](https://raw.githubusercontent.com/zlgopen/awtk-examples/master/docs/images/musicplayer_main.png)

## 二、最终目标：

* 支持开发嵌入式应用程序。
* 支持开发Linux应用程序。
* 支持开发MacOS应用程序。
* 支持开发Windows应用程序。
* 支持开发Web APP。
* 支持开发微信小程序。
* 支持开发支付宝小程序。
* 支持开发百度小程序。
* 支持开发Android应用程序。
* 支持开发iOS应用程序。
* 支持开发2D小游戏。

## 三、主要特色

### 1.跨平台

[AWTK](https://github.com/zlgopen/awtk)是跨平台的，这有两个方面的意思：

* AWTK本身是跨平台的。目前支持的平台有ZLG AWorks、Windows、Linux、MacOS、嵌入式Linux和嵌入式裸系统，可以轻松的移植到各种RTOS上。AWTK以后也可以运行在浏览器(即将公测)、各种小程序、Android和iOS等平台上运行。

* AWTK同时还提供了一套跨平台的基础工具库。其中包括链表、数组、字符串(UTF8和widechar)，事件发射器、值、对象、文件系统、互斥锁和线程、表达式和字符串解析等等，让你用AWTK开发的应用程序可以真正跨平台运行。

### 2.高效

[AWTK](https://github.com/zlgopen/awtk)通过一系列的手段保证AWTK应用程序高效运行：

* 通过脏矩算法只更新变化的部分。
* 支持3 FrameBuffer让界面以最高帧率运行(可选)。
* UI描述文件和主题文件使用高效的二进制格式，解析在瞬间完成。
* 支持各种GPU加速接口。如OpenGL、DirectX、Vulkan和Metal等。
* 支持嵌入式平台的各种2D加速接口。目前STM32的DMA2D和NXP的PXP接口，厂家可以轻松扩展自己的加速接口。

### 3.稳定

[AWTK](https://github.com/zlgopen/awtk)通过下列方式极力让代码稳定可靠：

* 使用cppcheck和facebook infer进行静态检查。
* 使用valgrind进行动态内存检查。
* 近两万行的单元测试代码。
* ZLG强大GUI团队的支持。
* 经过多个实际项目验证。
* 多平台/多编译器验证。
* 优秀的架构设计。
* Code Review。
* 手工测试。

### 4.强大

* 丰富的控件(持续增加中)。
* 支持各种图片格式(png/jpg/gif/svg)。
* 支持各种字体格式(点阵和矢量)。
* 支持窗口动画
* 支持控件动画
* 支持高清屏。
* 支持界面描述文件。
* 支持主题描述文件。
* 支持控件布局策略。
* 支持对话框高亮策略。
* 丰富的辅助工具。
* 支持从低端的Cortex M3到各种高端CPU。
* 支持无文件系统和自定义的文件系统。
* 支持裸系统和RTOS。

### 5.易用

* 大量的示例代码。
* 完善的API文档和使用文档。
* ZLG 强大的技术支持团队。
* 用AWTK本身开发的界面编辑器(开发中)。
* 声明式的界面描述语言。一行代码启用控件动画，启用窗口动画，显示图片(png/jpg/svg/gif)。

### 6.高度扩展性

* 可以扩展自己的控件。
* 可以扩展自己的动画。
* 可以实现自己的主循环。
* 可以扩展自己的软键盘。
* 可以扩展自己的图片加载器。
* 可以扩展自己的字体加载器。
* 可以扩展自己的输入法引擎。
* 可以扩展自己的控件布局算法。
* 可以扩展自己的对话框高亮策略。
* 可以实现自己的LCD接口。
* 可以扩展自己的矢量引擎(如使用skia/cairo)。
* 所有扩展组件和内置组件具有相同的待遇。


### 7.多种开发语言

[AWTK](https://github.com/zlgopen/awtk)本身是用C语言开发的，可以通过IDL生成各种脚本语言的绑定。生成的绑定代码不是简单的把C语言的API映射到脚本语言，而是生成脚本语言原生代码风格的API。目前支持以下语言(以后根据需要增加)：

* C
* Lua
* Javascript on jerryscript
* Javascript on nodejs

### 8.国际化

* 支持Unicode。
* 支持输入法。
* 支持字符串翻译(实时生效)。
* 支持图片翻译(实时生效)。
* 文字双向排版(计划中)。

### 9.开放源码，免费商用(LGPL)。

## 四、模拟运行：

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

> 在Ubuntu 14上无法启动，请参考[FAQ](docs/faq.md)修改。

### Windows

(请先安装python，scons和Visual Studio C++(版本>=2017)

编译运行(在命令行模式下，进入awtk所在的目录，并运行下列命令)：

```
scons
bin\demoui
```

## 五、在线演示

* [基本控件在线演示](http://zlgopen.bceapp.com/awtk/demos/demoui/index.html)

> 目前AWTK-WEB还处于测试阶段，部分功能不能正常工作。如有兴趣请在PC上自行编译运行。

## 六、各个平台的移植

#### 官方支持平台 

* [awtk-aworks-rt1052](https://github.com/zlgopen/awtk-aworks-rt1052)

#### 移植示例(不定期更新)

* [awtk-sylixos](https://github.com/zlgopen/awtk-sylixos)

* [stm32f103ze](https://github.com/zlgopen/awtk-stm32f103ze-raw)

* [stm32f429igtx](https://github.com/zlgopen/awtk-stm32f429igtx-raw)

* [stm32f767igtx](https://github.com/zlgopen/awtk-stm32f767igtx-raw)

* [linux-framebuffer(嵌入式Linux请用本项目)](https://github.com/zlgopen/awtk-linux-fb)

## 七、各语言的绑定

* [awtk-lua](https://github.com/zlgopen/awtk-lua)

* [awtk-js](https://github.com/zlgopen/awtk-js)

## 八、相关工程

* [模板工程](https://github.com/zlgopen/awtk-hello)

* [C示例代码](https://github.com/zlgopen/awtk-c-demos)

* [精彩示例](https://github.com/zlgopen/awtk-examples)

* [AWTK-WEB](https://github.com/zlgopen/awtk-web)

* [AWTK-MVVM](https://github.com/zlgopen/awtk-mvvm)

* [扩展控件实例: 二维码](https://github.com/CUITzhaoqi/awtk-qr)

## 九、文档

[文档](docs/README.md)

## 十、任务完成情况

[任务完成情况](TODO.md)

## 十一、最新动态

[最新动态](docs/changes.md)

