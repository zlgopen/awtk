# 参考手册

## 一、GUI相关组件

### 控件

![widget\_overview](images/widget_overview.png)

#### widget

* [widget](manual/widget_t.md) 所有控件、窗口和窗口管理器的基类。

#### 窗口管理器
* [window\_manager](manual/window_manager_t.md) 窗口管理器。

#### 窗口
* [window\_base](manual/window_base_t.md) 窗口基类。
* [dialog](manual/dialog_t.md) 对话框。
* [popup](manual/popup_t.md) 弹出窗口。
* [window](manual/window_t.md) 普通窗口。
* [system\_bar](manual/system_bar_t.md) 系统状态窗口。
* [calibration\_win](manual/calibration_win_t.md) 电阻屏校准窗口。
* [overlay](manual/overlay_t.md) 悬浮窗口窗口。

#### 基本控件
* [button](manual/button_t.md) 按钮控件。
* [label](manual/label_t.md) 文本控件。
* [edit](manual/edit_t.md) 单行编辑器控件。
* [image](manual/image_t.md) 图片控件。
* [dragger](manual/dragger_t.md) dragger控件。
* [image\_base](manual/image_base_t.md) 图片控件基类。
* [check\_button](manual/check_button_t.md) 多选按钮。
* [radio\_button](manual/check_button_t.md) 单选按钮。
* [spin\_box](manual/spin_box_t.md) 数值编辑器控件。
* [combo\_box](manual/combo_box_t.md) 下拉列表控件。
* [combo\_box\_item](manual/combo_box_item_t.md) 下拉列表项控件。
* [color\_tile](manual/color_tile_t.md) 色块控件。
* [dialog\_title](manual/dialog_title_t.md) 对话框标题控件。
* [dialog\_client](manual/dialog_client_t.md) 对话框客户区控件。
* [slider](manual/slider_t.md) 滑块控件。
* [progress\_bar](manual/progress_bar_t.md) 进度条控件。
* [tab\_control](manual/tab_control_t.md) 标签控件。
* [tab\_button](manual/tab_button_t.md) 标签按钮控件。
* [tab\_button\_group](manual/tab_button_group_t.md) 标签按钮分组控件。

#### 通用容器控件
* [row](manual/row_t.md) 行控件。
* [column](manual/column_t.md) 列控件。
* [grid](manual/grid_t.md) 网格控件。
* [view](manual/view_t.md) 通用容器控件。
* [grid\_item](manual/grid_item_t.md) 网格项控件。
* [group\_box](manual/group_box_t.md) 通用分组控件。
* [app\_bar](manual/app_bar_t.md) app\_bar控件。
* [button\_group](manual/button_group_t.md) 按钮分组控件。

#### 扩展控件
* [canvas\_widget](manual/canvas_widget_t.md) 画布控件。
* [color\_picker](manual/color_picker_t.md) 颜色选择器控件。
* [gif\_image](manual/gif_image_t.md) gif\_image控件。
* [guage](manual/guage_t.md) 仪表控件。
* [guage\_pointer](manual/guage_pointer_t.md) 仪表指针控件。
* [image\_animation](manual/image_animation_t.md) 图片动画控件。
* [image\_value](manual/image_value_t.md) 图片值控件。
* [keyboard](manual/keyboard_t.md)  软键盘控件。
* [progress\_circle](manual/progress_circle_t.md) 进度圆环控件。
* [rich\_text](manual/rich_text_t.md) 图文混排控件。
* [slide\_menu](manual/slide_menu_t.md) 左右滑动菜单控件。
* [slide\_view](manual/slide_view_t.md) 滑动视图控件。
* [slide\_view\_indicator](manual/slide_indicator_t.md) 滑动视图的指示器控件。
* [svg\_image](manual/svg_image_t.md) SVG图片控件。
* [switch](manual/switch_t.md) 开关控件。
* [text\_selector](manual/text_selector_t.md) 文本选择器控件。
* [time\_clock](manual/time_clock_t.md) 模拟时钟控件。
* [digit\_clock](manual/digit_clock_t.md) 数字时钟控件。
* [list\_view](manual/list_view_t.md) 列表视图控件。
* [list\_view\_h](manual/list_view_h_t.md) 水平列表视图控件。
* [list\_item](manual/list_item_t.md) 列表项控件。
* [scroll\_bar](manual/scroll_bar_t.md) 滚动条控件。
* [scroll\_view](manual/scroll_view_t.md) 滚动视图控件。
* [mledit](manual/mledit_t.md) 多行编辑器控件。
* [line_number](manual/line_number_t.md) 多行编辑器的行号控件。
* [mutable\_image](manual/mutable_image_t.md) mutable图片控件(用于辅助实现camera/video)。

