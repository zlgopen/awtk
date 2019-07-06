# 最新动态
* 2019/07/06
  * 增加hscroll\_label控件。
  * 修改mledit获取max\_lines的BUG。

* 2019/07/05
  * 修改一些编译警告。
  * 修改MacOS上双击启动时找不到资源的问题。
  * 修改scrollbar隐藏导致listview relayout的问题。

* 2019/07/04
  * 修改多行编辑器鼠标选择文本时的坐标偏差。
  * 增加TK\_GLYPH\_CACHE\_NR宏，允许开发者自行定义。
  * 更新文档(感谢俊杰提供补丁)。
  * 修改软件键盘删除键无效的问题。
  * set visible后设置relayout标志。

* 2019/07/03
  * 更新demoui
  * mledit增加max\_lines属性。
  * mledit支持pageup/pagedown
  * 动画停止时reversed设置为FALSE。
  * 修改svg图片fill=""时的问题。
  * widget的pointer/key事件处理增加can\_not\_destroy保护。
  * 增加行号控件，配合mledit控件使用。

* 2019/07/02
  * 修改OpenGL模式下SVG无法绘制的问题(感谢智明提供补丁)。
  * mledit 支持垂直滚动条。

* 2019/07/01
  * 增加self\_layouter\_menu文档和示例。

* 2019/06/30
  * 增加self\_layouter\_menu方便实现弹出菜单。

* 2019/06/29
  * 子控件缺省布局方式增加align_h的参数，

* 2019q/06/28
  * 完善edit。
  * 完善combobox

* 2019/06/26
  * 新增内存耗尽处理。
  * 修改 slide menu 的 BUG（感谢朝泽提供补丁）

* 2019/06/25
  * 完善 slider。dragger 以实际 icon 为准。
  * 让 bitmap 引用 image\_manager 以区分 bitmap 是由哪个 image\_manager 加载的。
  * 修改 cairo global alpha 的 BUG，目前 draw_image/fill/stroke 支持 global alpha, gradient 暂时不支持。

* 2019/06/24
  * 用 stb\_text\_edit 重新实现 edit 控件，支持取消、重做、全选、拷贝、剪切、粘贴和键盘选择等功能。
  * 增加多行编辑控件 (mledit)。

* 2019/06/22
  * 修改 canvas\_set\_font/vgcanvas\_set\_font 脚本绑定的问题（感谢尧燊提供补丁）。

* 2019/06/21
  * 修改 slide view/fps 脏矩形计算的 BUG（感谢陈潭提供补丁）。
  * 修改 idle 文档的问题（感谢俊杰提供补丁）。

* 2019/06/18
  * 修改 window\_manager\_update\_cursor 的 BUG（感谢智明提供补丁）
  * 修改 main\_loop\_post\_pointer\_event 的 BUG（感谢朝泽提供补丁）

* 2019/06/17
  * SDL FB 模式启用脏矩形。
  * 增加 lcd\_mem\_special 用于实现一些特殊的 FB 格式。

* 2019/06/15
  * 增加 timer_modify
  * 支持动态修改屏保时间。

* 2019/06/14
  * 增加 app_root 自动检测。

* 2019/06/13
  * 修改 lcd\_sdl2\_resize 的 bug（感谢尧燊提供补丁）。
  * image/svg/gif 支持从文件系统中加载图片。

* 2019/06/12
  * 修改全局字体缩放的问题。
  * 修改 scroll\_view\_on\_layout\_children 在特殊情况的 BUG。

* 2019/06/10
  * 修改 switch 控件，在启用脏矩形而且图片高度大于控件高度时，渲染残留的问题。

* 2019/06/8
  * 修改 tab\_button\_group\_on\_paint\_active\_button 中的 BUG。
  * 修改 tab\_button\_set\_value 时 pages 还没创建的问题。
  
* 2019/06/5
  * 更新 stm32 工程，修改了一些警告。
  * main loop 增加 app\_quited 标志，方便 awtk-iotjs 退出程序。
  * 增加 style border\_width

* 2019/06/4
   * 修复 tk\_abs 的 bug
   * 修改 guage\_pointer\_set\_anchor 的注释错误
   * 修改 widget\_animator\_manager\_find 的 name 可以为空（默认返回第一个有效元素），主要是为了判断控件是否有动画
   * 添加 slide\_indicator 控件，默认作为 slide\_view 的指示器
   * 修复 scroll\_view 的 bug（scroll\_bar\_m 不会自动隐藏的问题，比如 list\_view 中拖动单位高度后）
   * 修复 scroll\_view 的 fix\_end\_offset 失败的问题（比如在 list\_view 中上下拖动直到有空白后，横向移动，松开鼠标 list\_view 不会恢复）

> 感谢朝泽提供以上补丁。
    
    * 增加 main\_loop\_step，方便与 IOTJS 集成。
    * 修改 ui\_loader\_xml 中的昨天引入的 BUG。
    * 修改 stm32 编译警告。

* 2019/06/4
  * 修改 widget\_clone 支持拷贝 mutable style。
  * windows 平台缺省 BACKEND 使用 GLES2。
  * 修改 hscrollable 适用于没有 widget animator 的情况。
  * 控件的属性支持放到 property 标签中，详情请参考 UI 描述文档。

* 2019/06/3
  * 修改 add\_child/remove\_child，窗口管理器特殊处理。
  * 缺省启用 style mutable，具体用法请参考主题文档。

* 2019/06/2
  * 完善 expr eval 的函数调用支持多个参数。

