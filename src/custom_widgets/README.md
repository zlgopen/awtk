﻿# 自定义控件

可以把自定义控件放到这里，有以下好处：

* 自动生成 IDL，方便进行脚本语言绑定。
* 编译 android/ios/web 版本时可以自动拷贝源码。
* 自动编译到 awtk 动态库中，不需要额外的库和头文件依赖。

注意：

* 仍然需要自己注册控件，包含相应的头文件。
* 拷贝自定义控件下载到本目录。

如：
```
git clone https://gitee.com/zlgopen/awtk-widget-table-view
```
