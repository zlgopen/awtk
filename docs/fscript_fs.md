##  文件系统扩展函数

### 1.file\_read\_text

> 读取文本文件。
----------------------------

#### 原型

```
file_read_text(filename) => str
```

### 2.file\_read\_binary

> 读取二进制文件。
----------------------------

#### 原型

```
file_read_binary(filename) => binary
```

### 3.file\_write

> 写入文件。
----------------------------

#### 原型

```
file_write(filename, str) => bool
file_write(filename, binary) => bool
file_write(filename, data, size) => bool
```

### 4.file\_exist

> 检查文件是否存在。
----------------------------

#### 原型

```
file_exist(filename) => bool
```

### 5.file\_get\_size

> 获取文件大小。
----------------------------

#### 原型

```
file_get_size(filename) => int32_t
```

### 6.file\_remove

> 删除指定的文件。
----------------------------

#### 原型

```
file_remove(filename) => bool
```

### 7.path\_create

> 创建目录(递归创建不存在的父目录)。
----------------------------

#### 原型

```
path_create(path) => bool
```

### 8.path\_remove

> 删除目录(递归创建子目录和文件)。
----------------------------

#### 原型

```
path_remove(path) => bool
```

### 9.path\_exist

> 检查指定的目录是否存在。
----------------------------

#### 原型

```
path_exist(path) => bool
```

### 10.path\_get\_temp

> 获取临时目录的路径。
----------------------------

#### 原型

```
path_get_temp() => str
```

### 11.path\_get\_app\_root

> 获取应用程序根目录的路径。
----------------------------

#### 原型

```
path_get_app_root() => str
```

### 12.path\_get\_user\_storage\_root

> 获取用户目录或用户可以存储数据的目录的路径。
----------------------------

#### 原型

```
path_get_user_storage_root() => str
```

### 更多示例

```js
assert(file_write("test.txt", "hello"))
assert(file_exist("test.txt"))
assert(file_get_size("test.txt")==5)
assert(file_read_text("test.txt") == "hello")
assert(file_remove("test.txt"))
assert(!file_exist("test.txt"))

assert(file_write("test.bin", "hello", 5))
assert(file_exist("test.bin"))
assert(file_get_size("test.bin")==5)
a = file_read_binary("test.bin")
assert(value_get_binary_size(a) == 5)
assert(file_remove("test.bin"))
assert(!file_exist("test.bin"))

assert(file_write("test.bin", a))
assert(file_exist("test.bin"))
assert(file_get_size("test.bin")==5)
assert(file_remove("test.bin"))

assert(path_create("a/b/c"))
assert(path_exist("a/b/c"))
assert(path_remove("a/b/c"))
assert(!path_exist("a/b/c"))

print(path_get_temp())
print(path_get_app_root())
print(path_get_user_storage_root())
```

