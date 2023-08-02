# 在 windows 下使用 gcc 编译 AWTK

在 windows 下，如果不想使用 Visual C++编译，也可以使用 gcc 编译。具体方法如下：

* 1.根据实际情况下载不同的版本的 [mingw](https://github.com/niXman/mingw-builds-binaries/releases)

* 2.解压后将 路径 ${实际目录}/mingw64/bin 添加到系统环境变量 PATH 中。

* 3.修改 awtk_config_common.py，放开注释掉的 TOOLS_NAME。

```
TOOLS_NAME = 'mingw'
```

* 编译

```
scons
```