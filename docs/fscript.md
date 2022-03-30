# fscript 脚本引擎

## 1. 介绍

fscript 是一个极简的脚本引擎，借鉴了函数语言中一些思路，主要用于低端嵌入式系统，让用户轻松扩展现有系统，而不需要重新编译和下载固件。

* 特色：
  * 小内存。最低开销小于 400 字节。
  * 小巧。核心代码 1500 行，基本扩展函数 1000 行。
  * 灵活。支持多条语句、函数嵌套调用和变量定义。
  * 简单。熟悉任何一种编程语言的人，5 分钟内即可学会。
  * 强大。超过 60 个内置函数，支持类似 C 语言的条件语句、循环语句和注释方式，支持复杂的表达式，支持用 C 语言扩展函数。

> keil -O1 编译结果：

```  
      Code (inc. data)   RO Data    RW Data    ZI Data      Debug   Object Name
     10380       1580       1909          4          0      86893   fscript.o
```

实际内存使用与场景有关，请参考 [内存需求评估](fscript_ram_req.md)

## 2. 示例

```c
print("hello fscript")
```

* 在 PC 上测试运行：

如：
```
./bin/runFScript 'print("hello fscript")'
```

* 在 PC 上测试运行指定文件：

> 第一个命令行参数以 @ 开头表示它是一个脚本文件，读取其内容执行。

如：
```
./bin/runFScript @tests/fscripts/demo_while1.fs
```

## 3. 语法

### 数据类型

* 浮点数类型 (double)。
* 整数类型 (int32)。支持十进制、二进制和十六进制。
* 字符串类型 (string)。UTF-8 字符串，用英文双引号扩起来。
* 布尔类型 (bool)。标准取值为：true 和 false，非 0 的数值视为 true。
* 可以通过扩展实现混合数据类型的数组、固定类型的数组和 map。
* 可以通过类型转换函数得到各种基本类型。如 int8/int16/int32/int64/uint8/uint16/uint32/uintg64/float/double。

###  注释

格式：

```
//line comment
/*block comment*/
```

#### 示例

```
//this is line comment
/*this is block comment*/
```

### 函数调用

```
print(0xff)
print(100)
print(0b1010)
print("hello fscript")
print("hello", 123)
```

> 函数之间可以用空格、换行和英文分号分隔。

### 定义变量

* 通过 set 函数设置

如：

```
set(a, 123)
```

* 通过=赋值

如：

```
a = 123
b = "abc"
c = true
```

> 备注：在 fscript 脚本中定义的全局变量在执行完一段脚本后不会被清空，因此使用这些变量前通常需要初始化。

#### 变量名命名规则：

* 以字母开头，后面可用数字、英文下划线和英文点。

如：

```
age=123
name="fscript"
file_name="test.txt"
```

* 英文点"."可以用来访问对象的成员，如果对应的对象不存在则当作普通变量。

如：

```
msg.payload = "hello"
```

>在这里，如果对象 msg 存在，msg.payload 为 msg 对象的 payload 成员，否则 msg.payload 则是一个普通变量。

* a、b、c 和 d 为快速访问变量，具有更快的访问速度，建议在循环中优先使用。

如：

```
a=0
b=0
while(a < 100) {
  a=a+1
  b=b+a
  print(a, b)
}
```

* global. 开头的变量为全局变量，可以在同一进程的多个脚本中共享数据（全局变量并非是好的做法，尽量避免使用）

如：

```
global.count=400
global.name="fscript"
```

### 获取变量

```
abc
get(abc)
```

> * 获取变量时，如果变量不存在，自动当成字符串处理，并打印获取变量失败的警告。
> * 如果不希望变量被当成字符串，可以加上$前缀，或者使用 get 函数。
> * fscript 不支持将函数返回值当做变量名使用并通过英文点"."访问子变量，比如 array_create().size，此时会将 .size 当做字符串处理。
>
> 注意：fscript 不提倡直接使用变量名当做字符串，如需要如果使用字符串常量，请用双引号包起来。

如：

```
./bin/runFScript 'print($abc)' 

get var abc failed
(null) 
result:true
cost: 112 us
```

```
./bin/runFScript 'print(abc)' 

get var abc failed
abc 
result:true
cost: 112 us
```

判断一个变量是否有效，需要使用 get 函数：

```
print("abc is valid? ", value_is_valid(get(abc)))
abc=123
print("abc is valid? ", value_is_valid(get(abc)))
print(get(abc) + 321)
```

### 函数嵌套调用

