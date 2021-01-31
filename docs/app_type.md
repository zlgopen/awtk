## 应用程序的类型

在 AWTK 中，应用程序有以下类型：

```c
/**
 * @enum app_type_t
 * @prefix APP_
 * @annotation ["scriptable"]
 * 应用程序类型。
 */
typedef enum _app_type_t {
  /**
   * @const APP_MOBILE
   * 嵌入式或移动 APP
   */
  APP_MOBILE = 0,
  /**
   * @const APP_SIMULATOR
   * 模拟器。
   */
  APP_SIMULATOR,
  /**
   * @const APP_DESKTOP
   * 桌面应用程序。
   */
  APP_DESKTOP
} app_type_t;
```

### APP_SIMULATOR

> 默认运行方式，有以下特点：

* 使用 AWTK 的输入法
* 使用 AWTK 的剪切板
* 在 PC 上运行时，窗口不可调整大小。

### APP_DESKTOP
> 用于开发 PC 软件，有以下特点：

* 使用系统的输入法
* 使用系统的剪切板
* 窗口可以调整大小。

### APP_MOBILE

> 在 Android/iOS 上：
* 使用系统的输入法
* 使用系统的剪切板
* 窗口自动调整大小。

> 在嵌入式系统：
* 使用 AWTK 的输入法
* 使用 AWTK 的剪切板
* 窗口自动调整大小。

### 使用方法

建议使用 awtk_main.inc。

对于 PC 软件，需要 APP\_TYPE 为 APP\_DESKTOP，其它情况保持默认即可，编译时会自动判断。

> 示例(demos/demo_desktop.c)

```c
#define LCD_WIDTH 400
#define LCD_HEGHT 400
#define APP_TYPE APP_DESKTOP

#ifdef WITH_FS_RES
#define APP_DEFAULT_FONT "default_full"
#endif /*WITH_FS_RES*/

#include "awtk_main.inc"
```
