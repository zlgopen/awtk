# 最新动态
* 2019/04/16
  * window manager支持paint事件。

* 2019/04/15
  * 去掉widget\_is\_window\_opened中的警告。

* 2019/04/14
  * 为AWTK-WEB移植做了些修改。

* 2019/04/13
  * 增加cppcheck和infer静态检查。
  * 修改cppcheck和infer发现的警告。

* 2019/04/12
  * 增加函数ui\_loader\_load\_widget(ui\_loader.h)。

* 2019/04/10
  * 更新demo，增加back\_to\_home的demo。
  * 修改SDL的BUG(感谢大恒和陈谭提供补丁)
  * 增加函数bitmap\_save\_png帮助调试显示相关的BUG。
  * 修改widget\_destroy\_children的BUG(感谢朝泽和叶冬提供补丁)

* 2019/04/09
  * 修改window\_manager\_back\_to\_home。
  * dialog支持非模态用法(请参考dialog的手册)
  
> AWTK在浏览器中运行时无法支持模态对话框，
> 如果希望开发的AWTK应用程序在浏览器(包括各种小程序)中运行和演示，请不要使用模态对话框。

* 2019/04/08
  * 重构
  * 修改valgrind发现的内存泄露。
  * 增加assets\_manager\_preload
  * 合并optimization分支，fill\_image半透颜色性能提高3倍。

* 2019/04/07
  * 为适配AWTK-WEB做了写小的修改。
  * 修改资源脚本，生成文件列表，用于awtk-web。

* 2019/04/05
  * 生成的常量数据增加字节对齐。

* 2019/04/03
  * 恢复WITH\_MINI\_FONT宏。
  * 修改widget\_destroy\_in\_idle中潜在的BUG(感谢尧燊提供补丁)。
  * 修改edit软键盘无法关闭的BUG。
  * 修改窗口动画因为取整导致的问题。
  * text selector的options属性，指定范围时，支持指定格式。
  * 修改window\_base\_load\_theme\_obj从自己的assets\_manager中加载资源(感谢朝泽提供补丁)。
  * 修改color picker的BUG(感谢朝泽提供补丁)。