```
print(join(",", 1, 2, 3))
print(join(",",1,2,3,4), join(";",5,6,7,8))
```

### 提前返回 (return)

```
var a = 1;
if(a == 1) {
  print("return");
  return;
} else {
  print("end");
}
```

### 条件执行

* 语句方式

```
a=random(1, 100)
b=random(1, 100)

print("a is ", a);
print("b is ", b);

if(a < b) {
  print(a, "<", b)
} else {
  print(a, ">=", b)
}
```

else if

```
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

```
if(true, print("a"), print("b"))
```

### while 循环

* 语法

```
while（条件） {

}
```
* 示例

```
a=0
b=0

while(a < 100) {
  a=a+1
  b=b+a
  print(a, b)
}
```

### until 循环

> 重复执行直到条件成立

* 语法

```
until(条件）{

}
```
* 示例

```
a=0
b=0

until(a >= 100) {
  a=a+1
  b=b+a
  print(a, b)
}
```

### for 循环

* 语法

```
for（初始化表达式；结束条件；迭代表达式） {

}
```
* 示例

```
for(var a = 0; a < 100; a = a + 1) {
  print(a)
}
```

### for_in 循环

> 用于遍历数组对象

* 语法

```js
for_in（迭代变量, 数组对象） {

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

### repeat 循环

* 语法

```js
repeat(迭代变量, 开始值, 结束值, 增量） {

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

### repeat_times 循环

* 语法

```js
repeat_times(重复次数） {

}
```
* 示例

```js
repeat_times(100) {
  print("hello")
}
```

### 表达式

为了确保优先级正确，尽量用括号明确优先级。

#### 支持的操作符

* 四则运算：+ - * / %
* 逻辑运算：|| && !
* 位运算：| & ~
* 比较运算：< <= > >= == 
* 条件运算：(p) ? a : b

> 对于加法运算 (+)，参数中有字符串时，进行字符串连接。

#### 示例

```
a = 10
b = 20
```

```
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

####  assert

> 断言
----------------------------

##### 原型

```
assert(condition, crash_if_fail) => void
```

* crash\_if\_fail 可选。为 true 时，调用系统的 assert，否则只是打印警告。

##### 示例

```
assert(1<2)
```

####  eval

> 执行另外一段脚本。
----------------------------

##### 原型

```
eval(condition) => value
```

##### 示例

```
eval("1+2")
```

####  has_error

> 检查是否之前遇到错误
----------------------------

##### 原型

```
has_error() => bool
```

##### 示例

```
print(has_error())
```

#### get_last_error
> 获取前一个错误
----------------------------

##### 原型

```
get_last_error() => object | null
```

如果有错，返回对象，其成员如下：

* message 错误消息
* code 错误码
* line 出错的行
* col 出错的列

没有错误，返回无效值。

##### 示例

```
print(len())
var error = get_last_error();
print("XXX:", error.message, error.code, error.line, error.col);
```

####  clear_error

> 清除之前遇到错误
----------------------------

##### 原型

```
clear_error() => void
```

##### 示例

```
clear_error()
```

####  print

> 打印调试信息到控制台。
----------------------------

##### 原型

```
print(str,...) => void
```

##### 示例

```
print("hello fscript")
print("hello", "fscript")
print(1)
print(true)

a=100
b=200
print(a+b)
print(join(",", a, b))
```

#### sleep_ms

> 睡眠指定时间长（毫秒）
----------------------------

##### 原型

```
sleep_ms(time) => bool
```

##### 示例

```
sleep_ms(100);
```

#### set

> 设置变量的值。建议使用=赋值。

----------------------------

##### 原型

```
set(var, value) => bool
```

##### 示例

```
set(a, 1)
a=1
```

#### unset

> 清除变量。

对于不再使用的变量，特别是对象和字符串，请调用本函数清除掉。

当一个变量重复赋值时，会自动清除之前的数据。

----------------------------

##### 原型

```
unset(var) => void
```

##### 示例

```
unset(a)
```

#### int

> 转换为整数类型。

----------------------------

##### 原型

```
int(var) => int32_t
```

##### 示例

```
int("123")
```

#### i8

> 转换为 int8_t 类型。

----------------------------

##### 原型

```
i8(var) => int8_t
```

##### 示例

```
i8("123")
```

#### i16

> 转换为 int16_t 类型。

----------------------------

##### 原型

```
i16(var) => int16_t
```

##### 示例

```
i16("123")
```

#### i32

> 转换为 int32_t 类型。

----------------------------

##### 原型

```
i32(var) => int32_t
```

##### 示例

```
i32("123")
```

#### i64

> 转换为 int64_t 类型。

----------------------------

##### 原型

```
i64(var) => int64_t
```

##### 示例

```
i64("123")
```

#### u8

> 转换为 unt8_t 类型。

----------------------------

##### 原型

```
u8(var) => uint8_t
```

##### 示例

```
u8("123")
```

#### u16

> 转换为 unt16_t 类型。

----------------------------

##### 原型

```
u16(var) => value(uint16)
```

##### 示例

```
u16("123")
```

#### u32

> 转换为 unt32_t 类型。

----------------------------

##### 原型

```
u32(var) => uint32_t
```

##### 示例

```
u32("123")
```

#### u64

> 转换为 unt64_t 类型。

----------------------------

##### 原型

```
u64(var) => uint64_t
```

##### 示例

```
u64("123")
```

#### f32

> 转换为单精度类型。

----------------------------

##### 原型

```
f32(var) => float
```

##### 示例

```
f32("123")
```
#### float

> 转换为浮点数类型。
----------------------------

##### 原型

```
float(var) => double
```

##### 示例

```
float("123")
```

#### str

> 转换为字符串类型。
----------------------------

##### 原型

```
str(var [,force_pointer_as_str]) => str
```

> force\_pointer\_as\_str 如果输入参数是 POINTER 类型，是否将强制转换成字符串。

##### 示例

```
str(int(123))
str(msg.payload, true)
```

#### iformat

> 对整数进行格式化（生成的字符串长度不超过 63)
----------------------------

##### 原型

```
iformat(format, value) => str
```

##### 示例

```
iformat("hello:%d", 123)
```

#### fformat

> 对浮点数进行格式化（生成的字符串长度不超过 63)
----------------------------

##### 原型

```
fformat(format, value) => str
```

##### 示例

```
fformat("hello:%lf", 123)
```

#### exec

> 执行 object 里的一个命令。
----------------------------

##### 原型

```
exec(cmd, arg) => bool
```

##### 示例

```
exec("clear", "all")
```

> 具体功能与 object 的实现有关。

### 4.2 字符串函数

#### join

> 将多个变量用指定的分隔符拼接起来，形成一个字符串。

----------------------------

##### 原型

```
join(seperator, s1, s2, s3...) => str
```

##### 示例

```
join(",", 1, 2, 3, "abc")
```

#### one_of

> 从字符串数组中取出第 N 个字符串

----------------------------

##### 原型

```
one_of(str_array, index, sep) => str
```
> sep 为分隔符，默认为英文分号 (;)。

##### 示例

```
one_of("aa;bb;cc", 0) # => aa
one_of("aa;bb;cc", 1) # => bb
one_of("aa;bb;cc", 2) # => cc

one_of("aa.bb.cc", 0, ".") # ==> aa

```

#### len

> 取字符串/数组的长度。

----------------------------

##### 原型

```
len(str) => uint32_t
```

##### 示例

```
len("abc")
```

#### toupper

> 将字符串转换成大写。

----------------------------

##### 原型

```
toupper(str) => str
```

##### 示例

```
touppper("abc")
```

#### tolower

> 将字符串转换成小写。

----------------------------

##### 原型

```
tolower(str) => str
```

##### 示例

```
tolower("ABC")
```

#### trim

> 去掉字符串两端的空白字符串。
----------------------------

##### 原型

```
trim(str) => str
```

##### 示例

```
trm("  abc  ")
```

#### substr

> 取子字符串。
----------------------------

##### 原型

```
substr(str, from, len) => str
```

##### 示例

```
substr("abcd", 1, 2)
```

#### replace

> 替换子字符串。
----------------------------

##### 原型

```
replace(str, old, new) => str
```

##### 示例

```
replace("ab cd", "ab", "hello")
```

#### contains

> 检查是否包含指定的子字符串。
----------------------------

##### 原型

```
contains(str, substr) => bool
```

##### 示例

```
contains("ab cd", "ab")
```

### 4.3 运算函数

#### sum

> 对多个数值类型的参数求和。
----------------------------

##### 原型

```
sum(n1,n2...)
+(n1,n2...)
n1+n2
```

##### 示例

```
print(sum(1, 2, 3))
```

#### sub

> 计算两个数之差。
----------------------------

##### 原型

```
sub(n1,n2)
或
n1-n2
```

##### 示例

```
print(sub(2, 1))
print(2 - 1)
```

#### mul

> 计算两个数之积。
----------------------------

##### 原型

```
mul(n1,n2)
或
n1*n2
```

##### 示例

```
print(mul(2, 1))
print(2 * 1)
```

#### div

> 计算两个数之商。
----------------------------

##### 原型

```
div(n1,n2)
或
n1/n2
```

##### 示例

```
print(div(2, 1))
print(2/1)
```

#### %

> 计算两个数的模。
----------------------------

##### 原型

```
n1%n2
```

##### 示例

```
print(23%7)
```

#### and

> 逻辑与运算。
----------------------------

##### 原型

```
and(n1,n2)
n1 && n2
```

##### 示例

```
print(true && false)
print(true && true)
```

#### or

> 逻辑或运算。
----------------------------

##### 原型

```
or(n1,n2)
n1 || n2
```

##### 示例

```
print(a || b)
```

#### not

> 逻辑非运算。
----------------------------

##### 原型

```
not(n1)
!(n1)
```

##### 示例

```
print(not(true))
print(!(false))
```

### 比较函数

> 支持字符串比较。

#### <
----------------------------

> 小于。

##### 原型

```
<(a,b)
less(a,b)
a<b
```

#### <=

> 小于等于。
----------------------------

##### 原型

```
<=(a,b)
le(a,b)
a<=b
```

#### >

> 大于。
----------------------------

##### 原型

```
>(a,b)
great(a,b)
a>b
```

#### >=

> 大于等于。
----------------------------

##### 原型

```
>=(a,b)
ge(a,b)
a>=b
```

#### ==

> 等于。
----------------------------

##### 原型

```
==(a,b)
eq(a,b)
a==b
```

#### !=

> 不等于。
----------------------------

##### 原型

```
!=(a,b)
a != b
```

### 4.4 数学函数

#### random 

> 产生随机数。
----------------------------

##### 原型

```
random() => int
random(min, max) => int
```

#### round 

> 四舍五入。
----------------------------

##### 原型

```
round(v) => double
```

##### 示例

```
round(4.5)
```

#### floor 

> 返回小于等于指定数值的最大整数。
----------------------------

##### 原型

```
floor(v) => double
```

##### 示例

```
floor(4.5)
```

#### ceil 

> 返回大于等于指定数值的最小整数。
----------------------------

##### 原型

```
ceil(v) => double
```

##### 示例

```
ceil(4.5) 
```

#### abs 

> abs 函数。
----------------------------

##### 原型

```
abs(a) => double
```

##### 示例

```
abs(1)
```

#### min 

> min 函数。
----------------------------

##### 原型

```
min(a, b) => double
```

##### 示例

```
min(1, 2)
```

#### max 

> max 函数。
----------------------------

##### 原型

```
max(a, b) => double
```

##### 示例

```
max(1, 2) 
```

#### clamp 

> clamp 函数。
----------------------------

##### 原型

```
clamp(a, min, max) => double
```

##### 示例

```
clamp(2, 1, 3)
```

### 4.5 时间函数
> 已经移动到扩展模块。  

### 5. 自定义函数

#### 5.1 定义函数

```c
static ret_t func_foo(object_t* obj, fscript_args_t* args, value_t* v) {
  value_set_int(v, 123);

  return RET_OK;
}
```

#### 5.2 注册和使用私有函数

```c
  value_t v;
  object_t* obj = object_default_create();
  object_set_prop_pointer(obj, "function.foo", (void*)func_foo);

  fscript_eval(obj, "foo()", &v);
  value_reset(&v);

  OBJECT_UNREF(obj);
```

#### 5.3 注册全局函数

* 初始化时调用

```
fscript_register_func("foo", func_foo);

```

#### 5.4 定义脚本函数

* 函数定义

```
function foo1(v1, v2) {
  return v1 + v2; 
}
assert(foo1(100, 200) == 300)
```

* 使用 var 定义局部变量。

```
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

```
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

### 6. 性能测量与优化

runFScript 的第二个参数可以指定运行次数，方便测量某个函数的运行时间。

比如：

```
./bin/runFScript '123+234' 100000
```

通常设置变量和获取变量相对来说比较费时，为此 fscript 提供了 4 个寄存器变量：a,b,c,d。如果需要临时保存计算结果，优先使用寄存器变量。

比如：

```
 ./bin/runFScript 'set(a, 2); set(b, 3); print(a+b)'
```

### 7. 扩展模块

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
* [module扩展模块](fscript_module.md)

### 8. 更多示例

请参考 tests/fscripts 目录。
