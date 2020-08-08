# AWTK = Toolkit AnyWhere

![zlg](docs/images/zlg_adv.jpeg)

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

![AWTK Designer](docs/images/designer.png)

#### 运行效果截图：

![Chart-Demo](docs/images/chart_main.png)

![MusicPlayer-Demo](docs/images/musicplayer_main.png)

![Watch](docs/images/smartwatch_main.png)

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
* UI 描述文件和主题文件使用高效的二进制格式，解析在瞬间完成。
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
* 支持窗口动画
* 支持控件动画
* 支持高清屏。
* 支持界面描述文件。
* 支持主题描述文件。
* 主题切换实时生效。
* 支持控件布局策略。
* 支持对话框高亮策略。
* 丰富的辅助工具。
* 支持从低端的 Cortex M3 到各种高端 CPU。
* 支持无文件系统和自定义的文件系统。
* 支持裸系统和 RTOS。

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

## 四、模拟运行：

### 下载代码

* 用 git 下载

> [Windows 下的 git](https://git-scm.com/download/win)

**下载代码**

```
git clone https://github.com/zlgopen/awtk.git
```

> Windows 下可以用带 GUI 的 git 下载代码。

**更新代码（在 awtk 目录下运行）**

```
git pull
```

> 用 git 下载代码的好处是以后更新代码非常快。

* 直接下载 zip 文件

> 下载 [ zip 文件](https://github.com/zlgopen/awtk/archive/master.zip)。解压之后**需要把文件夹改名为 awtk**。

### Mac

如果没有安装 scons 和 sdl2，请在终端运行下面的命令 （假定已安装 brew)：

```
brew install scons sdl2
```

编译运行 （在终端下，进入 awtk 所在的目录，并运行下列命令）：

```
scons
./bin/demoui
```

### Ubuntu （版本 >=16)

如果没有安装 scons 和依赖的软件包，请在终端运行下面的命令：

```
sudo apt-get install scons libsndio-dev libgtk-3-dev libglu1-mesa libglu1-mesa-dev libgl1-mesa-glx libgl1-mesa-dev libasound2-dev git vim clang-format
```

编译运行 （在终端下，进入 awtk 所在的目录，并运行下列命令）：

```
scons
./bin/demoui
```

> 在 Ubuntu 14 上无法启动，请参考 [FAQ](docs/faq.md) 修改。

### Windows

> 请先安装 python(64 位系统要安装 64 位版本的 python)，scons 和 Visual Studio C++（版本 >=2017)

> 如果没有安装 pywin32，请 pip 安装：

```
pip install pywin32
```

编译运行 （在命令行模式下，进入 awtk 所在的目录，并运行下列命令）：

```
scons
bin\demoui
```

> 如果没有安装 PIL，请用 pip 安装。

```
pip install Pillow
```

## 五、在线演示

* [基本控件在线演示](http://zlgawtk.bceapp.com/awtk/demos/demoui/index.html)

* [图表演示](http://zlgawtk.bceapp.com/awtk/demos/Chart-Demo/index.html)

* [音乐播放器演示](http://zlgawtk.bceapp.com/awtk/demos/MusicPlayer-Demo/index.html)

* [智能手表演示](http://zlgawtk.bceapp.com/awtk/demos/SmartWatch-Demo/index.html)

## 六、各个平台的移植

* [详见 AWTK 生态共建计划](docs/awtk_ecology.md)

## 七、各语言的绑定

* [AWTK 绑定代码生成器](https://github.com/zlgopen/awtk-binding)

* [LUA 绑定](https://github.com/zlgopen/awtk-lua)

* [C++ 绑定](https://github.com/zlgopen/awtk-cpp)

* [JAVA 绑定](https://github.com/zlgopen/awtk-java)

* [Python 绑定](https://github.com/zlgopen/awtk-python)

* [MiniJVM 绑定](https://github.com/zlgopen/awtk-minijvm)

* [JS 绑定之 QuickJS](https://github.com/zlgopen/awtk-quickjs)

* [JS 绑定之 NodeJS](https://github.com/zlgopen/awtk-nodejs)

* [JS 绑定之 JerryScript](https://github.com/zlgopen/awtk-jerryscript)

## 八、相关工程

* [模板工程](https://github.com/zlgopen/awtk-hello)

* [C 示例代码](https://github.com/zlgopen/awtk-c-demos)

* [精彩示例](https://github.com/zlgopen/awtk-examples)

* [AWTK-MVVM](https://github.com/zlgopen/awtk-mvvm)

* [MVC/MVVM 演示示例](https://github.com/zlgopen/awtk-patterns)

* [扩展控件实例：控件代码生成器](https://github.com/zlgopen/awtk-widget-generator)

* [扩展控件实例：二维码](https://github.com/zlgopen/awtk-widget-qr)

* [扩展控件实例：TinyGL](https://github.com/zlgopen/awtk-widget-glview)

* [扩展控件实例：number-label](https://github.com/zlgopen/awtk-widget-number-label)

* [扩展控件实例：代码编辑器](https://github.com/zlgopen/awtk-widget-code-edit)

* [扩展控件实例：日期选择器](https://github.com/zlgopen/awtk-widget-date-picker)

* [扩展控件实例：表格视图](https://github.com/zlgopen/awtk-widget-table-view)

* [自定义字体加载器：加载部分字体](https://github.com/zlgopen/awtk-custom-font-loader)

第三方提供：

* [react-awtk](https://github.com/wenyufei2018/react-awtk)

* [扩展控件实例：二维码](https://github.com/CUITzhaoqi/awtk-qr)

* [扩展控件实例：圆形滑块](https://github.com/longyuwlz/awtk_slider_circle)


## 九、文档

* [开发文档](docs/README.md)

* [最新动态](docs/changes.md)

* [任务完成情况](TODO.md)

* [入门指南与手册](https://github.com/zlgopen/awtk-docs)

## 十、生态共建计划

* [生态共建计划](docs/awtk_ecology.md)
* 深度合作伙伴

[![](docs/images/logo_nxp.svg)](https://www.nxp.com)　[![](docs/images/logo_anyka.jpg)](http://www.anyka.com)　[![](docs/images/logo_ingenic.jpg)](http://www.ingenic.com.cn)　[![](docs/images/logo_mindmotion.png)](http://www.mindmotion.com.cn)
