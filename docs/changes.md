# 最新动态

* 2018/10/22
  * 完善控件动画和demo。
  * 支持鼠标指针。
  * 引入window\_base，重构window相关的组件。
  * 更新文档。

* 2018/10/21
  * 出于一致性的考虑，将widget的属性style\_name重命名为style，将widget的属性style重命名为style\_data。
  * 增加widget\_animator\_factory，从而支持在xml指定控件的动画。
  * 更新文档。

* 2018/10/20
  * pages控件切换页面时触发EVT\_VALUE\_CHANGED事件。
  * slide\_view控件的indicator颜色由主题确定。
  * slide\_view控件切换页面时触发EVT\_VALUE\_CHANGED事件。
  * 窗口动画支持指定时长，请参考[窗口动画](window_animator.md)文档。
  * 更新文档。

* 2018/10/19
  * 更新文档。
  * 修改文字渲染的bug。
  * 重新整理font\_loader相关代码，无需在初始化时就加载全部字体。

* 2018/10/18
  * 修改控件状态问题（在特殊情况下，mouse重新进入控件，状态没有改变）。
  * 修改MacOS升级后，打开窗口瞬间出现黑屏的问题。
  * 修改SDL+FB模式下刷新的问题。
  * 修改Windows下Mutex实现的BUG。
  * 改进ListView滑动效果。
  * 增加文档。

* 2018/10/17
  * 完善和重构bitmap、图片加载和图片数据生成等代码。
  * calibration控件支持设置点击事件处理的回调函数。

* 2018/10/16
  * 完善图片控件。绘制带缩放和旋转的图片时，不用WTIH\_VGCANVS宏，而根据get\_vgcanvas是否为空来判断。
  * 同步同事改进的SDL，Windows10/Linux支持高清屏。
  * 更新文档。

* 2018/10/15
  * 用agg实现nanovg的渲染后端。
  * 更新文档、编译脚本并测试。

* 2018/10/14
  * 用agg实现nanovg的渲染后端。

* 2018/10/13
  * 重新整理第三方库。
  * 集成修改过的SDL(Linux/Winodws支持高清屏)。

* 2018/10/12
  * 增加大字体demo。
  * 修改文本垂直居中的问题。

* 2018/10/11
  * 完善Qt UI文件转换工具。
  * 更新文档。
  * 优化。

* 2018/10/9-10
  * 重构lcd\_mem和blend，用代码产生器生成各种格式的代码。
  * 增加测试代码。
  * 更新相关工程。

* 2018/10/8
  * 修改ui\_loader\_xml，特殊属性优先设置。

* 2018/10/7
  * 更新文档。

* 2018/10/4
  * 完善内存管理，并增加内存泄露跟踪功能。
  * 增加内存需求评估文档。
  * 增加电阻屏校准窗口。

* 2018/10/2-3
  * 移植到sylixos

* 2018/10/1
  * 重构
  * 修改编译警告
  * 更新文档

* 2018/09/30
  * lcd\_reg支持RGB565的图片。

* 2018/09/28
  * 实现image\_animation控件并增加demo。

* 2018/09/27
  * 实现开关控件并增加demo。

* 2018/09/26
  * 完善text\_selector控件并增加demo。
  * 完善vgcanvas，增加paint接口。

* 2018/09/25
  * 完成text\_selector控件基本功能。

* 2018/09/24
  * 更新文档。

* 2018/09/23
  * 更新各个项目的工程。

* 2018/09/21-22
  * 移植awtk到stm32f767开发板。

* 2018/09/20
  * 移植nanovg-agge到stm32和aworks。

* 2018/09/12-19
  * 基于agge用纯软件实现nanovg的渲染接口，去掉对cairo/agg/piccaso的依赖，以最少的代码实现vgcanvas的接口。
  * 完善vgcanvas-nanovg支持基于OpenGL和纯软件两种实现。

* 2018/09/11
  * awtk-linux-fb在IM287A上测试通过。

* 2018/09/10
  * awtk-linux-fb支持linux fb设备.

