# 最新动态

2022/04/06
  * 完善软键盘grab_keys=true时物理键盘按键太快丢失按键的问题。

2022/04/03
  * 完善style\_mutable，去掉default style 为NULL时的警告。
  * 完善color\_picker处理alpha通道的问题。
  * 修复当打开了新窗口后旧窗口的软键盘关闭导致窗口动画播放不正确的问题（感谢智明提供补丁）。

2022/04/01
  * 完善给edit、mledit和combo\_box增加了empty\_over状态（感谢雨欣提供补丁）
  * 完善URL解析并增加一些新的schema。
  * 完善ubjson writer

2022/03/31
  * 增加带统计功能的stream （感谢林福提供)
  * 增加移除和替换 qaction 的 action\_darray\_thread（感谢林福提供)
  * 修复combox的鼠标弹起后无法释放拖拽状态的问题（感谢智明提供补丁）。
  * 给edit、mledit和combo\_box增加了empty\_over状态（感谢雨欣提供补丁）
  * 修复demoui中的多行编辑器右侧的字符会被遮挡的问题（感谢兆坤提供补丁）
  * 补充demoui中edit、mledit和combo\_box中empty\_over和over状态的样式 （感谢兆坤提供补丁）

2022/03/30
  * fscript 支持延迟解析函数。
  * 完善fscript局部变量的访问。
  * fscript支持通过require函数加载模块。

2022/03/29
  * 优化fscript局部变量的访问。
  * 更新aworkslp移植代码（感谢文静提供)
  * 修复兼容以前的lcd旋转贴图的断言判断（感谢智明提供补丁）
  * 修复在虚拟机上面lcd_mem字体错位的问题以及兼容以前lcd旋转运行的问题（感谢智明提供补丁）
  * 删除mingw的UNICODE宏和msvc统一宏的版本（感谢智明提供补丁）。

2022/03/28
  * 完善fscript。

2022/03/24
  * 修复切换语言时canvas的font对象被清空导致控件无法正常绘制的问题（感谢雨欣提供补丁） 

2022/03/23
  * 改用mingw默认的宏来定义MINGW宏（感谢智明提供补丁）。
  * 修复在离线画布多次数据回流导致内存泄露的问题（感谢智明提供补丁）。
  * 完善ubjson对字符的处理(感谢攀峰提供补丁)

2022/03/22
  * 修复软键盘让窗口上移后再次弹出软键盘导致窗口动画不正常的问题（感谢智明提供补丁）
  * 修复在 text_edit 在点击和拖拽的时候执行 layout 导致的卡顿问题以及修复翻页不正常的问题（感谢智明提供补丁）

2022/03/21
  * 完善3or5keys文档（感谢雨欣提供补丁）
  * 完善 mingw 编译（感谢智明提供补丁）。

2022/03/20
  * 完善 ubjson\_writer\_write\_object。
  * 完善 fscript 函数len

2022/03/19
  * 完善fscript函数bit\_not
  * 完善fargs\_get\_data\_and\_size
  * 完善object\_date\_time支持foreach。

2022/03/18
  * 完善 tk_\cond\_var\_wait（感谢兆坤提供补丁）。
  * 修复AWTK\_LITE编译问题（感谢智明提供补丁）
  * 完善edit 和 combox 的 over 和 enter 的状态风格切换（感谢智明提供补丁）

2022/03/17
  * 修复离线画布数据回流才可以使用的问题（感谢智明提供补丁）
  * 完善canvas\_measure\_text函数的缓存逻辑优化其速度（感谢智明提供补丁）

2022/03/16
  * 增加通配符匹配函数tk\_wild\_card\_match。
  * 完善canvas\_measure\_text函数的缓存逻辑优化其速度（感谢智明提供补丁）

2022/03/15
  * 增加canvas\_measure\_text函数的缓存逻辑优化其速度（感谢智明提供补丁）

2022/03/14
  * edit 和 combox 的 over 和 enter 的状态风格切换（感谢智明提供补丁）

2022/03/11
  * 修复theme_gen的模板部分编译出错问题（感谢智明提供补丁） 
  * 修复combox在没有默认值的时候创建控件会和mvvm的列表渲染赋值的覆盖问题 （感谢智明提供补丁）
  * 完善3_5key的说明文档（感谢雨欣提供补丁）

2022/03/10
  * 完善fscript debugger
  * 完善line\_number
  * 修复style\_get\_color函数处理渐变色时候崩溃的问题（感谢智明提供补丁）
  * 修复edit控件，输入类型为int/uint/float/ufloat时，edit\_set\_text错误把max当作最大长度使用。

2022/03/08
  * 支持[LCD快速旋转](how_to_use_fast_lcd_portrait.md) （感谢智明提供补丁）。
  * 修复 awtk 在安卓下划线不正常的问题（感谢智明提供补丁）。
  * 修复窗口管理宽高没有对话框的大时候播放对话框的窗口动画会导致断言的问题（感谢智明提供补丁）。
  * 修复控件动画停止时候导致裁剪区不正常的问题（感谢智明提供补丁）。

2022/03/04
  * 修复 icon\_at 靠边不正常的问题（感谢智明提供补丁）。
  * 修改csv中的内存泄漏（感谢林福提供补丁）。
  * 修复控件动画修改坐标和宽高导致残影的问题（感谢智明提供补丁）。

2022/03/03
  * 修复 icon\_at 靠边不正常的问题（感谢智明提供补丁）。

2022/03/02
  * AWorks的 serial\_helper 增加设置超时的接口（感谢林福提供补丁）。
  * image\_value 控件添加了 -+ 号的映射（感谢雨欣提供补丁）。
  * 完善widget\_set\_pointer\_cursor，实时生效。

2022/02/26
  * draggable 支持拖动原生窗口。

2022/02/25
  * 增加函数combo\_box\_has\_option\_text
  * 完善themegen （感谢兆坤提供补丁）。

2022/02/24
  * 增加函数str\_format/str\_append\_format。

2022/02/23
  * 修复导出IDL文件的时候因为查找不到依赖的类导致出现警告（感谢智明提供补丁）。
  * 集成了IoT.js，所以同步更新app\_helper\_base.py(感谢朝泽提供补丁)
  * 修复编译警告（感谢智明提供补丁）。

2022/02/22
  * 完善API注释。
  * 完善fscript调试器。
  * 统一lcd旋转的方向为逆时针和增加相关接口的说明（感谢智明提供补丁）。

2022/02/21
  * 修复fscript global对象初始化的问题。
  * 修复value\_int64/value\_uint64对大数的处理。

2022/02/18
  * 增加函数fscript\_set\_global\_object。

2022/02/16
  * 增加函数ui\_loader\_load\_widget\_from\_xml。
  * 增加 vg 加载字体的错误判断和提示 （感谢智明提供补丁）。

2022/02/15
  * conf对象支持foreach\_prop。
  * conf支持创建子对象。

2022/02/14
  * 完善combo\_box\_ex（感谢培煌提供补丁）

2022/02/11
  * 修复csv中的内存泄漏(感谢兆坤提供补丁)

2022/02/10
  * 完善str\_append\_json\_str。
  * 完善API注释(感谢朝泽提供补丁)
  * color\_tile 支持渐变色（感谢智明提供补丁）
  * 完善canvas\_draw\_image\_at（感谢智明提供补丁）
  * 增加输入框中支持按键和鼠标输入保持光标常亮的功能（感谢智明提供补丁）

2022/02/08
  * 增加widget\_find\_parent\_by\_type/widget\_find\_parent\_by\_name。

2022/02/07
  * 合并对环形进度条的优化。

2022/02/04
  * 增加函数path\_basename\_ex。

2022/02/02
  * 完善fscript语法错误检查。

2022/01/29
  * 完善fscript debugger

2022/01/25
  * 完善离线画布（感谢智明提供补丁）

2022/01/24
  * 完善fscript debugger
  * 优化tk\_strncpy（感谢智明提供补丁）
  * 增加slider增加修改前景色的线帽形状属性（感谢智明提供补丁）
  * 修复demoui界面上有edit控件且控件中存在文本时切换主题会打印警告的问题（感谢兆坤提供补丁）
  * 完善demoui（感谢兆坤提供补丁）

2022/01/23
  * 完善fscript debugger,支持attach/launch两种启动方式。

2022/01/22
  * line_number支持设置高亮行。
  * 完善fscript debugger

2022/01/21
  * 完善fscript debugger并增加fdb命令行调试工具。
  * 完善aworks平台的serial\_helper.c（感谢文静提供）

2022/01/20
  * 增加fscript的构造函数和析构函数（感谢智明提供补丁）

2022/01/19
  * 完善编译脚本（感谢雨欣提供补丁）
  * 完善fscript调试器(支持step in/step out/step over/reload code)。

2022/01/16
  * 完善fscript支持调试器。

2022/01/13
  * 优化tk\_object\_get\_child\_object（感谢雨欣提供补丁）

2022/01/12
  * 完善 awtk 退出流程（感谢雨欣提供补丁）

2022/01/11
  * 修复awtk_main中reboot残余代码导致嵌入式系统编译失败的问题。
  * 修复WITHOUT_FSCRIPT没有生效的问题。
  * 完善fscript表达式。

2022/01/10
  * 完善fscript if/elseif/else语句

2022/01/08
  * fscript的log函数改名为logf，避免与flow中的log重名。
  * fscript增加date\_time\_get\_prop/date\_time\_set\_prop等函数。 

2022/01/07
  * fscript增加str\_is\_empty/str\_len/array\_is\_empty/array\_size等函数。
  * object\_typed\_array支持通过TK\_OBJECT\_PROP\_SIZE获取元素个数。
  * object\_typed\_array支持通过set\_prop/get\_prop访问元素。
  * fscript支持for/for\_in语句。

2022/01/06
  * fscript增加random0to1。
  * fscript支持until/repeat_times/repeat语句。
  * fscript中增加is\_int/is\_odd/is\_even等函数
  * 还原awtk\_main.inc文件的循环代码（感谢智明提供补丁）
  * log后调用fflush（感谢福明提供补丁）

2022/01/05
  * fscript公开函数value\_id/value\_func。
  * fscript增加log10/pow10等函数。
  * 修复在多次半透叠加后数据不正常的问题（感谢智明提供补丁）
  * 修复注释错误(感谢朝泽提供补丁)
  * fscript中三角函数增加以度数为单位的相应函数。

2022/01/04
  * fscript支持保存函数名，方便调试。

2022/01/02
  * 修复fscript中while语句对return的处理。
  * 修复emitter被覆盖的问题（感谢智明提供补丁）

2022/01/01
  * 优化fscript函数调用。
  * 公开结构fscript\_func\_call\_t。
  * fscript 支持设置错误处理函数。
  * fscript 增加函数sleep\_ms。

2021/12/31
  * fscript允许重载print函数。
  * 修复emitter的事件中删除事件同时递归触发事件导致emitter不正常的问题（感谢zxc3489和智明提供补丁）
  * 修复使用thread\_with\_pthread时，线程运行完毕后调 用join函数出现内存泄漏的问题 (感谢雨欣提供补丁)

2021/12/29
  * 修复 fscript 中 unset 的问题。
  * fscript 增加 f64/double 数据类型转换函数。
  * 允许通过 widget\_get\_prop 获取 loading 属性（感谢兆坤提供补丁）
  * 修复t9ext输入法中输入完成后Enter键失效的问题（感谢雨欣提供补丁）
  * 解决 combo\_box 设置 text 后，select\_index 没有跟着变化的问题（感谢兆坤提供补丁）

2021/12/28
  * 完善 demoui（感谢兆坤提供补丁）。
  * 完善 serial\_helper.c（感谢朝泽提供补丁）

2021/12/27
  * 修复 T9Ext 输入法，输入 1 后删除 1，但是候选栏没有关闭的问题。
  * 修复 demoui kb\_ascii 软键盘输入内容错误问题（感谢兆坤提供补丁）。
  * 将 page\_tab\_ctrl 中的 view 改为 tab\_control，让 xml 更具有可读性（感谢兆坤提供补丁）。
  * 修复 mledit 选择文本时鼠标指针拖到滚动条上的滑块后，再拖回 mledit 时无法继续选择文本的问题（感谢兆坤提供补丁）。
  * 修复 SDL\_DestroyWindow 后 SDL\_GL\_DeleteContext 无法正常释放导致泄漏的问题（感谢朝泽提供补丁）
  * 修复 data\_reader\_factory、data\_writer\_factory 重复 register 会引起内存泄漏的问题（感谢朝泽提供补丁）

2021/12/26
  * 增加函数 array\_reverse
  * 增加 fscript 函数 array\_reverse

2021/12/25
  * 增加函数 str\_reverse/str\_count。
  * 增加 fscript 函数 text\_reverse/text\_count。

2021/12/24
  * 修复 mingw 编译说明文档（感谢智明提供补丁）
  * 修复 SDL 释放的时候内存泄露的问题（感谢智明提供补丁）
  * 增加状态机（感谢林福提供补丁）
  * 修复单元测试：兼容非 utf8 编码的编译器（VS2015）（感谢雨欣提供补丁）
  * 完善部分 cast 函数的文档。

2021/12/23
  * 增加 aworkslp 平台支持（感谢林福提供补丁）
  * fscript 增加 code\_id，方便以后支持调试。
  * 完善 fscript widget 扩展中的定时器相关接口（感谢雨欣提供补丁）。
  * 修正 Win32 串口操作速度被限制的问题（感谢陈谭提供补丁）

2021/12/22
  * 复原被覆盖的 tools/idl\_gen/README.md（感谢雨欣提供补丁）
  * 增加 tkc/sha256.h

2021/12/21
  * 增加小工具 expand\_text。请参考 [fongen 的说明](../tools/font_gen/README.md)

2021/12/20
  * 修复在 mvvm 中 edit 控件输入值超过限定值后没有 通知数据层修改数据的问题（感谢智明提供补丁）
  * fscript 支持全局变量，可在多个脚本之间共享数据 [请参考](docs/fscript.md)

2021/12/19
  * 支持 4bits 的点阵字体。请参考 [fongen 的说明](../tools/font_gen/README.md)

2021/12/18
  * 修复圆角矩形在画小圆形的时候上下边缘抗锯齿效果不好的问题（感谢智明提供补丁）
  * 修复 edit 和 slider 以及 mledit 的 inputing 的状态切换的问题 （感谢智明提供补丁）

2021/12/16
  * 修复在没有窗口动画的时候使用高亮对话框会导致 system\_bar 画面不正常的问题 （感谢智明提供补丁）
  * 修复 hscroll\_label 野计时器的问题 （感谢智明提供补丁）

2021/12/15
  * 工具 font\_gen 支持 [指定字符的范围](../tools/font_gen/README.md)
  * 增加函数 edit\_set\_double\_ex。
  * 修复 cairo 作为 vgcanvas 时的警告（感谢智明提供补丁）
  * 修复无文件系统时多国语言初始化失败的问题 （感谢雨欣提供补丁）

2021/12/14
  * 增加工具 font\_dump
  * 修复 hscroll\_label 控件失去焦点后停止滚动的问题（感谢智明提供补丁）

2021/12/11
  * 增加 tk\_str\_is\_in\_array。
  * 增加 tk\_utf8\_trim\_invalid\_char。
  * 完善 project.json（感谢俊杰提供补丁）

2021/12/09
  * 完善 json 解析对整数的处理。
  * 修复 guage\_pointer 控件锚点设置为整数时，打印异常警告的问题 （感谢雨欣提供补丁）
  * 将 combo\_box 与 combo\_box\_ex 的 layout 函数整合了一下，共用一个函数（感谢雨欣提供补丁）

2021/12/08
  * 修复 combo\_box\_ex 打开默认弹窗时 layout 不正确的问题 （感谢雨欣提供补丁）
  * 增加 widget\_move\_to\_center。

2021/12/02
  * 完善 API 注释（感谢朝泽提供补丁）

2021/12/01
  * 将 tkc 放到独立的仓库（以后通过 sync\_tkc.sh 同步 tkc 代码）。
  * edit 增加事件 EVT\_IM\_ACTION（感谢兆坤提供补丁）
  * 修复 awtk 的 nanovg\_plus 的获取当前裁剪区不正常的问题 （感谢智明提供补丁）
  * 修复 widget\_set\_as\_key\_target 递归各个父集后导致全屏刷新的问题 （感谢智明提供补丁）
  * 修复 int16\_endian\_invert（严重 BUG，请更新代码）。

2021/11/29
  * 完善 event\_from\_name（感谢兆坤提供补丁）
  * 完善 pages 方便绑定 MVVM  （感谢兆坤提供补丁）

2021/11/27
  * 完善 lcd\_vgcanvas，部分函数完成后清除路径。
  * 增加 point\_init/pointf\_init。

2021/11/26
  * edit 回车切换到下一个焦点。
  * 修复无文件系统下切换主题失败的问题 （感谢雨欣提供补丁）

2021/11/25
  * 修复部分宏拼写错误（感谢兆坤提供补丁）
  * 修复 API 注释错误。

2021/11/24
  * demouiold 增加 slider 示例（感谢雨欣提供补丁）
  * 修复触发按钮长按后导致丢失 up 消息的问题（感谢智明提供补丁）

2021/11/23
  * 完善 slide\_indicator 支持半透动画效（感谢智明提供补丁）
  * 完善资源脚本：修正了打包流图文件时生成的 inc 文件有误和没有 include 到代码里的问题（感谢朝泽提供补丁）
  * 增加 hscroll label 滚动速度设置，并在 demoui old 增加示例（感谢 yuanyuanyuanquan/兆坤提供补丁）。
  * 修复 在控件销毁后，定时器没有销毁而产生的警告 问题（感谢兆坤提供补丁）。
  * 修复 hscroll label 设置为往返滚动（yoyo）时，在滚动间隙时间会回到初始位置问题（感谢兆坤提供补丁）。 
  * 修复 offset\_change\_event 一些注释错误（感谢培煌提供补丁）

2021/11/22
  * 完善 fscript 的文档（感谢福明提供补丁）

2021/11/20
  * 增加函数 darray\_push\_unique
  * fscript 支持中文函数名和变量名。
  * 增加 slide\_indicator 控件的过渡效果（感谢智明提供补丁）

2021/11/19
  * 将控件属性动画前缀从 animate. 改为 animate:（感谢兆坤提供补丁） 
  * demoui 在 stm32f429 运行时，程序会先看到主界面闪过后再看到加载界面的问题（感谢兆坤提供补丁） 
  * 增加修改 scroll\_bar 翻页动画时间（感谢智明提供补丁）。

2021/11/17
  * 增加 tk\_min\_int/tk\_max\_int/tk\_max\_float/tk\_min\_float。

2021/11/16
  * 修复使用 fscript\_ext 时 rbuffer 头文件未定义导致的编译警告，新增文档 fscript\_ram\_req（感谢雨欣提供补丁） 
  * 修复 AWTK\_LITE 编译出错的问题 （感谢智明提供补丁）。

2021/11/15
  * 增加 hash table。

2021/11/11
  * 修复在 fb 为 bgra8888 格式时半透明边缘出现明显边缘的问题以及前景和背景色都为半透明时候颜色不正常的问题（感谢智明提供补丁）。

2021/11/10
  * 增加 widget\_get\_value\_int/widget\_set\_value\_int/widget\_add\_value\_int
  * 修复截屏出现显存泄露的问题（感谢智明提供补丁）。
  * 完善 rlog（感谢林福提供补丁）。

2021/11/09
  * 修复 mledit oy 计算不正确的问题 （感谢兆坤提供补丁） 
  * fscript 支持多国语言互译和主题切换（感谢雨欣提供补丁）
  * 修复 mledit 在 overwrite 模式下鼠标拖动，mledit 没有一起动的问题。（感谢兆坤提供补丁）
  * 新增了主题改变事件结构，并让 widget\_set\_theme 接口支持无文件系统时切换主题 （感谢雨欣提供补丁）

2021/11/08
  * 优化 OpenGL 每次全屏 clear 的逻辑（感谢智明提供补丁）。
  * 增加查询上一级目录的资源逻辑（感谢智明提供补丁）。
  * fscript 中变量名以英文点开头时报错并将其直接当做字符串处理（感谢雨欣提供补丁）
  * 修复 tab\_button\_group 由小变大后无法复原子控 件的位置的问题（感谢智明提供补丁）。

2021/11/05
  * 增加了 nanovg 获取当前裁剪区的函数和增加矩形是否在裁剪区的函数以及统一修改相关判断是否在裁剪区的代码（感谢智明提供补丁）
  * 修复在裁剪区下和高分屏情况下字体无法裁剪的情况（感谢智明提供补丁）。
  * 修复离线画布和切换主题同时释放导致位图数据 多次释放的问题（感谢智明提供补丁）。
  * 取消 nanovg 的脏矩形机（感谢智明提供补丁）。
  * 修改 text_selector 没有选项时打印日志的问题 （感谢智明提供补丁）。

2021/11/03
  * 增加 fs\_foreach\_file
  * 完善 demoui 界面（感谢兆坤提供补丁）
  * 修复 mledit oy 不正确的问题（感谢兆坤提供补丁）
  * 修复在 linuxfb 平台，mledit 下按 capslock 键会输入字符的问题（感谢兆坤提供补丁）
  * 修复在 linux 和 mac 平台，大键盘上的特殊字符（！@#￥）无法输入到 mledit 问题（感谢兆坤提供补丁）

2021/11/02
  * 增加 tk\_str\_end\_with
  * 修复 json 解析对 null 的处理。
  * 增加 str\_append\_int64/str\_append\_uint64
  * 完善 demoui/demouiold 动画演示（感谢兆坤提供补丁）
  * 修复 list 下 self\_layout 不能自动布局的问题（感谢智明提供补丁）。
  * 修复 lcd\_vgcanvas 画线颜色不正确的问题（感谢智明提供补丁）。

2021/11/01
  * 修复 agge 编译缺失的问题（感谢智明提供补丁）。
  * 修复当字库名字超过 32 后导致不断添加新类库的问题（感谢智明提供补丁）。

2021/10/30
  * 完善 RTT mutex 封装。
  * 完善 awtk\_main.inc，通过命令行参数决定是否打开控制台（感谢朝泽提供补丁）。
  * 完善 value\_anim 功能演示 demo 的补丁（感谢兆坤提供补丁）
  * 增加 nanovg_plus，优化 nanovg 性能（感谢智明提供补丁）。

2021/10/29
  * 修复两个窗口同时打开时候会导致第一个窗口丢失 EVT\_WINDOW\_OPEN 消息（感谢智明提供补丁）
  * 修复 dialog\_toast 在窗口动画比较慢的同时快速点击导致出现崩溃问题和窗口动画比 dialog_toast 要慢会出现断言的问题 （感谢智明提供补丁）

2021/10/27
  * 修复函数注释不正确的问题（感谢智明提供补丁）
  * 完善 candidates\_calc\_child\_width，去掉常数设置，完全由候选字按钮的 style 控制。

2021/10/26
  * 完善 socket 意外断开处理。
  * 增加文档 [如何在改变控件属性时附带动画](how_to_change_prop_with_animation.md) （感谢兆坤提供补丁）
  * 修改 demouiold，value_anim 功能演示改为键盘退出动画结束后再修改值（感谢兆坤提供补丁）
  * 完善 mledit\_insert\_text 函数 （感谢兆坤提供补丁）

2021/10/25
  * gif image 增加 loop 属性。
  * 完善 value\_float（感谢周聪提供补丁）

2021/10/23
  * 完善 draggable，可以拖动任意层级的父控件。
  * Android/iOS 调用 exit 退出进程。
  * 增加函数 str\_common\_prefix。

2021/10/22
  * 修改 value 值动画效果 （感谢兆坤提供补丁）
  * 完善离线 canvas （感谢智明提供补丁）

2021/10/21
  * 修改 T9 输入法某些情况下，编辑器不能退出预编辑状态的问题。
  * 修复 edit 在不可视状态下，预编辑时光标位置错误问题（感谢 zxc3489 和兆坤提供补丁）。
  * 修复 edit 在不可视状态下，预编辑时失去焦点，预选字母没有变回、*号（感谢 zxc3489 和兆坤提供补丁）。
  * 修复 mledit 按 tab 键没有触发 value_changing 事件的问题（感谢兆坤提供补丁）。

2021/10/20
  * 完善极简键盘支持，增加 [activated](how_to_support_5keys_3keys.md) 状态。
  * 修复在 agge 下程序窗口缩放有概率导致程序崩溃的问题（感谢智明提供补丁）
  * 修复警告和程序窗口缩放导致 layout 属性被修改的问题 （感谢智明提供补丁）
  * 修复释放位图时候 vgcanvas\_asset\_manager 已经被释放的问题 （感谢智明提供补丁）
  * 修复长按打开新窗口时，旧窗口收不到 pointer up 事件的问题。

