## BSVG 生成工具

负责把XML格式的SVG文件转换成二进制的SVG。


### 用法说明：

#### 1.BSVG 生成工具。

```
./bin/bsvggen svg_filename bsvg_filename [bin]
```

* svg\_filename svg文件名。
* bsvg\_filename bsvg文件名。
* bin 是否生成二进制格式(目标平台有文件系统时使用)，缺省生成C语言常量数组。

#### 2.BSVG dump工具(仅用于调试)。

```
./bin/bsvgdump bsvg_filename
```

* bsvg\_filename bsvg文件名。

