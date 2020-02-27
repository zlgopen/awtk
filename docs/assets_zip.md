
# AWTK 资源打包

在支持文件系统的时候，AWTK 的资源缺省是以独立文件的形式放在指定目录里的。但在有的情况下，把全部资源文件放到一个压缩包中，是更好的或唯一的选择。比如：

* 开发 PC 应用程序。

* 在没有文件系统的情况下，把资源放到外部 flash。

* 开发 IOS 应用程序（通过 cmake 创建 Xcode 项目时，似乎没有办法让资源保持原有的目录结构）。

## 一、生成资源包

为了去掉资源中没用到的文件，可以把 assets 目拷贝到一个临时目录，并删除以下文件：

* inc 目录
* ui 目录下的 xml 文件。
* strings 目录下的 xml 文件。
* styles 目录下的 xml 文件。

然后把 assets 目录压缩成一个文件 assets.zip。

> 在 bash 下，可以使用 scripts/create_assets_zip.sh 脚本完成以上工作。

## 二、启用从资源包中加载资源

启用从资源包中加载资源，需要用 asset\_loader\_zip 替换缺省的 asset loader。方法如下：

```c
assets_manager_set_loader(assets_manager(), asset_loader_zip_create(ASSETS_ZIP));
```

[asset\_loader\_zip](https://github.com/zlgopen/awtk/blob/master/src/base/asset_loader_zip.h) 支持从 zip 文件加载，也支持从抽象的 [data reader](https://github.com/zlgopen/awtk/blob/master/src/tkc/data_reader.h) 接口加载。希望从外部 flash 中加载，可以把读取 flash 的功能包装成 data reader 的接口。
