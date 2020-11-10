# fscript 脚本引擎

## 介绍

fscript 是一个极简的脚本引擎，借鉴了函数语言中一些思路，主要用于低端嵌入式系统，让用户轻松扩展现有系统，而不需要重新编译和下载固件。

* 特色：
  * 小巧。核心代码 300 行，扩展函数 600 行。
  * 灵活。支持多条语句、函数嵌套调用和变量定义。
  * 强大。超过 50 个内置函数，支持用 C 语言扩展函数。
  * 小内存。边解析边执行，释放不用的内存，最低开销小于 500 字节。

* 限制：
  * 不支持循环。

## 示例

```c
print("hello fscript")
```

在 PC 上测试运行：

```
./bin/runFScript 'print("hello fscript")'
```

## 语法

### 数据类型

* 数值类型 (double)
* 字符串类型 (string)。UTF-8 字符串。
* 布尔类型 (bool)。标准取值为：true 和 false，非 0 的数值视为 true。

### 函数嵌套调用

```
print(join(",", 1, 2, 3))
print(join(",", +(1, 2), -(5, 2), *(2, 3), /(9, 3)))
```

## 函数

### 基本函数 

#### print

> 打印调试信息到控制台。

##### 原型

```
print(str)
```

#### 示例

```
print("hello fscript")
```

#### noop

> 空函数。什么也不做，主要给 if 函数用。

##### 原型

```
noop()
```

#### if

> 条件执行。如果第一个参数为 true，执行第二个参数，否则执行第三个参数。

##### 原型

```
if(p, s1, s2)
```

#### 示例

```
set(a, 1)
if(<(get(a), 0), print(" a < 0"), print("a >= 0"))
```

#### set

> 设置变量的值。

##### 原型

```
set(var, value)
```

#### 示例

```
set(a, 1)
```

#### get

> 获取变量的值。

##### 原型

```
get(var, defvalue)
```

#### 示例

```
print(+(get(a, 0), 100))
```

#### unset

> 清除变量。

##### 原型

```
unset(var)
```

#### 示例

```
unset(a)
```

#### int

> 转换为整数类型。

##### 原型

```
int(var)
```

#### 示例

```
int("123")
```

#### float

> 转换为浮点数类型。

##### 原型

```
float(var)
```

#### 示例

```
float("123")
```

#### str

> 转换为字符串类型。

##### 原型

```
str(var)
```

#### 示例

```
str(int(123))
```

#### iformat

> 对整数进行格式化（生成的字符串长度不超过 63)

##### 原型

```
iformat(format, value)
```

#### 示例

```
iformat("hello:%d", 123)
```

#### fformat

> 对浮点数进行格式化（生成的字符串长度不超过 63)

##### 原型

```
fformat(format, value)
```

#### 示例

```
fformat("hello:%lf", 123)
```

#### exec

> 执行 object 里的一个命令。

##### 原型

```
exec(f, value)
```

#### 示例

```
exec("clear", "all")
```

> 具体功能与 object 的实现有关。

### 字符串函数

#### join

> 将多个变量用指定的分隔符拼接起来（最多 7 个字符串），形成一个字符串。

##### 原型

```
join(seperator, s1, s2, s3...)
```

#### 示例

```
join(",", 1, 2, 3, "abc")
```

#### len

> 取字符串的长度。

##### 原型

```
len(str)
```

#### 示例

```
len("abc")
```

#### toupper

> 将字符串转换成大写。

##### 原型

```
toupper(str)
```

#### 示例

```
tuppper("abc")
```

#### tolower

> 将字符串转换成小写。

##### 原型

```
tolower(str)
```

#### 示例

```
tolower("ABC")
```

#### trim

> 将字符串转换成小写。

##### 原型

```
trim(str)
```

#### 示例

```
trm("  abc  ")
```

#### substr

> 取子字符串。

##### 原型

```
substr(str, from, len)
```

#### 示例

```
substr("abcd", 1, 2)
```

#### replace

> 替换子字符串。

##### 原型

```
replace(str, old, new)
```

#### 示例

```
substr("ab cd", "ab", "hello")
```

### 运算函数

#### sum

> 对多个数值类型的参数求和。

##### 原型

```
sum(n1,n2...)
+(n1,n2...)
```

#### 示例

```
print(sum(1, 2, 3))
```

运行：

```
./bin/runFScript 'print(sum(1, 2, 3))'
```

输出：

```
6.000000
```

#### sub

> 计算两个数之差。

##### 原型

```
sub(n1,n2)
或
-(n1,n2)
```

#### 示例

```
print(sub(2, 1))
print(-(2, 1))
```

运行：

```
./bin/runFScript 'print(sub(2, 1))'
```

输出：

```
1.000000
```

#### mul

> 计算两个数之积。

##### 原型

```
mul(n1,n2)
或
*(n1,n2)
```

#### 示例

```
print(mul(2, 1))
print(*(2, 1))
```

运行：

```
./bin/runFScript 'print(mul(2, 1))'
```

输出：

```
2.000000
```

#### div

> 计算两个数之商。

##### 原型

