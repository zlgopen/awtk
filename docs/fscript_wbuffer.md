## 写缓冲扩展函数

### 1.wbuffer\_create
### 2.wbuffer\_attach
### 3.wbuffer\_skip
### 4.wbuffer\_rewind
### 5.wbuffer\_write\_uint8
### 6.wbuffer\_write\_uint16
### 7.wbuffer\_write\_uint32
### 8.wbuffer\_write\_uint64
### 9.wbuffer\_write\_int8
### 10.wbuffer\_write\_int16
### 11.wbuffer\_write\_int32
### 12.wbuffer\_write\_int64
### 13.wbuffer\_write\_float
### 14.wbuffer\_write\_double
### 15.wbuffer\_write\_string
### 16.wbuffer\_write\_binary
### 17.wbuffer\_get\_data
### 18.wbuffer\_get\_cursor
### 19.wbuffer\_get\_capacity

### 更多示例

```js
a=wbuffer_create()
wbuffer_write_int8(a, -10) 
assert(wbuffer_get_cursor(a)==1)
wbuffer_write_int16(a, -1122) 
assert(wbuffer_get_cursor(a)==3)
wbuffer_write_int32(a, -11223344) 
assert(wbuffer_get_cursor(a)==7)
wbuffer_write_int64(a, -1122334455667788)
assert(wbuffer_get_cursor(a)==15)

wbuffer_write_string(a, "hello")
assert(wbuffer_get_cursor(a)==21)

wbuffer_write_binary(a, "wolrd", 6)
assert(wbuffer_get_cursor(a)==27)
```