* 2019/05/31
  * 修改 cairo 显示图片的问题 (cairo 需要 alpha 预乘的图片）。
  * 修改 cairo SVG 动画的问题 (cairo 不支持 scale 时，xy 为 0 的情况）
  * 增加输入事件处理流程文档。
  * children\_layouter\_default 增加 keep\_disable 和 keep\_invisible 参数。
  * children\_layouter\_list\_view 增加 keep\_disable 和 keep\_invisible 参数。

* 2019/05/30
  * 增加颜色格式描述文档。
  * FAQ 增加半透明相关的问答。
  * tab button 放在 scrollview 中，激活时自动滚动到可见区域。
  * 候选字支持左右滚动。
  * 资源管理可以设置自己的 system info 和 locale info，方便 IDE 区分自己的窗口和被设计的窗口。

* 2019/05/29
  * 增加 overlay 窗口
  * 完善 expr\_eval
  * 去掉 TK\_DEFAULT\_FONT 宏定义，提供函数 system\_info\_set\_default\_font 设置缺省字体（一般不用调用）。
   
* 2019/05/28
  * 修改 enums.c 编译问题（感谢陈潭提供补丁）。
  * 修改 vgcanvas\_reinit 的问题（感谢陈潭提供补丁）。
  * 合并 bgfx 补丁（感谢大恒提供补丁）。
  * 完善 shortcut。
  * 完善事件处理流程。

* 2019/05/27
    * 窗口在切换到前台和后台时分别触发事件 EVT\_WINDOW\_TO\_FOREGROUND 和 EVT\_WINDOW\_TO\_BACKGROUND
    * 窗口被切换到前台时，把窗口管理器的 key\_target 设置到当前窗口。
    * pages 切换当前页时，自动切换 key\_target 到当前页面。
    * slide view 切换当前页时，自动切换 key\_target 到当前页面。
    * 增加 shortcut
    * 完善按键处理

* 2019/05/25
  * 修改窗口中存在 pages/slideview 时焦点切换的问题。

* 2019/05/24
  * 部分控件增加 inputable 属性。
  * 修改 progress circle 半径的问题（感谢均权提供补丁）。
  * 完善字符串翻译文，增加数字时钟中星期和月份的处理。

* 2019/05/23
  * 修改 edit 在 scrollview 中的问题。
  * resize 后设置 need\_relayout\_children 标志（感谢朝泽提供补丁）。
  * 修改 slide menu 中 find target 的 bug（感谢朝泽提供补丁）。
  * move focus 时跳过不可见和不可用的 widget。
  * 当前没有焦点控件时，窗口收到 tab 键时，让第一个 focusable 控件得到焦点。
  * 修改 guage pointer 在 load widget 时的警告信息。
  * 增加控件焦点相关问题的文档。  
   
* 2019/05/22
  * 完善事件处理流程，修改 slide menu 放在 slide view 中的问题。
  * 窗口增加 move\_focus\_next\_key/move\_focus\_prev\_key 两个属性，用于设置焦点的键值。具体用法请参考：demos/assets/raw/ui/basic.xml。
  * 将 ListView 的布局策略放到 children\_layouter\_list\_view 中，并支持边距和间距等参数。
  *   
* 2019/05/21
  * button/check\_button 在收到回车和空格键时触发 click 事件。
  * 支持用 tab 键切换任意控件的焦点。
  * 支持用 focusable 属性将控件设置可获得焦点，实现 tab stop 类似的效果。focusable 的控件，其 style 必须具有 focused 状态，否则无法显示。

* 2019/05/20
  * 修改 ui\_loader\_load\_widget 因为无窗口导致 set\_tr\_text 失败的问题。

* 2019/05/18
  * 修改 window\_manager\_open\_window 将 key\target 设置为新窗口。
  * 合并 text selector 的补丁（感谢智明提供补丁）。
  * slider 支持上下左右键盘。

* 2019/05/16
  * 去掉 slist\_remove 中一个警告。
  * 完善 slider。垂直 slider 过宽或水平 slider 过高时缩放图标，否则显示有些怪异。
  * image animitioin 的自动播放放到第一次绘制时启动。
  * color picker 的对子控件的 hook 放到第一次绘制时处理。
  * slide view 支持 list view 作为子控件。

* 2019/05/15
  * 合并 guage pointer 支持锚点的补丁（感谢智明提供补丁）。
  * 完善 color\_hex\_str（感谢朝泽提供补丁）。
  * 修改 guage pointer 中文档错误（感谢朝泽提供补丁）。
  * 增加 vgcanvas\_cairo。cairo 速度快功能全，但代码体积稍大，请酌情使用。

   
* 2019/05/14
  * style 增加 margin\_left/margin\_right/margin\_top/margin\_bottom 几个参数用于细化之前的 margin 参数。 这个只影响 icon/text，不影响子控件（子控件由布局算法决定）
  
* 2019/05/13
  * 增加 widget\_add\_idle。
  * 完善 window\_manager\_do\_open\_window。
  * 子控件布局算法和控件自身算法支持注册自定义算法。
  * 增加函数 widget\_find\_animator（感谢均权提供补丁）。

* 2019/05/11
  * guage pointer 支持锚点（感谢智明提供补丁）。
  * 修改在 Ubuntu 18 32bit 上编译的问题（感谢 [hardyun](http://www.hardyun.com/) 提供补丁）。
  * 增加条件变量 tk\_cond\_var。

* 2019/05/10
  * 修改 SDL 输入法位置的 BUG。
  * 增加下列事件，在子控件处理之前触发，让用户注册的事件处理函数，有机会拦截这些事件。
     * EVT\_POINTER\_DOWN\_BEFORE\_CHILDREN
     * EVT\_POINTER\_UP\_BEFORE\_CHILDREN
     * EVT\_KEY\_DOWN\_BEFORE\_CHILDREN
     * EVT\_KEY\_UP\_BEFORE\_CHILDREN

     
* 2019/05/08
  * 完善 idl gen 工具支持外部项目并编写文档。 
  * 完善 doc gen 工具支持外部项目并编写文档。 
  * 因为脚本绑定生成工具只接受一个 IDL 文件，所以增加 merge.js 工具用于将多个 IDL 文件合并成一个 IDL 文件。
  
* 2019/05/07
  * edit 在焦点状态时，如果文本为空，仍然显示 tips。
  * 重构 pointer/key 事件处理流程，分成三个阶段：子控件处理前、子控件和子控件处理后。
  * 修改 slider 支持放在 slide\_view 中。
  * 修改 slide\_menu 支持放在 slide\_view 中。
  * 重构 image loader 支持自定义的 image loader（使用 imae\_loader\_register 注册自己的 loader)。

> 新增 WIDGET\_STATE\_EMPTY\_FOCUS 状态，请参考缺省主题为 edit 控件增加该状态。

* 2019/05/06
  * rename thread\_t to tk\_thread\_t。
  * 软键盘支持 delete 键。

* 2019/05/02
  * 支持 AWTK-JS 的开发的应用程序在 WEB 上运行。

* 2019/05/01
  * 修改 image base 函数名在 jerryscript 中引起的问题。

* 2019/04/30
  * 修改在 32 位系统上编译的 BUG（感谢文涛提供补丁）。
  * 为 awtk-web 做了些修改，方便 web 版本中动画优化。
  * 修改 canvas\_fix\_rect 导致 text selector 重影的问题。

* 2019/04/29
  * 修改 AWTK WEB 脏矩形的 BUG。
  * 修改 SVG 解析器中一个 BUG。

* 2019/04/28
  * AWTK WEB 版本 GIF 目前在 iPhone 上正常，其它浏览器只显示第一张图片。

* 2019/04/26
  * 重构 color component.
  * mutable bitmap 支持 web 版本。

* 2019/04/25
  * 更新文档。
  * theme/fontmanger 支持脚本化（感谢朝泽提供补丁）。
  * widget 处理 RET\_STOP 返回值（感谢朝泽提供补丁）。
  * dragger 除 POINTER ABORT 消息（感谢朝泽提供补丁）。
  * 修改窗口管理器取整的问题（感谢朝泽提供补丁）。
  * 修改 slide menu 在 web 上裁剪的问题。
  * 修改 widget\_set\_as\_key\_target 导致自定义软键盘焦点的问题。

* 2019/04/24
  * 修改 AGGE 模式支持 APP\_DESKTOP 的问题（感谢尧燊提供补丁）
  * 更新文档。
  * fix toast bug
  * 修改 edit 在 scroll view 中 clip 的 BUG。
  * 修改 slider 在 scroll view 中的 BUG。
  * 准备发布 1.1，用 cppcheck/infer/valgrind 检查。

* 2019/04/23
  * 修改编译脚本支持 python3。建议大家都使用 python3，python2 很快不被官方支持了。
  * 修改 awtk-web 在 windows 下编译的问题。
  * 修改 edit 在 scrollview 中的问题（感谢朝泽提供补丁） 
  * 增强 scrollview 的扩展性（感谢朝泽提供补丁） 

* 2019/04/22
  * input method 支持脚本化，方便在脚本中实现自定义软键盘。
  * key code 支持脚本化。

* 2019/04/21
  * 为 awtk web 做了些改进。
  * 重构 time clock。

* 2019/04/20
  * 为 awtk web 做了些改进。

* 2019/04/19
  * 完善 scale 算法。
  * 优化 scale 窗口动画。
  * 完善 default self layouter，right/center/bottom/middle 支持百分比。
  * 内置对话框启用缺省高亮策略。
  * 完善 widget\_get\_window，支持 designing window。

* 2019/04/18
  * 增强 scrollview 的扩展性（感谢朝泽提供补丁） 
  * 优化软件渲染 (bgr565 缩放半透明图片快 30%，缩放不透明图片快一倍）。
  * 增加 image\_clear 函数。

* 2019/04/16
  * window manager 支持 paint 事件。

* 2019/04/15
  * 去掉 widget\_is\_window\_opened 中的警告。

* 2019/04/14
  * 为 AWTK-WEB 移植做了些修改。

* 2019/04/13
  * 增加 cppcheck 和 infer 静态检查。
  * 修改 cppcheck 和 infer 发现的警告。

* 2019/04/12
  * 增加函数 ui\_loader\_load\_widget(ui\_loader.h)。

* 2019/04/10
  * 更新 demo，增加 back\_to\_home 的 demo。
  * 修改 SDL 的 BUG（感谢大恒和陈谭提供补丁）
  * 增加函数 bitmap\_save\_png 帮助调试显示相关的 BUG。
  * 修改 widget\_destroy\_children 的 BUG（感谢朝泽和叶冬提供补丁）

* 2019/04/09
  * 修改 window\_manager\_back\_to\_home。
  * dialog 支持非模态用法（请参考 dialog 的手册）
  
> AWTK 在浏览器中运行时无法支持模态对话框，
> 如果希望开发的 AWTK 应用程序在浏览器（包括各种小程序）中运行和演示，请不要使用模态对话框。

* 2019/04/08
  * 重构
  * 修改 valgrind 发现的内存泄露。
  * 增加 assets\_manager\_preload
  * 合并 optimization 分支，fill\_image 半透颜色性能提高 3 倍。

* 2019/04/07
  * 为适配 AWTK-WEB 做了写小的修改。
  * 修改资源脚本，生成文件列表，用于 awtk-web。

* 2019/04/05
  * 生成的常量数据增加字节对齐。

* 2019/04/03
  * 恢复 WITH\_MINI\_FONT 宏。
  * 修改 widget\_destroy\_in\_idle 中潜在的 BUG（感谢尧燊提供补丁）。
  * 修改 edit 软键盘无法关闭的 BUG。
  * 修改窗口动画因为取整导致的问题。
  * text selector 的 options 属性，指定范围时，支持指定格式。
  * 修改 window\_base\_load\_theme\_obj 从自己的 assets\_manager 中加载资源（感谢朝泽提供补丁）。
  * 修改 color picker 的 BUG（感谢朝泽提供补丁）。

* 2019/04/02
  * 软键盘支持 tab 键。
  * 修改窗口动画内存溢出问题（感谢陈谭提供补丁）。
  * 修改 windows 平台编译问题（感谢陈谭提供补丁）。
  * 修改 slideview clip 的 BUG(issue #105)。
  * 顶层窗口为 dialog 时，window\_manager\_back\_to\_home 直接返回失败（感谢尧燊提供补丁）。
  * EVT\_NONE 移到 tkc 中（感谢尧燊提供补丁）。
  * 修改 widget\_destroy\_in\_idle 中潜在的 BUG（感谢尧燊提供补丁）。
  * 调用 SDL\_CaptureMouse 支持鼠标离开 SDL 窗口（感谢尧燊提供补丁）。 
  * 增加 style\_mutable\_get\_value
  * 增加 style\_mutable\_cast
  * 调用 assets\_manager\_unref 的 BUG（感谢朝泽提供补丁）。

* 2019/04/01
  * 从 win 获取 locale info。
  * 增加测试。
  * 修改 issue #104
  * 增加 font\_manager\_unload\_font

* 2019/03/30
  * 修改 back\_to\_home 的 BUG #102（感谢陈谭提供补丁）。
  * 更新 stm32 相关工程。

* 2019/03/29
  * 修改 window\_manager\_back\_to\_home 的 BUG。
  * 合并 reworkwa 分支：
    * 窗口动画增加 slide\_up/slide\_down/slide\_left/slide\_right.
    * 窗口动画支持自定义。
    * 支持对话框高亮策略。
    * 更新文档。
    * 新增 src/dialog\_highlighters 目录和一些文件，可能需要更新工程文件和文件拷贝脚本。

* 2019/03/28
  * 修改 3rd/SDL/SConscript（感谢大恒提供补丁）。
  * 修改 bool\_t 在 aworks 上重复定义的问题（感谢陈谭提供补丁）。
  * 修改编辑器的 BUG（感谢俊杰提供补丁）。

* 2019/03/27
  * 修改 style mutable 的 BUG（感谢朝泽提供补丁）
  * 修改 utils 的 BUG（感谢朝泽提供补丁）

* 2019/03/25
  * 修改 issue #101（感谢俊杰提供补丁）
  * 修改 issue #100。
  * 更新 STM32 相关工程。

* 2019/03/22
  * 完善资源生成脚本。
  * widget vtable 增加 inputable 属性。
  * 修改 listview 没有 scrollbar 时的问题（感谢陈谭/俊杰提供补丁）。
  * edit/spinbox 在修改 text 后处理 prop changed 事件，以更新控件状态。
  * 修改参数检查的存在的问题（感谢俊杰提供补丁）

* 2019/03/21
  * 增加 lcd\_profile 用于对绘制函数进行 profile。
  * data 资源支持同文件名，不同扩展名。
  * slider 增加 bar\_size 属性。

* 2019/03/20
  * 支持编译 Windows 32 位版本（感谢陈谭提供补丁）
  * 修改资源生成脚本，对于 data 数据，不限制文件名 (DATA 类型的资源需带扩展名才能访问）。
  * 修改 scrollbar 的 style 配置的 BUG。
  * 修改关闭 system\_bar 时没有注销事件的 BUG（感谢朝泽提供补丁）。
  * window 支持 fullscreen 属性。
  * 修改 edit 缺省焦点的 BUG。
  * 支持用 tab/up/down 键切换编辑器焦点。
  * 修改 wheel 事件在 windows 下方向相反的问题。

* 2019/03/19
  * 增加 window\_close\_force 函数。
  * 修改 window\_manager\_close\_window 多次关闭同一个窗口的问题。
  * 增加 RET\_ITEMS\_CHANGED。
  * 修改 EVAL\_MAX\_NAME\_LENGTH 为 32（以前为 16，有些短）。
  * 完善 scroll\_bar\_set\_params。

* 2019/03/18
  * 增加 color\_rgba\_str。
  * tk\_atof 和 tk\_atoi 等函数，使用系统自带实现。

* 2019/03/15
  * 完善资源脚本，支持 data/xml 格式。

* 2019/03/14
  * 修改 edit/widget 支持自定义的软键盘。
  * 增加自定义的软键盘的示例和文档。
  * 更新文档。

* 2019/03/13
  * 增加了 object\_get\_type 等函数。
  * 修改 widget\_use\_style 的问题。
  * 修改 emitter\_remove\_item 的 BUG。
  * 增加 object\_default\_clone。
  * 修改 widget\_clone 没有拷贝自定义属性的问题。

* 2019/03/12
  * 窗口管理器增加 window\_manager\_back/window\_manager\_back\_to\_home
  * 增加示例。
  * 更新文档。

* 2019/03/11
  * date time 中增加 wday 表示星期。
  * 数字时钟支持星期和月的英文。
  * 修改事件处理函数返回 RET\_REMOVE 时，同时注册事件处理函数的问题。
  * 更新资源。

* 2019/03/08
  * 支持屏幕密度不敏感的图片，放到 images/xx 中。
  * UI/图片资源名称支持表达式。

* 2019/03/07
  * 增加 str\_expand\_vars 函数。
  * 重新实现 system\_info。
  * 支持设置字体的全局缩放比例。

* 2019/03/06
  * slide view 支持设置不同的切换动画。
  * 修改 single fb 的 flush 函数相关问题（感谢陈谭提供补丁）。

* 2019/03/05
  * 更新文档。
  * slide view 支持循环切换。
  * 修改 canvas\_draw\_text 和 vgcanvas\_fill\_text 坐标不一致的问题。

* 2019/03/04
  * 完善 progressbar/slider 的显示函数。
  * 增加 widget\_stroke\_border\_rect/widget\_fill\_bg\_rect 等函数供子类使用。
  * 合并 issue #97（感谢陈谭提供补丁）
  * 更新 bgfx backend（感谢大恒提供补丁）。
  * 在类的 annotation 中增加 window/widget/design

* 2019/03/04
  * 更新文档。
  * 修改 progress\_bar 显示的 BUG（感谢俊杰提供补丁）。

* 2019/03/02
  * 增加 dialog\_warn/dialog\_info/dialog\_warn。

* 2019/03/01
  * 完善 label。
  * 更新文档。
  * 增加 dialog\_toast

* 2019/02/28
  * 增加 mutable image，用于 camem 和 video 显示图像。

* 2019/02/27
  * 同步 BGFX 最新代码。

* 2019/02/26
  * 合并 typecheck 分支。
  * 控件支持运行时类型检查（目前支持 Linux/Windows/Macos 和部分嵌入时平台）。
  * 修改 main\_loop\_sdl\_fb\_destroy 的问题（感谢大恒提供补丁）。
  * 修改 canvas\_t 中注释错误（感谢朝泽提供补丁）。
  * 完善 image\_animation，修改#85 

* 2019/02/25
  * 完善 image\_animation，修改#85
  * 重构 SConstruct，把公共代码放到 awtk\_config.py，可供其它项目使用。

* 2019/02/22
  * 完善控件文档（感谢俊杰提供补丁）。
  * 更新 IDL。
  * 修改 awtk-js 代码产生器中生成 cast 函数的问题。

* 2019/02/21
  * 增加 object\_copy\_prop
  * 修改 value\_copy/value\_deep\_copy 的 BUG。
  * 修改 named\_value\_set\_value 的 BUG。
  * 完善布局文档（感谢俊杰提供补丁）。
  * 支持动态设置屏保时间。

* 2019/02/20
  * 按钮增加 enable\_long\_press 属性用于启用长按事件，触发长按事件后不再触发点击事件。
  * 增加 timer\_reset 函数。
  * 增加屏保支持。

* 2019/02/19
  * add darray\_tail/darray\_head。

* 2019/02/18
  * 完善 image\_animation，支持指定一个范围。
  * 完善 dialog，避免 dialog\_model 应用到普通窗口上。

* 2019/02/15
  * bitmap/color/date\_time/rect/named\_value/object/object\_default/emitter/value js 绑定支持 gc。
  * lua 绑定还不支持 gc("\_\_gc"被调用多次，而且 udata 居然无法修改，如果哪位朋友熟悉，麻烦指点一下）
  * 更新 stm32f103/427/767 的工程。

* 2019/02/14
  * 增加 tk\_under\_score\_to\_camel
  * 增加 widget\_set\_prop\_pointer/widget\_get\_prop\_pointer

* 2019/02/13
  * 窗口去掉 script 属性。

* 2019/02/07
  * 资源支持脚本。
  * 修改 object\_has\_prop 的 BUG。

* 2019/02/05
  * 公开函数 str\_extend

* 2019/02/04
  * 增加测试程序。
  * 更新资源。
  * edit 处理属性 value。
  * 完善 str/tokenizer/combobox/checkbutton

* 2019/02/03
  * 增加 object\_notify\_changed。

* 2019/02/02
  * 增加 tk\_str\_ieq

* 2019/02/01
  * 增加测试。
  * 合并 mvvm 分支。
  * 将 mvvm 代码移动到独立的项目：awtk-mvvm

* 2019/01/31
  * 增加 binding context awtk

* 2019/01/30
  * 增加 binding context
  
* 2019/01/29
  * 增加 navigator

* 2019/01/28
  * 增加 navigator\_request
  * 增加 navigator\_handler

* 2019/01/27
  * 增加 view\_model

* 2019/01/26
  * 修改字符串翻译的问题。issue 83/86
  * 增加 value converter factory
  * 增加 value validator factory
  * add expr eval

* 2019/01/25
  * 增加 model\_delegate。

* 2019/01/24
  * 增加 value\_validator/value\_validator\_delegate

* 2019/01/23
  * 增加 value\_converter/value\_converter\_delegate

* 2019/01/22
  * 增加 binding rule parse

* 2019/01/21
  * 增加 data\_binding/command\_binding.
  * 增加测试程序。

* 2019/01/20
  * 完善 object，增加一些包装函数。
  * 增加 binding\_rule/binding\_rule\_data

* 2019/01/19
  * 修改 valgrind 发些的问题，并增加一些脚本。
  * 增加 MVVM 分支。 

* 2019/01/18
  * 增加文档。

* 2019/01/17
  * 整理 API 文档：list\_view/list\_view\_h/list\_view\_item/scroll\_view/scroll\_bar
  * 整理 API 文档：self\_layouter/self\_layouter\_default/children\_layouter/children\_layouter\_default
  * 去掉 array 类，使用 darray 类代替。
  * 修改 valgrind 发现的泄露内存问题。

* 2019/01/16
  * 增加 darray 类，逐步替换以前的 array 类。
  * 增加 widget pool 用于控件的缓存和重复利用，可以通过 WITH\_WIDGET\_POOL 启用。
  * 增加 widget\_create 函数，简化子类控件的创建函数，同时让通用控件本身的由 widget pool 集中管理。
  * 使用 array 的地方替换为 darray。
  * 更新 stm32 相关工程。

* 2019/01/15
  * 整理 API 文档：progress\_circle/rich\_text/slide\_menu/slide\_view/svg\_image/switch
  * 整理 API 文档：text\_selector/time\_clock/digit\_clock
  * 增加 progress\_circle c 语言 demo。
  * 增加 slide\_menu c 语言 demo。
  * 增加 slide\_view c 语言 demo。
  * 增加 svg\_image c 语言 demo。
  * 增加 switch c 语言 demo。
  * 增加 text\_selector c 语言 demo。
  * 增加 time\_clock c 语言 demo。
  * 增加 digit\_clock c 语言 demo。
  * 增加 INPUT\_CUSTOM 用于关闭系统缺省软键盘。

* 2019/01/14
  * 完善脚本绑定（增加函数和注释）。
  * 整理 API 文档：color\_picker/gif\_image/guage/guage\_pointer/image\_animation/image\_value/keyboard。

* 2019/01/13
  * 完善脚本绑定（增加函数和注释）。
  * 更新 awtk-js。
  * 更新 awtk-lua。

* 2019/01/12
  * 重新实现 timer，解决 timer 中打开模态窗口的问题。 
  * 重新实现 idle，解决 idle 中打开模态窗口的问题。 
  * 完善 object，在修改属性和销毁时触发事件。

* 2019/01/11
  * 增加 slist\_t。

* 2019/01/10
  * 增加 object\_t/object\_default\_t/named\_value\_t 等几个类。
  * value\_t 支持 object。

* 2019/01/09
  * 完善 slide menu，增加 mask color，实现两边的按钮淡出的效果。
  * 增加 canavs widget 的 demo
  * 整理 API 文档：canvas\_widget

* 2019/01/08
  * 完善 text\_selector，增加 mask color，实现伪 3D 效果。
  * 修改 font size 的类型为 font\_size\_t。
  * 修改 agge scan line 为负的 BUG（感谢陈谭提供补丁）。
  * 按钮支持长按事件。

* 2019/01/07
  * 修改"Copyright (c) 2018 - 2018"为"Copyright (c) 2018 - 2019"
  * 整理 API 文档：tab\_control/tab\_button/tab\_button\_group。

* 2019/01/04
  * slide\_menu 动态决定可见子控件的个数（由长度与宽度比例和子控件个数决定）。
  * 修改 combobox 的 BUG。

* 2019/01/03
  * 同步 SDL 代码。
  * 整理 API 文档：timer/idle
  * 用 valgrind 测试内存问题，并修改 progress\_circle 中的内存泄露问题。
  * 修改桌面版本窗口 resize 的问题。
  * 密码编辑器增加显示/不显示明文的开关。
  * 键盘增加关闭按钮。
  * 修改把控件状态改成字符串导致的 BUG。
  * 修改处理 capslock 的 BUG。

* 2019/01/02
  * 整理 API 文档：input\_method/input\_method\_default/input\_method\_sdl/input\_method\_null
  * 整理 API 文档：input\_engine/input\_engine\_pinyin/input\_engine\_null
  * 更新 stm32 相关工程。

* 2018/12/31
  * 整理 API 文档：ui\_loader/ui\_builder/ui\_xml\_writer/ui\_binary\_writer/ui\_builder\_default

* 2018/12/30
  * 整理 API 文档：style/style\_const/style\_mutable/theme/style\_factory
  * move ui\_loader.c/.h to base 目录。

* 2018/12/29
  * 整理 API 文档：bitmap/image\_loader/image\_manager/image\_loader\_stb
  * 整理 API 文档：asset\_info/assets\_manager/locale\_info
  * 重构 image 相关的代码  

* 2018/12/28
  * 整理 API 文档：glyph/font/font\_loader/font\_manager/font\_loader\_ft/font\_loader\_stb/font\_loader\_bitmap
  * 重构 font 相关的代码  

* 2018/12/27
  * 整理控件 API 文档：value/mem/str/wstr/array/wbuffer/rbuffer/rgba/color/color\_parser。
  * 修改 SDL 输入法的问题（感谢大恒提供补丁）。
  * 完善 freetype 字体渲染（感谢 zjm09 提供补丁）。
  * 完善 glyph\_cache，允许 glyph 有不同的实现。 
  * 重构 color\_parser 并增加单元测试。
  * 把 events 拆成两部分，基础部分移到 tkc 中。

* 2018/12/26
  * 为了语义的一致性，把 widget 的虚函数 destroy 改名为 on\_destroy。on\_destroy 只是通知子类控件即将销毁。
  * 重命名 NAME\_LEN 为 TK\_NAME\_LEN。
  * 增加函数：widget\_animate\_value\_to
  * 修改 widget\_animator\_time\_elapse 计算次数的 BUG。
  * 整理控件 API 文档：color\_tile/calibration\_win/combo\_box\_item/button\_group
  * 整理控件 API 文档：dialog\_title/dialog\_client/progress\_bar/slider/vgcanvas
  * 支持 freetype 字体渲染（感谢 zjm09 提供补丁）。
  * 完善 value\_t。

* 2018/12/25
  * 整理控件 API 文档：row/column/grid/grid\_item/view/group\_box/app\_bar/system\_bar
  * 修改 idle\_info\_destroy 的 BUG（感谢尧燊提供补丁）。
  * 修改 timer\_info\_destroy 的 BUG（感谢尧燊提供补丁）。
  * widget 状态由枚举格式改为字符串格式，这样方便第三方控件定义私有的状态。
  * style\_id 由枚举格式改为字符串格式，这样方便第三方控件定义私有的 style\_id。

* 2018/12/24
  * 整理控件 API 文档：dialog/popup/window/window\_base

* 2018/12/23
  * 优化。
  * 整理控件 API 文档：spin\_box/combo\_box
  * 增加 combo\_box 的 C 示例代码。

* 2018/12/22
  * 修改 combobox 事件的 BUG(issue #62)。
  * 增加函数 widgeti\_get\_prop\_default\_value。
  * 增加 widget\_recycle 函数，支持控件回收，提高运行速度，减少内存碎片。

* 2018/12/21
  * 整理控件 API 文档：button/label/image/edit

* 2018/12/20
  * 重新设计 layouter 的架构，把 layouter 分成 self\_layouter 和 children\_layouter 两个接口，方便扩展支持不同的布局方式（如 css flex)。
  * 更新 layout 的文档，为了保持兼容，旧的 layout 属性仍然支持，新的代码请参考新的文档 [layout](https://github.com/zlgopen/awtk/blob/master/docs/layout.md)。
  * 子控件支持浮动布局，不受父控件的 children\_layouter 的限制。
  * 支持等宽字体的问题（感谢 [zjm1060](https://github.com/zjm1060) 提供补丁）。 
  * 修改 vgcanvas 中 w/h 为负数时的 BUG（感谢朝泽提供补丁）。
  * 完善 widget\_clone/widget\_equal/widget\_to\_xml。
  * widget 增加 sensitive 属性。
  * 完善 main\_loop\_dispatch\_events，支持按键事件（感谢 [zjm1060](https://github.com/zjm1060) 提供补丁）。 

* 2018/12/19
  * 修改 combobox 弹出窗口位置的 BUG。
  * 修改 widget\_animator\_manager 中野指针的问题（感谢陈谭提供补丁）。

* 2018/12/18
  * widget\_destroy 自动判断是否需要异步销毁。
  * 修改 issue #61(list item 销毁时，自动移出定时器）（感谢陈谭提供补丁）。
  * 修改 color parser 的 BUG（感谢朝泽提供补丁）。
  * 修改 guage 的 BUG（感谢朝泽提供补丁）。

* 2018/12/17
  * 抽象 layouter 接口，方便支持不同的 layout 算法（如 CSS 的 flex)。（未完）

* 2018/12/16
  * 修改 issue #61
  * 主题的背景和边框支持圆角半径 round\_radius，仅在定义 WITH\_VGCANVAS 时有效。

* 2018/12/15
  * 调整目录结构。将 base 拆分成 tkc、base 和 widgets 三个目录。
  * awtk.h 包含所有头文件，应用程序只需包含 awtk.h 即可。
    * 好处：一是使用简单，二是避免漏头文件造成的指针被截断的 BUG。
    * 坏处：编译稍慢。对于小型应用程序影响不大。大型应用程序和移植层仍然可以选择只包含需要的头文件。
  * 更新相关工程的代码。

* 2018/12/14
  * 修改 layout 的 BUG。

* 2018/12/13
  * 修改 slide menu 的 BUG（感谢尧燊提供补丁）。

* 2018/12/12
  * 支持任何数值型属性的动画。如 x/y/w/h 等等。 
  * 完善 label，支持动态设置显示字符数，形成动画效果。
  * 修改 bgfx 闪烁的问题（感谢大恒提供补丁）。
  * 修改 canvas 的 BUG（感谢朝泽提供补丁）。
  * 更新文档。

* 2018/12/11
  * gradiant 支持 alpha 渐变。
  * 完善 slide menu 控件，增加测试和 demo。 
  * 完善 digit\_clock，支持 YY、MM、DD、HH、mm、dd 格式

* 2018/12/08-10
  * slide menu 控件。 

* 2018/12/07
  * VGCanvas 支持渐变色 (GL 版本 OK，AGGE 版本 OK，AGG 版本暂时不支持）。
  * demoui 增加 vgcanvas 的 demo。 
  * draw\_vline 高度允许为负。
  * draw\_hline 宽度允许为负。

* 2018/12/06
  * 更新文档。
  * 完善 scroll view 的滚动效果。
  * 修改 label 中换行的 bug。
  * 修改窗口动画时 system bar 闪烁的 BUG。

* 2018/12/05
  * 修改 scanline\_adapter<RendererT>::commit 的 BUG，强制转换导致数据错误覆盖。
  * label 支持多行显示（非自动换行，需自动换行请用 rich\_text)。

* 2018/12/04
  * 更新 stm32 相关工程。
  * 完善 system\_bar 控件。
  * 增加 system\_bar 控件的使用文档。
  * 窗口管理支持 style。有时通过 style 给窗口管理器设置一张大背景背景，就不需要给窗口和 system\_bar 分别设置背景图片了。

* 2018/12/02-03
  * 增加 system\_bar 控件（窗口 vtranslate 动画还需要相应完善）。

* 2018/12/01
  * 更新 js/lua 脚本绑定。
  * 增加 canvas 控件。
  * 增加 image\_value 控件，可以用图片来显示电池电量、WIFI 信号强度和各种数值。

* 2018/11/30
  * 增加一些测试代码。
  * 增加函数 widget\_insert\_child/widget\_restack。
  * 用户修改系统时间后，更新定时器的启动时间，否则可能导致定时器长时间不会触发。

* 2018/11/29
  * 修复 scroll view 中脏矩形的问题。
  * awtk-linux-fb 工程支持编译用户的项目。
  * 修改 switch 控件中存在的问题并优化。
  * 修改 layout 参数解析中存在的问题。
  * 修改输入法相关的一些 BUG。
  * 缺省键盘增加中英文切换按钮。

* 2018/11/28
  * 更新字符串翻译的文档和示例。
  * 键值的前缀有 FKEY 改为 TK\_KEY。
  * 把 is\_designing\_window/is\_window 移到 vtable 中。
  * 把 window\_manager 中输入设备状态相关的代码移到独立的文件中。
  * 修改 list\_item 中的 BUG。
  * 修复 issue #50
  * 修复 issue #51
  * widget\_invalidate\_force 增加一个参数。

* 2018/11/27
  * 更新 stm32767/stm429 的工程。
  * 更新资源。
  * 修改编译警告。

* 2018/11/26
  * 优化不透明的 gif。
  * 修改部分函数 widget\_invalidate
  * 增加 grid\_item。

* 2018/11/25
  * 支持 GIF 图片（参考：demos/assets/raw/ui/gif\_image.xml)。

* 2018/11/24
  * svg 支持圆弧路径 (A/a)

* 2018/11/23
  * 增加 svg\_image 控件。SVG 支持的特性请参考 src/svg/README.md。

* 2018/11/22
  * 完善 button/checkbutton/comboboxitem 对 pointer down abort 事件的处理。
  * 完善 slideview 拖动处理。
  * 完善脏矩形。
  * 完善输入法 (issue #40)。
  * 完善 edit.c，删除字符时触发 changing 事件 (issue #41)。
  * 增加 image\_base 类，把图片公共属性放到 image\_base 中，方便实现 svg 图片。可能导致接口不兼容，请酌情修改。

* 2018/11/21
  * 修改一些编译警告。
  * 修改资源生成工具支持 SVG。
  * 修改 slide\_view，增加一个拖动阀值，由该阀值和速度共同决定是否切换页面。
  * 增加 clip\_board 接口、基于 SDL 和缺省实现。

* 2018/11/20
  * 合并 BUG #35 的补丁。
  * 合并 BUG #25 的补丁。
  * bsvg\_draw（基本功能可用，A/S/T 路径不支持）
  * guage\_pointer 支持 bsvg 图片。
  * 修改屏幕旋转时，窗口动画混乱的 BUG。

* 2018/11/16-19
  * SVG 解析器和 BSVG（二进制格式的 SVG) 解析器。
  * bsvggen/bsvgdump 等工具。

* 2018/11/16
  * 修改 FPS 的颜色。

* 2018/11/15
  * 同步 bgfx 的代码。
  * 修改 SConstruct 定义 SDL2。
  * 修改 scroll\_view  #issues/30
  * 修改 dirty\_rect 的 BUG（增加 x\_offset/y\_offset 引起的）。
  * 完善 guage，支持用颜色绘制指针。

* 2018/11/14
  * 完善 digit\_clock
  * change  WITH\_NANOVG\_GL to WITH\_NANOVG\_GPU
  * guage 拆成表盘和表针两个控件。
  * 新增加几个 style：selected\_text\_color、selected\_text\_color 和 selected\_text\_color
  * 修改资源生成工具的 BUG。该 BUG 导致资源后面有 4 字节的随机数据（功能不影响正确性）

* 2018/11/13
  * 修改 agg/agge 为 nanovg 后端时 linecap 的 BUG。

* 2018/11/12
  * 增加 js demos。

* 2018/11/11
  * 完善 ProgressCircle 控件。
  * base/time.h 改名为 base/time\_now.h，避免和系统 time.h 冲突。
  * 更新文档。

* 2018/11/10
  * 增加 ProgressCircle 控件。

* 2018/11/09
  * 重正编译脚本，以便集成 bgfx。
  * 重正 vgcanvas，以便集成 bgfx。

* 2018/11/08
  * 将 icon\_x\_offset/icon\_y\_offset 改名为 x\_offset/y\_offset，进行整体偏移，效果更好。
  * 修改 dirty\_rect 的问题。
  * 注释增加@event 关键字，用来显示当前控件支持的事件。
  * 完善控件的事件，并增加测试程序。

* 2018/11/07
  * 修改屏幕旋转的 BUG。
  * 修改 get\_clip 的 BUG。
  * 更新文档。

* 2018/11/06
  * 更新动画文档。
  * slideview 支持使用图标作为指示器。
  * style 加了 icon\_x\_offset/icon\_y\_offset，方便实现按下效果。

* 2018/11/04-05
  * 控件动画支持暂停。
  * 控件动画支持时间倍率，让时间变快和变慢。
  * 控件动画支持按名称去开始、暂停、停止和销毁。
  * 更易用的 API。请参考 widget\_start\_animator/widget\_pause\_animator。

* 2018/11/03
  * 增加 date\_time 类（嵌入式平台需要调用 date\_time\_set\_impl 获取时间的函数）。
  * 增加数字时钟控件 (ext\_widgets/time\_clock/digit\_clock.c)。

* 2018/11/02
  * 修改 tools 下的资源生成工具，只在资源变化时才更新资源，以提高开发的效率。
  * update\_res.py 增加参数，可以只更新指定类型的资源，以提高开发的效率。
  * update\_res.py 放到 scripts 目录下（可在 awtk 目录或 scripts 目录运行）。
  * 增加函数 widget\_add\_timer，增加的 timer 在控件销毁时，自动销毁。
  * 改进 button，先收到 pointer\_down 事件，再收到 pointer\_up 事件，才触发 click 事件。

* 2018/11/01
  * 增加 style\_mutable，支持动态修改 style（仅在编辑模式下启用）。
  * 增强 style\_factory，支持不同格式的 style。比如想支持 CSS，可以提供一个 CSS 的 style\_factory。
  * 更新文档。

* 2018/10/31
  * 修改 keyboard 崩溃的问题（关闭窗口时自动关闭对应的 keyboard)。
  * 修改 keyboard 刷新的问题（新 keyboard 和旧 keyboard 的高度不一致导致的）。
  * 增加 tk\_roundi/tk\_clampi。
  * 修 3framebuffer 的 BUG。

* 2018/10/30
  * edit 的光标颜色从 style(fg\_color) 中获取。
  * 修改 canvas 裁剪算法存在的 BUG。
  * 修改 valgrind 查出来的内存泄露问题。
  * demoui 中的仪表增加动画。

* 2018/10/29
  * assets\_manager 支持设置资源路径。
  * font\_manager 支持设置 assets\_manager。
  * image\_manager 支持设置 assets\_manager。
  * 提供函数 widget\_load\_image 用于加载图片，实现新控件时请用本函数加载图片。
  * 控件需要的图片、字体和其它资源均与当前窗口关联，方便在实现 designer 时，designer 本身和被编辑的控件使用不同的资源。
  * Windows 支持多线程编译。
  * 将 UTF8 的源文件转换成 UTF8-BOM。
  * 修改编译警告。
  * nanovg-agge 使用双线性插值算法（感谢攀峰）。

* 2018/10/28
  * 调整主题的格式：tab\_button 处于 active 时的 style，combo\_box\_item/check\_button/radio\_button 处于 checked 时的 style，作为不同状态处理，而不是作为不同的 style 处理。如果有用到，请参考 demo 做相应修改。
  * 修改 valgrind 发现的一些内存泄露问题。
  * demoui 增加退出应用程序的按钮。
  * 更新文档。

* 2018/10/27
  * 引入 style/style\_factory 接口，方便实现动态改变 style。
  * 实现 style\_const 兼容以前从配置文件获取而 style 数据。
  * 重构。

* 2018/10/26
  * 为了避免不必要的误解，xml 中不再指定 window 的 name 属性，以 xml 的文件名为准。
  * 修改 edit crash 的 bug。
  * edit 支持右对齐 (style="number")。
  * Window 增加 stage 属性，用于标识 Window 的生命周期。
  * Windows 下的 demo 开启一个终端显示 LOG 信息。

* 2018/10/25
  * 重新实现 take\_snapshot，支持特殊的 stride。
  * 自动过滤重复的 pointer\_move 事件。
  * 完善 list\_item。
  * vgcanvas 支持特殊的 stride。

* 2018/10/24
  * 重构 blend。
  * framebuffer/bitmap 支持 line\_length（即 stride/pitch)

* 2018/10/23
  * 修改鼠标指针残留的问题。
  * 控件动画支持永久播放，yoyo\_times 或 repeat\_times 设置为 0 时，视为永久播放。
  * 公开函数 widget\_animator\_stop，请确保调用了 widget\_animator\_set\_destroy\_when\_done 禁止播放完成时自动销毁动画对象。

* 2018/10/22
  * 完善控件动画和 demo。
  * 支持鼠标指针。
  * 引入 window\_base，重构 window 相关的组件。
  * 更新文档。

* 2018/10/21
  * 出于一致性的考虑，将 widget 的属性 style\_name 重命名为 style，将 widget 的属性 style 重命名为 style\_data。
  * 增加 widget\_animator\_factory，从而支持在 xml 指定控件的动画。
  * 更新文档。

* 2018/10/20
  * pages 控件切换页面时触发 EVT\_VALUE\_CHANGED 事件。
  * slide\_view 控件的 indicator 颜色由主题确定。
  * slide\_view 控件切换页面时触发 EVT\_VALUE\_CHANGED 事件。
  * 窗口动画支持指定时长，请参考 [窗口动画](window_animator.md) 文档。
  * 更新文档。

* 2018/10/19
  * 更新文档。
  * 修改文字渲染的 bug。
  * 重新整理 font\_loader 相关代码，无需在初始化时就加载全部字体。

* 2018/10/18
  * 修改控件状态问题（在特殊情况下，mouse 重新进入控件，状态没有改变）。
  * 修改 MacOS 升级后，打开窗口瞬间出现黑屏的问题。
  * 修改 SDL+FB 模式下刷新的问题。
  * 修改 Windows 下 Mutex 实现的 BUG。
  * 改进 ListView 滑动效果。
  * 增加文档。

* 2018/10/17
  * 完善和重构 bitmap、图片加载和图片数据生成等代码。
  * calibration 控件支持设置点击事件处理的回调函数。

* 2018/10/16
  * 完善图片控件。绘制带缩放和旋转的图片时，不用 WTIH\_VGCANVS 宏，而根据 get\_vgcanvas 是否为空来判断。
  * 同步同事改进的 SDL，Windows10/Linux 支持高清屏。
  * 更新文档。

* 2018/10/15
  * 用 agg 实现 nanovg 的渲染后端。
  * 更新文档、编译脚本并测试。

* 2018/10/14
  * 用 agg 实现 nanovg 的渲染后端。

* 2018/10/13
  * 重新整理第三方库。
  * 集成修改过的 SDL(Linux/Winodws 支持高清屏）。

* 2018/10/12
  * 增加大字体 demo。
  * 修改文本垂直居中的问题。

* 2018/10/11
  * 完善 Qt UI 文件转换工具。
  * 更新文档。
  * 优化。

* 2018/10/9-10
  * 重构 lcd\_mem 和 blend，用代码产生器生成各种格式的代码。
  * 增加测试代码。
  * 更新相关工程。

* 2018/10/8
  * 修改 ui\_loader\_xml，特殊属性优先设置。

* 2018/10/7
  * 更新文档。

* 2018/10/4
  * 完善内存管理，并增加内存泄露跟踪功能。
  * 增加内存需求评估文档。
  * 增加电阻屏校准窗口。

* 2018/10/2-3
  * 移植到 sylixos

* 2018/10/1
  * 重构
  * 修改编译警告
  * 更新文档

* 2018/09/30
  * lcd\_reg 支持 RGB565 的图片。

* 2018/09/28
  * 实现 image\_animation 控件并增加 demo。

* 2018/09/27
  * 实现开关控件并增加 demo。

* 2018/09/26
  * 完善 text\_selector 控件并增加 demo。
  * 完善 vgcanvas，增加 paint 接口。

* 2018/09/25
  * 完成 text\_selector 控件基本功能。

* 2018/09/24
  * 更新文档。

* 2018/09/23
  * 更新各个项目的工程。

* 2018/09/21-22
  * 移植 awtk 到 stm32f767 开发板。

* 2018/09/20
  * 移植 nanovg-agge 到 stm32 和 aworks。

* 2018/09/12-19
  * 基于 agge 用纯软件实现 nanovg 的渲染接口，去掉对 cairo/agg/piccaso 的依赖，以最少的代码实现 vgcanvas 的接口。
  * 完善 vgcanvas-nanovg 支持基于 OpenGL 和纯软件两种实现。

* 2018/09/11
  * awtk-linux-fb 在 IM287A 上测试通过。

* 2018/09/10
  * awtk-linux-fb 支持 linux fb 设备。
* 2018/09/09
  * awtk-linux-fb 支持 linux tslib 设备。
* 2018/09/08
  * awtk-linux-fb 支持 linux input 设备。
* 2018/09/07
  * 录了一些演示视频，给不方便编译的朋友了解 AWTK。

* 2018/09/06
  * 移植到 linux framebuffer

* 2018/09/05
  * 增加 guage 控件。
  * 完善 time\_clock 控件。

* 2018/09/04
  * 修改 cairo 在 arm 平台上图片显示的问题 (stack size 需要 32K)

* 2018/09/03
  * 移植 cairo 到 arm 平台（基本绘图功能正常，图片和字体还不行）。

* 2018/09/02
  * 基于 cairo 实现 vgcanvas.

* 2018/09/01
  * 移植 cairo。

* 2018/08/31
  * 移植 pixman。

* 2018/08/30
  * 更新 stm32 相关工程。

* 2018/08/29
  * 完善 str 类，增加 str\_insert/str\_remove 等函数。
  * 完善 path 类，增加 path\_replace\_basename 等函数。
  * ui 描述文件支持 include 指令，方便包含公共组件（请参考：demos/assets/raw/ui/tab\_top.xml)。

* 2018/08/28
  * 完善 demos，运行双击直接运行 demos。
  * 完善 preview\_ui，支持在命令行指定宽度和高度。
  * 增加文档。

* 2018/08/27
  * 增加 skia 的编译脚本。

* 2018/08/26
  * 增加 path.c/path.h 提供基本的目录操作函数。

* 2018/08/25
  * 增加文件系统的接口和 posix 的实现。

* 2018/08/24
  * 整理 awtk 的目录结构，把部分控件移到 ext\_widgets 目录下。
  * 修改 stm32 编译的问题。

* 2018/08/23
  * 完善 Color Picker 控件。
  * 增加 Color Picker 控件 Demos。
  * 支持 mutable 的图片。

* 2018/08/22
  * 增加 Color Tile 控件。

* 2018/08/21
  * 增加 Color Picker 控件。

* 2018/08/20
  * 更新文档。

* 2018/08/19
  * 增加模板工程：https://github.com/zlgopen/awtk-hello。

* 2018/08/18
  * 更新 stm32 相关工程。

* 2018/08/17
  * 修改 AWTK 开发桌面软件的一些问题。

* 2018/08/16
  * 新增 awtk-c-demos 仓库，存放 c 语言的示例。

* 2018/08/15
  * javascript 绑定原理文档。

* 2018/08/14
  * 增加 javascript demos。

* 2018/08/13
  * 增加 javascript demos。
  * 修改 lua/javascript 回调函数泄露的问题。

* 2018/08/11
  * awtk.ts 代码产生器。

* 2018/08/10
  * 完善 jerryscript 绑定代码生成器。

* 2018/08/09
  * jerryscript 绑定代码生成器。

* 2018/08/08
  * 为每类控件增加 Cast 函数，方便脚本绑定。

* 2018/08/07
  * 完善 API docs
  * 完善 awtk-lua。

* 2018/08/06
  * 完善 API docs
  * lua 绑定放入独立的项目 awtk-lua。

* 2018/08/05
  * 重写 IDL 生成工具。

* 2018/08/04
  * 用 valgrind 运行单元测试，并修改发现的 BUG。

* 2018/08/03
  * 修改 linux 下编译的问题。

* 2018/08/02
  * 完善 api docs，增加 annotation。

* 2018/08/01
  * 重构 widget。

* 2018/07/31
  * 完善 rich\_text 控件并增加 DEMO。

* 2018/07/29
  * 完善 rich\_text 控件。

* 2018/07/28
  * 增加 rich\_text 控件（未完成）。

* 2018/07/27
  * 增加 unicode linebreak。
  * 增加 rich\_text\_parser。

* 2018/07/26
  * 增加 combo\_box/combo\_box\_item 控件和 demo。

* 2018/07/25
  * 增加 popup 窗口。

* 2018/07/24
    * 完善 tab\_control 及 demo。

* 2018/07/23
    * 增加 tab\_control 的 demo。

* 2018/07/22
    * 实现 tab\_button/tab\_button\_group/tab\_control。

* 2018/07/20
    * 增加 EVT\_POINTER\_DOWN\_ABORT，修改 ScrollView 中 ListItem/CheckButton/Button 在滚动时被误点的问题。

* 2018/07/19
    * 主题支持设置图标的位置，增加一些 demo。

* 2018/07/18
    * image 支持可选中和可点击属性。

* 2018/07/17
    * widget 支持自定义属性。
    * 出于易用性考虑，style\_name 改为字符串格式。

* 2018/07/15
    * 实现 widget\_to\_xml。

* 2018/07/14
    * 增加 ui\_xml\_writer
    * 增加 ui\_binary\_writer
    * 实现 widget\_clone/widget\_equal。

* 2018/07/13
    * 在 widget vtable 中增加 create/properies 方便实现 clone。
    * 重构 widget

* 2018/07/12
    * 重新整理 grab/ungrab 的流程。
    * 修改关闭窗口的 BUG。

* 2018/07/11
    * LCD 增加 flush 接口。
    * 完善图片绘制接口，增加相应 demo。
    * 在 Windows 7 上，如果显卡太旧不支持 OpenGL3 时，触发 assert。

* 2018/07/10
    * 框架支持滚轮事件。
    * ListView 和 SpinBox 支持滚轮事件。

* 2018/07/09
    * 按钮支持 repeat 触发 click 事件，键盘和滚动条支持 repeat 触发。
    * keyboard 点击 return 时提交之前输入的键值。
    * 滚动条支持禁止滚动效果。
    * ListView 支持自动隐藏滚动条。

* 2018/07/08
    * 更新文档，修改三个 framebuffer 的旋转时的 BUG。

* 2018/07/07
    * 支持 online framebuffer, offline framebuffer 和 next framebuffer 三个 framebuffer 的工作方式。

* 2018/07/06
    * 支持双 framebuffer 交换。

* 2018/07/05
    * 增加 ListView 和 ListView Demo。

* 2018/07/04
    * 增加 ScrollView。

* 2018/07/03
    * 更新 awtk on aworks rt1052，集成硬件加速。

* 2018/07/02
    * 实现 scrollbar 控件。

* 2018/07/01
    * 实现 drager 控件。

* 2018/06/30
    * 实现 spinbox 控件。

* 2018/06/29
    * 改进 edit。
    * 增加 edit 的 demo。
    * theme 支持 border 的类型。

* 2018/06/28
    * 修改脏矩形等 BUG。
    * 修改 ubuntu 18 上编译的问题。
    * 重构 widget。

* 2018/06/27
    * 实现输入法的联想字功能。

* 2018/06/26
    * 更新 aworks 工程并测试。
    * 增加小工具用于生成输入法联想词库。

* 2018/06/25
    * 完善候选字控件。
    * 编写输入法文档。

* 2018/06/24
    * 集成 google 拼音输入法（输入法字体和完整的字体，需要 4M 的 flash 空间）。

* 2018/06/23
    * 引入输入法引擎接口。
    * 增加候选字控件。

* 2018/06/22
    * 完善软键盘。
    * 修改一些 BUG。

* 2018/06/21
    * 输入框被软键盘覆盖时上推窗口。
    * 接入原生输入法（目前没有中文字体，编辑器中显示为空白）。

* 2018/06/20
    * 更新 lua 绑定及示例。
    * 优化软键盘并增加动画。
    * 更新 stm32f103/f429 的工程。

* 2018/06/19
    * 实现输入法框架。
    * 实现软键盘基本功能。

* 2018/06/18
    * 实现软键盘部分功能。
    * 完善现有代码。

* 2018/06/17
    * 增强 layout 的功能。
    * 修改 XML UI/String 文件中不能处理特殊字符的问题。

* 2018/06/16
    * demoui 增加预加载资源的窗口。
    * 增加 widget\_factory，UI 文件支持自定义的控件。

* 2018/06/15
    * 每个窗口支持独立的主题文件。
    * 修改 F429 上图片显示不正常的 BUG。

* 2018/06/14
    * LCD FrameBuffer 模式支持屏幕 90 度旋转。
    * 统一图片操作的硬件和软件实现方式。

* 2018/06/13
    * 集成 RT1052 PXP 硬件加速到 Aworks。

* 2018/06/11
    * 增加显示 FPS 的开关。

* 2018/06/10
    * 优化窗口动画。
    * 完善 lcd 的接口。
    * 完善资源管理。

* 2018/06/09
    * 修改 stb\_image/stb\_truetype 在开发板不工作的问题（内存分配/stack 大小）。
    * 图片加载支持 bgra/rgb565 格式。
    * 修 stm32\_g2d 中 blend\_image 的 BUG。

* 2018/06/08
    * 修改几个 BUG。
    * 完善 demos。

* 2018/06/07
    * slide\_view 实现基本功能。

* 2018/06/06
    * 增加 memset/memcpy 速度测试。
    * 增加 widget\_animator\_scroll 实现滚动的动画。
    * 增加滑动速度计算。

* 2018/06/05
    * event 结果中增加时间戳。
    * 完善 layout 参数，支持指定子控件的宽度和高度。

* 2018/06/04
    * 移植到正点原子号令者开发板。

* 2018/06/03
    * 把 systick 作为公共代码放到 platform/raw 下。
    * 去掉对 stdio 中部分函数的依赖。
    * 修改匿名 union 的问题。

* 2018/06/02
    * 控件动画文档。
    * misc/new 实现 C++的内存分配。

* 2018/06/01
    * 修改 STM F103ZE JTAG 无法下载的问题。
    * 优化 lcd\_reg.inc
    * STM F103ZE 实现获取时间的函数。

* 2018/05/31
    * 控件支持缩放动画（目前需要 OpenGL)。
    * 控件支持旋转动画（目前需要 OpenGL)。

* 2018/05/30
    * 控件支持淡入淡出动画。

* 2018/05/28
    * aworks 平台上的 memcpy 有问题，暂时使用自定义的 memcpy。

* 2018/05/27
    * 改进 lcd\_mem 的实现，编写部分文档。

* 2018/05/26
    * 优化图片缩放算法。

* 2018/05/25
    * 优化填充颜色算法。

* 2018/05/24
    * 优化 soft\_g2d。

* 2018/05/23
    * 移植到 aworks 平台。

* 2018/05/18
    * 编写 framebuffer 的文档。

* 2018/05/17
    * 支持非 GUI 线程添加 idle/timer 到 GUI 线程执行。

* 2018/05/16
    * 实现控件动画框架。
    * 实现控件移动位置的动画。
    * 实现控件修改值的动画。

* 2018/05/15
    * 重新实现 timer/idle，支持 timer/idle 处理函数内部操作 timer/idle。

* 2018/05/14
    * 修改内存管理器的 BUG，完善内存管理器的测试程序。

* 2018/05/13
    * 编写 stm32f429igtx 移植文档。

* 2018/05/12
    * lcd 支持 double framebuffer，移植到 stm32429igtx 裸系统。

* 2018/05/10
    * 为了方便代码管理，把硬件平台相关的代码移到独立的项目中。

* 2018/05/09
    * 重新整理了基于 framebuffer 的 LCD，并支持硬件加速功能。

* 2018/05/07
    * 实现 STM32 的 dma2d 加速的矩形填充和图片合成。

* 2018/05/05
    * lftk 改名为 awtk。

* 2018/05/03
    * 增加 locale，支持多国语言字符串翻译。

* 2018/05/01
    * 更新文档。

* 2018/04/30
    * 窗口动画的 anim\_hint 用字符串表示，方便附加参数。 

* 2018/04/29
    * 增加 str\_t，widget 的名称用 str\_t。

* 2018/04/28
    * 图片管理器支持缓存。
    * 图片管理器支持根据不同的 DPI 加载不同的图片。

* 2018/04/27
    * 资源管理器支持从文件中加载资源。

* 2018/04/26
    * 增加 picasso 实现的 vgcanvas。
    * 增加 system\_info

* 2018/04/21
    * 对话框支持从下往上弹出、从上往下弹出、中心缩放和淡入淡出几种动画。
    * 窗口支持左右平移动画。

* 2018/04/20
    * 实现窗口动画框架。

* 2018/04/15
    * 增加 nanovg 实现的 vgcanvas。
    * 增加基于 vgcanvas 实现的 lcd。

* 2018/04/14
    * PC 环境缺省使用 SDL2+nanovg 渲染，解决在 MacOS 上文字模糊的问题。
    * 调整 lcd 的接口，使之具有更强的适应性。 

* 2018/04/8
    * 重新实现 dialog 的主题。

* 2018/04/7
    * 增加按键定义。
    * 增加 edit 控件（未完成）

* 2018/04/5
    * Slider 控件支持 step 和事件。
    * Slider 和 ProgressBar 支持在主题中设置图片。
    * 因为性能原因，agg 中使用 float 代替 double。

* 2018/04/2
    * 增加 Slider 控件。

* 2018/04/1
    * 字体管理器支持 TTF 字体。 
    * 增加工具 resgen。
    * 增加主题文档。

* 2018/03/31
    * 重新设计 XML 主题的格式。
    * 重构 window/button/label/check\_buton，使用同一个绘制函数。

* 2018/03/27
    * [Luncher](https://github.com/Luncher) 加入 AWTK 核心团队。
    * 完善文档。

* 2018/03/26
    * 主题支持设置背景图片和背景图片的显示方式。

* 2018/03/25
    * 增加矢量图绘图函数接口，并用 agg 实现部分功能。

* 2018/03/24
    * 支持图片居中、缩放、9 宫格和平铺等 10 余种显示方式。

* 2018/03/18
    * 支持 anti-aliasing 直线。
    * 重新设计资源管理器。

* 2018/03/17
    * 支持将 Qt 的 UI 文件转成 AWTK 的 UI 文件。
    * 支持将 VC 的 RC 文件转成 AWTK 的 UI 文件。
    * 编写 AWTK UI 界面描述的文档。
    * 增加事件队列，解决在触屏中断中分发事件的问题。
    * 编写 STM32 移植文档。

* 2018/03/11
    * 增加 XML 界面描述文件预览工具。
    * 支持极速模式（定义 FAST\_MODE 启用），只绘制顶层的窗口中变化的控件，绘制效率非常高，但不支持透明背景。
    * 支持基本的 layout 功能。

* 2018/03/10 
    * lua 绑定及相关示例完成。
    * 支持从 UI 描述文件创建界面。
