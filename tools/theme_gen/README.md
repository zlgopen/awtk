## 主题生成工具

负责把XML格式的主题数据转换成二进制的主题数据。

### 用法说明：

```
./bin/themegen input output [bin]
```

* input XML格式的主题数据文件名。
* output 输出文件名。
* bin 是否生成二进制格式(目标平台有文件系统时使用)，缺省生成C语言常量数组。

