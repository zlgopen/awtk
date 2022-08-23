## 字符串扩展函数

### 1.ulen

> 获取 unicode 字符个数。
----------------------------

#### 原型

```js
ulen(str) => int
```

#### 示例

```js
print(ulen('致远电子'))
```

### 2.char\_at

> 获取指定位置的字符。
----------------------------

#### 原型

```js
char_at(str, index) => str
```

> index 为负数时，从后往前取。比如-1 表示最后一个元素。

#### 示例

```js
print(char_at('致远电子', 1))
print(char_at('ZLG', 1))
```

### 3.index\_of

> 查找指定子串的位置，如果没有找到返回-1。
----------------------------

#### 原型

```js
index_of(str, substr) => int
```

#### 示例

```js
print(index_of('hello AWTK and AWTK', 'AWTK'))
```

### 4.last\_index\_of

> 反向查找指定子串的位置，如果没有找到返回-1。
----------------------------

#### 原型

```js
last_index_of(str, substr) => int
```

#### 示例

```js
print(last_index_of('hello AWTK and AWTK', 'AWTK'))
```

### 5.trim\_left

> 去掉字符串左边的空白字符。
----------------------------

#### 原型

```js
trim_left(str) => str
```

#### 示例

```js
print(trim_left('   AWTK  '))
```

### 6.trim\_right

> 去掉字符串右边的空白字符。
----------------------------

#### 原型

```js
trim_right(str) => str
```

#### 示例

```js
print(trim_right('   AWTK  ') + "1234")
```

### 7.totitle

> 将字符串中单词的首字母转换成大写。

----------------------------

#### 原型

```js
totitle(str) => str
```

#### 示例

```js
print(totitle("it is ok!"))
```

### 8.prompt

> 在控制台提示用户输入字符串（仅当定义了 HAS\_STIOD 时有效）。
----------------------------

#### 原型

```js
prompt(tips) => str
```

#### 示例

```js
var a = prompt('input a number:');
print(int(a)+100);
```

### 9.usubstr

> 取子字符串(unicode)。
----------------------------

#### 原型

```js
usubstr(str, start, end) => str
```

> * 不包括end
> * start 为负数时，从后往前取。比如-1 表示最后一个元素。
> * end 为负数时，从后往前取。比如-1 表示最后一个元素。
> * end 省略表示到最后全部元素。

#### 示例

```js
var a="致远电子"
assert(usubstr(a) == '致远电子');
assert(usubstr(a,0) == '致远电子');
assert(usubstr(a,0,1) == '致');
assert(usubstr(a,0,-1) == '致远电');
assert(usubstr(a,-2,-1) == '电');
assert(usubstr(a, -3) == '远电子');
```

### 10.text\_reverse

> 字符串反向
----------------------------

#### 原型

```js
text_reverse(str) => str
```

#### 示例

```js
print(text_reverse("hello"));
```

### 11.text\_count

> 统计子串出现的次数。
----------------------------

#### 原型

```js
text_count(str, substr) => int
```

#### 示例

```js
print(text_count("hello zlg!", "zlg"));
```

### 12.str\_len

> 计算字符串的长度。
----------------------------

#### 原型

```js
str_len(str) => uint32
```

#### 示例

```js
print(str_len("hello"));
```

### 13.str\_is\_empty

> 判断字符串是否为空。
----------------------------

#### 原型

```js
str_is_empty(str) => bool
```

#### 示例

```js
print(str_is_empty(""));
print(str_is_empty("zlg"));
```

### 14.str\_append

> 追加字符串。
----------------------------

#### 原型

```js
str_append(str, append_str) => str
```

#### 示例

```js
print(str_append("hello ", "zlg"));
```

### 15.char\_at\_first

> 获取首字符。
----------------------------

#### 原型

```js
char_at_first(str) => str
```

#### 示例

```js
print(char_at_first("hello"));
```

### 16.char\_at\_last

> 获取尾字符。
----------------------------

#### 原型

```js
char_at_last(str) => str
```

#### 示例

```js
print(char_at_last("hello"));
```

### 17.char\_at\_random

> 获取随机位置的字符。
----------------------------

#### 原型

```js
char_at_random(str) => str
```

#### 示例

```js
print(char_at_random("hello"));
```
