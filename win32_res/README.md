# AWTK 应用程序在 Windows 下的图标资源

如果需要修改应用程序的图标，有两种方法：

* 1. 用 VC 创建的 .res 文件代替 awtk.res，

* 2. 用 [ResEdit](http://rsdt.free.fr/ResEdit-x64.7z) 直接编辑 awtk.res 文件。

兼容 mingw 的话：

1. awtk.rc 转为 utf8（或者 utf8 with BOM） 的编码。
2. 注释 rc 文件中的 #include "targetver.h" 代码
3. windres 工具重新导出 res 文件。（windres 是 mingw 提供的 exe）

~~~cmd
windres awtk.rc -O coff awtk.res
~~~



