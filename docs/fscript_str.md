## 字符串扩展函数

### 1. ulen

> 获取 unicode 字符个数。
----------------------------

#### 原型

```
ulen(str) => int
```

#### 示例

```
print(ulen('致远电子'))
```

### 2. char\_at

> 获取指定位置的字符。
----------------------------

#### 原型

```
char_at(str, index) => str
```

> index 为负数时，从后往前取。比如-1 表示最后一个元素。

#### 示例

```
print(char_at('致远电子', 1))
print(char_at('ZLG', 1))
```

### 3. index\_of

> 查找指定子串的位置，如果没有找到返回-1。
----------------------------

#### 原型

```
index_of(str, substr) => int
```

#### 示例

```
print(index_of('hello AWTK and AWTK', 'AWTK'))
```

### 4. last\_index\_of

> 反向查找指定子串的位置，如果没有找到返回-1。
----------------------------

#### 原型

```
last_index_of(str, substr) => int
```

#### 示例

```
print(last_index_of('hello AWTK and AWTK', 'AWTK'))
```

### 5. trim\_left

> 去掉字符串左边的空白字符。
----------------------------

#### 原型

```
trim_left(str) => str
```

#### 示例

```
print(trim_left('   AWTK  '))
```

### 6. trim\_right

> 去掉字符串右边的空白字符。
----------------------------

#### 原型

```
trim_right(str) => str
```

#### 示例

```
print(trim_right('   AWTK  ') + "1234")
```

### 7.totitle

> 将字符串中单词的首字母转换成大写。

----------------------------

#### 原型

```
totitle(str) => str
```

#### 示例

```
print(totitle("it is ok!"))
```

### 8.prompt

> 在控制台提示用户输入字符串。

> 仅当定义了 HAS\_STIOD 时有效。
----------------------------

#### 原型

```
prompt(tips) => str
```

#### 示例

```
a = prompt('input a number:');
print(int(a)+100);
```

### 9. usubstr

> 取子字符串(unicode)。
----------------------------

#### 原型

```
usubstr(str, start, end) => str
```

> * 不包括end
> * start 为负数时，从后往前取。比如-1 表示最后一个元素。
> * end 为负数时，从后往前取。比如-1 表示最后一个元素。
> * end 省略表示到最后全部元素。


### 10. text\_reverse

> 字符串反向
----------------------------

#### 原型

```
text_reverse(str) => str
```

### 11. text\_count

> 统计子串出现的次数。
----------------------------

#### 原型

```
text_count(str, substr) => int
```
