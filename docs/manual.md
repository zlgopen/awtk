# 参考手册

## 一、GUI相关组件

### 控件

![widget\_overview](images/widget_overview.png)

#### widget

* [widget](manual/widget_t.md) 所有控件、窗口和窗口管理器的基类。

#### 窗口
* [window\_base](manual/window_base_t.md) 窗口基类。
* [dialog](manual/dialog_t.md) 对话框。
* [popup](manual/popup_t.md) 弹出窗口。
* [window](manual/window_t.md) 普通窗口。
* [system\_bar](manual/system_bar_t.md) 系统状态窗口。
* [calibration\_win](manual/calibration_win_t.md) 电阻屏校准窗口。

#### 基本控件
* [button](manual/button_t.md) 按钮控件。
* [label](manual/label_t.md) 文本控件。
* [edit](manual/edit_t.md) 单行编辑器控件。
* [image](manual/image_t.md) 图片控件。
* [image\_base](manual/image_base_t.md) 图片控件基类。
* [spin\_box](manual/spin_box_t.md) 数值编辑器控件。
* [combo\_box](manual/combo_box_t.md) 下拉列表控件。
* [combo\_box\_item](manual/combo_box_item_t.md) 下拉列表项控件。
* [color\_tile](manual/color_tile_t.md) 色块控件。
* [dialog\_title](manual/dialog_title_t.md) 对话框标题控件。
* [dialog\_client](manual/dialog_client_t.md) 对话框客户区控件。
* [slider](manual/slider_t.md) 滑块控件。
* [progress\_bar](manual/progress_bar_t.md) 进度条控件。

#### 通用容器控件
* [row](manual/row_t.md) 行控件。
* [column](manual/column_t.md) 列控件。
* [grid](manual/grid_t.md) 网格控件。
* [view](manual/view_t.md) 通用容器控件。
* [grid\_item](manual/grid_item_t.md) 网格项控件。
* [group\_box](manual/group_box_t.md) 通用分组控件。
* [app\_bar](manual/app_bar_t.md) app\_bar控件。
* [button\_group](manual/button_group_t.md) 按钮分组控件。

### 画布

#### 矢量图画布

![vgcanvas\_overview](manual/images/vgcanvas_t_0.png)

* [vgcanvas](manual/vgcanvas_t.md)

### 资源相关组件

![assets overview](images/assets_overview.png)

* [asset\_info](manual/asset_info_t.md) 资源信息。
* [assets_manager](manual/assets_manager_t.md) 资源管理器。

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

#### 5.UI描述数据


### 输入法

### 控件动画

### 窗口动画

### 布局器

### 定时器

## 二、基本函数库

* [memory](manual/tk_mem_t.md) 内存管理相关函数和宏。
* [rgba](manual/rgba_t.md) RGBA颜色值。
* [color](manual/color_t.md) 颜色对象。
* [color\_parse](manual/color_parser_t.md) 颜色解析对象。
* [value](manual/value_t.md) 通用值对象。
* [str](manual/str_t.md) UTF-8字符串对象。
* [wstr](manual/wstr_t.md) Unicode字符串对象。
* [array](manual/array_t.md) 动态数组对象。
* [wbuffer](manual/wbuffer_t.md) Write Buffer。
* [rbuffer](manual/rbuffer_t.md) Read Buffer。
* [date\_time](manual/date_time_t.md) 时间日期函数。
* [event](manual/event_t.md) 事件基类。


