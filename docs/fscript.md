# FScript 脚本引擎

## 1. 介绍

FScript 是一个极简的脚本引擎，借鉴了函数语言中一些思路，主要用于低端嵌入式系统，让用户轻松扩展现有系统，而不需要重新编译和下载固件。

* 特色：

  * 小内存。最低开销小于 400 字节。
  * 小巧。核心代码 1500 行，基本扩展函数 1000 行。
  * 灵活。支持多条语句、函数嵌套调用和变量定义。
  * 简单。熟悉任何一种编程语言的人，5 分钟内即可学会。
  * 强大。超过 60 个内置函数，支持类似 C 语言的条件语句、循环语句和注释方式，支持复杂的表达式，支持用 C 语言扩展函数。

> keil -O1 编译结果：

```bash
      Code (inc. data)   RO Data    RW Data    ZI Data      Debug   Object Name
     10380       1580       1909          4          0      86893   fscript.o
```

实际内存使用与场景有关，请参考 [内存需求评估](fscript_ram_req.md)

## 2. 示例

```js
print("hello fscript")
```

* 在 PC 上测试运行：

如：
```bash
./bin/runFScript 'print("hello fscript")'
```

* 在 PC 上测试运行指定文件：

> 第一个命令行参数以 @ 开头表示它是一个脚本文件，读取其内容执行。

如：
```bash
./bin/runFScript @tests/fscripts/demo_while1.fs
```

## 3. 语法

### 3.1 数据类型

* 浮点数类型 (double)。
* 整数类型 (int32)。支持十进制、二进制和十六进制。
* 字符串类型 (string)。UTF-8 字符串，用英文双引号扩起来。
* 布尔类型 (bool)。标准取值为：true 和 false，非 0 的数值视为 true。
* 可以通过扩展实现混合数据类型的数组、固定类型的数组和 map。
* 可以通过类型转换函数得到各种基本类型。如 int8/int16/int32/int64/uint8/uint16/uint32/uintg64/float/double。

### 3.2 注释

* 格式：

```js
//line comment
/*block comment*/
```

* 示例：

```js
//this is line comment
/*this is block comment*/
```

### 3.3 函数调用

```js
print(0xff)
print(100)
print(0b1010)
print("hello fscript")
print("hello", 123)
```

> 函数之间可以用空格、换行和英文分号分隔。

### 3.4 定义变量

* 通过 set 函数设置

如：

```js
/* 系统变量：一个 AWTK 应用程序只有一个 global 对象，在程序初始化时创建，程序退出时销毁 */
set(global.a, 123)

/* 全局变量：保存在脚本执行的上下文对象中，通常由外部控制生命周期 */
set(a, 123)
```

* 通过=赋值

如：

```js
/* 系统变量：一个 AWTK 应用程序只有一个 global 对象，在程序初始化时创建，程序退出时销毁 */
global.a = 123
global.b = "abc"
global.c = true

/* 全局变量：保存在脚本执行的上下文对象中，通常由外部控制生命周期 */
a = 123
b = "abc"
c = true

/* 局部变量：又称临时变量，在脚本执行完毕或函数调用完成就销毁 */
var a = 123
var b = "abc"
var c = true
```

> 备注：
> 
> 1. global. 开头的变量为系统变量，可以在同一进程的多个脚本中共享数据（这并非是好的做法，尽量避免使用）
> 2. 在 FScript 脚本中定义的系统变量和全局变量在执行完一段脚本后通常不会被清空，因此使用这些变量前需要初始化。

#### 3.4.1 变量名命名规则：

* 以字母开头，后面可用数字、英文下划线和英文点。

如：

```js
age=123
name="fscript"
file_name="test.txt"
```

* 英文点"."可以用来访问对象的成员，如果对应的对象不存在则当作普通变量。

如：

```js
msg.payload = "hello"
```

> 在这里，如果对象 msg 存在，msg.payload 为 msg 对象的 payload 成员，否则 msg.payload 则是一个普通变量。

### 3.5 获取变量

```js
abc
get(abc)
```

> * 获取变量时，如果变量不存在，自动当成字符串处理，并打印获取变量失败的警告。
> * 如果不希望变量被当成字符串，可以加上$前缀，或者使用 get 函数。
> * FScript 不支持将函数返回值当做变量名使用并通过英文点"."访问子变量，比如 array_create().size，此时会将 .size 当做字符串处理。
>
> 注意：FScript 不提倡直接使用变量名当做字符串，**如需要如果使用字符串常量，请用双引号包起来**。

