## 读缓冲区扩展函数

### 1.rbuffer\_create
### 2.rbuffer\_skip
### 3.rbuffer\_rewind
### 4.rbuffer\_read\_uint8
### 5.rbuffer\_read\_uint16
### 6.rbuffer\_read\_uint32
### 7.rbuffer\_read\_uint64
### 8.rbuffer\_read\_int8
### 9.rbuffer\_read\_int16
### 10.rbuffer\_read\_int32
### 11.rbuffer\_read\_int64
### 12.rbuffer\_read\_float
### 13.rbuffer\_read\_double
### 14.rbuffer\_read\_string
### 15.rbuffer\_read\_binary
### 16.rbuffer\_get\_data
### 17.rbuffer\_get\_cursor
### 18.rbuffer\_get\_capacity

### 更多示例

```js
...

b=rbuffer_create(wbuffer_get_data(a), wbuffer_get_cursor(a))

assert(rbuffer_get_cursor(b)==0)
assert(rbuffer_read_int8(b)== -10)

assert(rbuffer_get_cursor(b)==1)
assert(rbuffer_read_int16(b)==-1122)

assert(rbuffer_get_cursor(b)==3)
assert(rbuffer_read_int32(b)==-11223344)

assert(rbuffer_get_cursor(b)==7)
assert(rbuffer_read_int64(b)==-1122334455667788)

assert(rbuffer_get_cursor(b)==15)
assert(rbuffer_read_string(b)=="hello")
assert(rbuffer_get_cursor(b)==21)
c = rbuffer_read_binary(b, 6)
assert(rbuffer_get_cursor(b)==27)

wbuffer_rewind(a)
assert(wbuffer_get_cursor(a)==0)
wbuffer_write_binary(a, c)
assert(wbuffer_get_cursor(a)==6)

unset(a)
unset(b)
```
