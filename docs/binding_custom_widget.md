# 自定义控件支持脚本绑定

本文 [二维码控件](https://github.com/zlgopen/awtk-widget-qr.git) 为例，介绍一下如何让自定义控件支持脚本绑定。

> 本文前提：

* 安装 node.js
* 安装 python
* awtk 编译环境
* 在 bash 中运行

## 1. 获取自定义控件的源码

```
git clone https://github.com/zlgopen/awtk-widget-qr.git
```

## 2. 把自定义控件的源码拷贝到 AWTK 的 ext_widgets 目录下

```
cp -rf awtk-widget-qr/src/qr  awtk/src/ext_widgets/qr
cp -rf awtk-widget-qr/src/qr_register.*  awtk/src/ext_widgets/
```

## 3. 注册控件

修改 awtk/src/ext\_widgets/ext\_widgets.c

```c
#include "qr_register.h"

ret_t tk_ext_widgets_init(void) {
...
qr_register();
...

}
```

## 4. 更新 idl

```
cd awtk/tools/idl_gen 
node index.js
cd - 
```

## 5. 更新 def

```
cd awtk/tools/dll_def_gen
 node index.js
 cd -
```

## 6. 编译 awtk

```
cd awtk
scons -j2
cd -
```

### 7. 更新绑定代码

```
git clone https://github.com/zlgopen/awtk-binding
cd awtk-binding
./gen.sh
cd -
```

### 8. 同步绑定代码到需要的语言

```
git clone https://github.com/zlgopen/awtk-jerryscript
cd awtk-jerryscript
./sync.sh
scons
```

### 9. 注意

 * 开发应用程序时记得拷贝自定义控件的 style 到资源中。
 * 如果代码下载太慢，可以从fastgit下载。
