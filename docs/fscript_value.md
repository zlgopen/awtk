
## value 扩展函数

value 相关的辅助函数。

### 1. value_is_valid

> 判断 value 是否有效。
----------------------------

#### 原型

```
value_is_valid(v) => bool
```

#### 示例

```
a = object_create()
print(value_is_valid(a))
```

### 2. value_is_null

> 判断 value 是否为 null。
----------------------------

#### 原型

```
value_is_null(v) => bool
```

#### 示例

```
a = object_create()
print(value_is_null(a))
```

### 3. value_get_binary_data

> 获取 binary 的数据指针。
----------------------------

#### 原型

```
value_get_binary_data(v) => pointer_t
```

#### 示例

```
print(value_get_binary_data(a))
```

### 4. value_get_binary_size

> 获取 binary 的数据长度。
----------------------------

#### 原型

```
value_get_binary_size(v) => uint32_t
```

#### 示例

```
a = file_read_binary("test.bin")
assert(value_get_binary_size(a) == 5)
```