* 2018/09/09
  * awtk-linux-fb支持linux tslib设备.

* 2018/09/08
  * awtk-linux-fb支持linux input设备.

* 2018/09/07
  * 录了一些演示视频，给不方便编译的朋友了解AWTK。

* 2018/09/06
  * 移植到linux framebuffer

* 2018/09/05
  * 增加guage控件。
  * 完善time\_clock控件。

* 2018/09/04
  * 修改cairo在arm平台上图片显示的问题(stack size需要32K)

* 2018/09/03
  * 移植cairo到arm平台(基本绘图功能正常，图片和字体还不行)。

* 2018/09/02
  * 基于cairo实现vgcanvas.

* 2018/09/01
  * 移植cairo。

* 2018/08/31
  * 移植pixman。

* 2018/08/30
  * 更新stm32相关工程。

* 2018/08/29
  * 完善str类，增加str\_insert/str\_remove等函数。
  * 完善path类，增加path\_replace\_basename等函数。
  * ui描述文件支持include指令，方便包含公共组件(请参考：demos/assets/raw/ui/tab\_top.xml)。

* 2018/08/28
  * 完善demos，运行双击直接运行demos。
  * 完善preview\_ui，支持在命令行指定宽度和高度。
  * 增加文档。

* 2018/08/27
  * 增加skia的编译脚本。

* 2018/08/26
  * 增加path.c/path.h提供基本的目录操作函数。

* 2018/08/25
  * 增加文件系统的接口和posix的实现。

* 2018/08/24
  * 整理awtk的目录结构，把部分控件移到ext\_widgets目录下。
  * 修改stm32编译的问题。

* 2018/08/23
  * 完善Color Picker控件。
  * 增加Color Picker控件Demos。
  * 支持mutable的图片。

* 2018/08/22
  * 增加Color Tile控件。

* 2018/08/21
  * 增加Color Picker控件。

* 2018/08/20
  * 更新文档。

* 2018/08/19
  * 增加模板工程: https://github.com/zlgopen/awtk-hello。

* 2018/08/18
  * 更新stm32相关工程。

* 2018/08/17
  * 修改AWTK开发桌面软件的一些问题。

* 2018/08/16
  * 新增awtk-c-demos仓库，存放c语言的示例。

* 2018/08/15
  * javascript绑定原理文档。

* 2018/08/14
  * 增加javascript demos。

* 2018/08/13
  * 增加javascript demos。
  * 修改lua/javascript回调函数泄露的问题。

* 2018/08/11
  * awtk.ts 代码产生器。

* 2018/08/10
  * 完善jerryscript绑定代码生成器。

* 2018/08/09
  * jerryscript绑定代码生成器。

* 2018/08/08
  * 为每类控件增加Cast函数，方便脚本绑定。

* 2018/08/07
  * 完善API docs
  * 完善awtk-lua。

* 2018/08/06
  * 完善API docs
  * lua绑定放入独立的项目awtk-lua。

* 2018/08/05
  * 重写IDL生成工具。

* 2018/08/04
  * 用valgrind运行单元测试，并修改发现的BUG。

* 2018/08/03
  * 修改linux下编译的问题。

* 2018/08/02
  * 完善api docs，增加annotation。

* 2018/08/01
  * 重构widget。

* 2018/07/31
  * 完善rich\_text控件并增加DEMO。

* 2018/07/29
  * 完善rich\_text控件。

* 2018/07/28
  * 增加rich\_text控件(未完成)。

* 2018/07/27
  * 增加unicode linebreak。
  * 增加rich\_text\_parser。

* 2018/07/26
  * 增加combo\_box/combo\_box\_item控件和demo。

* 2018/07/25
  * 增加popup窗口。

* 2018/07/24
    * 完善tab\_control及demo。

* 2018/07/23
    * 增加tab\_control的demo。

* 2018/07/22
    * 实现tab\_button/tab\_button\_group/tab\_control。

