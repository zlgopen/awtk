
## ZLG AWTK 1.7 Release Notes

## 一、介绍

[AWTK](README.md) 全称 Toolkit AnyWhere，是 [ZLG](http://www.zlg.cn/) 开发的开源 GUI 引擎，旨在为嵌入式系统、WEB、各种小程序、手机和 PC 打造的通用 GUI 引擎，为用户提供一个功能强大、高效可靠、简单易用、可轻松做出炫酷效果的 GUI 引擎。

> 欢迎广大开发者一起参与开发：[生态共建计划](docs/awtk_ecology.md)。

#### [AWTK](README.md) 寓意有两个方面：

* Toolkit AnyWhere。 
* ZLG 物联网操作系统 AWorksOS 内置 GUI。

#### [AWTK](README.md) 源码仓库：

* 主源码仓库：[https://github.com/zlgopen/awtk](https://github.com/zlgopen/awtk)
* 镜像源码仓库：[https://gitee.com/zlgopen/awtk](https://gitee.com/zlgopen/awtk)

#### AWTK Designer 界面设计工具：

* 不再需要手写 XML
* 拖拽方式设计界面，所见即所得
* 快速预览，一键打包资源
* 注册及下载地址：https://awtk.zlg.cn

![AWTK Designer](../images/designer.png)

#### 运行效果截图：

![Chart-Demo](../images/chart_main.png)

![MusicPlayer-Demo](../images/musicplayer_main.png)

![Watch](../images/smartwatch_main.png)

## 二、最终目标：

* 支持开发嵌入式应用程序。✔
* 支持开发 Linux 应用程序。✔
* 支持开发 MacOS 应用程序。✔
* 支持开发 Windows 应用程序。✔
* 支持开发 Web APP。✔
* 支持开发 Android 应用程序。✔
* 支持开发 iOS 应用程序。✔
* 支持开发微信小程序。
* 支持开发支付宝小程序。
* 支持开发百度小程序。
* 支持开发 2D 小游戏。

## 三、主要特色

### 1. 跨平台

[AWTK](README.md) 是跨平台的，这有两个方面的意思：

* AWTK 本身是跨平台的。目前支持的平台有 ZLG AWorksOS、Windows、Linux、MacOS、嵌入式 Linux、Android、iOS、Web 和嵌入式裸系统，可以轻松的移植到各种 RTOS 上。AWTK 以后也可以运行在各种小程序平台上运行。

* AWTK 同时还提供了一套跨平台的基础工具库。其中包括链表、数组、字符串 (UTF8 和 widechar)，事件发射器、值、对象、文件系统、互斥锁和线程、表达式和字符串解析等等，让你用 AWTK 开发的应用程序可以真正跨平台运行。

### 2. 高效

[AWTK](README.md) 通过一系列的手段保证 AWTK 应用程序高效运行：

* 通过脏矩算法只更新变化的部分。
* 支持 3 FrameBuffer 让界面以最高帧率运行 （可选）。
* UI 描述文件和窗体样式文件使用高效的二进制格式，解析在瞬间完成。
* 支持各种 GPU 加速接口。如 OpenGL、DirectX、Vulkan 和 Metal 等。
* 支持嵌入式平台的各种 2D 加速接口。目前 STM32 的 DMA2D 和 NXP 的 PXP 接口，厂家可以轻松扩展自己的加速接口。

### 3. 稳定

[AWTK](README.md) 通过下列方式极力让代码稳定可靠：

* 使用 cppcheck 和 facebook infer 进行静态检查。
* 使用 valgrind 进行动态内存检查。
* 近两万行的单元测试代码。
* ZLG 强大 GUI 团队的支持。
* 经过多个实际项目验证。
* 多平台 / 多编译器验证。
* 优秀的架构设计。
* Code Review。
* 手工测试。

### 4. 强大

* 丰富的控件 （持续增加中）。
* 支持各种图片格式 (png/jpg/gif/svg)。
* 支持各种字体格式 （点阵和矢量）。
* 支持窗口动画。
* 支持控件动画。
* 支持高清屏。
* 支持界面描述文件。
* 支持窗体样式描述文件。
* 主题切换实时生效。
* 支持控件布局策略。
* 支持对话框高亮策略。
* 丰富的辅助工具。
* 支持从低端的 Cortex M3 到各种高端 CPU。
* 支持无文件系统和自定义的文件系统。
* 支持裸系统和 RTOS。
* 支持事件录制与重放进行压力测试。
* 支持 Appium 进行全自动化 UI 测试。

### 5. 易用

* 大量的示例代码。
* 完善的 API 文档和使用文档。
* ZLG 强大的技术支持团队。
* 用 AWTK 本身开发的 [界面编辑器](https://awtk.zlg.cn)。
* 声明式的界面描述语言。一行代码启用控件动画，启用窗口动画，显示图片 (png/jpg/svg/gif)。

### 6. 高度扩展性

* 可以扩展自己的控件。
* 可以扩展自己的动画。
* 可以实现自己的主循环。
* 可以扩展自己的软键盘。
* 可以扩展自己的图片加载器。
* 可以扩展自己的字体加载器。
* 可以扩展自己的输入法引擎。
* 可以扩展自己的控件布局算法。
* 可以扩展自己的对话框高亮策略。
* 可以实现自己的 LCD 接口。
* 可以扩展自己的矢量引擎 （如使用 skia/cairo)。
* 所有扩展组件和内置组件具有相同的待遇。

### 7. 多种开发语言

[AWTK](README.md) 本身是用 C 语言开发的，可以通过 IDL 生成各种脚本语言的绑定。生成的绑定代码不是简单的把 C 语言的 API 映射到脚本语言，而是生成脚本语言原生代码风格的 API。目前支持以下语言 （以后根据需要增加）：

* C
* Go
* C++
* lua
* java
* python
* Javascript on jerryscript
* Javascript on nodejs
* Javascript on quickjs

### 8. 国际化

* 支持 Unicode。
* 支持输入法。
* 支持字符串翻译 （实时生效）。
* 支持图片翻译 （实时生效）。
* 文字双向排版。

### 9. 为嵌入式软件定制的 MVVM 框架，彻底分离用户界面和业务逻辑。
* 性能高。
* 内存开销小。
* 隔离更彻底。
* 可移植到其它 GUI。
* 代码小 (~5000 行）。
* 无需学习 AWTK 控件本身的 API。
* 支持多种编程语言（目前支持 C/JS)。

> 详情请参考：https://github.com/zlgopen/awtk-mvvm

### 10. 开放源码，免费商用 (LGPL)。

> 欢迎对照 [《GUI 引擎评价指标》](https://github.com/zlgopen/gui-lib-evaluation) 进行评测。

## 四、1.7 版本更新
-------------------

### 1. 细节完善
  * 优化 layout。
  * 修复完善圆角矩形。
  * 窗口完全进入后台时不再绘制。
  * 修复设置后台窗口焦点导致的问题。
  * 修改资源生成脚本不预先加载字体。
  * 修复 asset\_loader\_zip 在 Windows 上的问题。
  * 完善 pthread 实现的 thread 等待时不能及时响应。

> 大量细节完善请参考 [最新动态](https://github.com/zlgopen/awtk/blob/master/docs/changes.md)

### 2. 新增文档
 * [在 windows 下使用 gcc 编译 AWTK](https://github.com/zlgopen/awtk/tree/master/docs/mingw.md)
 * [优化 freetype 解析矢量字体的显示效果](https://github.com/zlgopen/awtk/tree/master/docs/optimation_freetype_gray_font.md)
 * [优化 stb\_truetype 在 mono 模式下的字体显示效果](https://github.com/zlgopen/awtk/tree/master/docs/optimation_stb_truetype_mono_font.md)
 * [极简键盘 (3 键键盘、5 键键盘和旋转按钮）](https://github.com/zlgopen/awtk/tree/master/docs/how_to_support_5keys_3keys.md)

### 3. 新增重要特性
  * 支持多脏矩形。
  * 重新设计 demoui。
  * edit 显示支持 bidi。
  * fscript 支持函数定义。
  * 增加多个 fscript 扩展模块。
  * gauge\_pointer 支持精确脏矩形。
  * 增加网络接口以及 Linux/Windows 实现。
  * style 支持渐变色（目前支持垂直方向）。
  * 键盘支持跟随编辑（指定 floating=true 即可）
  * 增加输入类型 INPUT\_ASCII，用于输入纯英文字符。
  * 增加对极简键盘 (3 键键盘、5 键键盘和旋转按钮）的支持。
  * style 支持 feedback/focusable。
  * window\_open 支持打开本地 xml 文件。
  * image value 支持支持水平和垂直对齐。
  * 修改 canvas\_draw\_image 改成用 rectf 类型。
  * 完善图片解码器，优先使用用户自定义的解码器。
  * stb 支持外部硬件加速 yuv 转 rgb 的宏和函数。
  * strgen 工具生成国际化字符串时保留文本两端的空格。
  * switch 支持 click 事件，支持 space/return 触发。
  * 如果 edit 的 keyboard 属性为空字符串，则不弹出软键盘
  * 增加宏 NATIVE\_WINDOW\_NOT\_RESIZABLE，用于禁止用户调整 desktop 应用程序的窗口大小。
  * 窗口增加属性 strongly\_focus。点击非 focusable 控件时，用于控制是否让当前焦点控件失去焦点。
  * 增加宏 WITHOUT\_WINDOW\_ANIMATOR\_CACHE 用于控制窗口动画是否使用缓存。如果内存紧缺，又希望启用窗口动画，可以打开本宏。

### 4. 新增重要 API
  * general\_factory
  * wstr\_set\_with\_len
  * tk\_utf8\_from\_utf16\_ex 
  * wstr\_set\_utf8\_with\_len
  * data\_writer\_flush。
  * fscript\_syntax\_check
  * event\_clone
  * widget\_dispatch\_async
  * widget\_auto\_scale\_children。
  * widget\_get\_focused\_widget。
  * gif\_image\_play
  * gif\_image\_stop
  * gif\_image\_pause
  * ui\_feedback\_deinit
  * path\_extname\_is
  * window\_manager\_set\_max\_fps
  * vgcanvas\_set\_fill\_gradient
  * vgcanvas\_set\_fill\_gradient。
  * image\_manager\_set\_max\_mem\_size\_of\_cached\_images

### 5. 新增控件

  * [HTML View](https://github.com/zlgopen/awtk-widget-html-view)

   ![](https://github.com/zlgopen/awtk-widget-html-view/raw/main/docs/images/image.png)
  
  * [rive 动画](https://github.com/zlgopen/awtk-widget-rive)

  ![](https://github.com/zlgopen/awtk-widget-rive/blob/main/docs/images/ui.png)
  
  * [rlottie 动画](https://github.com/zlgopen/awtk-widget-rlottie)

  ![](https://github.com/zlgopen/awtk-widget-rlottie/raw/main/docs/images/ui.png)
  
  * [Web View](https://github.com/zlgopen/awtk-widget-web-view)

  * 增加 vpage 扩展控件（配合 pages 控件使用，可以自动加载/卸载页面，并提供入场/出场动画）

### 6. 新增语言支持

  * [Go 语言](https://github.com/zlgopen/awtk-go)

### 7. 新增相关项目

  * [用 ReactJS 开发 AWTK](https://github.com/zlgopen/react-awtk)

  * [AWTK 回归测试](https://github.com/zlgopen/awtk-regression-test)

> 欢迎广大开发者一起参与开发：[生态共建计划](../awtk_ecology.md)。
