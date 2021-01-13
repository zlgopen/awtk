### 1.file\_read\_text
### 2.file\_read\_binary
### 3.file\_write
### 4.file\_exist
### 5.file\_get\_size
### 6.file\_remove
### 7.path\_create
### 8.path\_remove
### 9.path\_exist
### 10.path\_get\_temp
### 11.path\_get\_app\_root
### 12.path\_get\_user\_storage\_root

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