2021/10/19
  * 完善 fscript，增加函数 get。
  * 完善 fscript，增加函数 get\_last\_error。
  * 完善 fscript 中获取前缀为、$的变量，若变量不存在，则返回无效变量（感谢雨欣提供补丁）

2021/10/18
  * 修改编译警告（感谢朝泽提供补丁）
  * 优化 line parser（感谢智明提供补丁）

2021/10/16
  * 增加 WITH\_DEFAULT\_IM，方便在 android 上使用 AWTK 内置输入法。

2021/10/15
  * 完善 str\_from\_wstr\_with\_len。
  * 增加 vgcanvas\_draw\_image\_repeat（感谢智明提供补丁）
  * 完善 mledit 虚拟高度计算错误问题（感谢兆坤提供补丁）
  * 修复在 opengl 下离线画布不回传数据不能显示的问题 （感谢智明提供补丁）
  * 完善 mledit 修复多行编辑器改变字体时编辑器没有重新布局的问题（感谢兆坤提供补丁）

2021/10/13
  * 增加函数 str\_append\_n\_chars
  * 增加函数 data\_writer\_write\_all

2021/10/12
  * 修复透明渐变窗口动画影响到顶层悬浮窗口的问题（感谢智明提供补丁）
  * 增加修改 sdl 窗口 title 的接口的功能（感谢智明提供补丁）
  * 增加左右滚动是否禁止动画的功能（感谢智明提供补丁）
  * 修复 demoui 在 320\*480，agge 模式下，字体超出按钮范围的问题（感谢兆坤提供补丁）

2021/10/11
  * 完善 app\_help\_base，修复 mvvm-js 项目编译时找不到 jerryscript 路径的问题（感谢雨欣提供补丁）
  * 启用 fscript\_ext 时需定义宏 WITH\_DATA\_READER\_WRITER （感谢雨欣提供补丁）
  * 完善 mledit（感谢兆坤提供补丁）。
    * mledit 启动覆盖行时，选中文本不对的问题。
    * 追加功能：在 mledit 启动覆盖行时，滚动条值不为最大时，mledit 偏移位置则不随光标位置改变。
  * 修复当内存不足的时候导致 image\_value 的 bitmap 被释放导致断言的问题（感谢智明提供补丁）

2021/10/09
  * 修复拖拽控件在拖拽时且控件不移动时，没有发送 drag 事件（感谢兆坤提供补丁）

2021/09/30
  * 修复 demoui 内存泄露和分辨率适配的问题（感谢兆坤提供补丁）

2021/09/29
  * 修改 OpenGL 宏（感谢智明提供补丁）
  * AWTK-WEB 支持多主题（感谢兆坤提供补丁）
  * 修复圆角矩形渐变偏移不正常的问题（感谢智明提供补丁）

2021/09/28
  * 修复控件使用圆角矩形后半透明的问题（感谢智明提供补丁）

2021/09/25
  * 增加 object\_create\_ex

2021/09/24
  * 完善文档 [渐变色](gradient.md)（感谢雨欣提供补丁）

2021/09/22
  * 完善 object\_get\_child\_object（感谢雨欣提供补丁）
  * 完善 android 版本 demo（感谢兆坤提供补丁）
  * 完善 ios 版本。
  * 增加 object\_widget，把 widget 包装成 object，方便在 fscript 中访问 widget 的属性（感谢雨欣提供补丁）

2021/09/19
  * 修复 cppcheck 的一些警告。

2021/09/18
  * 完善 func\_eq（感谢雨欣提供补丁）
  * 取消升级 gtest。
  * 修复 asset\_loader\_zip 在 Windows 上的问题。

2021/09/16
  * 解除 assets-1m.c 和 demo\_ui\_app.c 之间的耦合（感谢兆坤提供补丁） 
  * object\_array\_set\_prop 函数，在 name 属性不符合要求时，返回设置失败（感谢雨欣提供补丁） 
  * 还原 pc 的编译优化等级（感谢智明提供补丁） 
  * 修改 gradient.c 文件为 inc（感谢智明提供补丁） 
  * 修复 overlay 窗口打开后导致之前的窗口切换到后台而被挂起的问题（感谢智明提供补丁） 

2021/09/15
  * 升级 gtest。
  * strgen 工具生成国际化字符串时保留文本两端的空格。
  * 修改自动生成资源文件脚本，分为 assets、assets\_old 和 assets\_all，assets 包含新 demoui 的资源，assets\_old 包含旧 demoui 的资源，assets\_all 包含所有资源（感谢兆坤提供补丁）。
  * 将 demo\_ui\_ex\_app.c 改名为 demo\_ui\_app.c，而之前的 demo\_ui\_app.c 改名为 demo\_ui\_old\_app.c（感谢兆坤提供补丁）。
  * 新 demoui 增加渐变色按钮示例（感谢兆坤提供补丁）。
  * 补充新 demoui 键盘中文翻译，修改 kb\_foo 样式（感谢兆坤提供补丁）。
  * 修复 gtest 编译警告（感谢智明提供补丁）
  * 修复编译警告（感谢智明和兆坤提供补丁）
  * 修复编译 demoui 警告（感谢兆坤提供补丁）
  * 完善 draw\_rounded\_rect（感谢智明提供补丁）

2021/09/14
  * 根据新增接口，完善 object\_array 以及 object\_default（感谢雨欣提供补丁）
  * 完善 progress\_circle 控件，兼容旧版的 unit 属性用法 i（感谢雨欣提供补丁）
  * 完善 progress\_bar 控件，format 属性与 designer 显示效果保持一致（感谢雨欣提供补丁）
  * 新增 object\_get\_child\_object 接口，方便 AWTK 内部的 object\_t 派生类和 mvvm 直接调用（感谢雨欣提供补丁）
  * 修复 object default 中的内存泄漏。
  * 完善 widget\_set\_as\_key\_target，修复 style 没有更新的问题。
  * 去掉 tk\_utf8\_from\_utf16\_ex 中不必要的警告。
  * 支持 style 支持 [渐变色](gradient.md)（感谢智明提供补丁）

2021/09/13
  * 修改 canvas\_draw\_image 改成用 rectf 类型（感谢智明提供补丁）
  * 增加宏 WITH\_GPU/WITH\_GPU\_GLES2（感谢智明提供补丁）

2021/09/11
  * 增加 object\_default\_create\_ex 用于禁止路径访问属性。

2021/09/10
  * 完善 demo\_ui\_ex\_app（感谢兆坤提供补丁）。
  * mledit 控件增加追加文本、输入模式（超出最大行数后移除旧的行）功能（感谢兆坤提供补丁）。
  * fscript\_object 中新增了 object\_forward\_events 接口，并完善了单元测试和说明文档（感谢雨欣提供补丁）
  * object\_array 添加事件触发，用于 mvvm 更新界面，并且在 set\_prop 和 get\_prop 函数中增加路径处理，向下查找一层，以便 mvvm 列表渲染逐层访问属性（感谢雨欣提供补丁）
  * 增加 WITHOUT\_FSCRIPT，方便低端平台禁用 fscript（感谢智明提供补丁）
  * object\_array/object\_default 增加 exec/can\_exec（感谢雨欣提供补丁）
  * edit 显示支持 bidi。

2021/09/09
  * fscript 增加 json\_save\_to\_string（感谢雨欣提供补丁）
  * 完善 fscript 文件系统相关接口（感谢雨欣提供补丁）
  * 完善 thread\_with\_sdl 中的 mutex（感谢雨欣提供补丁）
  * 完善 fscript 的文档（感谢雨欣提供补丁）
  * 增加 conf\_doc\_get\_ex 和 conf\_doc\_find\_node。

2021/09/08
  * 增加 image\_manager\_set\_max\_mem\_size\_of\_cached\_images 用于限制最大图片缓存占用的内存。
  * window\_open 支持打开本地 xml 文件。

2021/09/07
  * 完善 fscript，没有定义的变量，不再当作字符串。 
  * 完善 utf8（感谢智明提供补丁）

2021/09/06
  * inline style 改用"."分隔，仍然兼容":"，建议用新的方式书写。请参考 [样式](theme.md)
  * 重新开发 demoui（之前的改名为 demouiold)（感谢兆坤提供补丁）。
  * 完善 demoui（感谢兆坤提供补丁）。
  * 增加 wcsncpy（感谢智明提供补丁）
  * 修复嵌入式平台编译出错问题（感谢智明提供补丁）

2021/09/04
  * 用 darray 重新实现 object\_default。
  * 增加 darray\_find\_ex/darray\_find\_index\_ex/darray\_remove\_ex。

2021/09/03
  * 修复编译警告（感谢智明提供补丁）
  * 完善 list\_view\_h\_on\_scroll\_view\_layout\_children 让用户自定义 layout 失效的问题。
  * scroll view/hscrollale 不支持 WIDGET\_PROP\_LAYOUT\_W/WIDGET\_PROP\_LAYOUT\_H 属性，因为虚拟高度/宽度本身依赖于 children layout 的结果，children layout 的过程又依赖虚拟高度/宽度，会出现互相依赖。
  * 修复 widget\_use\_style 的问题。
  * button 支持 auto\_adjust\_size。
  * 修复不需要刷新也进入 end_frame 的问题（感谢智明提供补丁）

2021/09/02
  * 修改 idle 和定时器 id 翻转的问题（感谢智明提供补丁）
  * 完善 tick 的处理方法（感谢智明提供补丁）
  * 修改部分时间使用 32 位的问题（感谢智明提供补丁）
  * 修改资源生成脚本不预先加载字体。

2021/08/30
  * 完善 edit\_set\_text 
  * 增加 wstr\_set\_with\_len
  * 修复圆角矩形半透重复绘图的问题（感谢智明提供补丁）
  * 如果 edit 的 keyboard 属性为空字符串，则不弹出软键盘

2021/08/27
  * 增加 general\_factory。
  * 增加 darray\_set/darray\_replace/darray\_sorted\_insert。
  * 重构 widgets\_factory/fscript\_register\_func，有效降低内存使用。

2021/08/26
  * fscript 支持函数定义。
2021/08/25
  * 修改 OpenGL 的 lcd 旋转被裁剪的问题（感谢智明提供补丁）
  * 修复 linux-fb 的 lcd 旋转异常的问题（感谢智明提供补丁）
  * 修复 ctype 函数处理中文的问题。
  * 修复 on\_theme\_changed（感谢兆坤提供补丁）。
  * 修复 scroll\_view\_set\_curr\_page（感谢 Soky 智明提供补丁）

2021/08/23
  * 修复多次调用 lcd 旋转的功能的问题（感谢智明提供补丁）

2021/08/21
  * 修改 lcd\_profile 编译问题。

2021/08/20
  * 修复 scroll_view 在拖拽时且 scroll_view 里有子控件进行重新布局时，画面不正常问题（感谢兆坤提供补丁）。

2021/08/18
  * 修复 label\_resize\_to\_content 导致 confirm/info 等对话框大小计算有误的问题。

2021/08/17
  * 修复设置后台窗口焦点导致的问题。
  * 完善 slide menu（感谢兆坤提供补丁）。
  * 完善 text edit（感谢雨欣提供补丁）。
  * 修复 mono 编译和运行错误的问题（感谢智明提供补丁）
  * 修复 lcd 的脏矩形列表为空时画面不正常的问题（感谢智明提供补丁）
  * 修复 top 的 UI 的图片超过 overlay 导致在 mono 上面有残留的问题（感谢智明提供补丁）
  * 修复 Windows 下 send/recv 无法用 errno 判读是否正常的问题。
  
2021/08/16
  * check button 允许使用 radio 属性控制单选多选。
  * scroll bar 允许使用 is_mobile 属性控制 mobile/desktop

2021/08/12
  * 增加 lcd\_set\_orientation（感谢智明提供补丁）。

2021/08/11
  * 解决使用 self\_layout 后不能移动的问题（感谢智明提供补丁）
  * 修复 lcd\_fb\_dirty\_rects 头文件注释不对的问题（感谢智明提供补丁）
  * 修复 lcd\_sdl 的 lcd 旋转后画面不正常的问题（感谢智明提供补丁）
  * 修复 lcd 旋转导致脏矩形异常后导致中断的问题（感谢智明提供补丁）
  * 修复 vgcanvas\_asset\_manager 的 gtest 失败的问题（感谢智明提供补丁）
  * 修复其他的 layout 调用 widget\_move\_resize 函数的问题（感谢智明提供补丁）

2021/08/10
 * 修复 list\_view 没有滚动条的警告问题（感谢智明提供补丁）
 * 删除不断打印 relayout 的信息的代码（感谢智明提供补丁）
 * 给 text\_selector 增加可以设置是否修改值的时候启动动画（感谢智明提供补丁）
 * 修复 vgcanvas\_asset\_manager 内存泄露的问题（感谢智明提供补丁）

2021/08/06
  * 完善 image value。
  * 完善 awtk\_version.h（感谢智明提供补丁）。
  * 修复 vgcanvas\_cairo\_reset 中设置 global alpha 的问题。

2021/08/05
  * 增加 src/platforms/README.md
  * 增加函数 bitmap_transform

2021/08/04
  * 增加函数 combo\_box\_remove\_option。
  * 完善 demoui（感谢兆坤提供补丁）。

2021/08/03
  * 键盘支持跟随编辑（指定 floating=true 即可）
  * 更新文档。
  * 完善 vpage 事件处理。

2021/08/02
  * 公开函数 timer\_manager\_dispatch。
  * 修复 fragment fb 编译问题（感谢智明提供补丁）。

2021/07/30
  * 完善 ostream 的 fscript 绑定。
  * 完善 pages 和 button group（感谢兆坤提供补丁）。

2021/07/28
  * 完善 slide\_menu（感谢智明提供补丁）。
  * 修改 agge 兼容没有 d3d 会崩溃的问题（感谢智明提供补丁）。
  * 完善 slide view 删除当前 view 的处理（感谢兆坤提供补丁）。

2021/07/27
  * 裁剪了 assets-1m 的资源（感谢智明提供补丁）。
  * 修改 lcd\_mem\_flush 为按照脏矩形列表来拷贝（感谢智明提供补丁）。

2021/07/26
  * 完善脏矩形，以便 2fb 在 swap 的方式也可以支持脏矩形。
  * 完善 pages 和 button group，处理为空的情况（感谢兆坤提供补丁）。
  * 修复 edit input\_type 属性为 int，当 text 长度大于 32 时会有 assert 的问题（感谢兆坤提供补丁）。

2021/07/24
  * 完善和重构 slider。

2021/07/23
  * 完善 fscript 的 iostream\_serial\_create。

2021/07/22
  * 完善 emitter\_disable/emitter\_enable 支持嵌套调用。
  * 完善 switch（感谢陈谭提供补丁）

2021/07/21
  * 修复多个脏矩形时没有修改片段式 Framebuffer 的问题（感谢雨欣提供补丁）
  * 修复 agge 模式下 widget\_take\_snapshot\_rect 的逻辑错误问题（感谢智明提供补丁）。
  * 裁剪了 assets-1m 的资源（因为在 429 上面报 flash 不够大（感谢智明提供补丁））。

2021/07/20
  * fscript 修复对负号的处理。
  * fscript math 增加 d2r/r2d。
  * 完善 slider（感谢兆坤提供补丁）
  * 完善对话框高亮（感谢兆坤提供补丁）
  * 修复窗口创建完后的 loading 状态还是 TRUE 的问题（感谢智明提供补丁）

2021/07/19
  * fscript 增加 usubstr 函数。

2021/07/17
  * 增加 array\_clone\_and\_sort。
  * 增加 lcd\_is\_support\_dirty\_rect（感谢智明提供补丁）
  * 用 WITHOUT\_WINDOW\_ANIMATORS 替代 WITH\_WINDOW\_ANIMATORS（感谢智明提供补丁）。

2021/07/16
  * 增加 fscript str
  * 增加 [字符串扩展模块](fscript_str.md)
  * fscript math 增加 atan2/log/is\_prime/exp 等函数。
  * fscript ext 增加 trim\_left/trim\_right/ulen/totitle/char\_at
  * 完善 mem\_allocator\_std 内存统计（感谢 QQ276310465 提供补丁）
  * fscript array 增加 min/max/avg/sum 等函数。
  * 增加了 get\_offset 接口以及相关的控件（感谢智明提供补丁）
  * 修改 lcd\_mem 的编译警告问题（感谢智明提供补丁）
  * 修复兼容以前开启脏矩形机制，解决以前的三 fb 无脏矩形的情况（感谢智明提供补丁）
  * 修复滚动条在底部时候 layout 会偏移的问题和修复没有 WITH\_WIDGET\_TYPE\_CHECK 宏会导致非 mobile 的滚动条当做 mobile 滚动条的问题（感谢智明提供补丁）
  * 完善 event source manager（感谢林福提供补丁）
  * 完善 widget\_get，如果类型是 wstr，转换成 str。

2021/07/15
  * 完善 widget\_factory，允许覆盖之前注册的控件。
  * 增加 object\_array\_sort 及 fscript 的包装
  * 增加 object\_array\_dup 及 fscript 的包装
  * fscript ext 增加 index\_of/last\_index\_of/char\_at

2021/07/14
  * 增加 object\_array\_shift 及 fscript 的包装
  * 增加 object\_array\_index\_of 及 fscript 的包装
  * 增加 object\_array\_last\_index\_of 及 fscript 的包装
  * 增加 object\_array\_create\_with\_str 及 fscript 的包装
  * 增加 object\_array\_get\_and\_remove 及 fscript 的包装
  * 增加 array\_create\_with\_repeated\_value

2021/07/13
  * 修改编译警告（感谢智明提供补丁）
  * 完善 fscript 文档（感谢攀峰提供补丁）
  * 修复 fscript returned 没有重置的问题（感谢林福提供补丁）。

2021/07/12
  * 完善文档。
  * 优化 layout。
  * 完善 input\_method\_default，支持 T9/T9EXT 的默认键盘。
  * 修复异步 layout 和同步 invalidate 造成部分控件不刷新的问题。

2021/07/11
  * 增加函数 path\_extname\_is 函数。
  * 完善从 zip 文件加载资源的代码、工具和文档。

2021/07/10
  * 将 dirty\_rects 通过 begin\_frame 传递到 canvas 和 lcd。
  * 完善 tab\_button\_group（感谢智明提供补丁）
  * 修复开启了 WITH\_LCD\_CLEAR\_ALPHA 后使用透明窗口后，会导致刷新部分不正常的问题（感谢智明提供补丁）

2021/07/09
  * tokenizer\_next\_expr\_until 添加了对"{}"和"[]"的处理（感谢朝泽提供补丁）
  * 修复在 SDL 的桌面模式下拉伸窗口不正常的问题（感谢智明提供补丁）
  * lcd\_mem 类增加一个 lcd\_mem\_init 函数（感谢智明提供补丁）

2021/07/08
  * 修复不必要的警告（感谢兆坤提供补丁）
  * 导出 lcd\_get\_dirty\_rect（感谢智明提供补丁）
  * 修复限制最高绘图帧率机制的问题（感谢智明提供补丁）
  * 修改 mledit 输入提示对齐的问题（感谢智明提供补丁）。

2021/07/07
  * 窗口完全进入后台时不再绘制。
  * 去除因 hscroll_label 重复增加定时器所导致的警告（感谢兆坤提供补丁）
  * 补充 vpage 在打开页面时的 widget_layout（感谢兆坤提供补丁）
  * 修复 lcd 的 resize 失败后导致的崩溃问题（感谢智明提供补丁）
  * 修复限制最高绘图帧率机制的问题（感谢智明提供补丁）
  * 完善 pthread 实现的 thread 等待时不能及时响应（感谢陈谭提供补丁）。

2021/07/05
 * 完善 release 脚本（感谢朝泽提供补丁）
 * 完善 csv file（感谢雨欣提供补丁）
 * 修复圆角矩形编译警告（感谢智明提供补丁）

2021/07/02
 * 修复延迟动画刷新的问题（感谢智明提供补丁）
 * 修复 agge 的 ras 初始化的值不正常的问题（感谢智明提供补丁）
 * 修复 linux-fb 无法修改 lcd 分辨率问题（感谢智明提供补丁）

2021/07/01
  * 增加 rectf\_t。
  * 完善 switch。
  * 修复部分源文件的 UTF-8 BOM 标志（感谢陈谭提供补丁）。
  * 完善 canvas。如果图片缩放，而且只是绘制部分，为了避免取整出现坐标异位，使用 vgcanvas 绘制。

2021/06/30
  * 将 fps 计算放到 fps.h 中。
  * 还原 vg 资源管理器的宏 （感谢智明提供补丁）
  * 增加 vgcanvas\_set\_fill\_gradient/vgcanvas\_set\_fill\_gradient。

2021/06/29
  * 增加 vgcanvas\_clip\_path 和基于 cairo 的实现。

