
## ZLG AWTK 1.1 Release Notes

## 一、介绍

[AWTK](https://github.com/zlgopen/awtk)全称Toolkit AnyWhere，是[ZLG](http://www.zlg.cn/)开发的开源GUI引擎，旨在为嵌入式系统、WEB、各种小程序、手机和PC打造的通用GUI引擎，为用户提供一个功能强大、高效可靠、简单易用、可轻松做出炫酷效果的GUI引擎。

#### [AWTK](https://github.com/zlgopen/awtk)寓意有两个方面：

* Toolkit AnyWhere。 
* ZLG物联网操作系统AWorksOS内置GUI。

#### [AWTK](https://github.com/zlgopen/awtk)源码仓库：

* 主源码仓库：[https://github.com/zlgopen/awtk](https://github.com/zlgopen/awtk)
* 镜像源码仓库：[https://gitee.com/zlgopen/awtk](https://gitee.com/zlgopen/awtk)


#### 运行效果截图：

![Chart-Demo](../images/chart_main.png)

![MusicPlayer-Demo](../images/musicplayer_main.png)

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

* AWTK本身是跨平台的。目前支持的平台有ZLG AWorksOS、Windows、Linux、MacOS、嵌入式Linux和嵌入式裸系统，可以轻松的移植到各种RTOS上。AWTK以后也可以运行在浏览器(即将公测)、各种小程序、Android和iOS等平台上运行。

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

## 四、1.1 版本更新
-------------------

### 1.稳定性增强

* 多个实际项目验证。
* 新增数百个测试用例。
* 增加控件运行时类型检查。
* 使用infer静态检查，并修改发现的问题。
* 使用cppcheck静态检查，并修改发现的问题。
* 使用valgrind动态内存检查，并修改发现的问题。

### 2.优化性能

* 绘制半透明色矩形速度提高3倍。
* 缩放不透明图片快1倍(bgr565格式LCD)。
* 缩放半透明图片快30%(bgr565格式LCD)。


### 3.新增特性

* 窗口动画支持自定义。
* 新增4种内置窗口动画。
* 对话框高亮策略支持自定义。
* 内置动态和静态背景变暗的对话框高亮策略。
* 新增 object_t接口。
* 对话框支持非模态。
* 窗口支持全屏窗口。
* 增加toast/info/confirm等内置对话框。
* slide view支持循环切换。
* slide view增加更多切换动画。
* 数字时钟支持英文的星期和月份。
* 支持自定义软键盘。
* 增加lcd_profile用于对绘制函数进行profile。
* data资源支持同文件名不同扩展名。
* window manager支持paint事件。
* UI XML文件支持简单表达式。
* 改进编译脚本和资源生成脚本。

### 4.新增控件

* 增加mutable image，用于实现camera和video时显示图像。

### 5.新增重要API

* window\_manager\_back 返回上一级窗口。
* window\_manager\_back\_to\_home 返回主窗口。
* window\_close\_force 强制关闭窗口(忽略动画)。
* ui\_loader\_load\_widget 用于加载局部组件。

### 6.Bug修改和完善功能

详情请参考[docs/changes.md](https://github.com/zlgopen/awtk/blob/master/docs/changes.md)。

## 五、预告
-------------------

下一个版本计划推出下列功能：

* 新增控件。
* 抽象音频接口。
* 桌面版本支持多原生窗口。
* 在sylixos和linux上支持多进程。
* AWTK-WEB 让AWTK应用程序在浏览器中运行。
* AWTK-MVVM 为嵌入式系统定制的MVVM框架。


> 需要什么功能，请在github上留言，我们会优先安排。
