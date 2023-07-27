# ROMFS

一个简单的只读文件系统，并不是标准的 ROMFS，主要用于方便 WEB 访问配置数据。

## 将文件夹打包成 ROMFS 映像。

* 用法

```
./bin/romfs_make 目录 映像名
```

* 示例

```
./bin/romfs_make tests foo
```

> 将目录 tests 打包成 ROMFS 映像，foo.header 存放文件信息，foo.body 存放文件数据。

## 从 ROMFS 映像读取文件或目录

* 用法

```
./bin/romfs_read 映像名 文件名 输出文件名
```

* 示例

```
./bin/romfs_read foo fscripts/demo_udp.fs output.fs
```

> 从 foo 中读取文件 fscripts/demo_udp.fs，写入 output.fs。如果文件名是目录，则将目录列表输出到输出文件中。