如：

```js
./bin/runFScript 'print($abc)' 

get var abc failed
(null) 
result:true
cost: 112 us
```

```js
./bin/runFScript 'print(abc)' 

get var abc failed
abc 
result:true
cost: 112 us
```

判断一个变量是否有效，需要使用 get 函数：

> 注意：如果不使用 get 函数获取变量，那么当变量不存在时，FScript 会自动将其当成字符串处理，此时 value_is_valid 函数一定返回 true。

```js
print("abc is valid? ", value_is_valid(get(abc)))
var abc=123
print("abc is valid? ", value_is_valid(get(abc)))
print(get(abc) + 321)
```

### 3.6 函数嵌套调用

```js
print(join(",", 1, 2, 3))
print(join(",",1,2,3,4), join(";",5,6,7,8))
```

### 3.7 提前返回 (return)

```js
var a = 1;
if(a == 1) {
  print("return");
  return;
} else {
  print("end");
}
```

### 3.8 条件执行

* 语句方式

```js
/* if else */
var a=random(1, 100)
var b=random(1, 100)

print("a is ", a);
print("b is ", b);

if(a < b) {
  print(a, "<", b)
} else {
  print(a, ">=", b)
}
```

```js
/* else if */
var a=random(1, 6);
var b = "";

if(a == 1) {
  b = "a"
} else if(a == 2) {
  b = "b"
} else if(a == 3) {
  b = "c"
} else if(a == 4) {
  b = "d"
} else {
  b = "other"
}
```

> 代码块需要用 {} 扩起来，else 语句可以省略。

* 函数方式

```js
if(true, print("a"), print("b"))
```

### 3.9 while 循环

* 语法

```js
while （条件） {
}
```

* 示例

```js
var a=0
var b=0

while(a < 100) {
  a=a+1
  b=b+a
  print(a, b)
}
```

### 3.10 until 循环

> 重复执行直到条件成立

* 语法

```js
until（条件） {
}
```

* 示例

```js
var a=0
var b=0

until(a >= 100) {
  a=a+1
  b=b+a
  print(a, b)
}
```

### 3.11 for 循环

* 语法

```js
for（初始化表达式；结束条件；迭代表达式） {
}
```

* 示例

```js
for(var a = 0; a < 100; a = a + 1) {
  print(a)
}
```

### 3.12 for_in 循环

> 用于遍历数组对象

* 语法

```js
for_in（迭代变量，数组对象） {
}
```

* 示例

```js
var sum = 0;
var a = array_create(11, 22, 33, 44, 55) 
for_in(i, a) {
  sum = sum + i
  print(i)
}
print(sum)
```

### 3.13 repeat 循环

* 语法

```js
repeat（迭代变量，开始值，结束值，增量） {
}
```

* 示例

```js
repeat(i, 0, 10, 1) {
  print(i)
}

var times = 10
repeat(i, 0, times, 2) {
  print(i)
}

var times = 10
repeat(i, 20, times, -2) {
  print(i)
}
```

### 3.14 repeat_times 循环

* 语法

```js
repeat_times（重复次数） {
}
```

* 示例

```js
repeat_times(100) {
  print("hello")
}
```

### 3.15 表达式

为了确保优先级正确，尽量用括号明确优先级。

#### 3.15.1 支持的操作符

* 四则运算：+ - * / %
* 逻辑运算：|| && !
* 位运算：| & ~
* 比较运算：< <= > >= == 
* 条件运算：(p) ? a : b

> 对于加法运算 (+)，参数中有字符串时，进行字符串连接。

#### 3.15.2 示例

```js
var a = 10
var b = 20
```

```js
a<b
1+2
1-2
1*2
1/2
3%2
a||b
a&&b
a|b
a&b
!a
~a
a<b
a<=b
a>b
a>=b
(1+2)*(2+3)
sin(a) + sin(b)
a<b ? print(a) : print(b)
set(a, a+1)
set(a, a+b)
"abc" + "123"
```

## 4. 函数

### 4.1 基本函数 

#### 4.1.1 assert

> 断言
----------------------------

##### 原型

```js
assert(condition, crash_if_fail) => void
```

* crash\_if\_fail 可选。为 true 时，调用系统的 assert，否则只是打印警告。

##### 示例

```js
assert(1<2)
```

#### 4.1.2 eval

> 执行另外一段脚本。
----------------------------

##### 原型

```js
eval(condition) => value
```

##### 示例

```js
eval("1+2")
```

#### 4.1.3 has_error

