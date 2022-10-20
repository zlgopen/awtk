# 如何设置自定义键值

在实际的开发中，产品通常会有自身特有的按钮，而这些按钮的键值也不尽相同，为了用户更方便使用，故增加了设置自定义键值的功能。

## 一、设置方法

1. 在应用中的 main.c 定义开启自定义键值配置功能的宏 ENABLE_CUSTOM_KEYS：

```c
#include "awtk.h"
#include "mvvm_app.inc"

#define GLOBAL_INIT() mvvm_app_init()
#define GLOBAL_EXIT() mvvm_app_deinit()

BEGIN_C_DECLS
#ifdef AWTK_WEB
#include "assets.inc"
#else /*AWTK_WEB*/
#include "../res/assets.inc"
#endif /*AWTK_WEB*/
END_C_DECLS

extern ret_t application_init(void);

extern ret_t application_exit(void);

#define APP_LCD_ORIENTATION LCD_ORIENTATION_0

#define ENABLE_CUSTOM_KEYS TRUE /* 启动自定义键值配置功能 */

#include "awtk_main.inc"
```

2. 在应用项目文件夹下的 design\default\data 文件夹（没有data文件夹请自行创建）增加自定义键值配置文件 custom_keys.json。也可以在项目中的 main.c 定义 CUSTOM_KEYS_FILEPATH 宏指定配置文件的路径和名称，示例如下：

```c
#include "awtk.h"
#include "mvvm_app.inc"

#define GLOBAL_INIT() mvvm_app_init()
#define GLOBAL_EXIT() mvvm_app_deinit()

BEGIN_C_DECLS
#ifdef AWTK_WEB
#include "assets.inc"
#else /*AWTK_WEB*/
#include "../res/assets.inc"
#endif /*AWTK_WEB*/
END_C_DECLS

extern ret_t application_init(void);

extern ret_t application_exit(void);

#define APP_LCD_ORIENTATION LCD_ORIENTATION_0

#define ENABLE_CUSTOM_KEYS TRUE                           /* 启动自定义键值配置功能 */
#define CUSTOM_KEYS_FILEPATH "file://D:/custom_keys.json" /* 自定义键值配置文件路径 */

#include "awtk_main.inc"
```

3. 编辑自定义键值配置文件，如以下示例：

```json
{
  "ENABLE" : {
      "value" : 248
  },
  "START" : {
      "value" : 31
  },
  "STOP" : {
      "value" : 8
  }
}
```

   第一个键值对的键名为 ENABLE，键值为248，后面的键值对同理，不再赘述。

4. 打包资源。

执行以上步骤后，自定义键值就设置完成了。

## 二、简单使用示例

在 MVVM 工程中，实现点击 CLOSE 按钮关闭 home_page 窗口。

1. 使用 AWTK Designer 新建 MVVM 工程。
2. 根据上文步骤编写自定义键值配置文件：

```json
{
  "CLOSE" : {
      "value" : 4
  }
}
```

3. 绑定命令，实现点击 CLOSE 按钮关闭 home_page 窗口。

```xml
<window v-model="home_page_view_model" name="home_page" v-on:key_down:CLOSE="{nothing, CloseWindow=true}"/>>
```

4. 打包资源，编译运行。

关于 MVVM 命令参数的更多资料请参阅[ AWTK-MVVM 命令绑定 ](https://github.com/zlgopen/awtk-mvvm/blob/master/docs/11.command_binding.md)。

## 三、其它

* awtk-linux-fb，请在 awtk\_config.py 中定义下面的宏。

```python
COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DENABLE_CUSTOM_KEYS=1 ' # 启动自定义键值配置功能
COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DCUSTOM_KEYS_FILEPATH=\"file://D:/custom_keys.json\" ' # 与项目中 main.c 定义的一致，若 main.c 没定义，这里也不要定义。
```
