# debugger 命令行测试工具

## 1. 调试 fscript

* 先启动服务

```
./bin/runFScript @tests/fscripts/demo_function1.fs debug
```

* fdb 设置 target 为 default

```
[fdb] # target default
```

* 启动程序

>程序已经启动，无需额外操作。

## 2. 调试原生程序

* 先启动 [DAP](https://code.visualstudio.com/docs/extensionAPI/api-debugging) 服务

```
lldb-vscode -p 6789
```

* fdb 设置 target 为 lldb

```
[fdb] # target lldb
```

* 启动程序

> 用 r 命令启动指定的可执行文件。

```
[fdb] # r bin/demo
```

## 3. 调试 WASM 程序

* 启动 WASM 运行时

```
./bin/runWasmPLC -d -f bin/genc_while1_main.wasm -l verbose
```

* 先启动 [DAP](https://code.visualstudio.com/docs/extensionAPI/api-debugging) 服务

```
lldb-vscode -p 6789
```

* fdb 设置 target 为 lldb

```
[fdb] # target lldb
```

* 启动程序

> 用 r 命令启动 wasm

```
[fdb] # r wasm
```

## 3. 使用示例：

* 1. 设置 target 为 lldb

```
[fdb] # target lldb
```

* 2. 设置断点

```
[fdb] # b main
[fdb] # b foo
```

* 3. 列出断点

```
[fdb] # lb

```

>输出示例
```
breakpoints:
---------------------------
 [0] main
 [1] foo
```

* 4. 启动程序

```
[fdb] # r bin/demo
```

* 5. 查看源码

```
[fdb] # l
```

> 示例输出

```
0:   #include <stdio.h>
1:   #include "foo.h"
2:   #include "hello.hpp"
3:
4:   int s_foo = 100;
5:   const char* s_name = "abc";
6:
7:   int main(int argc, char* argv[]) {
8: =>  int a = 123;
9:     int b = 456;
10:     person_t p1;
11:     person_t p2;
```

* 6. 查看 callstack

```
[fdb] # bt
```

>示例输出

```
callstack:
---------------------------
=> [0] bar(int, int, float, double)
   [1] foo(int, int, float, double)
   [2] main
   [3] start
```

* 7. 查看变量

```
[fdb] # local
```

示例输出

```
[ 0] argc (int): 1
[ 1] argv (char **): 0x00007ff7bfeff7a8
[ 2] a (int): 1
[ 3] b (int): 108467
[ 4] p1 (person_t): person_t @ 0x7ff7bfeff600
[ 5] p2 (person_t): person_t @ 0x7ff7bfeff5a8
[ 6] i (int): 0
[ 7] ::s_name (const char *): 0x0000000100003f18 "abc"
[ 8] ::s_foo (int): 100
[ 9] General Purpose Registers (): 
[10] Floating Point Registers (): 
[11] Exception State Registers (): 
```

> 已知问题：虽然指定显示局部变量，lldb-vscode 还是提供了函数参数、函数内局部变量和文件内的全局变量。

* 8. 查看结构变量内部的值

```
[fdb] # p p1
```

示例输出
```
var:
---------------------
[ 0] p1.age (int): 50
[ 1] p1.weight (float): 55
[ 2] p1.name (name_t): name_t @ 0x7ff7bfeff608
[ 3] p1.names (name_t [4]): name_t [4] @ 0x7ff7bfeff618
```

* 9. 单步执行

```
[fdb] # n
```

* 10. 进入函数

```
[fdb] # si
```

* 11. 退出函数

```
[fdb] # so
```

* 12. 继续执行

```
[fdb] # c
```

* 13. 暂停执行

```
[fdb] # pause
```

* 14. 停止执行

```
[fdb] # stop
```

* 15. 停止执行

```
[fdb] # stop
```

* 16. 删除断点

```
[fdb] # d main
[fdb] # d test.c:10
[fdb] # d
```

> 本文以 Linux/MacOS 为例，Windows 可能会微妙差异，请酌情处理。