```
div(n1,n2)
或
/(n1,n2)
```

#### 示例

```
print(div(2, 1))
print(/(2, 1))
```

运行：

```
./bin/runFScript 'print(div(2, 1))'
```

输出：

```
2.000000
```

#### %

> 计算两个数的模。

##### 原型

```
%(n1,n2)
```

#### 示例

```
print(%(23, 7))
```

运行：

```
./bin/runFScript 'print(%(23, 7))'
```

输出：

```
2.000000
```
    
#### and

> 逻辑与运算。

##### 原型

```
and(n1,n2)
&&(n1,n2)
```

#### 示例

```
print(&&(true, false))
print(&&(true, true))
```

运行：

```
./bin/runFScript 'print(and(true, true))'
```

输出：

```
true
```

#### or

> 逻辑或运算。

##### 原型

```
or(n1,n2)
||(n1,n2)
```

#### 示例

```
print(and(true, false))
print(||(true, true))
```

运行：

```
./bin/runFScript 'print(or(true, false))'
```

输出：

```
true
```

#### not

> 逻辑非运算。

##### 原型

```
not(n1)
!(n1)
```

#### 示例

```
print(not(true))
print(!(false))
```

运行：

```
./bin/runFScript 'print(not(false))'
```

输出：

```
true
```

#### &

> 位与运算。

##### 原型

```
&(n1,n2)
```

#### 示例

```
print(&(1, 1))
```

运行：

```
./bin/runFScript 'print(&(1, 2))'
```

输出：

```
0
```

#### |

> 位或运算。

##### 原型

```
|(1,2)
```

#### 示例

```
print(|(1, 2))
```

运行：

```
./bin/runFScript 'print(|(1, 2))'
```

输出：

```
3
```

#### ~

> 按位取反运算。

##### 原型

```
~(n1)
```

#### 示例

```
print(~(1))
```

运行：

```
./bin/runFScript 'print(iformat("0x%x", ~(1)))'
```

输出：

```
0xfffffffe
```

### 比较函数
#### <

> 小于。

##### 原型

```
<(1,2)
less(1,2)
```

#### 示例

```
print(<(1, 2))
print(<("a", "b"))
```

运行：

```
./bin/runFScript 'print(<(1, 2))'
```

输出：

```
true
```

#### <=

> 小于等于。

##### 原型

```
<=(1,2)
le(1,2)
```

#### 示例

```
print(<=(1, 2))
print(<=("a", "b"))
```

运行：

```
./bin/runFScript 'print(<=(1, 2))'
```

输出：

```
true
```

#### >

> 大于。

##### 原型

```
>(1,2)
great(1,2)
```

#### 示例

```
print(>(1, 2))
print(>("a", "b"))
```

运行：

```
./bin/runFScript 'print(>(1, 2))'
```

输出：

```
true
```

#### >=

> 大于等于。

##### 原型

```
>=(1,2)
ge(1,2)
```

#### 示例

```
print(>=(1, 2))
print(>=("a", "b"))
```

运行：

```
./bin/runFScript 'print(>=(1, 2))'
```

输出：

```
false
```

#### ==

> 等于。

##### 原型

```
==(1,2)
eq(1,2)
```

#### 示例

```
print(==(1, 2))
print(==("a", "b"))
```

运行：

```
./bin/runFScript 'print(==(1, 2))'
```

输出：

```
false
```

### 数学函数

#### random 

> 产生随机数。

##### 原型

```
random()
```

#### 示例

```
print(random())
```

#### sin 

> sin 函数。

##### 原型

```
sin(a)
```

#### 示例

```
sin(0)
```

#### cos 

> cos 函数。

##### 原型

```
cos(a)
```

#### 示例

```
cos(0)
```

#### tan 

> tan 函数。

##### 原型

```
tan(a)
```

#### 示例

```
tan(1)
```

#### asin 

> asin 函数。

##### 原型

```
asin(a)
```

#### 示例

```
asin(1)
```

#### acos 

> acos 函数。

##### 原型

```
acos(a)
```

#### 示例

```
acos(1)
```

#### atan 

> atan 函数。

##### 原型

```
atan(a)
```

#### 示例

```
atan(1)
```

#### abs 

> abs 函数。

##### 原型

```
abs(a)
```

#### 示例

```
abs(1)
```

#### min 

> min 函数。

##### 原型

```
min(a, b)
```

#### max 

> max 函数。

##### 原型

```
max(a, b)
```

#### 示例

```
max(1, 2)
```

#### 示例

```
min(1, 2)
```

#### clamp 

> clamp 函数。

##### 原型

```
clamp(a, min, max)
```

#### 示例

```
clamp(2, 1, 3)
```

### 时间函数

#### time_now 

> 获取当前时间函数 (s)。

##### 原型

```
time_now()
```

#### 示例

```
time_now()
```

#### time_now_ms 

> 获取当前时间函数 (ms)。

##### 原型

```
time_now_ms()
```

#### 示例

```
time_now_ms()
```

#### time_now_us 

> 获取当前时间函数 (us)。

##### 原型

```
time_now_us()
```

#### 示例

```
time_now_us()
```
