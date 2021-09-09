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

### 4.file\_write\_append

> 写入文件尾部（追加内容）。
----------------------------

#### 原型

```
file_write_append(filename, str) => bool
file_write_append(filename, binary) => bool
file_write_append(filename, data, size) => bool
```

### 5.file\_exist

> 检查文件是否存在。
----------------------------

#### 原型

```
file_exist(filename) => bool
```

### 6.file\_get\_size

> 获取文件大小。
----------------------------

#### 原型

```
file_get_size(filename) => int32_t
```

### 7.file\_remove

> 删除指定的文件。
----------------------------

#### 原型

```
file_remove(filename) => bool
```

### 8.file\_rename

> 文件重命名。
----------------------------

#### 原型

```
file_rename(filename, new_name) => bool
```

### 9.file\_copy

> 文件拷贝。
----------------------------

#### 原型

```
file_copy(filename, dst_name) => bool
```

### 10.file\_stat

> 获取文件信息。
----------------------------

#### 原型

```
file_stat(filename) => object
```

返回的文件信息对象包含以下属性：

| 属性名称    | 属性类型 | 说明                   |
| ----------- | -------- | ---------------------- |
| dev         | uint32_t | 文件系统设备           |
| ino         | uint16_t | 文件序列号             |
| mode        | uint16_t | 文件类型和权限信息     |
| nlink       | int16_t  | 文件的符号链接数量     |
| uid         | int16_t  | 用户id                 |
| gid         | int16_t  | 组id                   |
| rdev        | uint32_t | 设备id                 |
| size        | uint64_t | 文件大小（单位：字节） |
| atime       | uint64_t | 最后访问时间           |
| mtime       | uint64_t | 最后修改时间           |
| ctime       | uint64_t | 创建时间               |
| is_dir      | bool_t   | 是否为目录             |
| is_link     | bool_t   | 是否为链接             |
| is_reg_file | bool_t   | 是否为普通文件         |

### 11.path\_create

> 创建目录(递归创建不存在的父目录)。
----------------------------

#### 原型

```
path_create(path) => bool
```

### 12.path\_remove

> 删除目录(递归创建子目录和文件)。
----------------------------

#### 原型

```
path_remove(path) => bool
```

### 13.path\_exist

> 检查指定的目录是否存在。
----------------------------

#### 原型

```
path_exist(path) => bool
```

### 14.path\_rename

> 目录重命名。
----------------------------

#### 原型

```
path_rename(path, new_path) => bool
```

### 15.path\_list

> 获取指定目录下的子目录/文件列表。
----------------------------

#### 原型

```
path_list(path) => array
```

返回的列表数组中每个元素都是一个 object 对象，每个 object 对象中包含以下属性：

| 属性名称    | 属性类型 | 说明           |
| ----------- | -------- | -------------- |
| is_dir      | bool_t   | 是否是目录     |
| is_link     | bool_t   | 是否是符号链接 |
| is_reg_file | bool_t   | 是否是普通文件 |
| name        | str      | 文件名称       |

### 16.path\_get\_exe

> 获取可执行文件所在目录。
----------------------------

#### 原型

```
path_get_exe() => str
```

### 17.path\_get\_cwd

> 获取当前所在目录。
----------------------------

#### 原型

```
path_get_cwd() => str
```

### 18.path\_get\_temp

> 获取临时目录的路径。
----------------------------

#### 原型

```
path_get_temp() => str
```

### 19.path\_get\_app\_root

> 获取应用程序根目录的路径。
----------------------------

#### 原型

```
path_get_app_root() => str
```

### 20.path\_get\_user\_storage\_root

> 获取用户目录或用户可以存储数据的目录的路径。
----------------------------

#### 原型

```
path_get_user_storage_root() => str
```

### 21.fs\_get\_disk\_info

> 获取文件系统信息。
----------------------------

#### 原型

```
fs_get_disk_info(path) => object
```

返回的文件系统信息对象包含以下属性：

| 属性名称    | 属性类型 | 说明             |
| ----------- | -------- | ---------------- |
| free_kb     | int32_t  | 空闲空间大小(KB) |
| total_kb    | int32_t  | 总共空间大小(KB) |

> 备注：该功能需适配 AWTK 通用文件系统（fs.h）中的 fs_get_disk_info 接口，目前 PC 上暂不支持。

### 更多示例

```js
assert(file_write("test.txt", "hello"))
assert(file_exist("test.txt"))
assert(file_get_size("test.txt")==5)
assert(file_read_text("test.txt") == "hello")
assert(file_write_append("test.txt","world"))
assert(file_read_text("test.txt") == "helloworld")
assert(file_rename("test.txt","rename.txt"))
assert(!file_exist("test.txt"))
assert(file_read_text("rename.txt") == "helloworld")
assert(file_copy("rename.txt","copy.txt"))
assert(file_read_text("copy.txt") == "helloworld")
assert(file_remove("copy.txt"))
assert(!file_exist("copy.txt"))

a = file_stat("rename.txt")
print(join(": ","dev",a.dev))
print(join(": ","ino",a.ino))
print(join(": ","mode",a.mode))
print(join(": ","nlink",a.nlink))
print(join(": ","uid",a.uid))
print(join(": ","rdev",a.rdev))
print(join(": ","size",a.size))
print(join(": ","atime",a.atime))
print(join(": ","mtime",a.mtime))
print(join(": ","ctime",a.ctime))
print(join(": ","is_dir",a.is_dir))
print(join(": ","is_link",a.is_link))
print(join(": ","is_reg_file",a.is_reg_file))
assert(file_remove("rename.txt"))
assert(!file_exist("rename.txt"))

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
assert(path_rename("a/b","a/bbb"))
assert(!path_exist("a/b"))
assert(path_exist("a/bbb"))
assert(file_write("a/test.txt", "hello"))
a = path_list("a")
assert(a.size==2)
b = array_get(a,0)
c = array_get(a,1)
assert(b.name=="bbb")
assert(c.name=="test.txt")
assert(path_remove("a"))
assert(!path_exist("a"))

print(path_get_exe())
print(path_get_cwd())
print(path_get_temp())
print(path_get_app_root())
print(path_get_user_storage_root())

unset(a)
unset(b)
unset(c)
```