> 检查是否之前遇到错误
----------------------------

##### 原型

```js
has_error() => bool
```

##### 示例

```js
print(has_error())
```

#### 4.1.4 get_last_error

> 获取前一个错误
----------------------------

##### 原型

```js
get_last_error() => object | null
```

如果有错，返回对象，其成员如下：

* message 错误消息
* code 错误码
* line 出错的行
* col 出错的列

没有错误，返回无效值。

##### 示例

```js
print(len())
var error = get_last_error();
print("XXX:", error.message, error.code, error.line, error.col);
```

#### 4.1.5 clear_error

> 清除之前遇到错误
----------------------------

##### 原型

```js
clear_error() => void
```

##### 示例

```js
clear_error()
```

#### 4.1.6 print

> 打印调试信息到控制台。
----------------------------

##### 原型

```js
print(str,...) => void
```

##### 示例

```js
print("hello fscript")
print("hello", "fscript")
print(1)
print(true)

var a=100
var b=200
print(a+b)
print(join(",", a, b))
```

#### 4.1.7 sleep_ms

> 睡眠指定时间长（毫秒）
----------------------------

##### 原型

```js
sleep_ms(time) => bool
```

##### 示例

```js
sleep_ms(100);
```

#### 4.1.8 set

> 设置变量的值。建议使用=赋值。

----------------------------

##### 原型

```js
set(var, value) => bool
```

##### 示例

```js
var a=1
set(a, 1)
```

#### 4.1.9 unset

> 清除变量。

对于不再使用的变量，特别是对象和字符串，请调用本函数清除掉。

当一个变量重复赋值时，会自动清除之前的数据。

----------------------------

##### 原型

```js
unset(var) => void
```

##### 示例

```js
var a = 10;
unset(a)
```

#### 4.1.10 int

> 转换为整数类型。

----------------------------

##### 原型

```js
int(var) => int32_t
```

##### 示例

```js
int("123")
```

#### 4.1.11 i8

> 转换为 int8_t 类型。

----------------------------

##### 原型

```js
i8(var) => int8_t
```

##### 示例

```js
i8("123")
```

#### 4.1.12 i16

> 转换为 int16_t 类型。

----------------------------

##### 原型

```js
i16(var) => int16_t
```

##### 示例

```js
i16("123")
```

#### 4.1.13 i32

> 转换为 int32_t 类型。

----------------------------

##### 原型

```js
i32(var) => int32_t
```

##### 示例

```js
i32("123")
```

#### 4.1.14 i64

> 转换为 int64_t 类型。

----------------------------

##### 原型

```js
i64(var) => int64_t
```

##### 示例

```js
i64("123")
```

#### 4.1.15 u8

> 转换为 unt8_t 类型。

----------------------------

##### 原型

```js
u8(var) => uint8_t
```

##### 示例

```js
u8("123")
```

#### 4.1.16 u16

> 转换为 unt16_t 类型。

----------------------------

##### 原型

```js
u16(var) => value(uint16)
```

##### 示例

```js
u16("123")
```

#### 4.1.17 u32

> 转换为 unt32_t 类型。

----------------------------

##### 原型

```js
u32(var) => uint32_t
```

##### 示例

```js
u32("123")
```

#### 4.1.18 u64

> 转换为 unt64_t 类型。

----------------------------

##### 原型

```js
u64(var) => uint64_t
```

##### 示例

```js
u64("123")
```

#### 4.1.19 f32

> 转换为单精度类型。

----------------------------

##### 原型

```js
f32(var) => float
```

##### 示例

```js
f32("123")
```

#### 4.1.20 f64

> 转换为双精度类型。
----------------------------

##### 原型

```js
f64(var) => double
```

##### 示例

```js
f64("123")
```

#### 4.1.21 str

> 转换为字符串类型。
----------------------------

##### 原型

```js
str(var [,force_pointer_as_str]) => str
```

> force\_pointer\_as\_str 如果输入参数是 POINTER 类型，是否将强制转换成字符串。

##### 示例

```js
str(int(123))
str(msg.payload, true)
```

#### 4.1.22 binary

> 转换为 binary_data_t 类型。

----------------------------

##### 原型

```js
binary(var) => binary_data_t
binary(var, size) => binary_data_t
binary(var, size, clone) => binary_data_t
```

