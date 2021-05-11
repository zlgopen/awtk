
## ZLG AWTK 1.5 Release Notes

## 一、介绍

[AWTK](README.md) 全称 Toolkit AnyWhere，是 [ZLG](http://www.zlg.cn/) 开发的开源 GUI 引擎，旨在为嵌入式系统、WEB、各种小程序、手机和 PC 打造的通用 GUI 引擎，为用户提供一个功能强大、高效可靠、简单易用、可轻松做出炫酷效果的 GUI 引擎。

> 欢迎广大开发者一起参与开发：[生态共建计划](docs/awtk_ecology.md)。

#### [AWTK](README.md) 寓意有两个方面：

* Toolkit AnyWhere。 
* ZLG 物联网操作系统 AWorksOS 内置 GUI。

#### [AWTK](README.md) 源码仓库：

* 主源码仓库：[https://github.com/zlgopen/awtk](https://github.com/zlgopen/awtk)
* 镜像源码仓库：[https://gitee.com/zlgopen/awtk](https://gitee.com/zlgopen/awtk)
* 稳定版整合包：https://pan.baidu.com/s/1_oRgj67M-I4kivk-YzwFWA   提取码：1cmi

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

## 四、1.5 版本更新
-------------------

### 1. 细节完善
  * 增加离线 canvas。
  * 修改切换语言奔溃的问题。  
  * 完善 android 资源加载。
  * 修改输入法中内存临时泄漏。
  * button 支持设置长按时间。
  * edit/mledit 支持翻译提示信息。
  * 支持 shift+tab 向后切换焦点。 
  * edit/mledit 支持 cancelable 属性。
  * 软键盘支持“clear"/"cancel"两个功能按钮。
  * 完善表达式，增加 iformat/fformat 函数。
  * Linux/MacOS/Windows 支持编译为动态库。
  * 候选字控件 (candidate) 支持设置 margin style。
  * 完善 keyboard 增加 back/back_to_home 按钮的支持。
  * edit/mledit/slider 支持 WIDGET_PROP_INPUTING 属性，用于辅助实现 MVVM。
  
> 大量细节完善去请参考： https://github.com/zlgopen/awtk/blob/master/docs/changes.md

### 2. 新增文档

  * 新增文档《如何使用对话框》  
  * 新增文档《如何实现弹出菜单》  
  * 新增文档《如何绘制旋转文字》  
  * 新增文档《为 512K Flash 优化空间》
  * 新增文档《如何用 edit 控件编辑日期》
  * 新增文档《如何用 edit 控件编辑时间》
  * 新增文档《如何用 edit 控件编辑 IP 地址》
  * 新增文档《如何存取应用程序的配置信息》
  * 新增文档《项目描述文件 project.json 介绍》  
  * 新增文档《如何使用软键盘上的 action 按钮》
  * 新增文档《如何用 valgrind 查找内存相关问题》
  * 新增文档《如何去掉不需要的控件，以节省 flash 空间》
  * 新增文档《如何定制 combo_box 控件中按钮的风格和大小》

### 3. 新增特性
  * 新增触屏版 T9 输入引擎。
  * 新增按键版 T9 输入引擎。
  * 支持 rgb565 格式的图片
  * 新增 app_conf 实现配置文件的访问。
  * edit 支持时间、日期和 IPV4 等格式的编辑。
  * 支持从 SD 卡加载资源，做了一些小的改动
  * 增加事件录制与重放功能，辅助手动测试和压力测试。
  * 新增自动化测试引擎，兼容Appium，实现全自动化测试。
  * 重新设计内存管理器架构，新增加锁、调试和防内存碎片功能。  
  * 基本控件的文本支持双向排版 (Unicode Bidirectional Algorithm)


### 4. 新增控件

 * [二维码](https://github.com/zlgopen/awtk-widget-qr)

 ![](https://github.com/zlgopen/awtk-widget-qr/raw/master/docs/images/ui.jpg)

 * [number-label](https://github.com/zlgopen/awtk-widget-number-label)

 ![](https://github.com/zlgopen/awtk-widget-number-label/raw/master/docs/images/ui.jpg)
 
 * [代码编辑器](https://github.com/zlgopen/awtk-widget-code-edit)

 ![](https://github.com/zlgopen/awtk-widget-code-edit/raw/master/docs/images/ui.jpg)

 * [日期选择器](https://github.com/zlgopen/awtk-widget-date-picker)
 
 ![](https://github.com/zlgopen/awtk-widget-date-picker/raw/master/docs/images/ui.jpg)

 * [表格视图](https://github.com/zlgopen/awtk-widget-table-view)

 ![](https://github.com/zlgopen/awtk-widget-table-view/raw/master/docs/images/ui.png)

 * [仿微信列表项](https://github.com/zlgopen/awtk-widget-slidable-row)
 
 ![](https://github.com/zlgopen/awtk-widget-slidable-row/raw/master/docs/images/ui1.png)


### 5. 新增重要 API

  * 新增类 mutex\_nest 
  * 新增类  object\_locker
  * 新增函数 emitter\_exist
  * 新增函数 str\_append\_more。
  * 新增函数 tk\_wstr\_dup\_utf8
  * 新增函数 fs\_os\_file\_tell
  * 新增函数 fs\_os\_file\_size
  * 新增函数 fs\_os\_file\_stat
  * 新增函数 fs\_os\_file\_sync
  * 新增函数 tk_mutex_try_lock
  * 新增函数 widget\_get\_text\_utf8
  * 新增函数 canvas\_get\_font\_height
  * 新增函数 date\_time\_get\_month\_name 
  * 新增函数 date\_time\_get\_wday\_name

### 7. 新增平台
  * [mvvm 在 stm32](https://github.com/zlgopen/awtk-stm32h743iitx-mvvms) 上的移植。

### 8. 新增相关项目
* [UI 自动测试引擎](https://github.com/zlgopen/awtk-ui-automation)
* [Restful WEB 框架](https://github.com/zlgopen/awtk-restful-httpd)
* [自定义控件生成器](https://github.com/zlgopen/awtk-widget-generator)
* [自定义字体加载器](https://github.com/zlgopen/awtk-custom-font-loader)
* [演示窗口之间的通讯方式](https://github.com/zlgopen/awtk-inter-win-communication)

> 欢迎广大开发者一起参与开发：[生态共建计划](../awtk_ecology.md)。
