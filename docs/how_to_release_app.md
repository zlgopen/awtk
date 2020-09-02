# 如何将生成的可执行文件和资源打包

在开发完成后，通常需要将可执行文件和运行时需要的资源拷贝出来，发给客户或放到板子上运行。AWTK 提供了一个脚本 scripts/release.py，将可执行文件和运行时需要的资源拷贝出来，放到当前目录的 release 子目录下。用法如下：

假如当前项目是 awtk-hello，在 awtk-hello 目录中运行下面的命令：

```
python ../awtk/scripts/release.py hello_awtk.exe
```

> Linux/MacOS/LinuxFB 不能带 .exe 扩展名。

> 本脚本适用于 Linux、Windows、MacOS、Linux_FB，不适用于其它嵌入式系统。对于其它嵌入式系统，本脚本可以将运行时需要的资源拷贝出来，放到开发板的 T 卡上，自己删除 release/bin 目录即可。