* var 是常见类型的变量。如：i8/i16/i32/i64/u8/u16/u32/u64/float/double/string/wstring/pointer 等。
* size 如果类型为 pointer 时，size 必须指定，其它类型可以不指定，此时用默认长度。
* clone 是否拷贝数据。基本数据类型 i8/i16/i32/i64/u8/u16/u32/u64/float/double 始终拷贝。string/wstring/pointer 由本参数决定是否拷贝。

> 如果不拷贝，请确保引用的变量的生命周期大于新的变量。

##### 示例

```js
binary(100)
binary("123")
binary("123", 2)
binary("123", 2, true)
binary(data, 4)
binary(data, 8, true)
```

#### 4.1.23 iformat

> 对整数进行格式化（生成的字符串长度不超过 63）
----------------------------

##### 原型

```js
iformat(format, value) => str
```

##### 示例

```js
iformat("hello:%d", 123)
```

#### 4.1.24 fformat

> 对浮点数进行格式化（生成的字符串长度不超过 63）
----------------------------

##### 原型

```js
fformat(format, value) => str
```

##### 示例

```js
fformat("hello:%lf", 123)
```

#### 4.1.25 exec

> 执行 object 里的一个命令。
----------------------------

##### 原型

```js
exec(cmd, arg) => bool
```

##### 示例

```js
exec("clear", "all")
```

> 具体功能与 object 的实现有关。

### 4.2 字符串函数

#### 4.2.1 join

> 将多个变量用指定的分隔符拼接起来，形成一个字符串。

----------------------------

##### 原型

```js
join(seperator, s1, s2, s3...) => str
```

##### 示例

```js
join(",", 1, 2, 3, "abc")
```

#### 4.2.2 one_of

> 从字符串数组中取出第 N 个字符串

----------------------------

##### 原型

```js
one_of(str_array, index, sep) => str
```

> sep 为分隔符，默认为英文分号 (;)。

##### 示例

```js
one_of("aa;bb;cc", 0) // => aa
one_of("aa;bb;cc", 1) // => bb
one_of("aa;bb;cc", 2) // => cc
one_of("aa.bb.cc", 0, ".") // ==> aa
```

#### 4.2.3 len

> 取字符串/数组的长度。

----------------------------

##### 原型

```js
len(str) => uint32_t
```

##### 示例

```js
len("abc")
```

#### 4.2.4 toupper

> 将字符串转换成大写。

----------------------------

##### 原型

```js
toupper(str) => str
```

##### 示例

```js
toupper("abc")
```

#### 4.2.5 tolower

> 将字符串转换成小写。

----------------------------

##### 原型

```js
tolower(str) => str
```

##### 示例

```js
tolower("ABC")
```

#### 4.2.6 trim

> 去掉字符串两端的空白字符串。
----------------------------

##### 原型

```js
trim(str) => str
```

##### 示例

```js
trim("  abc  ")
```

#### 4.2.7 substr

> 取子字符串。
----------------------------

##### 原型

```js
substr(str, from, len) => str
```

##### 示例

```js
substr("abcd", 1, 2)
```

#### 4.2.8 replace

> 替换子字符串。
----------------------------

##### 原型

```js
replace(str, old, new) => str
```

##### 示例

```js
replace("ab cd", "ab", "hello")
```

#### 4.2.9 contains

> 检查是否包含指定的子字符串。
----------------------------

##### 原型

```js
contains(str, substr) => bool
```

##### 示例

```js
contains("ab cd", "ab")
```

### 4.3 运算函数

#### 4.3.1 sum

> 对多个数值类型的参数求和。
----------------------------

##### 原型

```js
sum(n1,n2...)
+(n1,n2...)
n1+n2
```

##### 示例

```js
print(sum(1, 2, 3))
```

#### 4.3.2 sub

> 计算两个数之差。
----------------------------

##### 原型

```js
sub(n1,n2)
或
n1-n2
```

##### 示例

```js
print(sub(2, 1))
print(2 - 1)
```

#### 4.3.3 mul

> 计算两个数之积。
----------------------------

##### 原型

```js
mul(n1,n2)
或
n1*n2
```

##### 示例

```js
print(mul(2, 1))
print(2 * 1)
```

#### 4.3.4 div

> 计算两个数之商。
----------------------------

##### 原型

```js
div(n1,n2)
或
n1/n2
```

##### 示例

```js
print(div(2, 1))
print(2/1)
```

#### 4.3.5 %

> 计算两个数的模。
----------------------------

##### 原型

```js
n1%n2
```

##### 示例

```js
print(23%7)
```

#### 4.3.6 and

> 逻辑与运算。
----------------------------

##### 原型

