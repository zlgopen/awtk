# preview_ui 的使用说明

## 使用方法

必须指定 ui 参数，其余为可选参数，运行时优先使用命令行设置的参数，如果参数对应的资源找不到，则使用默认的资源。

``` shell
Usage: preview_ui.exe ui=xxx [lcd_w=800] [lcd_h=480] [res_root=xxx] [language=xxx] [theme=xxx] [system_bar=xxx] [bottom_system_bar=xxx] [plugins_path=xxx] [render_mode=xxx]

Example:
bin\preview_ui.exe ui=e:/zlgopen/awtk/design/default/ui/main.xml
bin\preview_ui.exe ui=e:/zlgopen/awtk/design/default/ui/main.xml lcd_w=800 lcd_h=480 res_root=e:/zlgopen/awtk/res
```

参数说明：

* 命令行参数无顺序要求。
* ui，必填项，表示 ui 文件的路径。
* lcd_w，可选项，表示 LCD 的宽度，默认为320。
* lcd_h，可选项，表示 LCD 的高度，默认为480。
* res_root，可选项，表示应用的 assets 资源目录，默认为 awtk 的 res 目录。
* language，可选项，表示运行时的默认语言，默认为awtk的默认语言。
* theme，可选项，表示运行时的默认主题，默认为 default 主题。
* system_bar，可选项，表示应用的顶部系统栏页面，默认为空。
* bottom_system_bar，可选项，表示应用的底部系统栏页面，默认为空。
* plugins_path，可选项，表示插件（比如自定义控件）的DLL目录，默认为无。
* render_mode，可选项，表示渲染模式，指 OpenGL 等，默认为 OpenGL。

> plugins_path 中的插件需提供诸如 ret_t xxxx_register(void) 的API。

