# 如何将资源编译到应用程序并使用它们

## 1 将所有资源编译到应用中

在不支持文件系统的嵌入式平台上，我们可以将应用程序使用的所有资源文件（比如UI、样式、图片、字库等）编译到应用程序中，即应用直接从 ROM 中加载资源，方法如下：

### 1.1 打包常量资源

在 AWTK Designer 的"项目设置"中指定"资源的打包方式"为"文件+常量"，并打包资源，或者直接在项目目录终端中执行以下命令打包所有资源：

```bash
python scripts/update_res.py all
```

### 1.2 取消宏定义 WITH_FS_RES

取消 AWTK 的宏定义 WITH_FS_RES，表示不支持文件系统，然后重新编译 AWTK 和应用程序，此时所有资源文件都会编译到应用程序中，程序运行时，资源初始化函数 `assets_init()` 会调用 `assets_manager_add()` 接口将这些资源添加到内存中，具体代码详见项目目录下的 `res/assets_default.inc` 文件。

> 注：如果是嵌入式 Linux 平台，使用 [awtk-linux-fb](https://github.com/zlgopen/awtk-linux-fb) 移植程序，那么还需要额外取消 `awtk-linux-fb/awtk_config.py` 中宏定义 LOAD_ASSET_WITH_MMAP，重新编译 AWTK。

## 2 将部分资源编译到应用中

如果平台支持文件系统，只是想手动将部分资源编译到应用中，那么可以先参考 1.1 章节打包常量资源，再将打包好的常量资源加入资源管理器，后续正常使用即可。

在程序初始化时，调用 `assets_manager_add()` 接口将打包好的常量资源加入资源管理器，代码如下：

```c
// app/src/application.c

// 包含 home_page 页面的 UI 常量资源数组 
#include "../res/assets/default/inc/ui/home_page.data"

ret_t application_init(void) {
  // 将 home_page 页面的 UI 常量资源数组添加到资源管理器
  assets_manager_t* am = assets_manager();
  assets_manager_add(am, ui_home_page);

  ...
  return RET_OK;
}
```

> 注：如果是嵌入式 Linux 平台，使用 [awtk-linux-fb](https://github.com/zlgopen/awtk-linux-fb) 移植程序，那么还需要额外取消 `awtk-linux-fb/awtk_config.py` 中宏定义 LOAD_ASSET_WITH_MMAP，重新编译 AWTK。
