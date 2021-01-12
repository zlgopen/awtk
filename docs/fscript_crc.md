## CRC 函数

Cyclic redundancy check

### 1.crc16 

> crc16 函数。
----------------------------

#### 原型

```
crc16(str)
crc16(data, size)
crc16(binary)
```

#### 示例

```
crc16("hello")
```

### 2.crc32

> crc32 函数。
----------------------------

#### 原型

```
crc32(str)
crc32(data, size)
crc32(binary)
```

#### 示例

```
crc32("hello")
```

### 3.cksum

> cksum 函数。
----------------------------

#### 原型

```
cksum(str)
cksum(data, size)
cksum(binary)
```

#### 示例

```
cksum("hello")
```

### 4. 更多示例

```js
//test string
assert(crc16("123") == 40116)
assert(crc32("123") == u32(2286445522))
assert(cksum("123") == u16(65385))

//test binary
a=typed_array_create("i8", 10) 
assert(typed_array_push(a, 1,2,3) == 3)
print(crc16(a.data, a.size))
print(crc32(a.data, a.size))
print(cksum(a.data, a.size))
unset(a);

//test binary
assert(file_write("test.bin", "hello", 5)) 
assert(file_exist("test.bin"))
a = file_read_binary("test.bin")
assert(crc16(a) == 13501)
assert(crc32(a) == 907060870)
assert(cksum(a)==65003)
assert(file_remove("test.bin"))
unset(a)
```