### 画布

#### 普通画布
* [canvas](manual/canvas_t.md)

#### 矢量图画布

![vgcanvas\_overview](manual/images/vgcanvas_t_0.png)

* [vgcanvas](manual/vgcanvas_t.md)

### 资源相关组件

![assets overview](images/assets_overview.png)

* [asset\_info](manual/asset_info_t.md) 资源信息。
* [assets\_manager](manual/assets_manager_t.md) 资源管理器。

#### 1.字体

![font\_overview](images/font_overview.png)

* [glyph](manual/glyph_t.md) 字模。
* [font](manual/font_t.md) 字体。
* [font\_loader](manual/font_loader_t.md) 字体加载器。
* [font\_loader\_ft](manual/font_loader_ft_t.md) 基于freetype实现的字体加载器。
* [font\_loader\_stb](manual/font_loader_stb_t.md) 基于stb实现的字体加载器。
* [font\_loader\_bitmap](manual/font_loader_bitmap_t.md) 基于位图实现的字体加载器。
* [font\_manager](manual/font_manager_t.md) 字体管理器。

#### 2.图片

![image\_overview](images/image_overview.png)

* [bitmap](manual/bitmap_t.md) 位图。
* [image\_loader](manual/image_loader_t.md) 图片加载器。
* [image\_loader\_stb](manual/image_loader_stb_t.md) stb图片加载器。
* [image\_manager](manual/image_manager_t.md) 图片管理器。

#### 3.字符串资源

* [locale\_info](manual/locale_info_t.md) 本地化信息。

#### 4.主题

![style\_overview](images/style_overview.png)

* [style](manual/style_t.md) style接口。
* [theme](manual/theme_t.md) 常量主题数据。
* [style\_factory](manual/style_factory_t.md) style工厂。
* [style\_const](manual/style_const_t.md) 只读的style。
* [style\_mutable](manual/style_mutable_t.md) 可修改的style。

#### 5.UI描述数据

![ui\_loader\_overview](images/ui_loader_overview.png)

* [ui\_loader](manual/ui_loader_t.md) loader接口。
* [ui\_builder](manual/ui_builder_t.md) builder接口。

* [ui\_builder\_default](manual/ui_builder_default_t.md) 缺省builder实现，负责构建widget树。
* [ui\_xml\_writer](manual/ui_xml_writer_t.md) 生成XML格式的UI描述数据。 
* [ui\_binary\_writer](manual/ui_binary_writer_t.md) 生成二进制格式的UI描述数据。  

* [ui\_loader\_default](manual/ui_loader_default_t.md) 二进制格式的UI资源加载器。 
* [ui\_loader\_xml](manual/ui_loader_xml_t.md) XML格式的UI资源加载器。

### 输入法

![input\_method\_overview](images/input_method_overview.png)

* [input\_method](manual/input_method_t.md) 输入法接口。
* [input\_method\_default](manual/input_method_default_t.md) 输入法缺省实现。
* [input\_method\_sdl](manual/input_method_sdl_t.md) 输入法SDL实现。
* [input\_method\_null](manual/input_method_null_t.md) 输入法空实现。

* [input\_engine](manual/input_engine_t.md) 输入法引擎接口。
* [input\_engine\_null](manual/input_engine_null_t.md) 空输入法引擎实现。
* [input\_engine\_pinyin](manual/input_engine_pinyin_t.md) 拼音输入法引擎实现。

### 定时器/IDLE

![timer\_overview](images/timer_overview.png)

* [timer](manual/timer_t.md) 定时器。

* [timer\_info](manual/timer_info_t.md) 单个timer的信息。

