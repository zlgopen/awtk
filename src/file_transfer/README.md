# 文件传输

主要用于在 PC 和开发板之间传输文件，通信方式可用 TCP 或串口。如果内部 flash 不够存放资源数据和程序数据，可用把资源数据做出文件系统镜像（或 zip 文件），下载到外部 flash 中。

File Transfer 提供了文件接收方和文件发送方两部分代码，文件发送方在 PC 端运行，文件接收方在开发板上运行。

* 在 PC 端运行 TCP 文件发送程序：

```
./bin/tcp_file_sender.exe port filename
```

如：
```
./bin/tcp_file_sender.exe 9999 assets.bin
```

* 在 PC 端运行串口文件发送程序：

```
./bin/serial_file_sender.exe port filename
```

如：
```
./bin/serial_file_sender.exe COM3 assets.bin
```
