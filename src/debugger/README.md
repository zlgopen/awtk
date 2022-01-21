# fscript 调试器

## 使用方法

### 启动服务端

> 使用 runFScript 运行 fscript 程序，第 2 个参数指定为 debug，runFScript 启动后监听 6789 端口，等待客户端连接。

如：
```
./bin/runFScript @test.fs debug
```

### 启动 fdb

```
./bin/fdb localhost
```
>localhost 代表本机，可以省略。

### fdb 常用命令

* h 显示帮助信息
* q 退出调试
* l 显示源码
* n 执行下一行代码
* s 执行下一行代码（可进入脚本函数）
* so 执行到当前函数完成
* u 执行下一行代码（跳过循环）
* c 执行到下一个断点
* lb 显示断点
* bt 显示调用堆栈
* b 设置断点。

>用法
```
b 行号
```

* d 删除断点

>用法（不指定行号删除全部断点）
```
d 行号
```

* local 显示局部变量
* self 显示成员变量
* global 显示全局变量
