
# AWTK 资源打包

在支持文件系统的时候，AWTK 的资源缺省是以独立文件的形式放在指定目录里的。但在有的情况下，把全部资源文件放到一个 zip 文件中，是更好的或唯一的选择。比如：

* 开发 PC 应用程序。

* 在没有文件系统的情况下，把资源放到外部 flash。

* 开发 IOS 应用程序（通过 cmake 创建 Xcode 项目时，似乎没有办法让资源保持原有的目录结构）。

## 一、生成 zip 文件

在 bash 下，可以使用 scripts/create_assets_zip.sh 脚本完成以上工作。

如：

```
../awtk/scripts/create_assets_zip.sh
```

> 假设当前在应用程序的根目录，应用程序和 awtk 并列在同一个目录中，生成的 zip 文件名为 assets.zip。

## 二、启用从 zip 文件中加载资源

实际使用时，只需要在包含 awtk_main.inc 之前，定义 ASSETS_ZIP 即可。如：

```c
#define ASSETS_ZIP "./assets.zip"

#include "awtk_main.inc"
```

> [asset\_loader\_zip](https://github.com/zlgopen/awtk/blob/master/src/base/asset_loader_zip.h) 支持从 zip 文件加载，也支持从抽象的 [data reader](https://github.com/zlgopen/awtk/blob/master/src/tkc/data_reader.h) 接口加载。希望从外部 flash 中加载，可以把读取 flash 的功能包装成 data reader 的接口。