![idle\_overview](images/idle_overview.png)

* [idle](manual/idle_t.md) 用于异步执行一些函数。 

* [idle\_info](manual/idle_info_t.md) 单个idle的信息。 

### 布局器
  
![self\_layouter](../docs/images/layout_self_layouter.png)

![children\_layouter](../docs/images/layout_children_layouter.png)

* [self\_layouter](manual/self_layouter_t.md) 控件自身排版布局器的接口

* [children\_layouter](manual/children_layouter_t.md) 子控件排版布局器的接口

* [self\_layouter\_factory](manual/self_layouter_factory_t.md) 控件自身排版布局器的工厂。

* [children\_layouter\_factory](manual/children_layouter_factory_t.md) 子控件排版布局器的的工厂。

* [self\_layouter\_default](manual/self_layouter_default_t.md) 缺省的控件自身布局算法实现。

* [self\_layouter\_menu](manual/self_layouter_menu_t.md) 菜单控件自身布局算法实现。

* [children\_layouter\_default](manual/children_layouter_default_t.md) 缺省的子控件布局算法实现。

* [children\_layouter\_list\_view](manual/children_layouter_list_view_t.md) 列表视图的子控件布局算法实现。

### 杂项

* [system\_info](manual/system_info_t.md) 系统信息。

## 二、基本函数库

* [darray](manual/darray_t.md) 动态数组。
* [slist](manual/slist_t.md) 单向链表。
* [memory](manual/tk_mem_t.md) 内存管理相关函数和宏。
* [rgba](manual/rgba_t.md) RGBA颜色值。

* [color\_parse](manual/color_parser_t.md) 颜色解析对象。
* [value](manual/value_t.md) 通用值对象。
* [str](manual/str_t.md) UTF-8字符串对象。
* [wstr](manual/wstr_t.md) Unicode字符串对象。

* [date\_time](manual/date_time_t.md) 时间日期函数。
* [event](manual/event_t.md) 事件基类。
* [tk\_thread](manual/tk_thread_t.md) 线程。
* [tk\_mutex](manual/tk_mutex_t.md) 互斥锁。
。
--------------------------

* [asset_info](manual/asset_info_t.md) 资源类型定义。
* [wbuffer](manual/wbuffer_t.md) Write Buffer。
* [rbuffer](manual/rbuffer_t.md) Read Buffer。
* [color](manual/color_t.md) 颜色对象。
* [color\_parse](manual/color_parser_t.md) 颜色解析器。
* [tk\_cond\_var](manual/tk_cond_var_t.md) 简化版条件变量。
* [darray](manual/darray_t.md) 动态数组。

* [date\_time](manual/date_time_t.md) 时间日期函数。
* [easing](manual/easing_type_t.md) 缓动作常量定义。
* [emitter](manual/emitter_t.md)事件分发器, 用于实现观察者模式。

* endian 大端/小端相关函数。

* [event](manual/emitter_t.md) 事件基类。

* [event_source](manual/event_source_t.md) 事件源接口。

* [event_source_fd](manual/event_source_fd_t.md) 基于文件描述符的事件源实现。

* [event_source_idle](manual/event_source_idle_t.md) 基于IDLE的事件源实现。

* [event_source_manager](manual/event_source_manager_t.md) 事件源管理器的接口。

* [event_source_manager_default](manual/event_source_manager_default_t.md) 事件源管理器的缺省实现。

* [event_source_timer](manual/event_source_timer_t.md) 基于Time的事件源实现。

* fs.h
* func_call_parser.h
* func_desc.h
* idle_info.h
* idle_manager.h
* int_str.h
* iostream.h
* istream.h
* matrix.h
* mem.h
* mime_types.h
* mutex.h
* named_value.h
* object.h
* object_array.h
* object_default.h
* ostream.h
* path.h
* platform.h
* rect.h
* ring_buffer.h
* rom_fs.h
* slist.h
* socket_pair.h
* str.h
* str_str.h
* stream_const.h
* thread.h
* time_now.h
* timer_info.h
* timer_manager.h
* tokenizer.h
* types_def.h
* utf8.h
* utils.h
* value.h
* value_desc.h
* wstr.h


