# fscript 内存需求评估

## 1. 测试环境

测试使用的 AWTK 版本：AWTKv1.7，SHA-1: 5f570df4fc42a5a0050bae1d2fe8f99d6aa42532。

测试所需栈大小的环境如下：

* 平台：RT1052。
* 编译器：Eclipse。
* 有无调试信息：有。
* 优化等级：-O0。

测试所需堆大小的环境如下：

* 平台：Windows7 x64。
* 编译器：Visual Studio C++ 2015。
* 有无调试信息：有。
* 优化等级：-O0。

> 备注：以下测试均为无 GUI 界面的结果，即只跑 fscript 脚本。

## 2. 推荐的栈堆大小

fscript **最低的栈（Stack）需求为 1.5 KB，最低的堆需求为 4 KB**，以上内存仅够初始化 fscript 基础模块和扩展模块，脚本代码只能做简单的运算以及打印信息。

使用 fscript 各个模块时建议栈堆大小详见下表，AWTK 内置了许多 fscript 的示例程序，它们放在 awtk/tests/fscripts 中，**如果想要将下表中的示例程序全部跑起来，建议栈（Stack）给到 5 KB，堆（Heap）给到 30 KB**。

| fscript 模块             | 相关宏定义                            | 建议 Stack 大小 | 建议 Heap 大小 | 示例脚本                                 |
| ------------------------ | ------------------------------------- | --------------- | -------------- | ---------------------------------------- |
| 基础模块                 | 默认开启，定义宏 WITHOUT_FSCRIPT 关闭 | 1.75 KB         | 6 KB           | demo_comments.fs                         |
| 扩展模块                 | WITH_FSCRIPT_EXT                      | ----            | ----           | ----                                     |
| value 扩展模块           | WITH_FSCRIPT_EXT                      | ----            | ----           | 该模块主要用于验证变量是否有效，开销极低 |
| 字符串扩展模块           | WITH_FSCRIPT_EXT                      | 2 KB            | 8KB            | demo_str.fs                              |
| object 扩展模块          | WITH_FSCRIPT_EXT                      | 2.5 KB          | 10.25 KB       | demo_object.fs                           |
| 位操作扩展模块           | FSCRIPT_WITH_BITS                     | 1.75 KB         | 9 KB           | demo_bits.fs                             |
| 数学扩展模块             | FSCRIPT_WITH_MATH                     | 2 KB            | 11 KB          | demo_expr.fs                             |
| 动态数组扩展模块         | FSCRIPT_WITH_ARRAY                    | 2.25 KB         | 13 KB          | demo_array.fs                            |
| 单一类型动态数组扩展模块 | FSCRIPT_WITH_TYPED_ARRAY              | 2.25 KB         | 12 KB          | demo_typed_array.fs                      |
| 日期和时间扩展模块       | FSCRIPT_WITH_DATE_TIME                | 2.25 KB         | 11 KB          | demo_date_time.fs                        |
| CRC 扩展模块             | FSCRIPT_WITH_CRC                      | 1.75 KB         | 9 KB           | demo_crc.fs                              |
| 大端小端扩展模块         | FSCRIPT_WITH_ENDIAN                   | 1.75 KB         | 6 KB           | demo_endian.fs                           |
| JSON 扩展模块            | FSCRIPT_WITH_JSON                     | 2.75 KB         | 11 KB          | demo_json.fs                             |
| 写缓冲区扩展模块         | FSCRIPT_WITH_BUFFER                   | 1.75 KB         | 15 KB          | demo_wbuffer.fs                          |
| 读缓冲区扩展模块         | FSCRIPT_WITH_BUFFER                   | 1.75 KB         | 15 KB          | demo_wbuffer.fs                          |
| app_conf 扩展模块        | FSCRIPT_WITH_APP_CONF                 | 1.75 KB         | 10.5 KB        | demo_app_conf.fs                         |
| 文件系统扩展模块         | FSCRIPT_WITH_FS                       | 2.5 KB          | 27.5 KB        | demo_fs.fs                               |
| 输入输出流扩展模块       | FSCRIPT_WITH_STREAM                   | ----            | ----           | 与文件流、串口流、TCP/UDP 流配合使用     |
| 文件流扩展模块           | FSCRIPT_WITH_STREAM_FILE              | 3 KB            | 17.25 KB       | demo_stream_file.fs                      |
| 串口流扩展模块           | FSCRIPT_WITH_STREAM_SERIAL            | ----            | ----           | demo_serial.fs                           |
| TCP/UDP 流扩展模块       | FSCRIPT_WITH_STREAM_INET              | ----            | ----           | demo_tcp.fs、demo_udp.fs                 |
| widget 扩展模块          | FSCRIPT_WITH_WIDGET                   | ----            | ----           | ----                                     |

一些特殊用法建议栈堆大小详见下表：

| fscript 模块 | 特殊用法                        | 建议 Stack 大小 | 建议 Heap 大小 | 示例脚本          |
| ------------ | ------------------------------- | --------------- | -------------- | ----------------- |
| 基础模块     | 大量 if else 判断               | 3 KB            | 18.5 KB        | demo_else_if.fs   |
| 基础模块     | 自定义函数 + 局部变量 + if else | 5 KB            | 14.25 KB       | demo_function1.fs |
| 基础模块     | 自定义函数 + 局部变量           | 3.25 KB         | 9 KB           | demo_function2.fs |
| 基础模块     | while 循环                      | 2.5 KB          | 6 KB           | demo_while1.fs    |