* 2019/04/02
  * 软键盘支持tab键。
  * 修改窗口动画内存溢出问题(感谢陈谭提供补丁)。
  * 修改windows平台编译问题(感谢陈谭提供补丁)。
  * 修改slideview clip的BUG(issue #105)。
  * 顶层窗口为dialog时，window\_manager\_back\_to\_home直接返回失败(感谢尧燊提供补丁)。
  * EVT\_NONE移到tkc中(感谢尧燊提供补丁)。
  * 修改widget\_destroy\_in\_idle中潜在的BUG(感谢尧燊提供补丁)。
  * 调用SDL\_CaptureMouse支持鼠标离开SDL窗口(感谢尧燊提供补丁)。 
  * 增加style\_mutable\_get\_value
  * 增加style\_mutable\_cast
  * 调用assets\_manager\_unref的BUG(感谢朝泽提供补丁)。

* 2019/04/01
  * 从win获取locale info。
  * 增加测试。
  * 修改issue #104
  * 增加font\_manager\_unload\_font

* 2019/03/30
  * 修改back\_to\_home的BUG #102(感谢陈谭提供补丁)。
  * 更新stm32相关工程。

* 2019/03/29
  * 修改window\_manager\_back\_to\_home的BUG。
  * 合并reworkwa分支：
    * 窗口动画增加slide\_up/slide\_down/slide\_left/slide\_right.
    * 窗口动画支持自定义。
    * 支持对话框高亮策略。
    * 更新文档。
    * 新增src/dialog\_highlighters目录和一些文件，可能需要更新工程文件和文件拷贝脚本。

* 2019/03/28
  * 修改 3rd/SDL/SConscript(感谢大恒提供补丁)。
  * 修改bool\_t在aworks上重复定义的问题(感谢陈谭提供补丁)。
  * 修改编辑器的BUG(感谢俊杰提供补丁)。

* 2019/03/27
  * 修改style mutable的BUG(感谢朝泽提供补丁)
  * 修改utils的BUG(感谢朝泽提供补丁)

* 2019/03/25
  * 修改issue #101(感谢俊杰提供补丁)
  * 修改issue #100。
  * 更新STM32相关工程。

* 2019/03/22
  * 完善资源生成脚本。
  * widget vtable增加inputable属性。
  * 修改listview没有scrollbar时的问题(感谢陈谭/俊杰提供补丁)。
  * edit/spinbox在修改text后处理prop changed事件，以更新控件状态。
  * 修改参数检查的存在的问题(感谢俊杰提供补丁)

* 2019/03/21
  * 增加lcd\_profile用于对绘制函数进行profile。
  * data资源支持同文件名，不同扩展名。
  * slider增加bar\_size属性。

* 2019/03/20
  * 支持编译Windows 32 位版本(感谢陈谭提供补丁)
  * 修改资源生成脚本，对于data数据，不限制文件名(DATA类型的资源需带扩展名才能访问)。
  * 修改scrollbar的style配置的BUG。
  * 修改关闭system\_bar时没有注销事件的BUG(感谢朝泽提供补丁)。
  * window支持fullscreen属性。
  * 修改edit缺省焦点的BUG。
  * 支持用tab/up/down键切换编辑器焦点。
  * 修改wheel事件在windows下方向相反的问题。

* 2019/03/19
  * 增加window\_close\_force函数。
  * 修改window\_manager\_close\_window多次关闭同一个窗口的问题。
  * 增加RET\_ITEMS\_CHANGED。
  * 修改EVAL\_MAX\_NAME\_LENGTH为32(以前为16，有些短)。
  * 完善scroll\_bar\_set\_params。

* 2019/03/18
  * 增加color\_rgba\_str。
  * tk\_atof和tk\_atoi等函数，使用系统自带实现。

* 2019/03/15
  * 完善资源脚本，支持data/xml格式。

* 2019/03/14
  * 修改edit/widget支持自定义的软键盘。
  * 增加自定义的软键盘的示例和文档。
  * 更新文档。

* 2019/03/13
  * 增加了object\_get\_type等函数。
  * 修改widget\_use\_style的问题。
  * 修改emitter\_remove\_item的BUG。
  * 增加object\_default\_clone。
  * 修改widget\_clone没有拷贝自定义属性的问题。

* 2019/03/12
  * 窗口管理器增加window\_manager\_back/window\_manager\_back\_to\_home
  * 增加示例。
  * 更新文档。

* 2019/03/11
  * date time中增加wday表示星期。
  * 数字时钟支持星期和月的英文。
  * 修改事件处理函数返回RET\_REMOVE时，同时注册事件处理函数的问题。
  * 更新资源。

* 2019/03/08
  * 支持屏幕密度不敏感的图片，放到images/xx中。
  * UI/图片资源名称支持表达式。

* 2019/03/07
  * 增加str\_expand\_vars函数。
  * 重新实现system\_info。
  * 支持设置字体的全局缩放比例。

* 2019/03/06
  * slide view支持设置不同的切换动画。
  * 修改single fb的flush函数相关问题(感谢陈谭提供补丁)。

* 2019/03/05
  * 更新文档。
  * slide view支持循环切换。
  * 修改canvas\_draw\_text和vgcanvas\_fill\_text坐标不一致的问题。

* 2019/03/04
  * 完善progressbar/slider的显示函数。
  * 增加widget\_stroke\_border\_rect/widget\_fill\_bg\_rect等函数供子类使用。
  * 合并issue #97(感谢陈谭提供补丁)
  * 更新bgfx backend(感谢大恒提供补丁)。
  * 在类的annotation中增加window/widget/design

* 2019/03/04
  * 更新文档。
  * 修改progress\_bar显示的BUG(感谢俊杰提供补丁)。

* 2019/03/02
  * 增加dialog\_warn/dialog\_info/dialog\_warn。

* 2019/03/01
  * 完善label。
  * 更新文档。
  * 增加dialog\_toast

* 2019/02/28
  * 增加mutable image，用于camem和video显示图像。

* 2019/02/27
  * 同步BGFX最新代码。

* 2019/02/26
  * 合并typecheck分支。
  * 控件支持运行时类型检查(目前支持Linux/Windows/Macos和部分嵌入时平台)。
  * 修改main\_loop\_sdl\_fb\_destroy的问题(感谢大恒提供补丁)。
  * 修改canvas\_t中注释错误(感谢朝泽提供补丁)。
  * 完善image\_animation，修改#85 

* 2019/02/25
  * 完善image\_animation，修改#85
  * 重构SConstruct，把公共代码放到awtk\_config.py，可供其它项目使用。

* 2019/02/22
  * 完善控件文档(感谢俊杰提供补丁)。
  * 更新IDL。
  * 修改awtk-js代码产生器中生成cast函数的问题。

* 2019/02/21
  * 增加object\_copy\_prop
  * 修改value\_copy/value\_deep\_copy的BUG。
  * 修改named\_value\_set\_value的BUG。
  * 完善布局文档(感谢俊杰提供补丁)。
  * 支持动态设置屏保时间。

* 2019/02/20
  * 按钮增加enable\_long\_press属性用于启用长按事件，触发长按事件后不再触发点击事件。
  * 增加timer\_reset函数。
  * 增加屏保支持。

* 2019/02/19
  * add darray\_tail/darray\_head。

* 2019/02/18
  * 完善image\_animation，支持指定一个范围。
  * 完善dialog，避免dialog\_model应用到普通窗口上。

* 2019/02/15
  * bitmap/color/date\_time/rect/named\_value/object/object\_default/emitter/value js绑定支持gc。
  * lua绑定还不支持gc("\_\_gc"被调用多次，而且udata居然无法修改，如果哪位朋友熟悉，麻烦指点一下)
  * 更新stm32f103/427/767的工程。

* 2019/02/14
  * 增加 tk\_under\_score\_to\_camel
  * 增加 widget\_set\_prop\_pointer/widget\_get\_prop\_pointer

* 2019/02/13
  * 窗口去掉script属性。

* 2019/02/07
  * 资源支持脚本。
  * 修改object\_has\_prop的BUG。

* 2019/02/05
  * 公开函数str\_extend

* 2019/02/04
  * 增加测试程序。
  * 更新资源。
  * edit处理属性value。
  * 完善str/tokenizer/combobox/checkbutton

* 2019/02/03
  * 增加object\_notify\_changed。

* 2019/02/02
  * 增加tk\_str\_ieq

* 2019/02/01
  * 增加测试。
  * 合并mvvm分支。
  * 将mvvm代码移动到独立的项目：awtk-mvvm

* 2019/01/31
  * 增加binding context awtk

* 2019/01/30
  * 增加binding context
  
* 2019/01/29
  * 增加navigator

* 2019/01/28
  * 增加navigator\_request
  * 增加navigator\_handler

* 2019/01/27
  * 增加view\_model

* 2019/01/26
  * 修改字符串翻译的问题。issue 83/86
  * 增加value converter factory
  * 增加value validator factory
  * add expr eval

* 2019/01/25
  * 增加model\_delegate。

* 2019/01/24
  * 增加value\_validator/value\_validator\_delegate

* 2019/01/23
  * 增加value\_converter/value\_converter\_delegate

* 2019/01/22
  * 增加binding rule parse

* 2019/01/21
  * 增加data\_binding/command\_binding.
  * 增加测试程序。

* 2019/01/20
  * 完善object，增加一些包装函数。
  * 增加binding\_rule/binding\_rule\_data

* 2019/01/19
  * 修改valgrind发些的问题，并增加一些脚本。
  * 增加MVVM分支。 

* 2019/01/18
  * 增加文档。

* 2019/01/17
  * 整理API文档：list\_view/list\_view\_h/list\_view\_item/scroll\_view/scroll\_bar
  * 整理API文档：self\_layouter/self\_layouter\_default/children\_layouter/children\_layouter\_default
  * 去掉array类，使用darray类代替。
  * 修改valgrind发现的泄露内存问题。

* 2019/01/16
  * 增加darray类，逐步替换以前的array类。
  * 增加widget pool用于控件的缓存和重复利用，可以通过WITH\_WIDGET\_POOL启用。
  * 增加widget\_create函数，简化子类控件的创建函数，同时让通用控件本身的由widget pool集中管理。
  * 使用array的地方替换为darray。
  * 更新stm32相关工程。

* 2019/01/15
  * 整理API文档：progress\_circle/rich\_text/slide\_menu/slide\_view/svg\_image/switch
  * 整理API文档：text\_selector/time\_clock/digit\_clock
  * 增加progress\_circle c语言demo。
  * 增加slide\_menu c语言demo。
  * 增加slide\_view c语言demo。
  * 增加svg\_image c语言demo。
  * 增加switch c语言demo。
  * 增加text\_selector c语言demo。
  * 增加time\_clock c语言demo。
  * 增加digit\_clock c语言demo。
  * 增加INPUT\_CUSTOM用于关闭系统缺省软键盘。

* 2019/01/14
  * 完善脚本绑定(增加函数和注释)。
  * 整理API文档：color\_picker/gif\_image/guage/guage\_pointer/image\_animation/image\_value/keyboard。

* 2019/01/13
  * 完善脚本绑定(增加函数和注释)。
  * 更新awtk-js。
  * 更新awtk-lua。

* 2019/01/12
  * 重新实现timer，解决timer中打开模态窗口的问题。 
  * 重新实现idle，解决idle中打开模态窗口的问题。 
  * 完善object，在修改属性和销毁时触发事件。

* 2019/01/11
  * 增加slist\_t。

* 2019/01/10
  * 增加object\_t/object\_default\_t/named\_value\_t等几个类。
  * value\_t支持object。

* 2019/01/09
  * 完善slide menu，增加mask color，实现两边的按钮淡出的效果。
  * 增加canavs widget的demo
  * 整理API文档：canvas\_widget

* 2019/01/08
  * 完善text\_selector，增加mask color，实现伪3D效果。
  * 修改font size的类型为font\_size\_t。
  * 修改agge scan line为负的BUG(感谢陈谭提供补丁)。
  * 按钮支持长按事件。

* 2019/01/07
  * 修改"Copyright (c) 2018 - 2018"为"Copyright (c) 2018 - 2019"
  * 整理API文档：tab\_control/tab\_button/tab\_button\_group。

* 2019/01/04
  * slide\_menu动态决定可见子控件的个数(由长度与宽度比例和子控件个数决定)。
  * 修改combobox的BUG。

* 2019/01/03
  * 同步SDL代码。
  * 整理API文档：timer/idle
  * 用valgrind测试内存问题，并修改progress\_circle中的内存泄露问题。
  * 修改桌面版本窗口resize的问题。
  * 密码编辑器增加显示/不显示明文的开关。
  * 键盘增加关闭按钮。
  * 修改把控件状态改成字符串导致的BUG。
  * 修改处理capslock的BUG。

* 2019/01/02
  * 整理API文档：input\_method/input\_method\_default/input\_method\_sdl/input\_method\_null
  * 整理API文档：input\_engine/input\_engine\_pinyin/input\_engine\_null
  * 更新stm32相关工程。

* 2018/12/31
  * 整理API文档：ui\_loader/ui\_builder/ui\_xml\_writer/ui\_binary\_writer/ui\_builder\_default

* 2018/12/30
  * 整理API文档：style/style\_const/style\_mutable/theme/style\_factory
  * move ui\_loader.c/.h to base目录。

* 2018/12/29
  * 整理API文档：bitmap/image\_loader/image\_manager/image\_loader\_stb
  * 整理API文档：asset\_info/assets\_manager/locale\_info
  * 重构image相关的代码  

* 2018/12/28
  * 整理API文档：glyph/font/font\_loader/font\_manager/font\_loader\_ft/font\_loader\_stb/font\_loader\_bitmap
  * 重构font相关的代码  

* 2018/12/27
  * 整理控件API文档：value/mem/str/wstr/array/wbuffer/rbuffer/rgba/color/color\_parser。
  * 修改SDL输入法的问题(感谢大恒提供补丁)。
  * 完善freetype字体渲染(感谢zjm09提供补丁)。
  * 完善glyph\_cache，允许glyph有不同的实现。 
  * 重构color\_parser并增加单元测试。
  * 把events拆成两部分，基础部分移到tkc中。

* 2018/12/26
  * 为了语义的一致性，把widget的虚函数destroy改名为on\_destroy。on\_destroy只是通知子类控件即将销毁。
  * 重命名NAME\_LEN为TK\_NAME\_LEN。
  * 增加函数：widget\_animate\_value\_to
  * 修改widget\_animator\_time\_elapse计算次数的BUG。
  * 整理控件API文档：color\_tile/calibration\_win/combo\_box\_item/button\_group
  * 整理控件API文档：dialog\_title/dialog\_client/progress\_bar/slider/vgcanvas
  * 支持freetype字体渲染(感谢zjm09提供补丁)。
  * 完善value\_t。

* 2018/12/25
  * 整理控件API文档：row/column/grid/grid\_item/view/group\_box/app\_bar/system\_bar
  * 修改idle\_info\_destroy的BUG(感谢尧燊提供补丁)。
  * 修改timer\_info\_destroy的BUG(感谢尧燊提供补丁)。
  * widget状态由枚举格式改为字符串格式，这样方便第三方控件定义私有的状态。
  * style\_id由枚举格式改为字符串格式，这样方便第三方控件定义私有的style\_id。

* 2018/12/24
  * 整理控件API文档：dialog/popup/window/window\_base

* 2018/12/23
  * 优化。
  * 整理控件API文档：spin\_box/combo\_box
  * 增加combo\_box的C示例代码。

* 2018/12/22
  * 修改combobox事件的BUG(issue #62)。
  * 增加函数widgeti\_get\_prop\_default\_value。
  * 增加widget\_recycle函数，支持控件回收，提高运行速度，减少内存碎片。

* 2018/12/21
  * 整理控件API文档：button/label/image/edit

* 2018/12/20
  * 重新设计layouter的架构，把layouter分成self\_layouter和children\_layouter两个接口，方便扩展支持不同的布局方式(如css flex)。
  * 更新layout的文档，为了保持兼容，旧的layout属性仍然支持，新的代码请参考新的文档[layout](https://github.com/zlgopen/awtk/blob/master/docs/layout.md)。
  * 子控件支持浮动布局，不受父控件的children\_layouter的限制。
  * 支持等宽字体的问题(感谢[zjm1060](https://github.com/zjm1060)提供补丁)。 
  * 修改vgcanvas中w/h为负数时的BUG(感谢朝泽提供补丁)。
  * 完善widget\_clone/widget\_equal/widget\_to\_xml。
  * widget增加sensitive属性。
  * 完善main\_loop\_dispatch\_events，支持按键事件(感谢[zjm1060](https://github.com/zjm1060)提供补丁)。 

* 2018/12/19
  * 修改combobox弹出窗口位置的BUG。
  * 修改widget\_animator\_manager中野指针的问题(感谢陈谭提供补丁)。

* 2018/12/18
  * widget\_destroy自动判断是否需要异步销毁。
  * 修改issue #61(list item销毁时，自动移出定时器)(感谢陈谭提供补丁)。
  * 修改color parser的BUG(感谢朝泽提供补丁)。
  * 修改guage的BUG(感谢朝泽提供补丁)。

* 2018/12/17
  * 抽象layouter接口，方便支持不同的layout算法(如CSS的flex)。(未完)

* 2018/12/16
  * 修改issue #61
  * 主题的背景和边框支持圆角半径round\_radius，仅在定义WITH\_VGCANVAS时有效。

* 2018/12/15
  * 调整目录结构。将base拆分成tkc、base和widgets三个目录。
  * awtk.h包含所有头文件，应用程序只需包含awtk.h即可。
    * 好处：一是使用简单，二是避免漏头文件造成的指针被截断的BUG。
    * 坏处：编译稍慢。对于小型应用程序影响不大。大型应用程序和移植层仍然可以选择只包含需要的头文件。
  * 更新相关工程的代码。

* 2018/12/14
  * 修改layout的BUG。

* 2018/12/13
  * 修改slide menu的BUG(感谢尧燊提供补丁)。

* 2018/12/12
  * 支持任何数值型属性的动画。如x/y/w/h等等。 
  * 完善label，支持动态设置显示字符数，形成动画效果。
  * 修改bgfx闪烁的问题(感谢大恒提供补丁)。
  * 修改canvas的BUG(感谢朝泽提供补丁)。
  * 更新文档。

* 2018/12/11
  * gradiant支持alpha渐变。
  * 完善slide menu控件，增加测试和demo。 
  * 完善digit\_clock，支持YY、MM、DD、HH、mm、dd格式

* 2018/12/08-10
  * slide menu控件。 

* 2018/12/07
  * VGCanvas支持渐变色(GL版本OK，AGGE版本OK，AGG版本暂时不支持)。
  * demoui增加vgcanvas的demo。 
  * draw\_vline高度允许为负。
  * draw\_hline宽度允许为负。

* 2018/12/06
  * 更新文档。
  * 完善scroll view的滚动效果。
  * 修改label中换行的bug。
  * 修改窗口动画时system bar闪烁的BUG。

* 2018/12/05
  * 修改scanline\_adapter<RendererT>::commit的BUG，强制转换导致数据错误覆盖。
  * label支持多行显示(非自动换行，需自动换行请用rich\_text)。

* 2018/12/04
  * 更新stm32相关工程。
  * 完善system\_bar控件。
  * 增加system\_bar控件的使用文档。
  * 窗口管理支持style。有时通过style给窗口管理器设置一张大背景背景，就不需要给窗口和system\_bar分别设置背景图片了。

* 2018/12/02-03
  * 增加system\_bar控件(窗口vtranslate动画还需要相应完善)。

* 2018/12/01
  * 更新js/lua脚本绑定。
  * 增加canvas控件。
  * 增加image\_value控件，可以用图片来显示电池电量、WIFI信号强度和各种数值。

* 2018/11/30
  * 增加一些测试代码。
  * 增加函数widget\_insert\_child/widget\_restack。
  * 用户修改系统时间后，更新定时器的启动时间，否则可能导致定时器长时间不会触发。

* 2018/11/29
  * 修复scroll view中脏矩形的问题。
  * awtk-linux-fb工程支持编译用户的项目。
  * 修改switch控件中存在的问题并优化。
  * 修改layout参数解析中存在的问题。
  * 修改输入法相关的一些BUG。
  * 缺省键盘增加中英文切换按钮。

* 2018/11/28
  * 更新字符串翻译的文档和示例。
  * 键值的前缀有FKEY改为TK\_KEY。
  * 把is\_designing\_window/is\_window移到vtable中。
  * 把window\_manager中输入设备状态相关的代码移到独立的文件中。
  * 修改list\_item中的BUG。
  * 修复issue #50
  * 修复issue #51
  * widget\_invalidate\_force增加一个参数。

* 2018/11/27
  * 更新stm32767/stm429的工程。
  * 更新资源。
  * 修改编译警告。

* 2018/11/26
  * 优化不透明的gif。
  * 修改部分函数widget\_invalidate
  * 增加grid\_item。

* 2018/11/25
  * 支持GIF图片(参考：demos/assets/raw/ui/gif\_image.xml)。

* 2018/11/24
  * svg支持圆弧路径(A/a)

* 2018/11/23
  * 增加svg\_image控件。SVG支持的特性请参考src/svg/README.md。

* 2018/11/22
  * 完善button/checkbutton/comboboxitem对pointer down abort事件的处理。
  * 完善slideview拖动处理。
  * 完善脏矩形。
  * 完善输入法(issue #40)。
  * 完善edit.c，删除字符时触发changing事件(issue #41)。
  * 增加image\_base类，把图片公共属性放到image\_base中，方便实现svg图片。可能导致接口不兼容，请酌情修改。

* 2018/11/21
  * 修改一些编译警告。
  * 修改资源生成工具支持SVG。
  * 修改slide\_view，增加一个拖动阀值，由该阀值和速度共同决定是否切换页面。
  * 增加clip\_board接口、基于SDL和缺省实现。

* 2018/11/20
  * 合并BUG #35的补丁。
  * 合并BUG #25的补丁。
  * bsvg\_draw(基本功能可用, A/S/T路径不支持)
  * guage\_pointer支持bsvg图片。
  * 修改屏幕旋转时，窗口动画混乱的BUG。

* 2018/11/16-19
  * SVG解析器和BSVG(二进制格式的SVG)解析器。
  * bsvggen/bsvgdump等工具。

* 2018/11/16
  * 修改FPS的颜色。

* 2018/11/15
  * 同步bgfx的代码。
  * 修改SConstruct定义SDL2。
  * 修改scroll\_view  #issues/30
  * 修改dirty\_rect的BUG(增加x\_offset/y\_offset引起的)。
  * 完善guage，支持用颜色绘制指针。

* 2018/11/14
  * 完善digit\_clock
  * change  WITH\_NANOVG\_GL to WITH\_NANOVG\_GPU
  * guage拆成表盘和表针两个控件。
  * 新增加几个style：selected\_text\_color、selected\_text\_color和selected\_text\_color
  * 修改资源生成工具的BUG。该BUG导致资源后面有4字节的随机数据(功能不影响正确性)

* 2018/11/13
  * 修改agg/agge为nanovg后端时linecap的BUG。

* 2018/11/12
  * 增加js demos。

* 2018/11/11
  * 完善ProgressCircle控件。
  * base/time.h改名为base/time\_now.h，避免和系统time.h冲突。
  * 更新文档。

* 2018/11/10
  * 增加ProgressCircle控件。

* 2018/11/09
  * 重正编译脚本，以便集成bgfx。
  * 重正vgcanvas，以便集成bgfx。

* 2018/11/08
  * 将icon\_x\_offset/icon\_y\_offset改名为x\_offset/y\_offset，进行整体偏移，效果更好。
  * 修改dirty\_rect的问题。
  * 注释增加@event关键字，用来显示当前控件支持的事件。
  * 完善控件的事件，并增加测试程序。

* 2018/11/07
  * 修改屏幕旋转的BUG。
  * 修改get\_clip的BUG。
  * 更新文档。

* 2018/11/06
  * 更新动画文档。
  * slideview 支持使用图标作为指示器。
  * style加了icon\_x\_offset/icon\_y\_offset，方便实现按下效果。

* 2018/11/04-05
  * 控件动画支持暂停。
  * 控件动画支持时间倍率，让时间变快和变慢。
  * 控件动画支持按名称去开始、暂停、停止和销毁。
  * 更易用的API。请参考widget\_start\_animator/widget\_pause\_animator。

* 2018/11/03
  * 增加date\_time类(嵌入式平台需要调用date\_time\_set\_impl获取时间的函数)。
  * 增加数字时钟控件(ext\_widgets/time\_clock/digit\_clock.c)。

* 2018/11/02
  * 修改tools下的资源生成工具，只在资源变化时才更新资源，以提高开发的效率。
  * update\_res.py增加参数，可以只更新指定类型的资源，以提高开发的效率。
  * update\_res.py放到scripts目录下(可在awtk目录或scripts目录运行)。
  * 增加函数widget\_add\_timer，增加的timer在控件销毁时，自动销毁。
  * 改进button，先收到pointer\_down事件，再收到pointer\_up事件，才触发click事件。

* 2018/11/01
  * 增加style\_mutable，支持动态修改style(仅在编辑模式下启用)。
  * 增强style\_factory，支持不同格式的style。比如想支持CSS，可以提供一个CSS的style\_factory。
  * 更新文档。

* 2018/10/31
  * 修改keyboard崩溃的问题(关闭窗口时自动关闭对应的keyboard)。
  * 修改keyboard刷新的问题(新keyboard和旧keyboard的高度不一致导致的)。
  * 增加tk\_roundi/tk\_clampi。
  * 修3framebuffer的BUG。

* 2018/10/30
  * edit的光标颜色从style(fg\_color)中获取。
  * 修改canvas裁剪算法存在的BUG。
  * 修改valgrind查出来的内存泄露问题。
  * demoui中的仪表增加动画。

* 2018/10/29
  * assets\_manager 支持设置资源路径。
  * font\_manager 支持设置assets\_manager。
  * image\_manager 支持设置assets\_manager。
  * 提供函数widget\_load\_image用于加载图片，实现新控件时请用本函数加载图片。
  * 控件需要的图片、字体和其它资源均与当前窗口关联，方便在实现designer时，designer本身和被编辑的控件使用不同的资源。
  * Windows 支持多线程编译。
  * 将UTF8的源文件转换成UTF8-BOM。
  * 修改编译警告。
  * nanovg-agge使用双线性插值算法(感谢攀峰)。

* 2018/10/28
  * 调整主题的格式：tab\_button处于active时的style，combo\_box\_item/check\_button/radio\_button处于checked时的style，作为不同状态处理，而不是作为不同的style处理。如果有用到，请参考demo做相应修改。
  * 修改valgrind发现的一些内存泄露问题。
  * demoui增加退出应用程序的按钮。
  * 更新文档。

* 2018/10/27
  * 引入style/style\_factory接口，方便实现动态改变style。
  * 实现style\_const兼容以前从配置文件获取而style数据。
  * 重构。

* 2018/10/26
  * 为了避免不必要的误解，xml中不再指定window的name属性，以xml的文件名为准。
  * 修改edit crash的bug。
  * edit支持右对齐(style="number")。
  * Window增加stage属性，用于标识Window的生命周期。
  * Windows下的demo开启一个终端显示LOG信息。

* 2018/10/25
  * 重新实现take\_snapshot，支持特殊的stride。
  * 自动过滤重复的pointer\_move事件。
  * 完善list\_item。
  * vgcanvas支持特殊的stride。

* 2018/10/24
  * 重构blend。
  * framebuffer/bitmap支持line\_length(即stride/pitch)

* 2018/10/23
  * 修改鼠标指针残留的问题。
  * 控件动画支持永久播放，yoyo\_times或repeat\_times设置为0时，视为永久播放。
  * 公开函数widget\_animator\_stop，请确保调用了widget\_animator\_set\_destroy\_when\_done禁止播放完成时自动销毁动画对象。

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