```js
and(n1,n2)
n1 && n2
```

##### 示例

```js
print(true && false)
print(true && true)
```

#### 4.3.7 or

> 逻辑或运算。
----------------------------

##### 原型

```js
or(n1,n2)
n1 || n2
```

##### 示例

```js
print(a || b)
```

#### 4.3.8 not

> 逻辑非运算。
----------------------------

##### 原型

```js
not(n1)
!(n1)
```

##### 示例

```js
print(not(true))
print(!(false))
```

### 4.4 比较函数

> 支持字符串比较。

#### 4.4.1 <

> 小于。
----------------------------

##### 原型

```js
<(a,b)
less(a,b)
a<b
```

#### 4.4.2 <=

> 小于等于。
----------------------------

##### 原型

```js
<=(a,b)
le(a,b)
a<=b
```

#### 4.4.3 >

> 大于。
----------------------------

##### 原型

```js
>(a,b)
great(a,b)
a>b
```

#### 4.4.4 >=

> 大于等于。
----------------------------

##### 原型

```js
>=(a,b)
ge(a,b)
a>=b
```

#### 4.4.5 ==

> 等于。
----------------------------

##### 原型

```js
==(a,b)
eq(a,b)
a==b
```

#### 4.4.5 !=

> 不等于。
----------------------------

##### 原型

```js
!=(a,b)
a != b
```

### 4.5 数学函数

#### 4.5.1 random 

> 产生随机数。
----------------------------

##### 原型

```js
random() => int
random(min, max) => int
```

##### 示例

```js
a = random();
```

#### 4.5.2 round 

> 四舍五入。
----------------------------

##### 原型

```js
round(v) => double
```

##### 示例

```js
round(4.5)
```

#### 4.5.3 floor 

> 返回小于等于指定数值的最大整数。
----------------------------

##### 原型

```js
floor(v) => double
```

##### 示例

```js
floor(4.5)
```

#### 4.5.4 ceil 

> 返回大于等于指定数值的最小整数。
----------------------------

##### 原型

```js
ceil(v) => double
```

##### 示例

```js
ceil(4.5) 
```

#### 4.5.5 abs 

> abs 函数。
----------------------------

##### 原型

```js
abs(a) => double
```

##### 示例

```js
abs(1)
```

#### 4.5.6 min 

> min 函数。
----------------------------

##### 原型

```js
min(a, b) => double
```

##### 示例

```js
min(1, 2)
```

#### 4.5.6 max 

> max 函数。
----------------------------

##### 原型

```js
max(a, b) => double
```

##### 示例

```js
max(1, 2) 
```

#### 4.5.7 clamp 

> clamp 函数。
----------------------------

##### 原型

```js
clamp(a, min, max) => double
```

##### 示例

```js
clamp(2, 1, 3)
```

### 4.6 时间函数

> 已经移动到扩展模块。  

## 5. 自定义函数

### 5.1 定义函数

```c
static ret_t func_foo(object_t* obj, fscript_args_t* args, value_t* v) {
  value_set_int(v, 123);

  return RET_OK;
}
```

### 5.2 注册和使用私有函数

```c
  value_t v;
  object_t* obj = object_default_create();
  object_set_prop_pointer(obj, "function.foo", (void*)func_foo);

  fscript_eval(obj, "foo()", &v);
  value_reset(&v);

  OBJECT_UNREF(obj);
```

### 5.3 注册全局函数

* 初始化时调用

```c
fscript_register_func("foo", func_foo);
```

### 5.4 定义脚本函数

* 函数定义

```js
function foo1(v1, v2) {
  return v1 + v2; 
}
assert(foo1(100, 200) == 300)
```

* 使用 var 定义局部变量。

```js
function foo2(v1, v2) {
  var v3 = v1 + v2; 
  return v3
}
assert(foo2(100, 200) == 300)

function foo3(v1, v2) {
  var v3 = v1 + v2; 
  if(v3 < 100) {
    return true
  } else {
    return false
  }
}
assert(foo3(10, 20))
assert(!foo3(100, 200))
```

> 函数内的临时变量，无论在哪里定义，一旦定义，在该函数内都可以使用。

```js
function foo4 (v1, v2) {
  var v3 = v1 + v2;
  if(v3 < 100) {
    var name = "awtk";
  } else {
    var name = "react-awtk";
  }

  return name;
}

assert(foo4(10, 20) == 'awtk')
assert(foo4(100, 200) == 'react-awtk')
```

## 6. 性能测量与优化