2021/06/28
  * 增加函数 matrix\_transform\_pointf。
  * 增加一个生成 object 的框架代码的小工具。
  * 完善编辑脚本，APP 支持输出到不同目录（感谢 [ufbycd](https://github.com/ufbycd) 提供补丁）。

2021/06/27
  * 修复输入法软键盘判断坐标的问题。

2021/06/26
  * 完善 edit
  * 完善 offline canvas（感谢智明提供补丁）
  * 修改 ui loader 的内存问题（感谢朱俊提供补丁）
  * 增加 window\_manager\_set\_max\_fps 用于限制最大帧率。

2021/06/25
  * 完善编译脚步（感谢雨欣提供补丁）

2021/06/24
  * 支持多个脏矩形。
  * gauge\_pointer 支持精确脏矩形（可大幅提高 gauge 界面的流畅度）。
  * 页面切换速度比动画速度快时导致显示不正常问题（感谢兆坤提供补丁）。
  * 消除 ui\_loader\_load edit、mledit 所产生的警告信息（感谢兆坤提供补丁）。

2021/06/23
  * 完善 awtk\_config.py（感谢朝泽提供补丁）
  * 完善 awtk\_main.inc（感谢朝泽提供补丁）

2021/06/19
  * 完善 vgcanvas\_asset\_manager（感谢智明提供补丁）

2021/06/18
  * 把截屏函数和离线画布都改用多 vg 的模式（感谢智明提供补丁）
  * 增加 vgcanvas\_asset\_manager 的机制，来保证不同 vg 公用同一份资源时候不会出现问题（感谢智明提供补丁）

2021/06/17
  * 增加 rect\_has\_intersect。
  * 完善 widget\_paint\_with\_clip。
  * 完善 awtk\_main.inc（感谢朝泽提供补丁）
  * 修改控件动画的刷新时间和最高帧率绑定一起（感谢智明提供补丁）
  * 增加 lcd 的接口并且完善其他的类型 lcd 的接口（感谢智明提供补丁）
  * 修复圆角矩形依赖宏来判断 lcd 类型来打点的问题（感谢智明提供补丁）
  * 增加加载没有文件后缀的图片资源（感谢智明提供补丁）
  * 完善 keyboard 对 hard\_key 的支持。

2021/06/16
  * 支持 [极简键盘 (3 键键盘、5 键键盘和旋转按钮）](how_to_support_5keys_3keys.md)
  * edit/mledit 不处理 escape 和功能键。 
  * 修复拼音输入法 assert: https://github.com/zlgopen/awtk/issues/563

2021/06/15
  * edit 不再用左右键切换焦点。 
  * 完善 is\_same\_col/is\_same\_row 的算法。
  * 完善 font manager 字体卸载（感谢智明提供补丁）
  * 完善 canvas\_draw\_text\_bidi\_in\_rect 对空行的处理。

2021/06/11
  * 修改一些编译警告。 
  * 增加文档 [如何使用 keil 的 AC6 工具链编译](how_to_build_with_keil_ac6.md)

2021/06/11
  * 完善焦点处理。
  * pages 支持 clip。
  * 完善 edit 对自动测试的支持。
  * 重构 clip view，将 clip 函数抽取出来方便重用。
  * 增加 data\_time\_format 注释，导出相关函数（感谢雨欣提供补丁）
  * 修复 widget\_set\_style\_color 函数的说明错误问题（感谢智明提供补丁）
  * 增加 text\_selector 控件设置 value 会触发动画滚动效果（感谢智明提供补丁）
  * 修复 label 显示多行阿拉伯文本的问题。

2021/06/10
  * 完善编辑器对左右键的处理。
  * 修复 vpage 中注释的问题（感谢朝泽提供补丁）   
  * 完善 widget\_on\_keydown，优先处理焦点切换按钮。

2021/06/09
  * 重构 pages。
  * 修复 ui\_builder 不能重入的问题。
  * 增加 window\_manager\_set\_ignore\_input\_events。
  * 增加 vpage 扩展控件（配合 pages 控件使用，可以自动加载/卸载页面，并提供入场/出场动画）

2021/06/08
  * 完善编译脚本（感谢兆坤提供补丁）
  * 完善 slide menu，支持用左右键切换。
  * 修复嵌入式系统硬键盘无法输入的问题。
  * 完善 slide view，垂直 slide view 用上下键切换页面。
  * 完善 slide view，处理滑动太快，中间没有 move 事件的问题。
  * 修复 hscrollable，点击边缘控件没有滚动效果的问题（感谢智明提供补丁）

2021/06/07
  * 重构和完善文档。
  * 完善 mmap.c（感谢陈谭提供补丁）
  * 完善 release 脚本拷贝自定义控件的动态库。
  * self layout default 的百分比使用浮点数。
  * 完善 file browser view 的 value changed 事件（感谢智明提供补丁） 

2021/06/06
  * 重构 widget 部分代码。

2021/06/05
  * 完善 switch 控件。修复滑动太快，pointer down 和 pointer up 之间没有 move 事件的问题。

2021/06/04
  * scroll bar mobile 支持 auto\_hide 属性。
  * 完善 scroll bar mobile 支持圆角。
  * 修复 hscroll\_label 在一开始未显示的 page 中无法自动滚动的问题（感谢智明提供补丁）

2021/06/03
  * 完善 scroll bar。
  * 完善 menu layout。
  * 修改 ubjson\_writer\_write\_key\_len 的问题（感谢攀峰提供补丁）。

2021/06/02
  * 优化 widget\_set\_visible（感谢智明提供补丁） 
  * 限制在 SDL 的 pc 模拟下才可以调用 msize（感谢智明提供补丁） 
  * 增加离线画布在 linux-fb 下支持 24 位色的功能（感谢智明提供补丁）
  * 修改当内存小于 32kb 则不分配内存池，如果内存很少的话，其实没有必要开辟一块内存池了，因为这个时候内存池反而占用了一大块的无法使用的内存碎片了（感谢智明提供补丁）。
  * 去掉不必要的运行时警告信息。

2021/06/01
  * 修复 text selector 通过 widget\_get\_text 返回 NULL 的问题。
  * 完善控件 EVT\_VALUE\_CHANGED/EVT\_VALUE\_WILL\_CHANGE 事件的文档。
  * 完善 slider，支持点击修改。

2021/05/31
  * 完善 vgcanvas\_null/vgcanvas\_nanovg\_soft 的编译宏，方便制作 keil 的 pack（感谢静儒提供补丁）

2021/05/28
	* 增加 log\_dummy 函数，方便在没有真正的 log 函数时设置断点调试。
	* 由于 mem.c 中导出了 malloc 等函数，没有必要重载 new/delete 等操作符，所以去掉 misc/new.cpp|.hpp 中的代码，但为了兼容保留文件。
	* 完善文档。

2021/05/28
  * 修改 scroll view/scroll bar/text selector/switch/slide view，在不启用控件动画时也可以使用。
  * 增加 EXPORT\_STD\_MALLOC，用于在没有定义 HAS\_STD\_MALLOC 时，是否将 AWTK 内存分配函数封装成标准 C 的内存分配函数。
  * 修复在 win7 平台下由于 SDL_Init 和 SDL_OpenAudioDevice 不在同一线程中调用导致音频设备打开失败的问题。（感谢兆坤提供补丁）

2021/05/27
  * 完善 std mem alloctor，增加内存使用统计。
  * 完善自定义控件文档。
  
2021/05/26
  * 修复 gauge\_pointer 控件旋转角度为浮点型（感谢智明提供补丁）。
  * 增加宏 WITHOUT\_WINDOW\_ANIMATOR\_CACHE 用于控制窗口动画是否使用缓存。如果内存紧缺，又希望启用窗口动画，可以打开本宏。

2021/05/25
  * 完善 edit。
  * 增加 ui\_feedback\_deinit。

2021/05/23
  * style 支持 feedback/focusable。

2021/05/22
  * 完善图片解码器，优先使用用户自定义的解码器（感谢智明提供补丁）。
  * stb 支持外部硬件加速 yuv 转 rgb 的宏和函数（感谢智明提供补丁）。

2021/05/21
  * 窗口增加属性 strongly_focus。点击非 focusable 控件时，用于控制是否让当前焦点控件失去焦点。

2021/05/20
  * Window 支持 EVT\_PAINT 事件。
  * 完善 widget\_to\_xml。
  * 完善后台线程 OOM 的处理和测试。

2021/05/19
  * 增加宏 NATIVE\_WINDOW\_NOT\_RESIZABLE，用于禁止用户调整 desktop 应用程序的窗口大小。
  * 修复设置屏保时间为 0 时候不能马上退出屏保的问题（感谢智明提供补丁）

2021/05/18
  * 完善 image 事件处理和测试。
  * 增加函数 widget\_get\_focused\_widget。
  * 为 AWTK\_LITE 优化大小（感谢智明提供补丁）
  * 增加禁止在 mmap 模式下增加外部 include 的资源（感谢智明提供补丁）
  * 完善 gif，增加 gif\_image\_play/gif\_image\_stop/gif\_image\_pause。

2021/05/17
  * 完善文档。
  * 增加 widget\_auto\_scale\_children。
  * 将 widget\_on\_pointer\_xxx\_children 公开。
  * 为 AWTK\_LITE 优化大小（感谢智明提供补丁）

2021/05/16
  * 完善 layout 处理流程。
  * 重构封装获取 lcd 大小的代码。

2021/05/15
  * 完善圆角矩形（感谢智明提供补丁）
  * 修改 label 高度的限制条件。
  * 完善 widget\_set\_need\_relayout。
  * 增加函数 event\_clone
  * 增加函数 widget\_dispatch\_async
  * 异步分发 space/return 触发的 click 事件。

2021/05/14
  * switch 支持 click 事件，支持 space/return 触发。
  * 修复圆角矩形的多次半透叠加后出现颜色不对的问题（感谢智明提供补丁）

2021/05/13
  * 完善和优化 layout 处理流程。
  * 完善 layout 中 auto adjust size 的处理。
  * 完善 event\_source\_manager\_default\_dispatch\_fds（感谢林福提供补丁）。
  * 完善 object\_compositor\_on\_destroy（感谢 zhujun19790920 提供补丁）。

2021/05/12
  * 完善 mledit（感谢兆坤提供补丁）
	* 针对 AWTK\_LITE 优化 canvas.c。
  * 完善 popup，使用 widget\_add\_timer 代替 timer\_add，以便窗口关闭时销毁 timer。

2021/05/11
  * 完善 dialog\_helper。
  * 完善 WINDOW\_CLOSABLE\_CONFIRM 的处理。
  * 增加 widget\_back/widget\_back\_to\_home。
  * 完善定时关闭的 popup: 有用户操作后重新开始计时。
  * 为了避免歧义，将 styles 资源名称由“主题”改为“窗体样式”（感谢雨欣提供补丁）。

2021/05/08
  * 重构 popup 和相关测试。

2021/05/07
  * 完善 mledit（感谢兆坤提供补丁）
  * 重构 button 和相关测试。
  * 重构 check\_button 和相关测试。

2021/05/06
  * 完善 combo\_box/combo\_box\_ex。

2021/04/30
  * image value 支持支持水平和垂直对齐。
  * image value 支持 scale/scale\_w/scale\_h/scale\_auto
  * image value 支持 margin\_left/margin\_right/margin\_top/margin\_bottom。
  * 完善 mledit/line\_number（感谢兆坤提供补丁）

2021/04/29
  * 重构内置对话框的代码。
  * 重构 label 去掉不需要的代码。

2021/04/28
  * 重构 label 并完善对 margin 的支持。
  * 完善 Windows 下应用程序图标的支持（感谢陈谭提供补丁）。

2021/04/27
  * 完善 label 对 auto adjust size 的处理。
  * 完善 line\_parser\_next，让空格在行尾。
  * 完善 label\_get\_text\_line\_max\_w，使用 line\_parser 确保使用同一种算法换行。
  * 修复 windows 下，wstat 无法处理"c:"之类的路径的问题。
  * 修复输入法没有键盘名字导致拷贝报错 （感谢智明提供补丁）
  * 修复全屏 lcd 旋转 270 的时候目标贴图的 buffer 溢出导致崩溃的问题 （感谢智明提供补丁）

2021/04/26
  * 完善 darray（感谢兆坤提供补丁）

2021/04/25
  * 完善内存管理器（感谢王程提供补丁）

2021/04/24
  * 完善内存管理器和测试程序。

2021/04/22
  * 修改内存泄漏问题。
  * 内存管理器支持管理多块不连续的内存。
  * 增加网络接口的命令行工具（感谢忠吉提供）。

2021/04/20
  * 完善 progress cirle demo（感谢雨欣提供补丁）
  * 添加 获取光标位置 接口（感谢兆坤提供补丁）
  * 添加 选中一段文本 接口 （感谢兆坤提供补丁）
  * 添加 获取当前选中的文本 接口（感谢兆坤提供补丁）
  * 添加 滚动到指定偏移位置 接口（感谢兆坤提供补丁）

2021/04/19
  * 修复 bitmap\_clone。

2021/04/18
  * 增加函数 fscript\_syntax\_check（感谢福明提供补丁）

2021/04/17
  * 完善编译脚本（感谢朝泽提供补丁）。

2021/04/15
  * 文件浏览器增加 top\_dir 属性用来限制用户 up 到更上层目录。

2021/04/13
  * 增加函数 data\_writer\_flush。

2021/04/11
  * 完善 fs\_create\_dir\_r。
  * 快捷键格式修改为用下划线连接，如 ctrl_s。（感谢福明提供补丁）

2021/04/10
  * 去掉 value\_t 对齐的限制。

2021/04/09
  * 修复 combox 一开始没有 text 的时候不显示选项的问题（感谢智明提供补丁）

2021/04/08
  * 修复 mutex\_nest 中的 BUG。
  * mutable\_image 增加 mutable\_image\_set\_need\_redraw。

2021/04/07
  * 完善控件 API 注释中的事件（感谢培煌提供补丁）。
  * gauge 拼写错误为 guage，修改成正确拼写。XML 和 API 做了兼容处理，两种写法都可以，新写的代码请用正确的拼写。

2021/04/06
  * 增加 idle\_queue\_ex/timer\_queue\_ex 方便脚本绑定时使用。
  * 增加 hscroll\_label 检测焦点状态下长度变化重新播放动画的机制（感谢智明提供补丁）

2021/04/04
  * 增加 gui\_app\_start\_ex，方便嵌入式系统指定资源路径。
  * 增加 WITH\_MBEDTLS 控制是否编译 stream\_mbedtls。

2021/04/03
  * 修复 T9 测试失败的问题（感谢智明提供补丁）
  * 完善 image animation（感谢 hantianheng 提供补丁，感谢智明验证）

2021/04/01
  * 修复 T9 编译问题（感谢智明提供补丁）

2021/03/29
 * 修复 slider 控件的 bar\_size 属性过大画面不正常的问题（感谢智明提供补丁）
 * 修改 mutable\_image 支持在 desiger 界面上面不刷新的功能（感谢智明提供补丁）
 * 修复 WHEEL\_BEFORE\_CHILDREN 事件无法通过 wheel\_event\_cast 转换的问题（感谢福明提供补丁）
 * 修改调用 edit\_set\_text 等函数后没有重绘的问题。

2021/03/28
  * 修改一些注释错误。
  * 修改部分代码，方便在 Go 语言中调用 (Go 不支持位域，无法访问 union 成员，type 是关键字）。

2021/03/26
 * 完善文档 how\_to\_use\_action\_text.md。

2021/03/25
 * 合并 mbedtls 分支，增加基于 mbedtls 的 io stream
 * 增加文档 [优化 freetype 解析矢量字体的显示效果](optimation_freetype_gray_font.md) （感谢雨欣提供）
 * 增加文档 [优化 stb\_truetype 在 mono 模式下的字体显示效果](optimation_stb_truetype_mono_font.md) （感谢雨欣提供）

2021/03/24
 * 修复 file\_browser\_view 控件返回上级目录时将文件夹显示为文件的问题（感谢雨欣提供补丁）
 * 优化 freetype 获取灰度图字模型的效果，添加 stb 二值化字模的功能并在 mono 模式下默认使用 stb（感谢雨欣提供补丁）

2021/03/19
 * 修复 list view layout 内存泄漏（感谢兆坤提供补丁）
 * 增加 tk\_utf8\_from\_utf16\_ex 和 wstr\_set\_utf8\_with\_len。

2021/03/18
 * 修复在 linux64 位平台下，因为 long 和 int 长度不同 而导致字符串转整形错误的问题（感谢兆坤提供补丁）

2021/03/17
 * 完善 linux-fb 的 egl 适配（感谢智明提供补丁）
 * 增加文档 [在 windows 下使用 gcc 编译 AWTK](mingw.md)
 * 为 progress\_bar 控件和 progress\_circle 控件添加 format 属性，方便用户可以自行格式化显示文本（感谢雨欣提供补丁）

2021/03/16
 * hal 网络接口增加 windows 实现（感谢忠吉提供）

2021/03/14
 * 增加 [HTML View](https://github.com/zlgopen/awtk-widget-html-view)

2021/03/13
 * 修复 label\_get\_text\_line\_max\_w 接口在 tr\_text 属性为空调用失败的问题（感谢雨欣提供补丁）

2021/03/10
 * 增加输入类型 INPUT\_ASCII，用于输入纯英文字符。
 * 修复 font\_manager\_unload\_font 在 OpenGL 模式下不能卸载字体的问题。
 * 修复 dialog\_info 等内置对话框显示翻译文本时 label 大小异常的问题（感谢雨欣提供补丁）

2021/03/07
 * 用 WITH\_SDL 代替 SDL2，没有必要同时使用两个宏。
 * 完善 window\_manager\_back\_to\_home\_async，处理遇到模态对话框时触发 assert 的问题。
 * 修复 window\_manager\_default vtable 初始化顺序。

2021/03/06
 * 完善 build.json，支持多主题。
 * 完善编译脚本，支持 rpath。

2021/03/04
 * 修复在 mac 上面的 OpenGL 模式下 lcd 旋转窗口动画的问题 （感谢智明提供补丁）
 * 修改 list\_view 设置 scroll\_bar\_mobile 的虚拟高最低为 widget 高度（感谢智明提供补丁） 
 * 增加右键和中键的触发事件（在 demoui 中加入了点击中键会打印信息出来测试，右键的话还会触发 EVT\_CONTEXT\_MENU 事件，中键会触发 EVT\_KEY\_UP/DOWN 事件） （感谢智明提供补丁）
 * 修改 stb\_truetype 缓冲机制避免由于内存不足导致程序崩溃的现象并添加相关文档（感谢雨欣提供补丁）
 * 修复 demoui 在 VS 中编译报错的问题（感谢雨欣提供补丁）

2021/03/03
 * 修复 slide indicator 没有更新的问题（感谢智明提供补丁）

2021/03/02
 * 删除将 QT XML 生成 AWTK UI 的转换工具（感谢陈谭提供补丁）。
 * 修复 widget\_clone 处理 tr\_text 的 BUG（感谢网友 Eagle 提供补丁）。
 * 增加函数 fs\_copy\_file、fs\_copy\_dir 和 dir\_exist。

2021/03/01
 * 修复 slide\_view 通过 indicator 快速切换 page 的时候导致有概 率出现播放动画异常（感谢智明提供补丁）
 * 修复 text\_selector 在播放动画的时候调用 text\_selector\_reset\_options 会导致崩溃的问题（感谢智明提供补丁）

2021/02/27
 * 修复缺省输入法改用 CLOSE 事件造成崩溃的问题（感谢智明提供补丁）

2021/02/26
 * 修复 SD L 内存泄露的问题（感谢智明提供补丁）
 * 修复 csv\_row\_to\_str（感谢林福提供补丁）
 * 修改退出 gpinyin 的时候释放缓存句柄，以免出现内存泄露的问题（感谢智明提供补丁）
 * 修复找不到字库导致使用非默认的字库显示的问题，统一修了查找字库的逻辑为先找指定的字库，如果找不到就找默认的字库，如果都找不到就返回 NULL，字体就不显示了。（感谢智明提供补丁）

2021/02/25
  * 修改 TK\_IM\_MAX\_CANDIDATE\_CHARS 为 0 的时候候选字长度才动态扩张的功能（发现之前的是默认候选字长度动态扩张的，应该改为不动态扩张才对，需要设置 TK\_IM\_MAX\_CANDIDATE\_CHARS 为 0 才动态扩张的）（感谢智明提供补丁）
  * 重新实现 window\_manager\_close\_all 允许窗口触发事件，同时避免在窗口关闭事件中关闭其它窗口的问题。
  * 修改默认输入法，在 keyboard 关闭时清除状态而不是销毁的时候，因为 EVT\_DESTROY 可能会异步执行。
  * 修改 system\_bar 在没有顶层窗口时的警告。
  * 修复生成 dark 主题资源的 json 文件（感谢智明提供补丁）
  * 修复内存泄露的问题（感谢智明提供补丁）

2021/02/24
  * 修复更新风格类型失败的问题（感谢智明提供补丁）
  * scripts.app\_helper 增加 use\_std\_cxx 指定 c++编译器的版本。
  * 修复 widget\_add\_idle 函数和 widget\_add\_time 函数内存泄露的问题和修复多个控件的 idle 上下文 widget 被释放的问题（感谢智明提供补丁）。  
  * 修复调用退出程序时候调用 window\_manager\_close\_all 函数无法触发 keyboard 销毁事件无法清除 idle 回调函数导致 idle 回调中出现野指针的问题（感谢智明提供补丁）

2021/02/23
  * 更新文档。
  * 增加 tk\_expr\_eval。
  * 修改 csv\_row\_to\_str 行尾多分隔符的问题（感谢林福提供）。 
  * 增加 xml 的 CDATA 的值为空的语法支持（感谢智明提供补丁）。 
  * combox 修改大小后导致字体偏移和修改了 tips\_text 默认左对齐（感谢智明提供补丁）。 
  * 增加 TK\_DEFAULT\_WAIT\_TIME，用来控制 event source manager 中 select 最长等待时间。

2021/02/22
  * 修改 stm32 上编译警告。
  * 修复 list\_view\_h 空子集的时候崩溃的问（感谢智明提供补丁）。 

2021/02/20
  * 完善编译脚本（感谢雨欣提供补丁）
  * 修复 ini 的值为空的问题。
  * fscript 增加 one\_of 函数。
  * 修复 slide\_view 保存焦点不正常的问题以及完善和统一 slide\_view 和 pages 控件的保存焦点的代码（感谢智明提供补丁）。
  * 修复创建 dialog 的时候没有根据 label 的风格来计算大小的导致显示不正常问题（感谢智明提供补丁）。
  * 修复在高亮背景的对话框中打开 popup 窗口导致高亮失效的问题以及修复了在对话框中打开另外的窗口后切换到指定窗口导致对话框截获 system\_bar 的消息问题（感谢智明提供补丁）。
  * 修复 awtk 搜索目录和生成不同主题的时候生成的资源不正确的问题（感谢智明提供补丁）

2021/02/19
  * fscript 支持 return 语句。
  * fscript 支持 else if 语句。

2021/02/05
  * 修改 awtk-web 编译错误（感谢智明提供补丁）

2021/02/04
  * 重构 pages（感谢智明提供补丁）
  * 增加函数 str\_decode\_hex （感谢林福提供）。
  * 增加函数 window\_set\_auto\_scale\_children。
  * 重命名 auto\_scale\_xxx 为 auto\_scale\_children\_xxx。
  * 增加 network\_interface 接口及 linux 平台实现（感谢国文提供）。
  * 增加 [《如何根据实际分辨率自动调整窗口中子控件的位置大小》](how_to_auto_scale_children.md)

2021/02/03
  * 修复缺少 SDL 线程函数的声明的问题（感谢智明提供补丁）
  * 修复改变 lcd 的大小后 vg 的裁剪区没有修改的问题（感谢智明提供补丁）
  * 修复 digit_clock 控件在后台窗口无法刷新的问题（感谢雨欣提供补丁）。
  * 修复 page 控件套 pages 控件的时候释放子 pages 控件导致父 pages 控件的 target 为野指针的问题（感谢智明提供补丁）
  * 窗口增加 design\_w/design\_h/auto\_scale\_x/auto\_scale\_y/auto\_scale\_w/auto\_scale\_h 等参数。

2021/02/02
  * 修改注释错误（感谢网友 QQ631757707 提供补丁）
  * 增加 widget\_get\_style\_type 和 style\_update\_state（感谢智明提供补丁）

2021/02/01
  * 完善文档（感谢忠吉提供补丁）。
  * 修复 theme\_t 重定义的问题（感谢雨欣提供补丁）。
  * 修复打包主题工具打包字符串的时候偏移值出错的问题（感谢智明提供补丁）

2021/01/31
  * 增加文档 [应用程序的类型](app_type.md)

2021/01/29
  * 修改 scroll view 注释错误（感谢朝泽提供补丁）
  * 修改 mmap 注释错误（感谢网友 (QQ631757707) 提供补丁）
  * 支持 Ubuntu 下使用输入法（感谢智明提供补丁）
  * 修复风格数据为负数的时候出现的 bug（感谢智明提供补丁）
  * 重构了 theme 类统一操作风格数据的方法（感谢智明提供补丁）
  * 修复 pages 控件设置焦点后马上被释放导致 idle 空对象问题（感谢智明提供补丁）

2021/01/28
  * 修改 pages\_restore\_target 后马上是否焦点对象的问题（感谢智明提供补丁）
  * 修改 pages 控件当 on\_target\_destroy 和 on\_view\_destroy 上下文被释放的时候导致出错的问题（感谢智明提供补丁）

2021/01/27
  * 为 object prop 增加了一些包装函数。

2021/01/26
  * 修改注释错误（感谢攀峰提供补丁）。
  * 增加支持直接使用 OpenGL API 的机制（感谢智明提供补丁）
  * 修复输入框输入大写的时候没有找到 key\_pressed\_info 类型而报错的问题（感谢智明提供补丁）
  * 修复 awtk 源码在、windows\ 平台的某些 PC 上编译 SDL 库出现符号重复定义的问题（感谢雨欣提供补丁）。  
  * 修复 scroll\_view 的 snap\_to\_page 删除 item 不更新页信息问题和增加了是否每次翻一页属性 （感谢智明提供补丁）

2021/01/25
  * 修复 list\_view\_h 中的滑动视图设置 children\_layout 后无法无法正常滑动的问题（感谢雨欣提供补丁）。
  * 感谢智明提供以下补丁：
    * 重构 list\_view 的布局代码
    * 完善原来的 auto\_hide\_scroll\_bar 属性的显示效果
    * 完善支持多个列时候，每一项高度不一样显示不正常的问题
    * 修复 widget\_ensure\_visible\_in\_scroll\_view 滚动不正常的问题。
    * 增加 floating\_scroll\_bar 属性，list\_view 支持滚动条悬浮效果（类似 vscode 中的滚动条效果，鼠标悬停在哪个 list\_view 上面，鼠标滚动既可以滚动 list\_view 不需要点击鼠标确定焦点）
    * scroll\_view 增加 recursive 属性可以遍历子控后计算最终的虚拟宽高的功能，由于 scroll\_view 的孙控件不在子控件的区域中也可以通过滚动条找到其孙控件。

2021/01/24
  * 修改 android 全屏的问题。

2021/01/22
  * 修改 pages 崩溃的问题。

2021/01/21
  * 完善文档。
  * 部分文件增加 UTF8-BOM（感谢陈谭提供补丁）。
  * 将销毁自定义属性代码放到 on\_destroy 回调函数之后。
  * 增加 csv file，请参考 [csv file](../src/csv/README.md)。
  * text\_selector 支持同时修改选项的步长和格式 （感谢智明提供补丁）

2021/01/20
  * 增加函数 slist\_insert。
  * 增加函数 darray\_insert。
  * 修复 page 的 default\_focused\_child 没效果的问题（感谢智明提供补丁）
  * 由于 Ubuntu 默认下不支持 c++11，所以编译 gtest 编译出错的问题（感谢智明提供补丁）
  * 修复 combox 控件设置 open\_window 属性在弹出 popup 窗体后改变整个程序窗口的大小后显示不正常的问题（感谢智明提供补丁）

2021/01/19
  * 修改 label 的歧义注释（感谢智明提供补丁）
  * 新增 widget\_load\_asset\_ex 函数（感谢智明提供补丁）
  * 修复 scroll\_view 控件的 snap\_to\_page 滚动不正常的问题（感谢智明提供补丁）

2021/01/18
  * 修复色块控件不能支持圆角矩形的问题（感谢智明提供补丁）
  * 支持平头线帽和增加圆形进度条支持平头线帽（感谢智明提供补丁）
  * 修改统一 rich\_text 控件的 margin 属性优先选择风格的问题（感谢智明提供补丁）
  * 修复 color\_picker 控件由于 rgb 精度不够导致 slider 控件移动的问题（感谢智明提供补丁）

2021/01/17
  * 完善 json 对空数组的处理。

2021/01/15
  * 增加 data\_url\_copy。
  * 修改 tools 中拼写错误 (floder->folder)。感谢 [suzp1984](https://github.com/suzp1984) 提供补丁。

2021/01/14
  * 增加 app\_conf\_reset 用于恢复出厂设置。
  * 增加 [《TCP/UDP 流扩展模块》](fscript_iostream_inet.md)
  * 增加 [《文件流扩展模块》](fscript_iostream_file.md)
  * 增加 [《串口流扩展模块》](fscript_iostream_serial.md)
  * 增加 [《动态数组扩展模块》](fscript_typed_array.md)
  * 增加 [《ostream 扩展模块》](fscript_ostream.md)
  * 增加 [《typed array 扩展模块》](fscript_typed_array.md)
  * 增加 [《写缓冲区扩展模块》](fscript_wbuffer.md)
  * 增加 [《读缓冲区扩展模块》](fscript_rbuffer.md)
  * 修复 combox 控件的 gtest 的错误（感谢智明提供补丁） 
  * 修复 children\_layouter\_default 计算偏移出现负数导致数据溢出的问题（感谢智明提供补丁） 
  * 修复 edit 控件初始化的设置 text 的时候触发 cancelable 属性的消息事件以及修复 edit 控件的步长不受到 ui 的定义属性的位置影响（感谢智明提供补丁） 

2021/01/13
  * 完善 fscript.md。
  * 完善 api 注释（感谢福明提供补丁） 
  * 增加 [《大端小端扩展模块》](fscript_endian.md)
  * 增加 [《JSON 扩展模块》](fscript_json.md)
  * 增加 [《文件系统扩展模块》](fscript_fs.md)
  * 增加 [《日期和时间扩展模块》](fscript_date_time.md)
  * 增加 [《读缓冲区扩展模块》](fscript_rbuffer.md)
  * 增加 [《istream 扩展模块》](fscript_istream.md)
  * 修复 tkc/event.c/event.h 中的错误（感谢忠吉提供补丁）
  * 修复 text\_selector 控件空选项时候选值出错问题（感谢智明提供补丁）
  * edit 控件的 text 值设置为错误的值时没有正确显示 error 状态和一开始 xml 上面设置错误值没有正确显示 error 状态（感谢智明提供补丁）
  * 修改 edit 和 mledit 的边距属性以及优化了这两个控件的结构体布局（感谢智明提供补丁）
  * 修改统一 tr\_text 由 widget\_re\_translate\_text 函数翻译（感谢智明提供补丁）

2021/01/12
  * 增加函数 tk\_atoul
  * 完善 widget\_exec\_code
  * fscript widget 增加 send\_key。
  * fscript 增加 eval 和 widget\_eval。
  * 修复 agge 初始化裁剪区宽高默认为-1 的问题（感谢智明提供补丁）
  * 修改 resgen 工具生成空文件的资源失败的输出信息（感谢雨欣提供补丁）

2021/01/11
  * 更新文档。
  * 修复 ios 编译问题。
  * 增加 [value 扩展函数](fscript_value.md)
  * 增加 [object 扩展函数](fscript_object.md)
  * 增加 [app\_conf 扩展函数](fscript_app_conf.md)
  * 增加 [CRC 扩展函数](fscript_crc.md)
  * 修改提示信息支持修改 tips\_text\_color 风格和多行显示（感谢智明提供补丁）
  * 增加通过风格来控制 scroll\_bar 控件无上下按钮的模式（感谢智明提供补丁）
  * 增加圆形进度条增加圆形底环效果（感谢智明提供补丁）

2021/01/10
  * 增加 [《fscript widget 扩展函数》](fscript_widget.md)
  * 用 valgrind 检查内存问题并修改发现的问题。

2021/01/09
  * 将基本的 widget 函数包装成 fscript 函数。
  * 支持用 fscript 写简单的事件处理函数。

2021/01/08
  * 完善 typed\_array
  * 完善 object\_array
  * 增加 object\_date\_time。
  * 完善 fscript 和 fscript\_ext。
  * 增加 date\_time\_set\_xxx 系列函数。
  * 将 date\_time/object\_array 包装成 fscript 的函数。
  * 完善 object\_default，支持通过。访问子对象的属性。
  * 修复 xml\_parser 中的注释错误（感谢忠吉提供补丁）。
  * 修复 widget\_const 中的注释错误（感谢福明提供补丁）。
  * 修复第一次打开软键盘输入单个字符无候选字的问题（感谢雨欣提供补丁）。

2021/01/07
  * 完善 fscript 和 fscript\_ext。
  * 增加函数 date\_time\_to\_time。
  * 更新《生态共建计划》文档（感谢陈谭提供补丁）
  * 将 fs/bits/app\_conf/json/ubjson 包装成 fscript 的函数。

2021/01/06
  * 完善 fscript 和 fscript\_ext。
  * 将 istream/ostream/iostream 包装成 fscript 的函数。
  * 将 tcp/udp/serial 包装成 fscript 的函数。

2021/01/05
  * 完善 fscript 和 fscript\_ext。
  * 支持修改输入候选字的个数或者候选字个数自动扩展功能（感谢智明提供补丁）
  * 修复 text\_selector 控件循环一圈后头二项的序号出错的问题（感谢智明提供补丁）
  * 增加线程通用优先级枚举和转换函数以及修改了线程池支持修改堆栈等功能（感谢智明提供补丁）

2021/01/04
  * 完善 value\_str\_ex。
  * fscript 增加 assert 函数。
  * 完善 xml builder 的文档。
  * 修复 crc32 中的问题，并增加 api 文档。
  * 将 crc/endian 相关函数包装到 fscript。
  * 将 wbuffer/rbuffer 相关函数包装到 fscript。
  * 完善《如何用 valgrind 查找内存相关》（感谢忠吉发现问题）

2021/01/03
  * 增加 typed\_array。
  * 将 object 包装成 fscript。
  * 增加函数 value\_type\_size。
  * 将 typed\_array 包装成 fscript。
  * rbuffer 包装成对象，方便生命周期管理。
  * wbuffer 包装成对象，方便生命周期管理。
  * typed\_array 包装成对象，方便生命周期管理。
  * fscript 中的数学函数移到 fscript\_math 中，作为可选组件。

2021/01/02
  * fscript 支持 break/continue。
  
2021/01/01
  * fscript 支持类似 C 语言的 if/while 语句。
  * 完善 fscript 错误处理。

2020/12/31
  * 完善串口流 iostream serial。
  * 完善《AWTK 是如何保证代码质量的》（感谢忠吉发现问题）
  * 修复 agge 的裁剪区宽或者高为 0 的时候裁剪区无效的问题（感谢智明提供补丁）
  * 修复圆角矩形有直角和圆角共存同时圆角半径大于宽度或者高度后画边显示不完整（感谢智明提供补丁）
  * rich\_text 控件在没有设置字体时，修改字体颜色不会生效问题（感谢福明提供补丁）
  * image\_value 控件在设置数值时由于类型转换导致显示的数据不正确的问题（感谢福明提供补丁）

2020/12/30
  * 修复 widget\_set\_as\_key\_target 导致 tab 控件焦点的问题。
  * 修改 edit 设置焦点后没有更新 style 的问题。
  * 增加 [《编写跨平台的代码》](docs/cross_platform_programming.md)

2020/12/29
  * 增加 tk\_dl\_error
  * 修复窗口动画百分比大于 1 或者小于 0 时候导致崩溃的问题（感谢智明提供补丁）

2020/12/28
  * 增加函数 str\_encode\_hex。

2020/12/27
  * 完善编译脚本，自动生成 idl/def 文件。
  * 修改 TK\_IM\_MAX\_CANDIDATE\_CHARS 偏小，导致输入法部分汉字无法输入的问题（感谢智明提供补丁）

2020/12/26
  * 完善 soft\_blend\_image，在定义了 LCD\_BGR565\_LITE 时仍然支持 BGRA8888 格式。

2020/12/25
  * 增加 url 解析器 (url\_t)。
  * 修复 infer 发现的警告（感谢智明提供补丁）
  * 修复圆角矩形缺失边的时候少了一个像素点（感谢智明提供补丁）
  * 修复生成 ui 资源时，如果资源名称存在大写，被替换成小写的问题（感谢培煌提供补丁）。
  * 修复 scroll\_view\_set\_prop 获取 SCROLL\_VIEW\_SNAP\_TO\_PAGE 属性的类型错误（感谢福明提供补丁） 。

2020/12/24
  * 完善多点手势识别算法（感谢智明提供补丁）
  * 修复 data\_reader\_mem 在 Windows 下编译不过的问题。

2020/12/23
  * 完善文档。
  * 增加 data\_reader\_mem。
  * 增加 data\_writer\_wbuffer。
  * 增加 conf\_ini\_create。
  * 增加 conf\_json\_create。
  * 增加 conf\_ubjson\_create。

2020/12/22
  * 增加 FLOW 资源类型。
  * 增加函数 tk\_strncpy\_s。
  * data\_reader\_asset 支持除 data 之外的其它资源类型。
  * 修复圆角矩形 agge 填充下边的角的时候没有复位的问题（感谢智明提供补丁） 
  * 修复加载 UI 比字库资源要早导致 UI 显示不正常的问题（感谢智明提供补丁） 
  * 修复 text_selector 控件在高度无法整除显示个数后导致偏移不正常的问题（感谢智明提供补丁）  

2020/12/21
  * 增加 slide\_view\_set\_active\_ex 以控制是否启用动画。
  * slide\_view\_set\_active 默认启用动画。
  * 修复 T9 软键盘错位的问题。
  
2020/12/18
  * 增加函数 fs\_get\_temp\_path、fs\_remove\_dir_r、fs\_create\_dir\_r 和 fs\_change\_dir。
  * 修改 date\_time\_get\_wday 的 BUG（感谢忠吉发现问题）

2020/12/17
  * fscript 字符串支持换行符。
  * 完善 text selector 的注释（感谢福明提供补丁）
  * 修复强制退出的时候释放部分窗口没有释放的问题 （感谢智明提供补丁）
  * 修复圆角矩形边框类型为上下左右的时候显示不正常（感谢智明提供补丁） 

2020/12/16
  * 完善 hscroll\_label，点击时取消暂停。

2020/12/15
  * 修复 slider 的 step 浮点数会退化为整数的问题（感谢智明提供补丁）
  * 修复 font\_manager 增加了消息事件后导致内存泄漏（感谢智明提供补丁）
  * 移除多点触控消息类型的 touch\_id 和 fingers 成员变量（感谢智明提供补丁）

2020/12/14
  * overlay 窗口支持 always\_on\_top 属性。
  * 完善多少触摸手势算法注释（感谢智明提供补丁）
  * how\_to\_use\_multi\_gesture 文档（感谢智明提供补丁）
  * 增加资源管理器的消息机制和修改字库管理器接收资源管理器的消息（感谢智明提供补丁） 

2020/12/12
  * 在支持 mmap/FileMapping 的系统，优先使用 mmap 加载大资源。
  
2020/12/11
  * 增加 mmap。
  * 完善测试（感谢智明提供补丁）。
  * 增加多点手势算法（感谢智明提供补丁）
  * 增加函数 fs\_build\_user\_storage\_file\_name。
  * 水平视图支持按页滚动，请参考 design/default/ui/list\_view\_h1.xml （感谢智明提供补丁）

2020/12/10
  * 完善 fscript，增加函数 seq。
  * 增加函数 fs\_file\_read\_line。
  * 优化融合算法针对目标位图为非 cache 的情况（感谢智明提供补丁） 
  * 修改窗口设置字体管理器和图片管理器不正常的问题（感谢智明提供补丁） 
  * 修复 slide\_indicator 改变大小后无法刷新点的位置和清除点区域列表的问题（感谢智明提供补丁） 

2020/12/07
 * 增加宏说明文档（感谢智明提供补丁）
 * 进度条增加 reverse 属性，用于反向显示进度。
 * 修复 theme\_gen 可能存在没有初始化变量的问题（感谢朝泽提供补丁）。
 * 修复 combox 弹出下拉框后改变窗口大小导致显示不正常以及在有下拉框的时候直接关闭对应的窗口导致下拉框没有被释放的问题（感谢智明提供补丁）
 * 修改 blend_a 函数提高效率（在 1170 板子上面测试，这样的修改会快很多，具体原因可能是内存交替进入 cache，导致速度变慢了）（感谢智明提供补丁）

2020/12/06
  * 窗口全部关闭后清屏。
  * 完善 draw\_rounded\_rect（感谢智明提供补丁）

2020/12/05
  * 修复 theme\_gen 可能存在没有初始化变量的问题（感谢朝泽提供补丁）。
  * 增加 EVT\_IM\_START 和 EVT\_IM\_STOP 事件。
  * 增加函数 window\_manager\_switch\_to。
  
2020/12/04
  * fscript 支持注册全局函数。
  
2020/12/03
  * 完善编译脚本（感谢培煌提供补丁）。
  * 修复 image gen 多写 4 字节数据的问题。

2020/12/02
  * 完善 fscript。
  * 修复 shdlc 内存泄露（感谢朝泽提供补丁）。
  * 减小 assets-1m.c 的体积（感谢智明提供补丁）
  * 修复 slide\_menu.xml 的问题（感谢智明提供补丁）
  * 将 get\_time\_us64 缺省实现移动 time\_now.c，避免部分嵌入式平台编译问题（感谢陈谭提供补丁）

2020/12/01
  * 修改 image\_manager\_get\_bitmap 优先使用 assets\_manager（感谢福明提供补丁）。
  * 复关闭程序销毁 system\_bar 时未解绑事件出现野指针导致程序崩溃（感谢雨欣提供补丁）。
  * fscript 支持 != 比较。 

2020/11/30
  * 完善注释（感谢朝泽提供补丁）。
  * 完善 combo\_box（感谢朝泽提供补丁）。
  * 修改部分编译器的问题（感谢陈谭提供补丁）。

2020/11/28  
  * 增加函数 date\_time\_add\_delta。
  * 完善 lcd\_set\_font\_name。

2020/11/27
  * 修复 window manager 中的注释（感谢朝泽提供补丁）。
  * 修复 switch 中 get prop 没有初始化的问题（感谢俊杰提供补丁）。
  * 修复资源大写时程序无法使用资源问题（感谢培煌提供补丁）。
  * 修复国家语言为空字符串时图片资源获取不正确的问题（感谢培煌提供补丁）。
  * 修复 pc 上面 mono 的桌面应用程序模式改变窗口大小后导致程序崩溃（感谢智明提供补丁）。
  * fscript 支持表达式解析，用以替换 expr\_eval。

2020/11/26
  * 添加 object\_has\_prop\_by\_path 接口（感谢朝泽提供补丁）。
  * 修正 expr\_eval 内存泄漏的问题（感谢朝泽提供补丁）。
  * 增加 window\_manager\_close\_all。
  * popup 窗口增加 close\_when\_timeout 属性。

2020/11/25
 * 完善 theme gen 支持更长属性（感谢福明提供补丁）。

2020/11/24
  * 更新编译脚本。
  * 更新文档《如何集成第三方库》
  * 修复 opengl 的矩形边框放大后虚边的问题（感谢智明提供补丁）。
  * 修复背景色为全透明拷贝前景色透明值出错的问题（感谢智明提供补丁）。
  * 修复 progress_circle 的 show_text 属性引起的扩大脏矩形区域的问题（感谢智明提供补丁）。
  * 修改支持特定平台的 OpenGL 加速和把 fbo 的 draw arrays 的方法统一和 nanovg 一样（感谢智明提供补丁）。

2020/11/23
  * fscript 支持 comment。
  * fscript 支持 while 语句。

2020/11/20
  * 完善 label（感谢智明提供补丁）。
  * 增加 timer\_resume/timer\_suspend（感谢智明提供补丁）。
  * 完善 window\_set\_fullscreen 支持 native window。

2020/11/19
  * 增加函数 tk\_ostream\_file\_create\_ex

2020/11/18
  * 增加 rlog 用于循环记录 log。

2020/11/15
  * 增加函数 object\_to\_json。
  * expr\_eval 添加对类似‘$temp.[0].value’格式的变量的支持（感谢朝泽提供补丁）
  * object 添加 object\_set\_prop\_by\_path、object\_can\_exec\_by\_path、object\_exec\_by\_path 的接口（感谢朝泽提供补丁）
  * 完善 fscript（感谢剑超提供补丁）。
  * 完善调试信息（感谢 MooYin 提供补丁）。

2020/11/14
  * 修复 android 不能熄屏的问题。

2020/11/13
  * 修复圆角矩形启用局部圆角导致边无法缺失的问题（感谢智明提供补丁）。
  * 完善 slide\_indicator（感谢智明提供补丁）。

2020/11/11
  * 增加 fscript 的文档。
  * 完善 text\_selector, 支持设置选中项的风格（感谢智明提供补丁）
  * 增加 bitmap\_lock\_buffer\_for\_write 参数检测（感谢俊杰提供补丁）

2020/11/09
  * 增加一个简单的函数式执行引擎 (fscript)。 
  * 修复不同的控件来回获取焦点导致脏矩形为全屏的问题（感谢智明提供补丁）
  * 修复在播放 dialog\_toast 打开动画的同时点击退出导致 dialog\_toast 无法退出的问题（感谢智明提供补丁）

2020/11/08
  * 增加函数 tk\_vsnprintf

2020/11/07
	* 完整 FAQ
	* 增加函数 timei\_now\_us

2020/11/06
  * 完善资源管理器（感谢智明提供补丁）。

2020/11/05
  * 完善 ubjson 保存逻辑（感谢王程提供补丁）。

2020/11/04
  * 完善直接加载 file://资源的逻辑（感谢智明提供补丁）。
  * 增加文档 [自定义控件支持脚本绑定](binding_custom_widget.md)
  * 修复测试用例的错误（感谢智明提供补丁）。
  * 修复 datai\_reader\_asset 如果加载资源失败，导致修改计数器失败，从而导致崩溃的问题（感谢智明提供补丁）。

2020/11/03
  * 修复资源名字超过 31 个字符导致查找不了，导致内存泄漏的问题，修改对应生成资源工具以及测试用例（感谢智明提供补丁）。 

2020/11/02
  * 增加支持过滤 UI 文件中属性中包含 <?include 字符串（感谢智明提供补丁）。
  * 增加支持 edit 控件可以添加非规定的按键触发事件（感谢智明提供补丁）。
  * 修复了 grab_keys 属性和按键事件冲突的问题（感谢智明提供补丁）。
  * 修复了 keyboard 对顶部窗口的移动位置不正确的问题（感谢雨欣、智明提供补丁）。
  * 修复了使用 global_emitter 后，在窗口退出的时候释放对应的消息同时之前的消息分发返回值为 RET_STOP 会导致无法删除 global_emitter 队列中的对应消息（感谢智明提供补丁）。

2020/11/02
  * 修改 system info 中的内存泄露（感谢朝泽提供补丁）。
  * image/tab\_button 支持 space/return 触发 click 事件。
2020/10/30
  * 完善编译脚本（感谢朝泽提供补丁）。
  * 增加 main\_loop\_remove\_event\_source\_by\_tag 函数。
  * 完善 API 注释。
  * 完善 object\_default，支持按序号获取属性。

2020/10/29
  * 完善 dialog\_highlighter\_factory\_create\_highlighter （感谢俊杰提供补丁）。
  * 完善 tab button pointer 事件处理（感谢智明提供补丁）。

2020/10/28
  * label 控件增加 word\_wrap 属性（感谢智明提供补丁）。
  * 修复 edit 光标的 bug（感谢智明提供补丁）。

2020/10/27
  * 完善 object\_array（感谢朝泽提供补丁）。

2020/10/26
  * 更新文档。
  * 修改英文版中的语法错误（感谢攀峰提供补丁）。

2020/10/25
  * 增加 object\_clone。
  * 窗口切换时，旧窗口按下的键，一直不松开，会持续触发 PRESS，可能会干扰新窗口的用户功能。在窗口切换时，abort 全部已经按下的键，直到按键松开。

2020/10/24
  * 增加函数 conf\_ini\_save\_as
  * 增加函数 conf\_json\_save\_as
  * 增加函数 conf\_ubjson\_save\_as
  * 增加工具 json\_to\_ubjson
  * 增加工具 ubjson\_to\_json
  * 更新 build config 文档
  
2020/10/23
	* 完善 text_selector 控件循环显示功能（感谢智明提供补丁）。
  * 修改设置 visible 没有触发 layout 的问题（感谢朝泽提供补丁）。
  * 修改ＳＤＬ注释乱码的问题（感谢智明提供补丁）。

2020/10/22
  * OpenGL 模式下的图片采样算法由相邻采样改为线性采样，主要为了提高图片旋转缩放效果（感谢智明提供补丁）。 
  * 修改 darray 的 elms 为 NULL 是内存泄漏的问题（感谢兆坤提供补丁）。

2020/10/21
  * awtk-web 修复编译报错问题（感谢智明提供补丁）。
  * awtk 修复 web 编译报错的问题（感谢智明提供补丁）。
  * 给 awtk 的 text_selector 控件增加循环显示功能（感谢智明提供补丁）。
  * awtk 修复在 label 某些条件下调整宽高不正常的问题（感谢智明提供补丁）。

2020/10/20
  * 完善单元测试（感谢智明提供补丁）。
  * 修改高亮对话框的叠加效果（感谢智明提供补丁）。
  * 重构了窗口动画的 begin\_frame 的方法（感谢智明提供补丁）。
  * 修改 label 的 auto\_adust\_size 功能的换行效果（感谢智明提供补丁）。
  * 修改 rich\_text 支持风格文字颜色和字号，字库以及水平对齐（感谢智明提供补丁）
  * 修改 label 控件支持修改字符串的时候 auto\_adust\_size 功能（感谢智明提供补丁）
  * 修复无法触发 label 控件的 auto\_adust\_size 的问题（感谢智明提供补丁）
  * 修改了一下自定义控件在 Designer 中的图标的命名规则（感谢朝泽提供补丁）。

2020/10/19
  * 修复窗口焦点控件获取不正常的问题（感谢智明提供补丁）。
  * 恢复 bitmap\_destroy 里的改动（会导致 JS 版本野指针的问题）。
  * 修改 auto\_adjust\_size 缩小功能和注释问题（感谢智明提供补丁）。
  * 修复 edit 由长字符串变为短字符串导致不显示的问题（感谢智明提供补丁）。
  * 修复 text\_selector 控件使用翻译和 tr\_text 导致重置显示顺序问题（感谢智明提供补丁）。
  * 修改了由于路径问题导致 asset\_loader\_zip 无法正常加载资源的问题（感谢朝泽提供补丁）。

2020/10/18
  * 引入 wm->global\_emitter，避免 window manager 按键事件触发两次的问题。
  * 修改 valgrind 发现的内存问题。
  
2020/10/16
  * image\_animation 支持运行时修改 interval。
  * image\_animation 支持 WIDGET\_PROP\_EXEC 控制动画的启动、暂停和停止。

2020/10/15
  * 完善脚本，支持同时生成。data/.res（感谢朝泽提供补丁）。
  * 修复 mledit 的拉动条不能拖动的问题（感谢智明提供补丁）。
  * text\_selector 增加 y 轴速度比例和支持本地翻译功能 （感谢智明提供补丁）。
  * 修复 dialog\_toast 不支持文字长度自适应和修改字号的问题（感谢智明提供补丁）。
  * 修改 widget 的 auto\_adjust\_size 为 true 的时候，也支持 xywh 为绝对值时自动调整（感谢智明提供补丁）。

2020/10/13
  * 增加 plugin\_manager。 
  * label 的 line\_wrap 属性，缺省改为 FALSE。
  * 将 tkc 编译成独立的动态库，给非 GUI 的库使用。
  * 完善 label 的 line\_wrap 处理（感谢朝泽提供补丁）。 
  * 增加属性 disallow\_children\_focusable（感谢智明提供补丁）。

2020/10/12
  * 完善 api 注释（感谢朝泽提供补丁）。
  * 完善 fragment framebuffer（感谢智明提供补丁）。 
  * 完善 label 对 line\_wrap 的处理。

2020/10/11
  * label 增加 line\_wrap 属性。
  * 完善输入法。如果 keyboard 与 edit 没有重叠，不再上推窗口。

2020/10/10
  * window\_manager\_back 支持 overlay 和 popup。
  * 完善 API 注释（感谢兆坤提供补丁）。
  * canvas 增加了 clear\_rect 的函数，和 fill\_rect 函数公用一个颜色（感谢智明提供补丁）。
  * 优化了 blend 函数的拷贝判断，当背景色为透明时候，把前景色拷贝到背景上面，不在做混合计算（感谢智明提供补丁）。

2020/10/09
  * 把 grab\_focus 和 return\_key\_to\_grab\_focus 属性移到 title\_value 自定义控件。

2020/10/08
  * 增加 auto\_adjust\_size 属性。为 true 时，根据子控件以及文本调整空间自身大小（此时最好不要使用 child\_layout，否则可能有冲突）。
  * 窗口增加 single\_instance 属性。为 true 时，如果窗口已经打开，先关闭旧的，再打开新的。
  * 增加 grab\_focus 和 return\_key\_to\_grab\_focus 属性，用于辅助实现只用方向键和 OK 键的硬件。

2020/10/07
  * label 支持自动换行。
  
2020/10/06
  * 完善 README.md，增加 nodejs 和 glob 安装方法。
  * 删除 design 目录中临时文件 (.bin)。
  * 完善资源目录中的文档。
  
2020/10/05
  * 增加用于生成智能提示数据的工具。
  
* 2020/10/03-04
  * 解析时遇到没有注册的控件打印警告，并用 view 代替。
  * 增加 [vscode 实时预览插件](https://github.com/zlgopen/awtk-vscode-plugin)。

* 2020/10/02
  * 更新文档。
  * 修复 macos 编译问题（感谢提 [suzp1984t pu](https://github.com/suzp1984) 供补丁）

* 2020/10/01
  * mledit 增加 open\_im\_when\_focused 和 close\_im\_when\_blured 属性。
  * 重构 label，把部分函数移动到 line_break 中。
  * 增加 [垂直文本控件](https://github.com/zlgopen/awtk-widget-vlabel)
* 2020/09/29
  * 删除了 assets\_manager\_ref 函数的空字符串的警告信息（感谢智明提供补丁）。
  * 修复 vgcanvas\_create 函数返回值的注释（感谢兆坤提供补丁）。
  * 修改 sdl release mouse capture 的时机，缓解 Ubuntun 调试时鼠标僵死的问题（感谢商 YUN 涛提供建议）。
  * 增加函数 tk\_run\_in\_ui\_thread，让后台线程在 UI 线程执行指定的函数，它是对 idle\_queue 的包装，支持等待调用完成。

* 2020/09/28
  * 完善自定义控件文档（感谢朝泽提供补丁）。
  * 完善 project.json 和文档（感谢朝泽提供补丁）。
  * 完善离线 canvas 对旋转的处理（感谢智明提供补丁）。
  
* 2020/09/25
  * 修改 image dither 工具 mac 编译问题（感谢 suzp1984 提供补丁）。
  * 完善 canvas 换行处理（感谢智明提供补丁）。
  * 修复 slider 刷新的问题（感谢智明提供补丁）。

* 2020/09/23
  * 完善 dll 导出函数（感谢朝泽提供补丁）。
  * 修复部分事情初始化的问题（感谢俊杰提供补丁）。
  * 修复 assets.c 在 RT1052 上路径的问题（感谢俊杰提供补丁）。
  * 将 contre 改为 center（保留 centre，兼容以前的代码）。

* 2020/09/22
  * 完善 scroll bar。
  * 修复密码选中不居中的问题。
  * rich text 增加 yslidable 属性。
  * 完善 edit 的 action 事件。

* 2020/09/21
  * 更新 dll 导出函数列表。
  * 增加 TK\_KEY\_LONG\_PRESS\_TIME，让触屏长按和键盘长按可以设置为不同的时间。
  * 修复 demo 资源的问题，方便 designer 打开。
  * 修复部分事情初始化的问题（感谢俊杰提供补丁）。
  * 完善编译脚本（感谢朝泽提供补丁）。
  * 修复 valgrind 发现的内存泄露。

* 2020/09/18
  * EVT\_VALUE\_WILL\_CHANGE、EVT\_VALUE\_CHANGING 和 EVT\_VALUE\_CHANGED 事件结构改为 value\_change\_event\_t，从中可以获得旧值和新值。
  * EVT\_VALUE\_WILL\_CHANGE 事件返回 RET\_STOP 可以阻止修改值。
  * event\_t 结构体增加 size，避免结构体用错导致内存越界访问。
  * 修改 stm32 编译问题（感谢智明提供补丁）。
  * 完善编译脚本（感谢朝泽提供补丁）。
  * 在 application_init 之前设置默认语言。
  
* 2020/09/17
  * 修复一些警告。
  * 将一些参数改成常量指针。
  * 修复快速点击有概率导致 scroll_bar 控件在同一个位置来回滑动（感谢智明提供补丁）。
  * 增加了圆角矩形支持缺失边框的功能，同时修复了背景色为透明的颜色融合算法（感谢智明提供补丁）。

* 2020/09/16
  * 完善 API 注释。
  * 将一些参数改成常量指针。
  * 修改离线 canvas 的文档（感谢智明提供补丁）。
  * 修复 edit 带格式编辑时，输入没有清除选中内容的 bug。
  * 支持用户外部重载自定义离线 canvas 和统一窗口动画的代码（感谢智明提供补丁）。
  * 统一窗口动画配合修改 awtk-web 的 fbo 和窗口动画的代码（感谢智明提供补丁）。
  * 修复 dialog_toast 在 web 卡主的问题和修复同时插入导致动画异常的问题以及增加注释（感谢智明提供补丁）。

* 2020/09/15
  * 支持长按键事件
  * edit 增加 close\_im\_when\_blured 属性。

* 2020/09/11
  * 支持 packed 图片。
  * 增加 [如何使用 packed 图](docs/how_to_use_packed_image.md)
  * 修复 32 位 LCD 窗口动画截图的问题（感谢朝泽提供补丁）。
  * 消除一些不必要的告警；
  * 修正在 key\_down 事件销毁 grab\_keys 属性为 true 的控件会导致访问 NULL 指针的问题

* 2020/09/10
  * 重构 widget。
  * 修复使用标准内存分配器同时启用内存泄漏检测时没有加锁的问题。

* 2020/09/09
  * istream 增加 eos/tell 接口。
  * 增加函数 tk\_istream\_read\_line\_str
  * 去掉一些不必要的警告信息（感谢福明提供补丁）。
  * 完善 image_gen（感谢朝泽提供补丁）。

* 2020/09/06
  * 支持多点手势识别事件（旋转和缩放），目前对接 SDL 的多点手势事件，支持 Android、iOS、windows、linux(desktop) 和 MacOS。其它系统需要自己识别后上报事件。
  * 增加函数 widget\_set\_child\_text\_with\_int
  * 增加 [《利用 app_helper 编写 SConstruct》](docs/app_helper_usage.md)

* 2020/09/04
  * 完善 ringbuffer（感谢网友 J, 提供补丁）
  * 完善窗口动画截图（感谢智明提供补丁）。

* 2020/09/03
  * 完善 toast（感谢智明提供补丁）。
  * 完善 calibration win（感谢俊杰提供补丁）。 

* 2020/09/02 
  * 修改了圆角矩形的函数名字和修改了 vg 接口的注释（感谢智明提供补丁）。
  * 按照上星期说的 dialog\_toast 问题（感谢智明提供补丁）。
  * 更新主题文档。
  * 完善 demoui，增加指定单个圆角半径的示例。
  * 增加文档 [《如何将生成的可执行文件和资源打包》](docs/how_to_release_app.md)
  
* 2020/09/01 
  * 增加 FAQ《如何在打开新窗口时关闭当前窗口？》
  * 增加 FAQ《如何设置当前的语言？》
  * 增加 FAQ《如何将板子键盘的键值映射到 AWTK？》
  * 增加 FAQ《如何定制软键盘/候选字的风格？》
  * 完整主题文档，增加颜色格式的说明。

* 2020/08/30
  * 完善 app helper 脚本，从 project.json 读取 lcd width/height

* 2020/08/28
  * overlay 增加 click_throgh 属性，允许 overlay 窗口在没点击到子控件时，穿透到底层窗口。

* 2020/08/27
  * 增加函数 wst\_count\_char
  * edit 增加对 password/email 长度限制。
  * 修改 edit password 居中的问题。
  * 修复 agge 模式下修改窗口（SDL 窗口）大小后出现报错警告（感谢智明提供补丁）。
  * 增加圆角矩形支持设置不同的圆角半径，不改变原来的函数，新增两个函数 canvas\_fill\_rounded\_rect\_ex 和 canvas\_stroke\_rounded\_rect\_ex 函数来支持改功能（感谢智明提供补丁）。
  * 修复了离线 canvas 在使用在线 vg 绘图的时候，保存和还原 vg 的数据堆栈信息不正确导致画面异常的问题，给 vg 新增了一个 vgcanvas\_reset\_curr\_state 来清除当前的 vg 状态数据（给 agge 模式下使用，清除 vg 堆栈对离线 canvas 的绘图产生影响）和在 nanovg 中新增了 nvgBeginFrameEx 函数（用于给绑定 fbo 时候使用，以免破坏 vg 的堆栈数据）（感谢智明提供补丁）。

* 2020/08/25
  * 修复 nanovg 在解析部分字体的是有可能因为顶点过多导致缓冲区不够大产生中断问题（感谢智明提供补丁）。
  * 修复 nanovg 在 win10 或者 win8 的高分屏上有概率出现裁减区虚化的问题（感谢智明提供补丁）。

* 2020/08/24
  * mledit 支持 cancelable 属性，为 TRUE 时，如果内容有变化，会设置编辑器的状态为 changed，所以此时编辑器需要支持 changed 状态的 style。
  * 完善 windows 时间函数。
  * 完善 edit pattern
  * edit 格式编辑时，获得焦点时，把光标设置到最前面。
  * 修复 serial helper 和测试程序中内存泄漏（感谢智明提供补丁）。

* 2020/08/23
  * 修复 edit 编辑 ip/date/time 时，cancel 编辑时的问题。
  * 新增 [awtk-restful-httpd](https://github.com/zlgopen/awtk-restful-httpd)
  * 新增 [自动测试引擎](https://github.com/zlgopen/awtk-ui-automation)
  * 新增函数 widget\_get\_text\_utf8
  
* 2020/08/21
  * 修复 main_loop_simple.c 事件部分成员没有初始化的问题（感谢梦幻櫻花提供补丁）。

* 2020/08/20
  * 软键盘支持“clear"/"cancel"两个功能按钮。
  * 修复 opengles 截屏的时候反算预乘时候数据溢出问题（感谢智明提供补丁）。
  * 修复离线 canvas 在 agge 下有可能导致原来（感谢智明提供补丁）。
  * edit 支持 cancelable 属性，为 TRUE 时，如果内容有变化，会设置编辑器的状态为 changed，所以此时编辑器需要支持 changed 状态的 style。

* 2020/08/19
  * 完善 app conf 保存 json 时字符串转义处理。
  * 完整 ui serializer，增加 xml 预处理指令。
  * 增加 [字符集编码转换函数](https://github.com/zlgopen/awtk/tree/master/src/charset/README.md)。
  * t9ext 输入法引擎支持设置语言（感谢梦幻櫻花提供补丁）。

* 2020/08/18
  * 修复了一些潜在的内存泄露。
  * button 支持设置长按时间。
  * 完善编译脚本（感谢朝泽提供补丁）

* 2020/08/17
  * 增加文档《项目描述文件 project.json 介绍》（感谢朝泽提供补丁）
  * 完善 awtk-web 及配置文件和脚本。
  
* 2020/08/16
  * 调整资源目录，支持 designer 打开。

* 2020/08/15
  * 完善 《自定义控件规范》（感谢朝泽提供补丁）
  * 完善 scripts/app\_helper\_base.py（感谢朝泽提供补丁）
  * 修复 web 版本切换语言时 canvas 重置的问题（感谢智明提供补丁）。
  
* 2020/08/14
  * 完善编译脚本和资源更新脚本。
  * 完善 app conf init（感谢俊杰提供补丁）。

* 2020/08/13
  * 完善 tab button（感谢朝泽提供补丁）。
  * 完善 《自定义控件规范》（感谢朝泽提供补丁）

* 2020/08/11
  * 完善 conf_node 增加一些 wrapper 函数。
  * 完善 《自定义控件规范》（感谢朝泽提供补丁）。

* 2020/08/10
  * 完善 conf_node 增加一些 wrapper 函数。
  * 修复 agge 截屏矩形区域原点不为 0 的时候崩毁的问题（感谢智明提供补丁）。
  
* 2020/08/08
  * 完善 fontgen，支持生成二进制数据。
  * 增加 [自定义字体加载器](https://github.com/zlgopen/awtk-custom-font-loader)，实现加载部分字体。
  * 修改输入法中内存临时泄漏（在窗口关闭时会释放）。

* 2020/08/07
  * 候选字控件 (candidate) 支持设置 margin style。

* 2020/08/06
  * 完善离线 lcd（感谢智明提供补丁）。
  * 完善 slist （感谢智明提供补丁）。
  * 增加 vgcanvas_clear_cache（感谢智明提供补丁）。

* 2020/08/04
  * 完善 keyboard 增加 back/back_to_home 按钮的支持。
  * edit/mledit/slider 支持 WIDGET_PROP_INPUTING 属性，用于辅助实现 MVVM。
  
* 2020/08/03
  * 完善 emitter 的注释（感谢智明提供补丁）。
  * 完善 fragment lcd framebuffer（感谢智明提供补丁）。
  * 完善文档《避坑指南》
  
* 2020/07/31
  * 增加 EVT_RESET
  * 部分文件增加 UTF8-BOM（感谢陈谭提供补丁）。

* 2020/07/30
  * 如果新窗口的 sensitive 属性为 FALSE，不切换窗口焦点。
  * 完善 text edit, 修复 mledit 在非缺省字体时的问题（感谢俊杰提供补丁）。

* 2020/07/29
  * 增加文档《如何续期使用 AWTK Designer》（感谢俊杰提供补丁）。

* 2020/07/28
  * 英文版 README（感谢陈谭提供）。
  * mutable_image 增加注释（感谢智明提供补丁）。
  * 修复 opengl 下截屏的反算预乘时候出现数据溢出问题（感谢智明提供补丁）。

* 2020/07/25
  * 完善 shift 键的处理。 

* 2020/07/24
  * 增加函数 emitter\_exist。

* 2020/07/24
  * 完善 api 注释（感谢朝泽提供补丁）。
  * 完善 save png（感谢智明提供补丁）。
  * 完善 android 资源加载（感谢智明提供补丁）。
  * asset\_loader 接口增加函数 asset\_loader\_exist。
  * 增加函数 widget\_is\_parent\_of/widget\_is\_direct\_parent\_of。

* 2020/07/23
  * 修复 opengl 下没有考虑 fbo 返回来的数据为预乘数据的问题和让 opengl 支持预乘贴图显示（感谢智明提供补丁）。

* 2020/07/22
  * 增加 OBJECT\_CMD\_REMOVE\_CHECKED 、OBJECT\_PROP\_CHECKED 和 OBJECT\_PROP\_SIZE。
  * 完善 value\_double，支持 bool 到 double 转换。
  * 完善 API 注释（感谢俊杰提供补丁）。

* 2020/07/21
  * 完善文档。
  * 完善 API 文档增加导出函数。
  * 修改 gles 和 agge 的 svg 效果不一致的问题（感谢智明提供补丁）。
  * 给 bitmap 的函数注释，方便动态类库编译的时候调用其函数（感谢智明提供补丁）。

* 2020/07/20
  * 完善校准窗口。

* 2020/07/19
  * 完善 API 文档。
  * 增加 [表格视图](https://github.com/zlgopen/awtk-widget-table-view)

* 2020/07/17
  * 完善 scrollbar 支持大范围滚动。
  * 完善 widget animator，使用 double 保存属性，避免精度问题。

* 2020/07/16
  * 完善 bidi 文档。
  * 修改 mem pool 内存对齐的问题（感谢 imxood 提供补丁）
  * 完善 rtt 的 tk_thread_set_priority（感谢玖玲提供补丁）
  * 优化圆角矩形在脏矩形下的效率和修复警告（感谢智明提供补丁）

* 2020/07/15
  * 完善 app conf 文档。

* 2020/07/14
  * 修复 gles 的贴图旋转关闭抗锯齿的后导致效果问题（感谢智明提供补丁）

* 2020/07/13
  * 完善 API 注释（感谢尧燊提供补丁）

* 2020/07/12
  * 完善 date\_time：增加 date\_time\_get\_month\_name 和 date\_time\_get\_wday\_name。
  * 增加 [日期选择控件](https://github.com/zlgopen/awtk-widget-date-picker)。

* 2020/07/11
  * 完善 bidi，widget 支持 bidi 属性。
  * 完善 hscroll_label 对 bidi 的支持。
  * 完善 slide view indicator（感谢朝泽提供补丁）。 

* 2020/07/10
  * 完善 tokenizer。
  * 完善表达式，增加 iformat/fformat 函数。
  * 完善资源管理器（感谢俊杰提供补丁）。
  * 增加离线 canvas（感谢智明提供补丁）。

* 2020/07/09
  * 基本控件的文本支持双向排版 (Unicode Bidirectional Algorithm)
  * 完善 font、image 和字符串工具（感谢大恒提供补丁）
  * 完善 native windows 销毁 lcd 对象。
  * 完善 expr 支持浮点数和整数格式化。
   
* 2020/07/08
  * 完善软键盘关闭流程。
  * 完善 stb 预乘处理（感谢智明提供补丁）。
  * 完善对无效 bitmap 对处理（感谢智明提供补丁）。
  * 完善字体、主题、字符串和 UI 生成工具（感谢大恒提供补丁）
  * date time 中增加函数 date\_time\_is\_leap、date\_time\_get\_days 和 date\_time\_get\_wday。

* 2020/07/07
  * 完善资源生成脚本（感谢朝泽提供补丁）。
  * 增加 [mvvm 在 stm32](https://github.com/zlgopen/awtk-stm32h743iitx-mvvms) 上的移植。
  
* 2020/07/06
  * 支持图片预乘功能（感谢智明提供补丁）。
  * 完善 demoui（感谢俊杰提供补丁）。
  * 完善 xml to ui 工具（感谢俊杰提供补丁）。
  
* 2020/07/04-06
  *增加代码 [编辑控件](https://github.com/zlgopen/awtk-widget-code-edit)。

* 2020/07/03
  * 完善 graphic\_buffer\_default
  * 完善焦点快捷键的处理。支持 shift+tab 向后切换焦点。 
  
* 2020/07/02
  * 完善 graphic\_buffer\_default
  * 增加 tk\_mem\_is\_valid\_addr
  * 完善 edit 注释（感谢俊杰提供补丁）。
  *  gif 隐藏后不再刷新。 
  * 完善 ui loader（感谢 dog2nd_Zhao 提供补丁）。

* 2020/06/30
  * 完善 font gen（感谢智明提供补丁）。
  * 完善 image dither（感谢大恒提供补丁）。
  * add vgcanvas\_get\_text\_metrics.
   
* 2020/06/29
  * 完善 listview layout（感谢尧燊提供补丁）
	* 完善 input\_type\_to\_keyboard\_name（感谢俊杰提供补丁）
	·
* 2020/06/28
  * 完善 font cache（感谢智明提供补丁）。
  * font\_bitmap\_index\_t 中的偏移量改为 32 位（如果使用位图字体，需要重新生成资源）。
  * 修复位图字体空格字膜宽度的不正确的问题（感谢智明提供补丁）。

* 2020/06/27
  * 完善 font cache（感谢智明提供补丁）。
  * 窗口切换到前台时，将其为窗口管理的 target/key\_arget。

* 2020/06/26
  * edit 支持 date/time/time\_full 输入类型。
  * 增加文档 [如何用 edit 控件编辑日期](how_to_edit_date.md)
  * 增加文档 [如何用 edit 控件编辑时间](how_to_edit_time.md)
  * 增加文档 [如何用 edit 控件编辑 IP 地址](how_to_edit_ip_addr.md)

* 2020/06/25
  * edit 支持 ip v4 输入类型。

* 2020/06/24
  * 增加函数 tk\_thread\_self。
  * 增加 mutex\_nest 嵌套互斥锁。
  * object_locker 改用嵌套互斥锁实现。
  * 增加事件 cmd\_exec\_event\_t，方便拦截 object exec。
  * 完善 rich text（感谢尧燊提供补丁）
  * 修改 app conf/file browser 中的内存泄露。

* 2020/06/23
  * 完善 FAQ，增加《如何去掉不需要的控件，以节省 flash 空间》
  
* 2020/06/22
  * 完善 conf-io 支持 add 命令，新建一个空的子节点。
  * 修复 Build script 问题（感谢智明/陈谭提供补丁）。
  * 修改 mutable\_image 文档（感谢智明提供补丁）。
  * 将部分文件转成 UTF8-BOM 格式（感谢陈谭提供补丁）。

* 2020/06/21
  * 增加 object\_cmd\_t 定义对象支持的常见命令。
  * 完善 conf-io 实现常见命令。

* 2020/06/20
  * 完善 tab\_button\_group 支持缺省的 active button。

* 2020/06/19
  * 完善 DLL 导出符号。
  * 完善 conf-io，支持自读模式。
  * 完善 conf-io，支持缺省配置。
  * 完善 conf-io，修改 json 文件不存在的问题。
  * 增加 object\_compositor 将两个对象包装成一个对象。
  * 修复特殊情况下软键盘无法关闭的问题（感谢陈谭提供补丁）。
  * 修复控件动画放在窗口上有概率出现显示不正常的问题（感谢智明提供补丁）。

* 2020/06/18
  * 增加文档《内存分配》
  * 完善 mem allocator pool
  * 完善 API 注释（感谢俊杰提供补丁）。
  * 增加函数 check\_button\_get\_checked\_button
  * 增加 tk\_mem\_init\_stage2 用于初始化 pool/oom，否则在嵌入式式平台会死锁。

* 2020/06/17
  * 保留 xml 属性中的空格。
  * 完善 edit（感谢培煌提供补丁）
  * 修复空格字体的显示问题（感谢智明提供补丁）。
  * 修复圆角矩形在宽高很小的时候出现异常的问题（感谢智明提供补丁）。
  * 重新设计内存管理器架构，新增加锁、调试和防内存碎片功能。

* 2020/06/15
  * 完善 app conf/object locker
  * 完善 toast （感谢培煌提供补丁）
  * 完善资源生成脚本（感谢朝泽提供补丁）。
  * 完善 window manager simple（感谢智明提供补丁）。
  * 修复旋转后 online\_fb 的 line\_length 不正确的问题（感谢智明提供补丁）。

* 2020/06/14
  * 完善 scroll view，增加滚动相关事件。
  * 增加文档《如何实现弹出菜单》
  * 完善 conf-io，支持 reload。
  
* 2020/06/13
  * 完善资源生成脚本，兼容 AWTK 和 Designer（感谢朝泽提供补丁）。
  * 完善 str\_count\_sub\_str（感谢朝泽提供补丁）。 
  * 完善 《自定义控件规范》（感谢朝泽提供补丁）。 
  * 增加 data\_reader\_asset。
  * 完善 conf_io，支持移动配置项的顺序，增加#index 获得序数。
  
* 2020/06/12
  * 增加 app\_conf\_set\_wstr/app\_conf\_get\_wstr
  * 完善 window\_manager\_back\_to\_home 对软键盘窗口的处理。
  * 增加 object\_locker 对 object 进行装饰，提供互斥的功能。
  * 用 object\_locker 重新实现 app\_conf。

* 2020/06/11
  * 完善 conf\_io，支持#name/#size
  * 增加 dll 导出函数（感谢俊杰提供补丁）。 
  * 修改 canvas\_draw\_char\_impl baseline 的 BUG。
  * 完善文档。
  
* 2020/06/10
  * 增加宏 NATIVE\_WINDOW\_BORDERLESS 控制是否去掉 native window 的标题栏。
  * 增加 FAQ《应用程序在 Windows 的手持设备中运行，如何去掉窗口的标题栏》
  * image value 支持 16 进制格式。
  * 增加一些导出函数（感谢尧燊提供补丁）
  * 增加 AWTK 应用程序的图标。

* 2020/06/09
  * 增加 dll 导出函数（感谢俊杰提供补丁）。 
  * 完善 app_conf 的注释（感谢瑞安提供补丁）。 

* 2020/06/08
  * 优化圆角矩形的抗锯齿效果（感谢智明提供补丁）。

* 2020/06/07
  * 增加 conf-io 用于存取应用程序的配置信息。
  * 增加文档《如何存取应用程序的配置信息》

* 2020/06/05
  * 增加文档《如何绘制旋转文字》
  
* 2020/06/05
  * 增加 lcd 相关文档。
  * 完善 API 注释，导出一些函数。
  * 完善 switch（感谢俊杰提供补丁）。 

* 2020/06/04
  * 完善 file_chooser 支持初始目录和过滤规则。
  * 完善 widget_dispatch，保证事件处理函数中，e->target 指向当前的 widget。
  * 增加文档《为 512K Flash 优化空间》
  
* 2020/06/03
  * 完善 path_normalize 函数。

* 2020/06/02
  * 完善文档
  * 完善 label
  * 增加 [awtk-inter-win-communication](https://github.com/zlgopen/awtk-inter-win-communication) 用于演示窗口之间的通讯方式。
  * 完善 image（感谢智明提供补丁）。 
  * 完善 file browser（感谢瑞安提供补丁）。 

* 2020/06/01
  * 增加 [awtk-widget-generator](https://github.com/zlgopen/awtk-widget-generator) 用于生成自定义控件。

  * 增加 [awtk-widget-qr](https://github.com/zlgopen/awtk-widget-qr) 二维码控件。

  * 完善 list item

* 2020/05/31
  * 修改 event id 潜在的问题。

* 2020/05/30
  * 增加文档《如何使用软键盘上的 action 按钮》 

* 2020/05/29
  * 完善 assets\_manager\_ref（感谢俊杰提供补丁）。 
  * 增加文档《如何使用对话框》
  * 修改无文件系统时输入法无法使用的问题。
  

* 2020/05/27
  * 去掉 assets_manager 中的 log。
  * 增加文档《如何访问控件的属性》。
  * 修改了 tk\_dl\_open 支持中文路径（感谢大恒提供补丁）。

* 2020/05/27
  * 增加文档《如何定制 combo_box 控件中按钮的风格和大小》
  * 增加文档《如何用 valgrind 查找内存相关问题》
  * 修改 agge 中内存未初始化的 BUG（感谢智明提供补丁）
  * 修改 file browser 在 Linux 下无法回到根目录的 BUG（感谢瑞安提供补丁）

* 2020/05/26
  * 完善 widget_dispatch。
  * 完善 scroll view 事件处理。
  * 完善 switch 事件处理（感谢俊杰提供补丁）。
  * 完善 draggable 事件处理（感谢俊杰提供补丁）。
  * 完善 list item 事件处理（感谢俊杰提供补丁）。 
  * 增加 1.4.1 分支
  * 增加工具 image\_dither（感谢智明提供）

* 2020/05/25
  * 修改切换语言奔溃的问题。
  * 修改 valgrind 发现的内存泄露。
  * 增加一些 DLL 导出函数（感谢尧燊提供补丁）。

* 2020/05/23
  * 支持控件设置独立的鼠标指针。
  * 更新文档《如何启用鼠标指针》
  
* 2020/05/22
  * 修改 list item 快速点击无效的问题（感谢 yeslee 提供补丁）
  * 完善缺省字体处理（感谢俊杰提供补丁）
  * 修改快速圆角矩形支持 mono 模式（感谢智明提供补丁）

* 2020/05/21
  * 增加文档《自定义控件规范》
  * 修正了 WITH\_BITMAP\_FONT 模式下 font\_manager\_unload\_all（比如语言切换）后由于字体名称设置错误导致字体加载失败的问题（感谢朝泽提供补丁）。
  * edit 控件增加 action text 属性，用于控制软键盘上 action 按钮的文本。
  * 用 key\_code\_is\_enter 判断是否是回车（感谢大恒提供补丁）。
  * 修复 gtest 的 Pixel 测试问题和优化了 dark 的混合效率（感谢智明提供补丁）。
  * 修复 mingw 编译报错的问题，同时让 mingw 和 32 位 linux 的 float_t 指定为 float 类型（感谢智明提供补丁）。
  * 修改 layout 的执行时机（感谢智明提供补丁）。

* 2020/05/20
  * 增加 canvas\_get\_font\_height
  * 增加导出一些函数方便自定义控件使用。
  * 完善编译脚本解决 DLL 一些问题。

* 2020/05/19
  * 增加《事件录制与重放》文档。
  * image 的 draw type 为 ceneter 时支持旋转和缩放。
  
* 2020/05/18
  * 增加事件录制与重放功能，辅助手动测试和压力测试。
  * 完善 text edit（感谢大恒提供补丁）。
  * 完善 system settings 和 release 脚本（感谢俊杰提供补丁）。
  * 支持硬件多层合并 (GUI + Camera)（感谢智明提供补丁）。

* 2020/05/14
  * 修复 clip view 中 type 的问题（感谢尧燊提供补丁）。
  * 修复 file browser up 中的问题（感谢瑞安提供补丁）。

* 2020/05/13
  * 完善文档。
  * 完善 tr_text 处理（感谢尧燊提供补丁）。
  * 增加状态 WIDGET\_STATE\_DISABLE\_OF\_CHECKED/WIDGET\_STAT\_DISABLE\_OF\_ACTIVE（感谢尧燊提供补丁）。
  * 完善文件选择器（感谢瑞安提供补丁）

* 2020/05/11
  * 将 google 拼音数据纳入资源管理，根据有无文件系统决定放到文件系统还是和代码一起编译。
  * 将联想字数据纳入资源管理，根据有无文件系统决定放到文件系统还是和代码一起编译。

* 2020/05/10
  * 更新文档。
  * 增加 semaphore_null.c

* 2020/05/09
  * 更新文档（感谢俊杰提供补丁）
  * 更新文档。
  * 增加 semaphore_null.c

* 2020/05/09
  * 更新文档（感谢俊杰提供补丁）

* 2020/05/08
  * 增加函数 tk\_wstr\_dup\_utf8
  * 增加函数 fs\_os\_file\_tell
  * 增加函数 fs\_os\_file\_size
  * 增加函数 fs\_os\_file\_stat
  * 增加函数 fs\_os\_file\_sync
  * 增加函数 tk_mutex_try_lock 以及各个平台的实现。
  * 重构 src/platforms/pc/fs_os.c 中的函数。
  * 完善 draggable 控件，enable=false 时，禁止拖动。
  * 修复如果 slide_view 控件其他控件遮挡时候，刷新 slide_view 控件会出现显示异常的问题和修复了 switch 控件对 片段式显存的支持（感谢智明提供补丁）。
  * 修改了片段式显存下 canvas 设置裁减区收到当前脏矩形控制，防止裁减区越界的问题（感谢智明提供补丁）。
  * 修复了 awtk 的警告问题（感谢智明提供补丁）。

* 2020/05/07
  * 完善 input\_method\_dispatch\_candidates。
  * 完善 widget_set_theme（感谢大恒提供补丁）
  
* 2020/05/06
  * 支持从 SD 卡加载资源，做了一些小的改动。
  
* 2020/05/04
  * 完善文件系统，函数指针放到 vtable 中，增加测试。

* 2020/05/03
  * 增加函数 str\_append\_more。
  * 在 EVT_BLUR 的事件回调中 widget_destroy_children（当前控件的父控件等）, 会导致 widget_dispatch_blur_event() 访问野指针（感谢朝泽提供补丁）。
  * 控件事件的回调被调用时，如果当前控件的 ref_count == 2，且回调中同时执行了 widget_destroy_children（当前控件的父控件等）、widget_destroy（自身），会导致 widget_unref_in_idle() 访问野指针（感谢朝泽提供补丁）。
  * 如果在窗口某控件的 EVT_BLUR 事件回调中调用 widget_set_need_relayout_children()，当焦点在该控件时关闭程序，会导致当前窗口的 destroy 在 widget_destroy_sync(window_manager()) 之后执行，引起 wm_on_destroy_child() 访问野指针（感谢朝泽提供补丁）。

* 2020/05/02
  * 完善控件动画（感谢朝泽提供补丁）。

* 2020/05/01
  * 完善 draw_image_repeat（感谢智明提供补丁）。
  * edit/mledit 支持用按键关闭、打开和 toggle [输入法](https://github.com/zlgopen/awtk/blob/master/docs/input_method.md)。

* 2020/04/30
  * 完善 update res 脚本（感谢朝泽提供补丁）。 
  * 完善 draw_image_repeat（感谢智明提供补丁）。

* 2020/04/29
  * 完善 rich text view 滚动条的处理。
  * lcd 增加 draw_image_repeat 函数，方便特定平台优化（感谢智明提供补丁）。
  * 完善 str gen 名称支持引号。
  
* 2020/04/28
  * 完善 color picker（感谢尧燊提供补丁）。 
  * 修复 guage_pointer 函数设置锚点不正确的问题（感谢智明提供补丁） 
  * 修复 nanovg 在高分屏下裁剪区裁剪字体显示不正常的问题（感谢智明提供补丁） 
  * 修复 vgcanvas 的着色器精度问题导致显示不正常的 bug（感谢智明提供补丁） 
  * 完善 image gen（感谢大恒提供补丁）。
  * Linux/MacOS/Windows 支持编译 awtk 为动态库。
  * 增加文档《静态链接与动态链接》
  * 完善 update res 脚本（感谢朝泽提供补丁）。 

* 2020/04/27
  * edit/mledit 支持翻译提示信息（感谢瑞安提供补丁）。 
  * 更新文档。

* 2020/04/24
  * 增加 rich text view 控件用于协调 rich text 和 scroll bar。
  * 修复在板子上面使用位图资源时候出现内存泄漏的问题（感谢智明提供补丁）
  * 修复圆角矩形画线的 bug 和圆角矩形支持片段式显存（感谢智明提供补丁）。
  * 修复 input method 中注释错误（感谢大恒提供补丁）。
  * 修复 is\_dialog\_opened 错误（感谢大恒提供补丁）。

* 2020/04/23
  * 去掉 widget 中不必要的 log。
  * rich text 支持滚动。
  * 完善 t9ext 输入法。
  * 完善 list view（感谢尧燊提供补丁）。 
  * 修复 window\_manager\_animate\_done 同时关闭两个窗口，打开一个窗口时的问题。

* 2020/04/20
  * 完善 tab button，允许兄弟控件中存在非 tab button 控件。
  * 增加触屏版 T9 输入引擎。
  * 增加按键版 T9 输入引擎。
  * 支持 rgb565 格式的图片（感谢智明提供补丁）。

* 2020/04/19
  * 修改 strgen 中处理注释的 BUG（感谢大恒提供补丁）。
  * 修复 agge 和 agg 的对齐图片资源的支持（感谢智明提供补丁）。
  * 修复 bitmap 的一些警告和统一命名（感谢智明提供补丁）。
  * 修复当月份等于零的时候出错的问题（感谢智明提供补丁）。
  * 修复警告（感谢智明提供补丁）。

* 2020/04/18
  * 修复在 SDL+gl 状态下旋转后后出现脏矩形不正常的问题（感谢智明提供补丁）。
  * 修复圆角矩形在大圆角+半透明的情况下，会出现重复绘制导致部分区域颜色不一致的问题（感谢智明提供补丁）。
  
* 2020/04/17
  * 修改 dialog 弹 dialog 然后两个都关掉的问题（感谢尧燊提供补丁）。
  * 修改 edit 的在 agge 中截图时 c 会被换成临时变量，在 setcustor 时 c 会非法的问题（感谢尧燊提供补丁）。
  * 修改 event\_source\_manager\_default 在 idle 中弹 dialog，manager->dispatching\_sources 会被 clear 的问题（感谢尧燊提供补丁）。

* 2020/04/11
  * 完善文档 lcd.md
  
* 2020/04/10
  * 完善 widget\_set\_need\_relayout\_children（感谢尧燊提供补丁）。

* 2020/04/09
  * 完善 main\_loop\_dispatch\_events（感谢陈谭提供补丁）。 
  * 完善 widget\_set\_need\_relayout\_children（感谢朝泽提供补丁）。
  * 完善编译脚本和工具（感谢大恒提供补丁）。

* 2020/04/08
  * 完善资源更新脚本（感谢朝泽提供补丁）。
  * 完善 key event cast（感谢大恒提供补丁）。

* 2020/04/07
  * 完善 slide\_view\_set\_active/pages\_set\_active
  * 优化 list view paint
  * 优化 widget\_set\_need\_relayout\_children
  * 完善 edit （感谢尧燊提供补丁）。
  
* 2020/04/05
  * pages 切换时保存和恢复焦点。
  * view 增加属性 default\_focused\_child
  * 增加文档 《slide_view/pages 各个页面缺省的焦点控件》

* 2020/04/05
  * list view 增加上下键的处理。
  * slide view 切换时保存和恢复焦点。

* 2020/04/02
  * 增加 darray\_bsearch
  * 完善 ui loader （感谢尧燊提供补丁）。
  * 完善 window base 窗口切换事件对 designer 处理（感谢培煌提供补丁）。

* 2020/04/02
  * 增加 waitable ring buffer
  * 修改资源的问题 pagespages 提供补丁）。
  * 修改 ti 编译器的问题（感谢陈谭提供补丁）。
  * 完善 image 名称为空的处理。
  
* 2020/04/01
  * 修正 overlay 的注释（感谢大恒提供补丁）。
  * 完善 fs 的创建和删除目录的处理（感谢大恒提供补丁）。
  * 增加函数 fs\_get\_user\_storage\_path 用于统一 PC 和 android 平台保存数据的目录。

* 2020/03/31
  * 支持一次关闭多个模态窗口（感谢尧燊提供补丁）。

* 2020/03/28
  * 修改 progress circle 中的内存泄露。

* 2020/03/27
  * 完善 window\_base（感谢尧燊提供补丁）。
  * 完善 main\_loop\_sdl2（感谢尧燊提供补丁）。

* 2020/03/26
  * 完善 edit/mledit 对 enter 键的处理（感谢大恒提供补丁）。
  * 完善窗口切换时焦点恢复的问题。
  * 修复部分 vg 控件有可能看不到的问题（感谢智明提供补丁）。
  * 修改软键盘无法关闭的 BUG。
  * edit/mledit 支持自己指定软键盘名称。
  
* 2020/03/25
    * 支持 scroll view 放在 slide view 中。（感谢 yeslee 提供补丁）。

* 2020/03/24
    * 增加工具 remove\_utf8\_bom

* 2020/03/23
  * 增加函数 tk_strlen。
  * 完善圆角矩形（感谢智明提供补丁）。
  * 完善工具支持多主题（感谢大恒提供补丁）。
  * 完善 text edit（感谢尧燊提供补丁）。

* 2020/03/22
  * 增加函数 widget\_set\_child\_text\_utf8
  * 增加函数 widget\_set\_child\_text\_with\_double
  * keyboard 在 grab_keys 时，keyboard 处理 key 事件后，应用窗口不再处理。
  * 完善 combobox，选择之后重新打开输入法。
  
* 2020/03/20
  * 去掉 main_loop_sdl.c 中重复的 quit（感谢尧燊提供补丁）。
  
* 2020/03/19
  * 完善资源管理（感谢朝泽提供补丁）。

* 2020/03/18
  * 增加 cjson。
  * str 中增加了辅助一些 json 打包的函数。
  * 完善资源管理（感谢朝泽提供补丁）。
  * WITH_BITMAP_FONT 时 locale change 后异常的问题；（感谢朝泽提供补丁）。
  * update_res_common.py 添加多主题处理时遗漏的 images/*.res、strings/*.data，fonts/*.data 等处理（感谢朝泽提供补丁）。
  * 完善圆角矩形（感谢智明提供补丁）。
  * 修改 dragger 放在 scroll view 中的问题（感谢大恒提供补丁）。
  * 完善 wstr\_format\_date\_time（感谢尧燊提供补丁）。

* 2020/03/17
  * 修复在 ubuntu 编译 fb_gl 模式下出现编译出错的问题（感谢智明提供补丁）。
  * 修复圆角矩形半径为负数时候出现的 bug（感谢智明提供补丁）。
  * progress circle 支持 line cap 属性。
  * 增加 vgcanvas\_line\_join\_t 定义。
  * 增加 vgcanvas\_line\_cap\_t 定义。
  * 更新文档和 idl.json
  * 修改 dragger 放在 list view 中的问题（感谢大恒提供补丁）。
  * 完善 combo_box （感谢尧燊提供补丁）。
  * 完善 edit（感谢尧燊提供补丁）。
  * 完善 slide menu（感谢尧燊提供补丁）。

* 2020/03/16
  * 无文件系统是支持多主题（感谢朝泽提供补丁）。

> 请注意，原有项目需要做如下修改：
> * 重新生成资源。
> * 加入 assets_default.ic
> * 使用 awtk_main.inc 实现 main 函数。

* 2020/03/15
  * 修改 ios 上运行的一些问题。
   
* 2020/03/14
  * 完善文档。
  * 把 socket_helper 放到 tkc 目录。
  
* 2020/03/13
  * 修改 mledit tips 颜色不正确的问题。 
  * 完善 awtk_main.inc
  * 增加动态链接库的接口 dl.h
  * 完善 demo_desktop
  * 完善 system_info（感谢朝泽提供补丁）。 
  * 修复 android back 键黑屏的问题。
  
* 2020/03/12
  * 完善 《 特殊平台编译配置》文档
  * 修改 android resume 后界面黑屏的问题。
  * 修复 widget 坐标为负数的时候导致脏矩形变大的问题（感谢智明提供补丁）。
   * 完善函数 str\_from_wstr\_with\_len。
  
* 2020/03/11
  * 修改 font manager 中的警告 （感谢智明提供补丁）。
  * 修复 ring buffer peek 中的问题 （感谢 [abelbilbil](https://github.com/abelbilbil) 提供补丁）。

* 2020/03/10
  *  完善 idle\_queue/timer\_queue 支持 web

* 2020/03/09
  * 增加 async.c/.h

* 2020/03/06
  * 完善 mutable image，支持自定义的图片创建函数。
  * 完善 asset manager 注释（感谢尧燊提供补丁）。
  * 完善 children layout （感谢尧燊提供补丁）。

* 2020/03/05
  * 增加 path\_replace\_extname 函数。
  * 完善 path\_replace\_basename 函数。
  * 完善 path\_build 函数。
  * 增加 data\_reader\_read\_all 函数。
  * 完善 clip\_view（感谢智明提供补丁）。
  * 完善 hscrollable、scroll\_view 和 slide\_menu（感谢智明提供补丁）。
  * 完善 font manager 关于 bitmap font 查找算法（感谢智明提供补丁）。

* 2020/03/04
  * slider 拖动时不允许调用 set value。

* 2020/03/03
  * 增加 STYLE\_ID\_HIGHLIGHT 相关定义。
  * 增加 widget\_dispatch\_recursive 函数。
  * 完善窗口管理器事件分发，窗口相关事件同时分发到控件。
  * 完善 SVG （感谢钟伟提供补丁）。
  * 完善 OpenGL shader（感谢智明提供补丁）。
  * 完善 image value，支持点击时加上一个增量，增加到最大值后回到最小值。

* 2020/03/02
  * 完善 README 中 Ubuntu 依赖软件包（感谢陈谭提供补丁）。
  * 优化内存使用（感谢智明提供补丁）。

* 2020/03/01
  * 增加 data writer file 
  * 增加 data reader factory
  * 增加 data writer factory
  * 完善 awtk\_main.inc（感谢陈谭提供补丁）。
  * 完善 demos（感谢陈谭提供补丁）。

* 2020/02/29
  * 完善 dialog 的 layout 处理（感谢朝泽提供补丁）。
  * 完善 fs stat 处理（感谢陈谭提供补丁）。
  * 修复 Linux SDL Audio 的问题（感谢智明提供补丁）。

* 2020/02/28
  * 完善 dialog set prop 对 layout 参数的处理（感谢朝泽提供补丁）。

* 2020/02/27
  * 完善 style const 和 ui loader（感谢尧燊提供补丁）
  * 增加文档 《AWTK 应用程序的入口》
  * 增加文档 《AWTK 资源打包》
  * 增加脚本 scripts/create\_assets\_zip.sh

* 2020/02/26
  * 增加 data reader 接口和 data writer，用于抽象外部 flash 等设备。
  * asset\_loadder\_zip 支持从 reader 读取数据，以便支持在没有文件系统时，从外部 flash 加载资源。

* 2020/02/26
  * 删除 slide\_view\_invalidate（感谢培煌提供补丁）。
  * 完善 edit 控件（感谢尧燊提供补丁）
  * 完善 guage 控件（感谢尧燊提供补丁）
  * 增加 asset\_loader 接口和缺省实现。
  * 增加 asset\_loader\_zip 用于从 zip 文件加载资源。

* 2020/02/25
  * 修复 canvas 裁剪区域导致测试失败的问题（感谢智明提供补丁）。

* 2020/02/24
  * 增加 awtk\_main.inc，用于标准程序的主函数。
  * 修复 mledit scroll line 无法通过 set\_prop/get\_prop 访问的问题（感谢尧燊提供补丁）
  * 修改 thread\_with\_pthread.c 中的编译问题（感谢陈谭提供补丁）
  * 修改 ostream_retry 中内存溢出的问题（感谢培煌提供补丁）。

* 2020/02/23
  * 用 SDL 重新实现 PC 版本的线程和同步相关函数。
  * 完善 canvas 裁剪区域（感谢智明提供补丁）。

* 2020/02/21
  * 处理 jpeg 文件扩展名（感谢朝泽提供补丁）。
  * 修改 xml\_to\_ui 内存问题（感谢培煌提供补丁）。
  * 修改一些警告（感谢培煌提供补丁）。
  * 修复 edit 密码可见性的问题（感谢尧燊提供补丁）
  * 修复 SDL 警告（感谢智明提供补丁）。
  * 完善 systembar 支持动态改变窗口的 title 和 closable 属性。
  * 修复 list\_view\_h 中 spacing 属性类型的问题（感谢尧燊提供补丁）
  * 修复 widget\_animator\_time\_elapse 中的问题（感谢朝泽提供补丁）。

* 2020/02/20
  * 增加文档《如何集成第三方库》
  * 完善 slide view 脏矩形（感谢培煌提供补丁）。
  * 修复控件旋转缩放的情况下裁剪区域的问题（感谢智明提供补丁）。

* 2020/02/19
  * 增加 widget\_remove\_timer（感谢陈谭提供补丁）。

* 2020/02/18
  * native window 增加 center/maximize/minimize/restore/fullscreen 等函数。
  * native window 支持脚本化。
  * 增加 widget\_get\_native\_window。
  * 更新 awtk_ecology（感谢陈谭提供补丁）。）

* 2020/02/17
  * 修改在修复 clip\_view 控件在没有 vg 的时候的报错问题（感谢智明提供补丁）。
  * 修改在 idle/timer 中调用 dialog\_modal 导致死循环的问题（感谢智明提供补丁）。
  * 完善 value\_double（感谢尧燊提供补丁）。

* 2020/02/15
  * 修改 assets\_manager\_deinit 中的内存错误（感谢俊杰提供补丁）。

* 2020/02/14
  * 修复 idle 的队弹出数据后出现超时导致弹出的队无法执行（感谢 sqqdfny 和智明提供补丁）。
  * 完善 edit/mledit 快捷键的处理（感谢大恒提供补丁）。
  * 完善 digit clock 的注释（感谢俊杰提供补丁）。

* 2020/02/13
  * 修复 list\_view 在切换 auto\_hide\_scroll\_bar 属性 scroll\_bar 不显示问题（感谢大恒提供补丁）。
  * 修复 error\_event\_t 的注释（感谢大恒提供补丁）。

* 2020/02/12
  * 修复在 agge 的状态下圆角矩形不受裁剪区的影响的 bug （感谢智明提供补丁）。 
  * 修复 agge 模式下编译错误（感谢培煌提供补丁）。

* 2020/02/11
  * opengles 支持 snapshot（感谢智明提供补丁）。
  * 修复了 demoui 的 combox 功能的第一个 combox 在点击 edit 弹出输入法后，再点击按钮时候，导致输入法的动画异常的问题（感谢智明提供补丁）。

* 2020/02/09
  * 增加 action thread pool

* 2020/02/08
  * 增加 action thread

* 2020/02/07
  * 增加 waitable\_action\_queue

* 2020/02/06
  * 增加 action\_queue
  * 修改内存统计错误（感谢培煌提供补丁）。

* 2020/02/05
  * 增加拼音输入数据更新工具。
  * 完善联想词库工具生成文档。
  * 增加拼音输入数据更新文档。
  * 增加宏 WITHOUT\_SUGGEST\_WORDS 用于关闭联想字功能。

* 2020/02/04
  * 片段 lcd 的片段内存大小刚好等于脏矩形大小的问题（感谢智明提供补丁）

* 2020/01/28
  * 优化 list view paint children
  * 增加 emitter\_forward

* 2020/01/20
  * compare\_color 不区分大小写（感谢 Toby 提供补丁）
  * 为移植到 iOS 做了一下改进。

* 2020/01/17
  * 修复 list view on remove child 的问题（感谢培煌提供补丁）
  * 完善 image animation （感谢大恒提供补丁）

* 2020/01/16
  * 修复 progressbar 类型问题（感谢尧燊提供补丁）
  * 修复 arm linux 编译问题（感谢俊杰提供补丁）
  * 完善 mledit/edit 文档 （感谢大恒提供补丁）
  * 完善 str\_expand\_vars（感谢朝泽提供补丁）
  * 修复 edit 和 mledi t 控件不断 invalidate 的问题 （感谢智明提供补丁）
  * 更新生态文件 （感谢陈谭提供补丁）

* 2020/01/15
  * 点击鼠标右键触发 context menu 事件。
  * 完善 color picker （感谢朝泽提供补丁）
  * 完善 func call parser （感谢俊杰提供补丁） 
  * 完善 progress bar 文档 （感谢大恒提供补丁）
  * 修复 edit 和 mledi t 控件不断 invalidate 的问题 （感谢智明提供补丁）
  * 修改 使用 sdl 后屏幕旋转的显示不正常问题（感谢智明提供补丁）
  * 修复不启用脏矩形的时候，设置 vg 脏矩形的不正确的问题（感谢智明提供补丁）

* 2020/01/14
  * awtk 修复 opengles 下 vg 缩放后填充颜色不正常的问题（感谢智明提供补丁）
  * 完善 paint children（感谢大恒提供补丁） 
  * 完善 comments（感谢大恒提供补丁） 
  * awtk 修复 opengles 下 vg 缩放后填充颜色不正常的问题（感谢智明提供补丁）
  * 修改了 stbi__shiftsigned()，C 语言中负数按位右移高位补 1 导致触发 assert 的问题（感谢朝泽提供补丁）
  * 完善 direct.inc （感谢尧燊提供补丁）

* 2020/01/13
  * widget clone 不复制 focused 属性。

* 2020/01/12
  * 增加函数 date\_time\_from\_time
  * 将格式化日期的函数放到 date\_time\_format.c 中。

* 2020/01/11
  * 完善 combobox, readonly 时点击任意位置，弹出 popup 窗口。
   
* 2020/01/10
  * 完善 gif image（感谢尧燊提供补丁） 
  * 完善 widget 状态 （感谢朝泽提供补丁） 
  * 完善 source manager （感谢智明提供补丁）
  * 完善 widget\_get\_text
  * 完善 README.md（感谢陈谭提供补丁） 
  * 修复 svg 初始位置还有一个关于 S 元素贝塞尔曲线算法的补充的 不然压缩包里的示图会显示不正确（感谢钟伟提供补丁） 
  * 修复当高分屏的比例为非整数的时候，在改变窗口大小后会导致 fbo 不断的新建，导致 UI 显示不正常的问题（感谢智明提供补丁）

* 2020/01/09
  * 完善 rich text（感谢尧燊提供补丁） 
  * 完善 color picker（感谢尧燊提供补丁） 
  * 完善 combo box item（感谢尧燊提供补丁） 
  * 完善 popup（感谢朝泽提供补丁）
    
* 2020/01/08
  * 修改在 stm32f429 上编译的 BUG。
  * xml 解析器支持不去掉两端的空格。
  * 完善 slide menu （感谢朝泽提供补丁） 
  * rich text 支持换行符和水平对齐（感谢尧燊提供补丁） 
  * （感谢智明提供下列补丁）
    * darray 部分函数改为 static
    * 修复当裁剪区设置错误时候，会导致绘图的矩形 w 和 h 出现负数导致板子上面硬件加速出现崩毁的情况，所以在判断的时候加多一个负数的判断

* 2020/01/07
  * 完善 func\_call\_parser，允许出现"\r\n\t"。
  * 完善 main loop。用户可以为 main loop simple 提供两个函数，以实现在中断里发送事件。
  * （感谢大恒提供下列补丁）
    * text_edit 增加非空判断
    * dragger 增加鼠标弹起时拖动状态判断
    * 修复 gif 缩放时不会动的问题

  * 完善 combo box item（感谢尧燊提供补丁） 
  * （感谢智明提供下列补丁）
    * 修复了使用位图资源在 nanovg 显示不正常的问题
    * 修改 agge 模式下，使用 nanovg 绘图中释放 specific 数据的逻辑
    * 修改了删除当画圆角边框类型不为全边框线的断言，改为 log_warn 输出日志和改画直角边框
    * 修改一些警告地方
    * 修改 fill rect 的问题。

* 2020/01/06
  * 完善 rich text（感谢尧燊提供补丁） 
  * （感谢智明提供下列补丁）
    * 修复调用 image_manager_unload_unused 函数释放图片的时候，会导致 agge 中的图片队列不正常的 bug 。
    * 修复窗口动画播放前，会先把新窗口绘画出来，然后再下一帧才播放窗口动画，会导致画面有闪烁一下的问题。（给 window_manager_default_t 类型增加了一个成员变量 ready_animator 用来标记新的窗口已经创建了但是窗口动画在下一帧才会播放，让 paint 函数跳过绘画当前帧，避免当前帧出现新窗口的画面）
    * 在 widget_is_window_opened 函数的判断中加入 WINDOW_STAGE_SUSPEND 状态判断。
    * 修改了 edit 和 mledit 控件在触发 EVT_POINTER_DOWN_ABORT 事件后，导致 focused 属性异常无法触发 EVT_BLUR 事件导致无法正常关闭软键盘。
    * 修复在 agge 状态下切换主题的时候会使用 canvas 的裁减区设置 vg 的裁减区，有概率导致使用 vg 绘制的控件无法正常显示。
    * 降低了画圆角矩形的圆角判断条件要求。

* 2020/01/05
  * 完善 fs 接口。
  * 修改 widget_is_opened_popup 函数注释（感谢大恒提供补丁）。
  * list_view 增加重复绑定 scroll_view 和 scroll_bar 处理（感谢大恒提供补丁）。
  * 优化 list_view 按键弹起 scroll_bar_m 的隐藏处理（感谢大恒提供补丁）。

* 2020/01/03
  * 修复了画圆角矩形边框的 BUG。（感谢智明提供下列补丁）
  * 完善 widget.h 注释（感谢大恒提供补丁）。 
  * 完善 ui loader xml（感谢大恒提供补丁）。 
  * 完善 guage pointer（感谢尧燊提供补丁）
  * 增加 widget\_take\_snapshot 目前仅在非 OpenGL 下有效。
  * 完善 popup 窗口动画（感谢智明提供补丁）
  * 完善修改 locale 的处理。
  * 修复 clip_view 剪裁区域计算有误的问题（感谢大恒提供补丁）。
  * 修复 scroll_bar_m 在拖动后没有自动隐藏的问题（感谢大恒提供补丁）。

* 2020/01/02
  * 修改 kb_default.xml 解决中文状态下无法输入空格。
  * 支持软键盘选择候选字。
  * 增加 dirty\_rect\_tolerance 属性。表示脏矩形超出控件本身大小的最大范围。缺省为 4，一般不用指定。如果 border 太粗或 offset 太大等原因，导致脏矩形超出控件本身大小太多（大于缺省值）时，才需要指定。
  * tab\_button 和 combo\_box\_item 增加类型检查（感谢钟伟提供补丁）。
  * 增加支持图片中间部分平铺绘图方法（解决很小的图片使用 patch3_xxxx 的 draw_type，在启用鼠标后鼠标滑过图片会出现没有办法重画的问题）（感谢智明提供下列补丁）
  * 修复 agge 支持字体透明度（感谢智明提供下列补丁）
  * 新增了 window_manager_resize() 用于调整原生窗口的大小（感谢朝泽提供补丁）
  * 修复了画圆角矩形边框，边框为 1 的情况导致 agge 的 cache 不正常为难题。（感谢智明提供下列补丁）
  * 修改圆角矩形填充前景色和背景色叠加的显示效果（感谢智明提供补丁）

* 2020/01/01
  * 优化 widget\_clone
  * （感谢智明提供下列补丁）
    * 修复对话框再弹出对话框时，前一个窗口的截图的显示问题
    * 修复 agge 的填充颜色半透效果
    * 增加 agge 对 image 支持半透效果
    * 增加 nanovg 查找 nvgFindTextureRaw 函数，用来判断 agge 的时候图片是否在 nanovg 图片数组中（lcd 的 resize 函数释放 nanovg 的图片会把图片数组释放，导致下次调用崩毁）
    * 修复画部分边框并且宽度大于 1 的情况显示问题。
    * 修改一下警告地方

* 2019/12/31
  * 完善 RichText（感谢尧燊提供补丁）

* 2019/12/30
  * assset\_info 相关函数移到到 tkc/asset\_info.c 中。
  * asset manager 支持自定义资源加载。
  * 输入法候选字支持数字选择候选字
  * 输入法候选字支持左右键切换候选字的焦点
  * 修改 calibration 窗口内存泄露问题（感谢培煌提供补丁）。
  * 完善 RichText（感谢尧燊提供补丁）
  * 完善 children layout（感谢尧燊提供补丁）
  * 输入法候选字支持脚本化（感谢大恒提供补丁）。
  * （感谢智明提供下列补丁）
    * 播放窗口动画前查找 prev\_win 不正确的问题
    * 修复了 slide\_view 控件在 agge 的状态下，无法裁剪使用 vg 绘制的控件问题
    * 修复 asset\_info\_create 函数在没有开启 WITH\_FS\_RES 宏下编译出错的问题
    * 修复 window 在切换为后台的时候，保存 save\_focus\_widget 为自己的时候，就会导致引用计数出错，导致出现一棵无主的窗口树。
    * 修复控件动画在播放的时候退出程序，导致控件动画内存泄漏问题
    * 修复主题切换后，新的主题的引用计数出错，导致内存泄漏问题
    * 修复了弹出输入法中只增加 EVT\_WINDOW\_CLOSE 事件，却不触发的事件问题，导致消息分发器内存泄漏的问题
    * 修复 opengles 的 fbo 退出程序时候释放内存
    * 修复 gpinying 拼音退出程序时候内存泄漏的问题
    *调整了 tk\_deinit\_internal 函数让 widget 等资源可以顺利通过 idle 来释放。

* 2019/12/29
  * 使用 event\_source\_manager 实现主循环。
  * iostream 增加几个包装函数。

* 2019/12/27
  * 增加 str\_from\_wstr\_with\_len
  * 增加 tk\_utf8\_from\_utf16\_ex
  * 感谢福明提供以下补丁： 
    * hscroll\_label 的修改：默认情况下鼠标移入控件时文本停止滚动，鼠标离开控件时开始滚动。
    * 使用 label 控件的文本太长时会出现不显示的情况，主要是因为 lcd\_vgcanvas.inc 里面定义的一个文本长度限制。

* 2019/12/26
  * 完善 mem 中的计数（感谢大恒提供补丁）。
  * 增加 widget\_close\_window
  * 修改了 lcd 的宽高被修改后脏矩形的宽高不修改导致裁剪去有问题（感谢智明提供补丁）
  * 增加了把一个资源 data 加载到 awtk 的资源管理器的函数（感谢智明提供补丁）
  * 完善 window\_base\_on\_event（感谢尧燊提供补丁）

* 2019/12/25
  * 完善 line number（感谢大恒提供补丁）。
  * 感谢瑞安提供以下补丁
    * 打包工具对中文路径的支持
    * imagegen.exe 不用断言判断图片尺寸超出范围而是直接返回错误码，避免程序因断言失败而崩溃。
    * 为了与加载 bin 文件时的资源名保持一致，所以路径也将作为资源名的一部分。
    * 在 xml\_to\_ui.exe 增加第四个传入参数（资源名），在生成、*.data 填入资源名时作为参考。

* 2019/12/24
  * 完善 SVG（感谢钟伟提供补丁）

* 2019/12/23
  * 完善 edit（感谢尧燊提供补丁）

* 2019/12/19
  * 完善 emitter\_off\_ex（感谢尧燊提供补丁）
  * 修复 clone 的 widget 没有拷贝到 feedback 属性（感谢智明提供补丁）
  * 完善部分控件的事件处理（感谢大恒提供补丁）。
  * calibration win 支持脚本化。

* 2019/12/18
  * 完善窗口管理器支持 dialog 叠加（感谢尧燊提供补丁）。
  * utf8 中内部函数改为 static。
  * 完善 str\_unescape。
  * 修复了 gles 在部分系统上面颜色显示不正常的问题（感谢智明提供补丁）
  * 修复了 svg 工具生成 bsvg 的时候对 svg 的默认颜色取值问题（感谢智明提供补丁）
  * 完善 widget\_set\_tr\_text
  * 完善 color component 事件处理（感谢大恒提供补丁）。
  * 完善 combobox ex 的 style 处理（感谢大恒提供补丁）。
  * 完善 tab button，支持动态加载指定页面（感谢智明提供补丁）
  * 增加函数 window\_create\_default
  * 完善 emitter\_off\_ex（感谢尧燊提供补丁）

* 2019/12/17
  * 完善 dialog\_confirm/dialog\_info 等函数。
  * 增加 widget\_set\_focusable 函数。
  * 重命名：utf8\_to\_utf16 to tk\_utf8\_to\_utf16 （感谢智明提供补丁）
  * 重命名：utf8\_from\_utf16 to tk\_utf8\_from\_utf16 （感谢智明提供补丁）

* 2019/12/13
  * 修改窗口切到 FOREGROUND 把焦点设回去会导致如果当前页面有滚动控件时，页面会滚动的问题（感谢大恒提供补丁）。
  * 修改 widget 在 point_down 少发了个 leave 事件的问题（感谢大恒提供补丁）。
  * 完善 set tr text （感谢尧燊提供补丁）

* 2019/12/12
  * WIDGET LOAD 事件改在子控件加载完成时触发。
  * hscroll label 在文本改变时重置 xoffset。
  * 完善 combobox 使用 hscroll label 的处理。
  * set key target 时触发 focus 事件。
  * 修复圆角巨型在 gles 的时候，设置 vg 的 Scale 显示不正常的问题（感谢智明提供补丁） 
  * 修改了 border_width 为 0 还是绘制边框的问题（感谢智明提供补丁） 
  * 完善 rich text 控件（感谢尧燊提供补丁）。 
  * 完善动画脏矩形（感谢培煌提供补丁）。 
  * 增加 clip view 控件（感谢智明提供） 

* 2019/12/11
  * 增加 WIDGET LOAD 事件和状态。
  * 修复 text edit 中一些警告。
  * 入选码云最有价值开源项目，根据要求修改 READMEM 中的链接。
  * 增加文档 [如何在主题文件中写控件布局参数](how_to_write_layout_params_in_style.md)
  * 完善窗口切换的处理，窗口切换到前台时，重新设置原来的焦点控件为焦点。
  * combobox 支持 hscroll label 作为子控。
  * 修改 mledit tips 泄露的问题（感谢尧燊提供补丁）。 
  * switch 增加对 image name 为空串的检查（感谢尧燊提供补丁）。 
  * 修复 scroll_view 拖动时计算有误的问题（感谢大恒提供补丁）。 
  * 增加 TK\_CLEAR\_IMAGE\_ALPHA 用于填充 framebuffer 时，同时设置 framebuffer 的 alpha 值。

> 主题数据格式变化，请使用资源生成脚本，重新生成资源数据！！！

* 2019/12/10
  * 更新文档（感谢陈谭提供补丁）。
  * 完善 style const （感谢尧燊提供补丁）。 
  * 优化主题数据的存储方式，控件减少 30%。
  * 主题支持 self layout/children layout
  * 修复画圆角矩形在画布的 line_lenght != w*bpp 时候打点不正常的 bug（感谢智明提供补丁）  
  * 修复 mledit 显示 bug（感谢智明提供补丁）。  
  * 修复 fbo 脏矩形和裁减区不重叠的显示不正常 bug（感谢智明提供补丁）。  
  * 修改 lcd_men.inc 文件的每次获取 vg 的时候都会设置裁减区为全屏的问题（感谢智明提供补丁）。  
  * 更新文档。

> 主题数据格式变化，请使用资源生成脚本，重新生成资源数据！！！

* 2019/12/09
  * 完善 dialog ex 的文本翻译。
  * 支持设置 combobox popup 的 style。
  * edit 增加 input type "custom_password"类型。
  * 完善 text selector （感谢大恒提供补丁）。
  
* 2019/12/07
  * 修改 widget\_set\_state 脚本绑定时的问题（感谢大恒提供补丁）。
  * switch 控件：修正 max_xoffset_ratio 属性修改后 xoffset 没有更新的问题；（感谢朝泽提供补丁）。 
  * slider 控件：（感谢朝泽提供补丁）。 
    *（1）修正当控件的值比较小时滑块与前景部分之间有空白的问题（修改为始终在滑块的中间）；
    *（2）修改控件的背景和前景没有贴合的问题；
    *（3）修改 dragger_size 属性允许设置，以便修改滑块的宽度或高度；
    *（4）添加 dragger_adapt_to_icon 属性，用于标识滑块大小是否与 icon 适应
    *（5）添加 slide_with_bar 属性，用于标识是否允许在轴上滑动来改变滑块位置

* 2019/12/06
  * 完善脚本绑定。

* 2019/12/05
  * 修改了 image_animation 控件的 format、sequence 属性设置，为空字符串时设为空，避免显示有误（感谢朝泽提供补丁）。 
  * 增加文档 how\_to\_use\_mutable\_image.md（感谢智明提供补丁）。  

* 2019/12/04
  * awtk 增加对 lcd 为 rgb888 的支持 （感谢智明提供补丁）。 
  * 修改了 bitmap 设置不透明度的标志（感谢智明提供补丁）。  
  * awtk 增加图标所在的位置支持垂直水平居中功能 （感谢智明提供补丁）。  
  * 完善增加了 scroll_view 控件可以调节控件自动播放的速度比例接口 （感谢智明提供补丁）。  
  * 完善 tk\_mem\_stat （感謝王程提供补丁）。  
  * 完善 assset_manager 关于自定义路径的处理（感谢大恒提供补丁）。
  * 完善 scroll view 的注释（感谢大恒提供补丁）。
  * 增加函数 window\_manager\_back\_to 回到指定的窗口。
  
* 2019/12/03
  * 完善 widget\\_destroy（感谢尧燊提供补丁）。 
  * 上次 EVT\_POINTER\_DOWN\_ABORT 触发多次，破坏了 grab\_widget 链的问题还是有点问题，改成在 widget\_ungrab 时限制；（感谢朝泽提供补丁）。 
  * 由于 scroll\_view 的 EVT\_POINTER\_UP 响应没有判断 scroll\_view->pressed，导致在外部点击鼠标并拖动到 scroll\_view 内部释放时可能引起视图滚动；（感谢朝泽提供补丁）。 
  * idle\_manager\_exist 判断条件有误（感谢朝泽提供补丁）。  
  * 完善 widget\_create\_animator（感谢培煌提供补丁）。  
  * 在窗口打开时递归 update style。
  * 完善输入避免 google pinyin 崩溃
  * 更新 stb truetype.h
  * slide\_menu 的 children 为空，点击会崩溃的问题（感谢大恒提供补丁）。 
  * slide\_view 在 set\_active 时判断 foused 为 false，不进行焦点的切换 （感谢大恒提供补丁）。 
  * 完善 dragger 事件处理（感谢大恒提供补丁）。 
  * 完善 text selector 事件处理（感谢大恒提供补丁）。 
  * 修改 dialog/window/popup/calibration 里的注释问题。（感谢温宇飞 <wenyufei@aliyun.com>提供补丁）。 

* 2019/12/02
  * 完整 list item 支持 return/space 激活 （感谢 [jianmin zhuang](https://github.com/zjm1060) 提供补丁）。
  * 比如一个控件（比如 edit）的父控件响应 EVT\_POINTER\_DOWN\_ABORT 并 widget\_ungrab，在 edit 内按下鼠标，拖到控件外后释放鼠标，edit 控件没有收到 EVT\_POINTER\_UP，导致状态没复位。原因是 EVT\_POINTER\_DOWN\_ABORT 触发多次，破坏了 grab\_widget 链；（感谢朝泽提供补丁）。
  * 修正在 scroll\_view 外部按下鼠标之后移动到 scroll\_view 内，scroll\_view 会跟随滚动的问题（感谢朝泽提供补丁）。

* 2019/11/30
  * 修改 widget\_on\_attach\_parent 为 widget\_on\_attach\_parent\_t，widget\_on\_detach\_parent 为 widget\_on\_detach\_parent\_t （感谢朝泽提供补丁）。
  * 完善 slide\_view：（感谢朝泽提供补丁）。
    （1）切换页面过程中由于脏矩形或裁剪区域计算错误导致会有残影的问题，比如内部 view 有背景、内部控件有边框、或者控件超出 slide_view 范围；
    （2）修改 overlap 方式切换过程中如果拖动距离超出单页的宽或高时，则停留在上一页或下一页

* 2019/11/29
  * 修改定制软键盘密码不能隐藏的问题。
  * 修改 widget\_unref/object\_unref 的问题。
  * 修复 nanovg 画举行边框时候边框大小不为 1 的问题（是之前修复高分屏填充问题引起的）（感谢智明提供补丁）。
  * 修改 widget 透明度为 0 的显示问题，如果 widget 的 opacity 低于 8 的时候，就视为不可见，就跳出 widget\_paint 函数的执行（感谢智明提供补丁）。
  * 修改 native\_window\_fb\_gl 和 native\_window\_sdl 在编译 agge 的时候出现的警告（感谢智明提供补丁）。
  * 修复了 mledit 设置 margin 后不会马上刷新界面的问题（感谢智明提供补丁）。

* 2019/11/28
  * 修复 preview ui 预览 dialog 时，拖动 dialog 出现残影的问题。
  * 引入引用计数协助管理 widget 的生命周期。
  * 完善 combobox 弹出对话框位置的问题。
  * 完善 圆角矩形绘制（感谢智明提供补丁）。
  * 完善 vgcanvas（感谢智明提供补丁）。
  * 完善 scroll view（感谢朝泽提供补丁）。
  * 完善 销毁子控件的处理（感谢培煌提供补丁）。
  * 完善 native\_window\_sdl\_resize （感谢 [qianfan-Zhao](https://github.com/qianfan-Zhao) 提供补丁）。 
  * 完善 set need relayout children。

* 2019/11/27
  * 完善 edit/mledit （感谢智明提供补丁）。
  * 完善 agge （感谢智明提供补丁）。

* 2019/11/26
  * 完善 log （感谢大恒提供补丁）。
  * 完善 combo\_box\_ex（感谢大恒提供补丁）。
  * 完善 edit/mledit 在不可见时的事件处理（感谢大恒提供补丁）。
  * 完善 list view（感谢朝泽提供补丁）。
  * 增加 progress event。
  * 增加 dialog\_quit\_code\_t。
  * 增加 file transfer 用于将资源下载到外部 flash（未完成）。
  * window\_manager\_back/window\_manager\_back\_to\_home 允许关闭对话框，DIALOG\_QUIT\_NONE 退出码表示对话框可能是被强行关闭。

* 2019/11/24
  * 增加函数 str\_append\_int。

* 2019/11/22
  * 完善 log 函数。
  * 完善 list item（感谢朝泽提供补丁）。
  * 完善 emitter（感谢朝泽提供补丁）。
  * edit 增加属性 open\_im\_when\_focused （感谢 [qianfan-Zhao](https://github.com/qianfan-Zhao) 提供补丁）。
  * 完善和优化绘图函数（感谢智明提供补丁）。

* 2019/11/21
  * 完善 edit 焦点处理。
  * 完善 keyboard 焦点处理。
  * 完善 combobox （感谢大恒提供补丁）。
  * edit 增加属性 select\_none\_when\_focused。

* 2019/11/20
  * 更新生态文档（感谢陈谭提供补丁）。
  * 更新资源生成脚本文档（感谢俊杰提供补丁）。
  * 完善 [控件焦点](widget_focus.md)
  * 完善 keyboard，切换 page 时设置第一个 focusable 控件为焦点。
  * 修改 combobox 焦点问题（感谢大恒提供补丁）。

* 2019/11/19
  * 完善 darray\_sort。
  * 完善 widget\_calc\_icon\_text\_rect （感谢大恒提供补丁）。
  * 完善 WIDGET\_PROP\_TEXT 先从控件获取，控件没有特殊处理，最后才取默认值。
  * 完善 image（感谢朝泽提供补丁）。
  * 完善 list item （感谢朝泽提供补丁）。

* 2019/11/18
  * 完善 hscroll label，在 only\_parent\_focus 为 TRUE 时，根据父控件设置自己的 focus 状态。
  * 完善 list item （感谢朝泽提供补丁）。
  * 完善 awtk\_ecology.md

* 2019/11/16
  * 完善文档。

* 2019/11/15
  * 完善文档。
  * 完善 image 绘制（感谢大恒提供补丁）。
  * hscroll label 支持在父控件获得焦点是滚动。

* 2019/11/14
  * 完善进度条（感谢朝泽提供补丁）。
  * 修改 edit 注释问题（感谢朝泽提供补丁）。
  * 增加键值 TK\_KEY\_BACK。
  * 增加 lcd\_mem\_fragment。
  * 增加 graphic\_buffer\_attach。
  * 重构 lcd\_mem
  * 完善 tkc 的文档（感谢俊杰提供补丁）。
  * 完善 medit（感谢智明提供补丁）。
  * 完善 vgcanvas fill rect（感谢智明提供补丁）。

* 2019/11/13
  * 把部分变量修改为常量，以减少内存使用。
  * 更新 IDL 修复脚本绑定的问题。
  * 增加 ICON\_AT\_BOTTOM 的处理（感谢朝泽提供补丁）。
  * 修改 android 编译问题（感谢智明提供补丁）。
  * 完善 medit（感谢智明提供补丁）。

* 2019/11/12
  * 修复了 vg 旋转显示 bug 和修改了贴图兼容性（感谢智明提供补丁）。
  * digit clock 移到 widgets 目录。
  * 增加 demo\_basic 用于在低端平台演示使用 UI 文件。

* 2019/11/11
  * 完善 list item click 事件处理（感谢培煌提供补丁）。
  * 完善部分控件的类型检查（感谢尧燊提供补丁）。
  * 更新生态文档（感谢陈谭提供补丁）。
  * 简化 tab\_button\_group 边框处理。

* 2019/11/10
  * 增加 darray\_sort。

* 2019/11/09
  * 完善 combo\_box 按键处理。
  * AWTK\_ecology.md（感谢陈谭提供补丁）。

* 2019/11/08
  * 完善 rich text （感谢大恒提供补丁）。
  * 完善 text edit（感谢尧燊提供补丁）。

* 2019/11/07
  * 完善 cmsis\_os 移植。
  * 支持重新启动已经完成的动画。
  * 完善 event source/event source manager。
  * 修改输入法中，软键盘弹出时，保存窗口位置的问题。
  * OpenGL 模式支持脏矩形（感谢智明提供补丁）。
  * 修改 slider 在 list view 中 grab 的问题（感谢培煌提供补丁）。
  * 增加 WIDGET\_PROP\_EXEC 方便 MVVM 通过属性来控制动画的状态。
  * 增加 indicator\_default\_paint\_t 枚举类型导出（感谢大恒提供补丁）。
  * 修改动态设置 default\_paint 属性为 auto 时没有效果的问题（感谢大恒提供补丁）。
  * slide\_view 增加/删除，或者改变位置/大小时，没有同步更新的问题（感谢朝泽提供补丁）。
  * auto\_hide 动画时间设置错误，以及 auto\_hide 之后设置 visible 不能显示的问题（感谢朝泽提供补丁）。
  * 完善 assets\_manager\_set\_theme（感谢朝泽提供补丁）。

* 2019/11/06
  * 修改 emitter 中的错误（感谢朝泽提供补丁）。
  * 修改 image manager 对 raw image 的处理，增加 graphic buffer 之后，raw image 也需要由 image manager 管理。
  * 增加 scripts/release.sh 脚本，用于将运行时需要的文件拷贝到 release 目录。
  * 允许在 XML 文件中指定 combobox 按钮的位置大小和样式。

* 2019/11/05
  * list\_view/list\_view\_h 增加 pageup/pagedown 按键处理。
  * 修复 native window resize 时没有更新 system info 中 lcd 大小的问题（感谢朝泽提供补丁）。
  * 修复文档中的错误（感谢朝泽提供补丁）。
  * 完善旋转的处理（感谢智明提供补丁）。
  * 增加键值映射功能，请参考 [map\_key.md](map_key.md)
  * 优化 lcd\_mem（感谢智明提供补丁）。

* 2019/11/04
  * 完善字体和文字绘制。

* 2019/11/03
  * 修改 graphic\_buffer 改动引入的问题。
  * 完善 close window force 的流程。
  * 完善 back/back_to_home 的处理。
  * 完善 text selector 数据绑定的问题。
  * 增加 date_time_set 函数和初始化函数。请使用 date\_time\_global\_init 初始化时间函数。
  
* 2019/10/31
  * 增加 graphic\_buffer，用于管理 bitmap 的内存。
  * 完善 vgcanvas，兼容 JZ\_GPU。
  * 完善 darray，增加 darray\_find\_all。
  * lcd nanovg 支持 LCD 旋转（感谢智明提供补丁）。
  * 增加 native\_window\_fb\_gl （感谢智明提供补丁）。
  * 修改 valgrind 发现的问题。

> 如果有支持访问 bitmap-\>data 的代码，请使用 bitmap\_lock\_buffer\_for\_write/read 函数。

* 2019/10/30
  * 增加 system\_bar\_bottom，支持 system_bar 在顶部、底部和两者。

* 2019/10/29
  * 公开函数 image\_animation\_next。
  * 修改 SDL 输入按键事件的问题（感谢智明提供补丁）。

* 2019/10/29
  * 完善 slideview indicator（感谢朝泽提供补丁）。
  * 修改 svg 路径太长的问题（感谢尧燊提供补丁）。
  * 修改 popup 支持点击外部关闭的问题（感谢尧燊提供补丁）。
  * 完善 vgcanvas\_nanovg\_set\_font（感谢俊杰提供补丁）。
  * 修改 semaphore.c 在 im28x 上编译的问题（感谢俊杰提供补丁）。
  * 修改 build\_asset\_filename\_custom 的问题（感谢瑞安提供补丁）。

* 2019/10/27
  * 增加 ostream\_retry。

* 2019/10/26
  * 完善 ring buffer。

* 2019/10/25
  * widget\_off\_by_\ctx/on\_detach\_parent/on\_attach\_parent
  * 增加 draggable 控件。
  * 修改文档中的错误（感谢俊杰提供补丁）。
  * 完善主题切换，字体卸载的问题（感谢智明提供补丁）。

* 2019/10/24
  * 优化 fragment frame buffer，在 STM32F103 等低端平台，内存不足以实现 framebuffer 时，性能大幅提高（感谢智明提供补丁）。
  * 修改文档中的错误（感谢俊杰提供补丁）。
  * 更新 awtk config sample。
  * 修改软键盘弹出时出现屏保的问题。
  * 完善 new.cpp/new.hpp （感谢陈谭提供补丁）。
  * 修改 windows 中文件名的问题（感谢大恒提供补丁）
  * assets\_manager\_set\_custom\_build\_dir 增加回调函数上下文。

* 2019/10/23
  * 修改 clone 出来的 edit 的显示问题。
  * 增加 widget\_on\_with\_tag 和 widget\_off\_by\_tag，方便一次注销多个同类事件。
  * 完善 list view 对 wheel 事件的处理。
  * 完善 text selector 处理 up/down 按键和 wheel 事件。
  * 修改 SVG 无效图片崩溃的 BUG。
  * 增加函数 assets\_manager\_set\_custom\_build\_asset\_dir，有时我们需要优先加载用户自定义的资源，加载失败才加载系统缺省的，可用设置一个函数去实现这类功能。
  
* 2019/10/19
  * 完善主题切换功能。
  * 增加文档 [资源目录变更通知](assets_dir_changed.md)
  * 增加文档 [主题实时切换](theme_switch.md)
  * 修改 edit 在 listview 中时，处理 Wheel 事件的问题。

* 2019/10/18
  * 增加主题切换功能。

* 2019/10/17
  * 修改文档中的错误（感谢俊杰提供补丁）。
  * 完善 streams。
  
* 2019/10/16
  * 更新文档。
  * 修改文档中的错误（感谢俊杰提供补丁）。

* 2019/10/15
  * 增加 fs\_file\_eof。
  * 增加 tk\_iostream\_noisy。
  * 完善窗口管理器（感谢大恒提供补丁）
  * 前一个窗口为 normal window 时才支持窗口动画。
  * glyph 的 x/y/w/h 改为 16 位，以防超大字体溢出问题。

* 2019/10/14
  * 修改 widget\_set\_value 把 value 当 uint32 的 BUG.
  * 完善 label 换行（感谢俊杰提供补丁）。
  * edit\_set\_int/double 后调用 text layout（感谢俊杰提供补丁）。
  * 窗口被切换到后台后更新 grab\_widget（感谢尧燊提供补丁）。
  * 完善 combo box（感谢尧燊提供补丁）。
  * 完善 layouters（感谢尧燊提供补丁）。
  * 修改 edit 在 slide view 中的问题（感谢培煌提供补丁）。
  * keyboard 增加 scriptable 属性。

* 2019/10/13
  * 增加 stream\_shdlc 文档。

* 2019/10/12
  * 增加 stream\_shdlc。
  
* 2019/10/11
  * 合并君正提供的屏幕旋转补丁（感谢君正提供补丁，感谢智明在各个平台验证）。
  * 合并君正提供的 popup 支持 highlight 补丁（感谢君正提供补丁，感谢智明在各个平台验证）。
  * 合并君正提供的 bitmap 使用 line\_length 补丁（感谢君正提供补丁，感谢智明在各个平台验证）。
  * 修改 dialog 上面的 combox 弹出时焦点的问题。
  
* 2019/10/10
  * 修改 canvas draw image repeatx/repeaty 的问题（感谢俊杰提供补丁）。
  * canvas 增加 repeat\_y\_inverse 图片绘制方式。
  * 完善 widget\_invalidate（感谢培煌提供补丁）。

* 2019/10/09
  * slider 的 min/max/value/step 使用浮动数。
  * combo\_box 增加 item\_height 属性（感谢朝泽提供补丁）。
  * 修改 mledit 软键盘回车的问题（感谢俊杰提供补丁）。

* 2019/10/08
  * 增加 istream buffered。

* 2019/10/07
  * 增加 ostream\_buffered。
  * 按 issue 241，把 get\_time\_ms 改名为 get\_time\_ms64。

* 2019/10/06
  * 增加 ring buffer。

* 2019/10/03
  * 完善 stream serial。

* 2019/10/02
  * 完善 ubjson。

* 2019/09/30
  * 修改文档的 BUG（感谢俊杰提供补丁）。
  * edit/mledit 的 margin 从 style 中获取，从属性获取仍然保留（但不支持在 IDE 中设置）。
  * 使用 sokol 库中的函数获取时间。
  * 增加 event source。

* 2019/09/29
  * 修改 combo\_box\_ex 获取 type 的问题（感谢尧燊提供补丁）。

* 2019/09/27
  * time clock 支持指针支持锚点（感谢智明提供补丁）。
  * edit/mledit 获得焦点时选中文本。
  * combo\_box 增加 localize\_options 选项
  * edit min/max/step/type 支持脚本绑定。
  * progress bar 支持 max 属性。

* 2019/09/26
  * rename stream\_socket to stream\_tcp。
  * 修改 switch 的 BUG（感谢朝泽提供补丁）。

* 2019/09/24
  * 合并君正提供的部分补丁。

* 2019/09/23
  * 完善 ubjson/value
  * 修改前几天修改 widget\_update\_style 留下的问题。

* 2019/09/19
  * 修改"color\_component" "digit\_clock" "mutable\_image"的注释，支持 designer 编辑（感谢大恒提供补丁）。
  * 增加 object\_array
  * 增加 object\_get\_prop\_by\_path
  * 完善 children\_layouter\_list\_view（感谢尧燊提供补丁）。
  * 完善 children\_layouter\_default（感谢尧燊提供补丁）。
  * 增加 EVT\_REQUEST\_QUIT\_APP 事件，点击原生窗口关闭按钮时，通过窗口管理器触发，注册该事件并返回 RET_STOP，可以阻止窗口关闭。
  
* 2019/09/18
  * 增加 iostream\_mem。
  * 重构 theme gen。
  * 完善 style\_mutable，set value 时和 theme gen 保持一致。
  * edit 输入类型为 email 时允许输入空格（感谢俊杰提供补丁）。

* 2019/09/17
  * 修改 LinuxCPU 占用高的问题（感谢俊杰提供补丁）。
  * 修改 edit 快捷键在 DESKTOP 模式下无效的问题（感谢朝泽提供补丁）。
  * 增加《如何开启按键音》的 [文档](docs/how_to_enable_key_touch_sound.md)。
  * 完善 image\_manager\_update\_specific（感谢尧燊提供补丁）。
  * edit keydown 事件处理后返回 RET\_STOP（感谢朝泽提供补丁）。
  * mledit keydown 事件处理后返回 RET\_STOP。

* 2019/09/16
  * 重构 ubjson parser
  * 完善 switch（感谢朝泽提供补丁）。
  * 完善 color component（感谢朝泽提供补丁）。
  * 完善 main loop sdl（感谢朝泽提供补丁）。

* 2019/09/15
  * ubjson reader
  * ubjson parser

* 2019/09/14
  * ubjson writer

* 2019/09/13
  * 窗口切换到后台时自动失去焦点。

* 2019/09/11
  * 优化 update style 的处理流程。
  * 修改 style 和 state 不一致的 BUG（感谢王程提供补丁）。
  * 修改 window\_manager\_create\_animator，只有一个窗口时不创建动画（感谢王程提供补丁）。
  * 修改 text selector 设置 selected index 的 BUG。
  * 完善 dialog highligher 对 native window resize 事件的处理。

* 2019/09/10
  * 更新文档。
  * 修改 color picker 控件动态调整大小的问题。

* 2019/09/05
  * 完善 fontgen（感谢智明提供补丁）。
  * 完善 sdl main loop（感谢朝泽提供补丁）。
  * 增加 socket stream 实现。

* 2019/09/04
  * 修改 freetype 字体大小的问题（感谢智明提供补丁）。
  * change time to uint64
  * 修改 PC 版本获取系统时间的函数（感谢陈谭提供补丁）。

* 2019/09/03
  * edit/mledit 失去焦点时，unselect 当前选中的文本。
  * 缺省使用 stb 字体加载器。

* 2019/09/02
  * merge patch issue #224（感谢陈谭提供补丁）。

* 2019/09/01
  * children layouter list view 增加 cols 参数。

* 2019/08/30
  * 修改 windows 下使用单头文件 freetype 的问题（感谢陈谭提供补丁）。
  * 修改 list view children layouter 的 BUG（感谢尧燊提供补丁）。
  * 完善《硬件资源需求评估》（感谢智明提供补丁）。  
  * 位图字体比较时，忽略字体大小（感谢智明提供补丁）。  

* 2019/08/29
  * 修改重载 new 的问题（感谢陈谭提供补丁）。
  * 修改 combox 在 dialog 上焦点设置错误的问题。
  * 优化数字时钟，时间没变时不重绘。
  * 修改 widget 焦点切换的 BUG（感谢尧燊提供补丁）。
  * 修改 children layouter 的 BUG（感谢尧燊提供补丁）。

* 2019/08/28
  * 增加 widget\_reset\_canvas 用于 designer 打开新工程时清除 vgcanvas 中的资源。
  * hscroll\_label 支持右对齐（感谢智明提供补丁）。
  * 修改密码编辑器的 BUG（感谢俊杰提供补丁）。
  * vgcanvas 支持设置 assets manager, 以便从指定的 assets manager 加载字体。
    
* 2019/08/27
  * 完善 lcd mono（感谢智明提供补丁）。
  * 完善 wuxiaolin draw line（感谢培煌提供补丁）。 
  * 增加 stream 接口，以及基于内存和文件的实现。

* 2019/08/24
  * 更新文档。
  * 完善 mledit，支持 readonly 属性。

* 2019/08/23
  * 增加 prop desc 结构。
  * 完善 object/object default

* 2019/08/22
  * 增加 combo\_box\_ex 控件（感谢大恒提供）
  * 增加属性 WIDGET\_PROP\_FOCUSED 表示当前的焦点状态，WIDGET\_PROP\_FOCUS 属性为过时。

* 2019/08/20
  * svg 中 float\_t 改为 float。
  * 更新 stm32 相关项目。
  * 更新 awtk-mvvm/awtk-nogui/awtk-linux-fb/awtk-iotjs
  * 增加 awtk-lite
  * combobox 处理 resize 事件。
  * combobox 支持自定义打开弹出窗口。

* 2019/08/19
  * awtk 编译成 tkc/base/widgets/extwidgets/awtk 几个独立的库，并调整部分文件的位置。

* 2019/08/18
  * 去掉 widget pool。

* 2019/08/17
  * slider 支持非正方形的 icon（感谢俊杰提供补丁）
  * edit/mledit 处理 resize 事件。

* 2019/08/16
  * 引入片段帧缓存 (Fragment Frame Buffer)，部分解决 STMF103 上由于没足够内存做 FrameBuffer 导致闪烁的问题。
  * 支持 8bit gray+alpha png

* 2019/08/15
  * 增加单色 LCD 的文档。
  * 支持 mingw 编译（感谢智明提供补丁）
  * 修改 cairo/pixman config.h 冲突的问题（感谢陈谭提供补丁）
  * 由于生成单色字体需要 freetype，而且 freetype 的效果比 stb 稍微好一点，所以 PC 版本采用 freetype 作为字体引擎。

* 2019/08/14
  * 完善文档。
  * 增加宏 TK\_XML\_MAX\_ATTRS 用于控制 XML 属性的最大个数（缺省 24)。
  * 通过 widget factory 触发 EVT\_WIDGET\_CREATED 事件。
  * 修改在 stm32f103 上的问题。
  * 完善 window\_animator（感谢智明提供补丁）

* 2019/08/13
  * 模拟器支持单色屏。
  * 修复 native window 初始化顺序的 BUG（感谢智明提供补丁）

* 2019/08/12
  * 增加 mono 图片生成工具。

* 2019/08/11
  * 增加 mono 字体生成工具。

* 2019/08/10
  * edit/mledit set\_text 触发 EVT\_VALUE\_CHANGED。
  * 增加 lcd mono。

* 2019/08/09
  * 重构窗口管理器。
  * 完善缺省输入法。
  * 当前窗口为 keyboard 时，不触发 TO\_BACKGROUND/TO\_FOREGROUND。
  * 修改 slider 资源加载的问题（感谢朝泽提供补丁）
  * 修改 fill rect 坐标的问题（感谢智明提供补丁）
  * 修改 color tile 边框颜色的问题（感谢智明提供补丁）

* 2019/08/08
  * 重构
  * 完善 mutable image（感谢智明提供补丁）
  * 完善 expr eval，支持十六进制数。
  * mledit 支持 value changing 事件。
  * 完善 slider 拖动效果。

* 2019/08/07
  * 修改 valgrind 发现的问题。
  * 修改 shortcut 的 BUG。
  * 完善 SDL 事件处理（感谢朝泽提供补丁）
  * 完善 pages 控件（感谢朝泽提供补丁）
  * 修改 event 文档的 BUG（感谢朝泽提供补丁）

* 2019/08/06
  * 优化 nanovg GL fragment shader(draw text)。
  * 禁用点击 scrollview 中控件时自动滚动到可视区的功能。
  * 完善 slider 拖动效果。

* 2019/08/05
  * 修改 uiloader/window base/style mutable/layouter 等 BUG（感谢尧燊提供补丁）。

* 2019/08/04-05
  * 优化 nanovg GL fragment shader。
  * 完善 Android 移植。
  * 完善 slide view 控件（感谢朝泽提供补丁）

* 2019/08/03
  * 完善 Android 移植。

* 2019/07/20-8/1
  * window manager 抽象成接口。
  * 引入 native\_window 接口。

* 2019/08/02
  * 禁用 label/button/image/hscroll\_label 的缓存。

* 2019/07/29
  * fix edit clone 的问题。

* 2019/07/26
  * 完善 text edit（感谢智明提供补丁）

* 2019/07/24
  * 完善 preview\_ui（感谢俊杰提供补丁）

* 2019/07/20
  * 完善 list\_view\_h。

* 2019/07/19
  * Slider 的图标用 ICON 方式绘制。
  * 增加属性 with\_focus\_state，如果希望控件具有焦点状态，有不希望焦点停留，可以用本属性代替 focusable 属性。
  * 完善控件动画。

* 2019/07/18
  * gles 的 canvas 画直线和画矩形边框的线宽问题，gles 统一和 agge 一样为 1 线宽（感谢智明提供补丁）。

* 2019/07/17
  * 完善 switch 控件（感谢朝泽提供补丁）

* 2019/07/15
  * 完善 agge 后端（感谢智明提供补丁）
  * 完善 cairo 后端（感谢智明提供补丁）
  * 完善 demoui。

* 2019/07/12
  * 改进 widget\_get\_type 通过属性获取。
  * 完善 scrollbar 子控件布局。

* 2019/07/11
  * edit 支持自定义的字符有效性检查函数。
  * tab button group 支持 scrollable 属性。
  * 增加获取全局鼠标状态的函数。tk_get\_pointer\_x/tk\_get\_pointer\_y/tk\_is\_pointer\_pressed

* 2019/07/10
  * 修改 wcs 几处编译警告（感谢陈谭提供补丁）。
  * 增加生成控件模板的代码生成器。
  
* 2019/07/09
  * 完整弹出窗口动画，支持在不同位置弹出。
  * 支持 bmp（感谢俊杰提供补丁）。

* 2019/07/08
  * 表达式支持条件运算。

* 2019/07/07
  * 完善 mledit(awtk-web 支持 mledit)。
  * 修改 valgrind 发现的问题。

* 2019/07/06
  * 增加 hscroll\_label 控件。
  * 修改 mledit 获取 max\_lines 的 BUG。

* 2019/07/05
  * 修改一些编译警告。
  * 修改 MacOS 上双击启动时找不到资源的问题。
  * 修改 scrollbar 隐藏导致 listview relayout 的问题。

* 2019/07/04
  * 修改多行编辑器鼠标选择文本时的坐标偏差。
  * 增加 TK\_GLYPH\_CACHE\_NR 宏，允许开发者自行定义。
  * 更新文档（感谢俊杰提供补丁）。
  * 修改软件键盘删除键无效的问题。
  * set visible 后设置 relayout 标志。

* 2019/07/03
  * 更新 demoui
  * mledit 增加 max\_lines 属性。
  * mledit 支持 pageup/pagedown
  * 动画停止时 reversed 设置为 FALSE。
  * 修改 svg 图片 fill=""时的问题。
  * widget 的 pointer/key 事件处理增加 can\_not\_destroy 保护。
  * 增加行号控件，配合 mledit 控件使用。

* 2019/07/02
  * 修改 OpenGL 模式下 SVG 无法绘制的问题（感谢智明提供补丁）。
  * mledit 支持垂直滚动条。

* 2019/07/01
  * 增加 self\_layouter\_menu 文档和示例。

* 2019/06/30
  * 增加 self\_layouter\_menu 方便实现弹出菜单。

* 2019/06/29
  * 子控件缺省布局方式增加 align_h 的参数，

* 2019q/06/28
  * 完善 edit。
  * 完善 combobox

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
  * 完成 text\_selector 控件基本��能。

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
