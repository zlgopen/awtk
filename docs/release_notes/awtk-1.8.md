
## ZLG AWTK 1.8 Release Notes

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
* 可以扩展自己的矢量引擎 （如使用 skia/cairo）。
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
* 代码小（~5000 行）。
* 无需学习 AWTK 控件本身的 API。
* 支持多种编程语言（目前支持 C/JS）。

> 详情请参考：https://github.com/zlgopen/awtk-mvvm

### 10. 开放源码，免费商用 (LGPL)。

> 欢迎对照 [《GUI 引擎评价指标》](https://github.com/zlgopen/gui-lib-evaluation) 进行评测。

## 四、1.8 版本更新
-------------------

### 1. 细节完善

> 大量细节完善请参考 [最新动态](https://github.com/zlgopen/awtk/blob/master/docs/changes.md)

### 2. 新增文档
  * [拖入文件事件](https://gitee.com/zlgopen/awtk/blob/master/docs/how_to_handle_drop_file_event.md)
  * [如何使用 packed image](https://gitee.com/zlgopen/awtk/blob/master/docs/how_to_use_packed_image.md)
  * [如何自定义资源加载方式](https://gitee.com/zlgopen/awtk/blob/master/docs/how_to_set_custom_load_asset.md)
  * [如何使用 CMake 构建 AWTK 应用](https://gitee.com/zlgopen/awtk/blob/master/docs/how_to_use_cmake_to_build_awtk_app.md)
  * [如何将资源编译到应用程序并使用它们](https://gitee.com/zlgopen/awtk/blob/master/docs/how_to_compile_res_into_app_and_use_them.md)
  * [关于自定义控件的 offset 的使用注意事项](https://gitee.com/zlgopen/awtk/blob/master/docs/how_to_use_offset_in_custom_widget.md)

### 3. 新增重要特性
  * 使用 svgtiny 解析 svg，增加渐变等功能。
  * 完善 log 系统，支持调试器拦截日志。
  * fscript 支持注册常量。
  * combo\_box\_ex 支持分组显示。 
  * combo\_box\_ex 支持自动调整弹出窗口的宽度。
  * 文件浏览器增加关闭按钮。
  * fscript 新增设置屏保时间函数。
  * 增加 api doc 错误检查工具。
  * 添加注册 easing 接口。
  * 增离线画布缓存功能。
  * 扩展 debugger 接口支持 VSCode DAP 协议。
  * 增加图片绘制类型：IMAGE\_DRAW\_FILL。
  * 增加 cmd_args 用于解析命令行参数。
  * 增加控制台应用类型。
  * 增加 ROMFS。
  * agge 模式增加 argb 和 abgr 的颜色支持。
  * 增加 UI 自动测试框架。
  * 支持加载 8 位深度的图片。
  * 完善内存管理，支持直接使用 OS 提供的函数。
  * toast 按任意键退出。
  * progress circle 支持扇形绘制。
  * awtk-web 支持 fscript 扩展。
  * icon\_at 增加 left\_top/left\_bottom/right\_top/right\_bottom 四种位置。

### 4. 新增重要 API
  * value\_min
  * value\_max
  * value\_dup\_wstr
  * str\_create
  * str\_destroy
  * str\_equal
  * str\_attach
  * str\_append\_uint32
  * str\_attach\_with\_size
  * str\_shrink
  * wstr\_shrink
  * wstr\_attach
  * wstr\_create
  * wstr\_destroy
  * wstr\_append\_int
  * wstr\_append\_more
  * file\_read\_as\_unix\_text
  * tk\_sha256\_file
  * ubjson\_writer\_write\_kv\_array\_begin
  * conf\_node\_get\_child\_value
  * emitter\_forward\_retarget
  * event\_source\_manager\_set\_min\_sleep\_time
  * slist\_tail
  * slist\_head
  * ret\_code\_from\_name
  * ret\_code\_to\_name
  * combo\_box\_remove\_option\_by\_index
  * conf\_ini\_load\_from\_buff
  * conf\_ini\_save\_to\_buff
  * conf\_json\_load\_from\_buff
  * conf\_json\_save\_to\_buff
  * conf\_xml\_load\_from\_buff
  * conf\_xml\_save\_to\_buff
  * conf\_ubjson\_load\_from\_buff/conf\_ubjson\_save\_to\_buff
  * bits\_stream\_set
  * bits\_stream\_get
  * hash\_table\_size
  * url\_get\_param\_int32
  * url\_get\_param\_bool
  * stream\_factory
  * conf\_node\_get\_child\_value\_int32
  * conf\_node\_get\_child\_value\_bool
  * conf\_node\_get\_child\_value\_str
  * wbuffer\_create
  * wbuffer\_destroy
  * rbuffer\_create
  * rbuffer\_destroy
  * data\_reader\_http
  * path\_expand\_vars
  * tk\_islower
  * tk\_isupper
  * tokenizer\_next\_str\_until
  * conf\_doc\_foreach
  * tk\_tcp\_connect\_ex
  * goto\_error\_if\_fail\_ex

### 5. 新增控件
  * [异形进度条](https://github.com/zlgopen/awtk-widget-progress-polygon)
   ![](https://gitee.com/zlgopen/awtk-widget-progress-polygon/raw/main/docs/images/ui.gif)

  * [滑动圆环](https://github.com/zlgopen/awtk-widget-slider-circle) 
   ![](https://gitee.com/zlgopen/awtk-widget-slider-circle/raw/master/docs/images/ui.png)

  * [流体效果](https://github.com/zlgopen/awtk-widget-liquid-flow)
   ![](https://gitee.com/zlgopen/awtk-widget-liquid-flow/raw/master/docs/images/ui.gif)

### 6. 新增相关项目
  * [awtk-ogre](https://gitee.com/zlgopen/awtk-ogre)
  * [AWTK 开源智能串口屏方案](https://gitee.com/zlgopen/awtk-hmi)

> 欢迎广大开发者一起参与开发：[生态共建计划](../awtk_ecology.md)。