runFScript 的第二个参数可以指定运行次数，方便测量某个函数的运行时间。

比如：

```bash
./bin/runFScript '123+234' 100000
```

## 7. 内置常量

| 名称                 | 说明                   |
| -------------------- | ---------------------- |
| RET\_OK              | 成功。                 |
| RET\_OOM             | Out of memory。        |
| RET\_FAIL            | 失败。                 |
| RET\_NOT\_IMPL       | 没有实现/不支持。      |
| RET\_QUIT            | 退出。通常用于主循环。 |
| RET\_FOUND           | 找到。                 |
| RET\_BUSY            | 对象忙。               |
| RET\_REMOVE          | 移出。通常用于定时器。 |
| RET\_REPEAT          | 重复。通常用于定时器。 |
| RET\_NOT\_FOUND      | 没找到。               |
| RET\_DONE            | 操作完成。             |
| RET\_STOP            | 停止后续操作。         |
| RET\_SKIP            | 跳过当前项。           |
| RET\_CONTINUE        | 继续后续操作。         |
| RET\_OBJECT\_CHANGED | 对象属性变化。         |
| RET\_ITEMS\_CHANGED  | 集合数目变化。         |
| RET\_BAD\_PARAMS     | 无效参数。             |
| RET\_TIMEOUT         | 超时。                 |
| RET\_CRC             | CRC 错误。              |
| RET\_IO              | IO 错误。               |
| RET\_EOS             | End of Stream          |
| RET\_NOT\_MODIFIED   | 没有改变。             |

示例：

```js
return RET_OK;
```
  
## 8. 注册常量

fscript 本身只是一个胶水语言，它负责把各个原生扩展模块组合起来。扩展模块中往往有一些常量，有些常量是了方便记忆（比如 PI)，有的常量是降低变化带来的影响（比如 TK\_NAME\_LEN) ，有的常量用于表示特定的意义（比如一些枚举）。为了保持这些常量在 fscript 和原生代码中的一致性，和函数一样，fscript 提供了一种注册机制，让原生模块可以注册常量，这些常量在 fscript 中就可以访问了。

* 注册常量

```c
/**
 * @method fscript_register_const_value
 * 注册常量。
 * @param {const char*} name 常量名。
 * @param {const value_t*} value 数据。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t fscript_register_const_value(const char* name, const value_t* value);
```

* 注册整数常量(只是为了使用方便而提供的包装函数)

```c
/**
 * @method fscript_register_const_int
 * 注册整数常量。
 * @param {const char*} name 常量名。
 * @param {int} value 数据。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t fscript_register_const_int(const char* name, int value);
```

* 注册浮点数常量(只是为了使用方便而提供的包装函数)

```c
/**
 * @method fscript_register_const_double
 * 注册浮点数常量。
 * @param {const char*} name 常量名。
 * @param {double} value 数据。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t fscript_register_const_double(const char* name, double value);
```

示例：

```c
fscript_register_const_double("PI", 3.1415926);
```

* 在 fscript 中引用常量。引用常量时需要加 fconsts. 前缀。

示例：

```js
print(fconsts.PI)
```

## 9. 扩展模块

* [字符串扩展模块](fscript_str.md)
* [位操作扩展模块](fscript_bits.md)
* [数学扩展模块](fscript_math.md)
* [CRC 扩展模块](fscript_crc.md)
* [value 扩展模块](fscript_value.md)
* [widget 扩展模块](fscript_widget.md)
* [object 扩展模块](fscript_object.md)
* [文件系统扩展模块](fscript_fs.md)
* [app\_conf 扩展模块](fscript_app_conf.md)
* [日期和时间扩展模块](fscript_date_time.md)
* [JSON 扩展模块](fscript_json.md)
* [大端小端扩展模块](fscript_endian.md)
* [istream 扩展模块](fscript_istream.md)
* [ostream 扩展模块](fscript_ostream.md)
* [iostream 扩展模块](fscript_iostream.md)
* [TCP/UDP 流扩展模块](fscript_iostream_inet.md)
* [文件流扩展模块](fscript_iostream_file.md)
* [串口流扩展模块](fscript_iostream_serial.md)
* [动态数组扩展模块](fscript_typed_array.md)
* [单一类型的动态数组扩展模块](fscript_typed_array.md)
* [写缓冲区扩展模块](fscript_wbuffer.md)
* [读缓冲区扩展模块](fscript_rbuffer.md)
* [module 扩展模块](fscript_module.md)

## 9. 更多示例

请参考 tests/fscripts 目录。