* 2018/07/20
    * 增加EVT\_POINTER\_DOWN\_ABORT，修改ScrollView中ListItem/CheckButton/Button在滚动时被误点的问题。

* 2018/07/19
    * 主题支持设置图标的位置，增加一些demo。

* 2018/07/18
    * image 支持可选中和可点击属性。

* 2018/07/17
    * widget支持自定义属性。
    * 出于易用性考虑，style\_name改为字符串格式。

* 2018/07/15
    * 实现widget\_to\_xml。

* 2018/07/14
    * 增加ui\_xml\_writer
    * 增加ui\_binary\_writer
    * 实现widget\_clone/widget\_equal。

* 2018/07/13
    * 在widget vtable中增加create/properies方便实现clone。
    * 重构widget

* 2018/07/12
    * 重新整理grab/ungrab的流程。
    * 修改关闭窗口的BUG。

* 2018/07/11
    * LCD 增加flush接口。
    * 完善图片绘制接口，增加相应demo。
    * 在Windows 7上，如果显卡太旧不支持OpenGL3时，触发assert。

* 2018/07/10
    * 框架支持滚轮事件。
    * ListView和SpinBox支持滚轮事件。

* 2018/07/09
    * 按钮支持repeat触发click事件，键盘和滚动条支持repeat触发。
    * keyboard点击return时提交之前输入的键值。
    * 滚动条支持禁止滚动效果。
    * ListView支持自动隐藏滚动条。

* 2018/07/08
    * 更新文档，修改三个framebuffer的旋转时的BUG。

* 2018/07/07
    * 支持online framebuffer, offline framebuffer和next framebuffer三个framebuffer的工作方式。

* 2018/07/06
    * 支持双framebuffer交换。

* 2018/07/05
    * 增加ListView和ListView Demo。

* 2018/07/04
    * 增加ScrollView。

* 2018/07/03
    * 更新awtk on aworks rt1052，集成硬件加速。

* 2018/07/02
    * 实现scrollbar控件。

* 2018/07/01
    * 实现drager控件。

* 2018/06/30
    * 实现spinbox控件。

* 2018/06/29
    * 改进edit。
    * 增加edit的demo。
    * theme支持border的类型。

* 2018/06/28
    * 修改脏矩形等BUG。
    * 修改ubuntu 18上编译的问题。
    * 重构widget。

* 2018/06/27
    * 实现输入法的联想字功能。

* 2018/06/26
    * 更新aworks工程并测试。
    * 增加小工具用于生成输入法联想词库。

* 2018/06/25
    * 完善候选字控件。
    * 编写输入法文档。

* 2018/06/24
    * 集成google拼音输入法(输入法字体和完整的字体，需要4M的flash空间)。

* 2018/06/23
    * 引入输入法引擎接口。
    * 增加候选字控件。

* 2018/06/22
    * 完善软键盘。
    * 修改一些BUG。

* 2018/06/21
    * 输入框被软键盘覆盖时上推窗口。
    * 接入原生输入法(目前没有中文字体，编辑器中显示为空白)。

* 2018/06/20
    * 更新lua绑定及示例。
    * 优化软键盘并增加动画。
    * 更新stm32f103/f429的工程。

* 2018/06/19
    * 实现输入法框架。
    * 实现软键盘基本功能。

* 2018/06/18
    * 实现软键盘部分功能。
    * 完善现有代码。

* 2018/06/17
    * 增强layout的功能。
    * 修改XML UI/String文件中不能处理特殊字符的问题。

* 2018/06/16
    * demoui增加预加载资源的窗口。
    * 增加widget\_factory，UI文件支持自定义的控件。

* 2018/06/15
    * 每个窗口支持独立的主题文件。
    * 修改F429上图片显示不正常的BUG。

* 2018/06/14
    * LCD FrameBuffer模式支持屏幕90度旋转。
    * 统一图片操作的硬件和软件实现方式。

* 2018/06/13
    * 集成RT1052 PXP硬件加速到Aworks。

* 2018/06/11
    * 增加显示FPS的开关。

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
