# 在 windows 下使用 gcc 编译

在 windows 下，如果不想使用 Visual C++编译，也可以使用 gcc 编译。

* 下载并安装 [mingw](https://udomain.dl.sourceforge.net/project/mingw-w64/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/mingw-w64-install.exe)

* 修改 awtk_config.py，放开注释掉的 TOOLS_NAME。

```
-# TOOLS_NAME = 'mingw'
+TOOLS_NAME = 'mingw'
```

* 编译

```
scons
```