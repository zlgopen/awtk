# UI 自动化测试工具

[AWTK UI 自动化](https://github.com/zlgopen/awtk-ui-automation) 提供了兼容 [Appium](http://appium.io) 的接口，可以使用 [Appium](http://appium.io) 的工具来进行 UI 自动化测试。但是使用起来有点麻烦，用的人不多，所以最终决定开发一个 AWTK 专用的 UI 自动化测试工具。相比 [Appium](http://appium.io)，这个工具有下列特点：

* 不依赖额外的库。
* 无需安装额外的软件。
* 无需学习额外的编程语言和 API。
* 支持串口和网络两种方式连接 APP。
* 更加适合 AWTK 的测试方式。

主要用途：

* 自动化测试。
* 自动化演示。
* 压力测试。
* 查看系统状态（辅助调试）。

## 1. 启用 UI 自动化

* 包含头文件
 
```c
#include "remote_ui/service/remote_ui_service.h"
```

* 启动服务

```
  tk_service_start(main_loop_get_event_source_manager(main_loop()), REMOTE_UI_URL, remote_ui_service_create, NULL);
```

> 完整示例请参考 demo_ui_old_app.c

## 2. 编写测试配置

测试用 ini 文件格式编写。通常一个应用程序需要多组测试用例，每个测试用例放到一个配置文件中，并取一个有意义的文件名。

下面给出一个简单的例子，后面会详细介绍每个命令的含义。

```ini
[create]
  url=tcp://localhost:2233

[move_widget]
  target=window_manager.top
  x=100
  y=200

[get_prop]
  target=window_manager.top
  name=x
  value=100

[get_prop]
  target=window_manager.top
  name=y
  value=200

[resize_widget]
  target=window_manager.top
  w=200
  h=100

[get_prop]
  target=window_manager.top
  name=w
  value=200

[get_prop]
  target=window_manager.top
  name=h
  value=100

[close_window]
  name=top

```

## 3. 运行测试

* 先启动被测试的应用程序。如

```sh
./bin/demouiold 
```

* 再运行测试程序。如

```sh
./bin/ui_test tests/ui_test_data/demouiold/overlay.ini
```

> 如果存在多组测试用例，可以把它们放到一个批处理中。

## 4. 测试命令

### 4.0 常用参数

部分参数在很多命令中都会出现，这里统一说明。

#### 4.0.1 target 

通常用来指定被操作的控件。它的值可以是控件的名字，也可以是控件的路径。如果是控件的路径，可以使用 `.` 来分隔控件的名字。如 `window_manager.top`。

有几个特殊的名称：

* window_manager 代表窗口管理器。
* window 代表当前主窗口。比如 window.close 表示当前窗口上名为 close 的控件。路径默认为当前窗口，比如 close 也表示当前窗口上名为 close 的控件。 
* global 代表全局对象。用来设置语言和主题之类的全局属性。
* [序数] 表示第几个子控件。如果序数为非负表示正向序数，为负数表示反向序数。
* 如果按名称查找失败，将名字作为控件类型进行查找。

示例：

* close_button 表示名为 close_button 的控件。
* window_manager.top 表示 window_manager 窗口中名为 top 的窗口。
* window_manager.top.close_button 表示 window_manager 窗口中，名为 top 的窗口中名为 close_button 的控件。
* window.[0] 表示当前窗口中第一个控件。
* window.[-1] 表示当前窗口中倒数第一个控件。
* window.slide\_view，如果窗口中存在名为 slide\_view 的控件，则表示该控件，否则表示窗口中第一个 slide\_view 类型的控件。

### 4.1 create

* 功能：创建连接。
* 参数：
  * url - 连接地址，如 tcp://localhost:2233。

* 示例：
    
```ini
[create]
  url=tcp://localhost:2233
```

> url 参数可以为串口名，通过串口连接到开发板。如：serial:///dev/ttyUSB0

### 4.2 take_snapshot

* 功能：截屏。如果指定的文件不存在，则将截屏保存为指定的文件。如果存在，则与之进行比较，如果不一致，则提示测试失败。
* 参数：
  * target - 目标。
  * filename - 截屏文件名 (.png)。

* 示例：

```ini
[take_snapshot]
  filename=win1.png

[take_snapshot]
  target=window
  filename=win2.png

[take_snapshot]
  target=window_manager
  filename=wm.png

[take_snapshot]
  target=window_manager.top
  filename=top.png
```

### 4.2 get_source

* 功能：获取控件源码。如果指定的文件不存在，则将源码保存为指定的文件。如果存在，则与之进行比较，如果不一致，则提示测试失败。
* 参数：
  * target - 目标。
  * filename - 源码文件名 (.xml)。

* 示例：

```ini
[get_source]
  filename=win1.xml

[get_source]
  target=window
  filename=win2.xml

[get_source]
  target=window_manager
  filename=wm.xml

[get_source]
  target=window_manager.top
  filename=top.xml
```

### 4.3 open_window

功能：打开窗口。
参数：
  * name - 窗口名。
  * xml -  xml 文件（可选，以@开头表示从文件读取），如果指定，表示从 XML 内容加载窗口，通常不需要。
  * init - 初始化数据 (JSON 格式，可选，以@开头表示从文件读取）。

```ini
[open_window]
  name=basic

[open_window]
  name=foo
  xml=@tests/ui_test_data/demouiold/data/foo.xml
  init=@tests/ui_test_data/demouiold/data/foo.json
```

> xml 文件可以用 AWTK designer 生成，也可以手工编写。

初始化数据，第一级为控件名称（路径），第二级为控件的属性和值。如：

```
{
  "bar1": {
    "value":50
  },  
  "bar2": {
    "value":80
  }
}
```

在上面的例子中，bar1 和 bar2 是窗口中的两个进度条控件，这里分别设置它们的值为 50 和 80。

### 4.4 close_window

功能：关闭窗口。

参数：
  * name - 窗口名。

```ini
[close_window]
  name=top
```

### 4.5 back

功能：关闭当前窗口，返回上一级窗口。

参数：无。

```ini
[back]
```

### 4.6 home

功能：关闭除主窗口外的窗口，返回主窗口。

参数：无。

```ini
[home]
```

### 4.7 exec_fscript

功能：执行脚本。

参数：
  * fscript - 脚本内容（以@开头表示从文件读取）。

示例：

```ini
[exec_fscript]
  fscript=print("hello");
```

fscript 的用法请参考 [FSCRIPT 参考文档](https://awtk.zlg.cn/docs/awtk_docs/FScript/fscript.html)

### 4.8 create_widget

功能：创建控件。

参数：
  * target - 目标（父控件）。
  * xml -  xml 内容（以@开头表示从文件读取）。

示例：

```ini
[create_widget]
  target=window
  xml=@tests/ui_test_data/demouiold/data/quit.xml
```

控件的 xml 比较简单，一般手写就可以了。如下面的例子在窗口的中间创建一个按钮：

```xml
<button name="quit" text="Quit" on:click="quit()" x="c" y="m" w="128" h="30"/>
```

### 4.9 destroy_widget

功能：销毁控件。

参数：
  * target - 目标控件。

示例：

```ini
[destroy_widget]
  target=quit
```

### 4.10 move_widget

功能：移动控件。

参数：
  * target - 目标控件。
  * x - x 坐标。
  * y - y 坐标。

示例：

```ini
[move_widget]
  target=window_manager.top
  x=100
  y=200
```

> x 和 y 的值为相对于父控件的坐标。

### 4.11 resize_widget

功能：调整控件大小。

参数：
  * target - 目标控件。
  * w - 宽度。
  * h - 高度。

示例：

```ini
[resize_widget]
  target=window_manager.top
  w=200
  h=100
```

### 4.12 set_prop

功能：设置对象（通常为控件）的属性。

参数：
  * target - 目标控件（或 global 全局对象）。
  * name - 属性名。
  * value - 属性值。可以使用表达式，以$开头视为表达式。

示例：

> 设置是否显示 FPS

```ini
[set_prop]
  target=window_manager
  name=show_fps
  value=false
```

```ini
[set_prop]
  target=temperator
  name=value
  value=$ random0to1() * 100
```

> 设置屏保时间 (ms)

```ini
[set_prop]
  target=window_manager
  name=screen_saver_time
  value=1000000
```

> 设置语言

```ini
[set_prop]
  target=global
  name=language
  value=zh_CN
```

> 设置主题

```ini
[set_prop]
  target=global
  name=theme
  value=default
``` 

### 4.13 get_prop

功能：获取对象（通常为控件）的属性。

参数：
  * target - 目标控件（或 global 全局对象）。
  * name - 属性名。
  * value - 属性值（预期的值，如果实际值与之不同，认为测试失败）。可以使用表达式，以$开头视为表达式。
  * save\_to\_var - 将返回的值保持到变量。

示例：

> 获取控件的文本

```ini
[get_prop]
  target=button
  name=text
  value=按钮
  save_to_var=button_text
```

> 获取当前的语言

```
[get_prop]
  target=global
  name=language
  value=zh_CN
```

### 4.14 click

功能：点击控件（只支持按钮等可以点击的控件）。

参数：
  * target - 目标。

```ini
[click]
  target=start
```

### 4.15 key

功能：发送按键事件。

参数：
  * target - 目标。
  * key - 按键。

```ini
[key]
  target=open_edit
  key=RETURN
```

key 的取值请参考（去掉前缀 TK\_KEY\_） [按键定义] https://gitee.com/zlgopen/awtk/blob/master/docs/manual/key_code_t.md

### 4.16 send_event

功能：发送事件。

参数：
  * target - 目标。
  * type - 事件类型。
    * key - 按键按下/弹起。
    * key_down - 按键按下。
    * key_up - 按键弹起。
    * pointer_down - 指针按下。
    * pointer_up - 指针弹起。
    * pointer_move - 指针移动。
    * click - 点击。
  
  * x - x 坐标 (pointer 事件）。可以使用表达式，以$开头视为表达式。
  * y - y 坐标 (pointer 事件）。可以使用表达式，以$开头视为表达式。
  * key - key (key 事件）。可以使用表达式，以$开头视为表达式。不指定时，如果type=key，发送随机按键按下和弹起事件。

```ini
[send_event]
  type=key_down
  key=RIGHT

[send_event]
  type=key_up
  key=RIGHT

[send_event]
  type=key
  key=RIGHT

[send_event]
  type=key

[send_event]
  target=window
  type=pointer_down
  x = 100
  y = 300

[send_event]
  target=window
  type=pointer_move
  x = 101
  y = 301

[send_event]
  target=window
  type=pointer_up
  x = 100
  y = 300
```    

### 4.17 sleep

功能：休眠。

参数：
  * ms - 等待时间（毫秒）。

```ini

```ini
[sleep]
  time=1000
```

### 4.18 get_loaded_images_info

功能：获取已经加载的图片信息（辅助内存相关的调试）。

参数：
  * filename - 保存文件名。

示例：

```ini
[get_loaded_images_info]
  filename=loaded_image_info.txt
```

### 4.19 get_loaded_assets_info

功能：获取已经加载的资源信息（辅助内存相关的调试）。

参数：
  * filename - 保存文件名。

示例：

```ini
[get_loaded_assets_info]
  filename=loaded_assets_info.txt  
```

### 4.20 stop
功能：停止测试

参数：
  * condition 停止条件(可选，表达式为非0时停止)

示例：

```ini
[stop]
  condition=$ value > 0
```

### 4.21 goto
功能：跳转到指定的目标

参数：
  * condition 跳转条件(可选，表达式为非0时跳转)
  * target 目标

示例：

```ini
[goto]
  target=key
  condition=$ value < 50
```

* [参考示例](https://github.com/zlgopen/awtk/tree/master/tests/ui_test_data/demouiold/)