需要注意的是：

1. 以上表格中建议的 Heap 大小均为示例脚本运行时所需的**内存峰值**，具体请根据实际情况调整；
2. 在使用多个功能模块时，**不需要将建议的 Stack 大小进行累加，取最大值即可**，比如，同时使用了字符串扩展模块和 object 扩展模块，那么 Stack 大小取 object 扩展模块建议的 2.5 KB 即可。
3. 使用 if else 条件判断或 while 循环时，堆栈大小请根据条件分支数量以及循环次数适当调整；
4. 函数嵌套调用时，会消耗较多的堆栈空间，请尽量避免使用。

## 3. 堆空间的计算

### 3.1 fscript 本身所需的 Heap

fscript 初始化所需的 Heap 详见下表：

| fscript 模块                                           | Heap 需求 | 备注                                                                                                      |
| ------------------------------------------------------ | --------- | --------------------------------------------------------------------------------------------------------- |
| 基础模块 fscript                                       | 0         | 注册 fscript 基础模块函数的数组为静态全局变量，因此不占用 Heap                                            |
| 扩展模块 fscript_ext                                   | 约 2.4 KB | 注册 fscript 扩展模块函数的工厂和动态数组占 2 KB左右；注册 data_writer/data_reader 相关函数占 0.4 KB 左右 |
| 执行 fscript 脚本所需的上下文（object_default_t 对象） | 144 字节  | 此处指 object 上下文对象为空，即不保存任何 fscript 变量时的情况                                           |

> 备注：在 fscript 脚本中定义的全局变量都会保存到 object 上下文对象中，局部变量则会保存到 fscript 对象的 locale 属性中。

### 3.2 fscript 定义数据所需的 Heap

在 fscript 脚本中定义变量所需的 Heap 详见下表：

| 数据类型 | Heap 需求（每定义一个该类型的数据） | 备注                                                                       |
| -------- | ----------------------------------- | -------------------------------------------------------------------------- |
| Number   | 约 40 字节                          | 执行脚本时峰值增长约 180 字节                                              |
| String   | 最低约 56 字节                      | 与字符长度有关，每增加一个字符多占用 1 字节，执行脚本时峰值增长约 192 字节 |
| Array    | 最低约 264 字节                     | 执行脚本时峰值增长约 488 字节                                              |
| Object   | 最低约 208 字节                     | 执行脚本时峰值增长约 432 字节                                              |

> 备注：以上描述中的"最低"表示该数据类型为空时所占用的字节数；"峰值"表示在 fscript 脚本执行过程中每定义一个该类型的数据就会导致此时的内存峰值增长，定义完成后这些内存会被释放。

### 3.3 fscript 函数所需的 Heap

#### 3.3.1 调用内置函数

在 fscript 脚本中调用内置函数内存（Heap）峰值会增加 600 字节左右。函数嵌套调用时，每嵌套多一层，内存峰值会增加 170 字节左右，建议在调用函数时尽量避免多层嵌套。

#### 3.3.2 自定义函数

在 fscript 脚本中自定义函数（无参数、无返回值、无执行代码）内存（Heap）峰值会增加 350 字节左右。如果有传入参数以及返回值，请参考 3.2 章节计算内存。调用自定义函数与嵌套调用时消耗的内存与 fscript 内置函数一样。

## 4. 堆栈计算示例

比如，我们需要计算以下 fscript 脚本所需的堆栈空间，步骤如下：

```js
obj = object_create()
object_set(obj, "age", 123)
object_set(obj, "name", "awtk")

print(object_get(obj, "age"))
print(object_get(obj, "name"))
print(len(obj.name))

object_remove(a, "age")
assert(!value_is_valid(object_get(a, "age")))

unset(obj)
```

首先计算 Stack，以上脚本中使用了 fscript 的基础模块、value 扩展模块、字符串扩展模块以及 Object 扩展模块，根据上文中推荐的 Stack 大小，**取这四个模块中的最大值，即 2.5 KB**。

接着来计算所需的 Heap，参考 3.1 中的表格，初始化 fscript 需要约 2.5 KB，以上脚本中定义的数据详见下表：

| 变量名称                        | 变量类型 | 占用 Heap   | Heap 峰值       |
| ------------------------------- | -------- | ----------- | --------------- |
| obj                             | Object   | 208 字节    | 增长约 432 字节 |
| "age"                           | Number   | 40 字节     | 增长约 180 字节 |
| "name"                          | String   | 56 + 4 字节 | 增长约 192 字节 |
| 静态字符串 "age"                | String   | 56 + 3 字节 | 增长约 192 字节 |
| 静态字符串 "name"               | String   | 56 + 4 字节 | 增长约 192 字节 |
| object_get 函数返回 "age" 属性  | Number   | 40 字节     | 增长约 180 字节 |
| object_get 函数返回 "name" 属性 | String   | 56 + 4 字节 | 增长约 192 字节 |

根据上表可知，变量储存需要约 0.6 KB，定义变量时的内存峰值要达到 1.5 KB，并且脚本中调用了内置函数，且最多嵌套了四层，因此内存峰值需要再加 600 + 170 * 4 字节（约 1.25 KB），即脚本所需的 Heap 为：

```bash
2.5 + 0.6 + 1.5 + 1.25 = 5.85 KB
```

最后，计算好 Heap 之后，**考虑到程序运行过程中可能会出现内存碎片，再乘以系数 1.2 到 2**，即可得到项目大概需要的内存。
