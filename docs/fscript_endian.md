## 大端小端扩展函数

### 1.is\_little
### 2.htonl
### 3.ntohl
### 4.htons
### 5.ntohs
### 6.htonf
### 7.ntohf
### 8.htonll
### 9.ntohll

### 更多示例

```js
assert(is_little())
assert(htonl(0x11223344) == 0x44332211)
assert(ntohl(0x11223344) == 0x44332211)
assert(htons(0x1122) == 0x2211)
assert(ntohs(0x1122) == 0x2211)
```

