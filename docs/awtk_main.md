# AWTK 应用程序的入口

## 一、背景

虽然 main 函数是 C 语言的标准入口，但是不同平台的入口函数仍然有多种不同的形式。比如：

* 嵌入式系统用 gui\_app\_start
* ios/android 用 SDL\_main
* Linux/MacOS 用 main
* Windows 用 wWinMain

如果希望开发的程序要在多个平台上运行，就需要处理多种入口函数。为了简化这个过程，AWTK 把入口函数的处理封装到 awtk_main.inc 中，开发者只需要 include 进来即可。

## 二、基本用法

* 实现 application_init 函数

```c
ret_t application_init() {
  /*do sth.*/
  return RET_OK;
}
```

* 实现 application_exit 函数

```c
ret_t application_exit() {
  log_debug("application_exit\n");
  return RET_OK;
}
```

* 包含 awtk_main.inc

```c
#include "awtk_main.inc"
```

> 请参考[完整的例子](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/button.c)

## 三、高级用法

* 指定APP TYPE和窗口大小

```c
#define LCD_WIDTH 800
#define LCD_HEGHT 600
#define AWTK_APP_TYPE APP_DESKTOP

#include "awtk_main.inc"
```

> 请参考[完整的例子](https://github.com/zlgopen/awtk/blob/master/demos/demo_desktop.c)

* 处理命令行参数

```c
static ret_t on_cmd_line(int argc, char* argv[]) {
  script_file = argc == 2 ? argv[1] : "./demos/demoui.js";

  return RET_OK;
}

#define ON_CMD_LINE on_cmd_line

#include "awtk_main.inc"
```
> 请参考[完整的例子](https://github.com/zlgopen/awtk-jerryscript/blob/master/src/c/main.c)

* 指定应用程序的名称

```c

#define APP_NAME "AWTK-JS"

#include "awtk_main.inc"
```

* 指定缺省字体

```c

#define APP_DEFAULT_FONT "default_full"

#include "awtk_main.inc"
```

> 请参考[完整的例子](https://github.com/zlgopen/awtk-jerryscript/blob/master/src/c/main.c)
