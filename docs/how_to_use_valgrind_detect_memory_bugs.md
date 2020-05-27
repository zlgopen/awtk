
# 如何用 valgrind 查找内存相关问题

valgrind 是 Linux 下一个强大的内存调试工具，特别擅长查找内存越界、内存泄露和野指针等问题，如果所有的程序都应该在 valgrind 上运行一下，把各种潜在的问题暴露出来，在出现莫名其妙的奔溃时，更是应该借助 valgrind 来快速定位问题。

由于 valgrind 对 SDL 应用程序不太友好，需要在 linux-fb 模式下编译和运行 AWTK 应用程序。

在 VmWare 中，安装 Ubuntu 18 后，可以参考这个文档编译和运行 AWTK: [UbuntuVM](https://github.com/zlgopen/awtk-linux-fb/blob/master/README.UbuntuVM.md)

* 安装 valgrind

```
sudo apt-get install valgrind
```

* 用 valgrind 运行 demoui

```
valgrind --leak-check=full ./bin/demoui
```
