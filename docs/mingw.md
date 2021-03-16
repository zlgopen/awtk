# 在 windows 下使用 gcc 编译 AWTK

在 windows 下，如果不想使用 Visual C++编译，也可以使用 gcc 编译。

* 下载并安装 [mingw](https://udomain.dl.sourceforge.net/project/mingw-w64/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/mingw-w64-install.exe)

> 64 位系统安装时 architecturex 选择 x86_64。

> 安装之后将 gcc 的路径加入系统环境变量 PATH 中。默认路径如下：

```
C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin
```

* 修改 awtk_config.py，放开注释掉的 TOOLS_NAME。

```
TOOLS_NAME = 'mingw'
```

* 编译

```
scons
```