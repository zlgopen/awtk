## tk\_sha256\_t
### 概述
sha256
----------------------------------
### 函数
<p id="tk_sha256_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_sha256_t_tk_sha256">tk\_sha256</a> | 计算数据的sha256哈希值。 |
| <a href="#tk_sha256_t_tk_sha256_done">tk\_sha256\_done</a> | 完成计算。 |
| <a href="#tk_sha256_t_tk_sha256_file">tk\_sha256\_file</a> | 计算文件的sha256哈希值。 |
| <a href="#tk_sha256_t_tk_sha256_hash">tk\_sha256\_hash</a> | 更新数据。 |
| <a href="#tk_sha256_t_tk_sha256_init">tk\_sha256\_init</a> | 初始化。 |
#### tk\_sha256 函数
-----------------------

* 函数功能：

> <p id="tk_sha256_t_tk_sha256">计算数据的sha256哈希值。

* 函数原型：

```
ret_t tk_sha256 (const void* data, uint32_t len, str_t* hash);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| data | const void* | 数据。 |
| len | uint32\_t | 数据长度。 |
| hash | str\_t* | 用于返回计算结果。 |
#### tk\_sha256\_done 函数
-----------------------

* 函数功能：

> <p id="tk_sha256_t_tk_sha256_done">完成计算。

* 函数原型：

```
ret_t tk_sha256_done (tk_sha256_t* sha256, uint8_t* hash);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| sha256 | tk\_sha256\_t* | sha256对象。 |
| hash | uint8\_t* | 用于返回计算结果。 |
#### tk\_sha256\_file 函数
-----------------------

* 函数功能：

> <p id="tk_sha256_t_tk_sha256_file">计算文件的sha256哈希值。

* 函数原型：

```
ret_t tk_sha256_file (const char* filename, uint32_t block_size, str_t* hash);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| filename | const char* | 文件名。 |
| block\_size | uint32\_t | 数据块长度。 |
| hash | str\_t* | 用于返回计算结果。 |
#### tk\_sha256\_hash 函数
-----------------------

* 函数功能：

> <p id="tk_sha256_t_tk_sha256_hash">更新数据。

* 函数原型：

```
ret_t tk_sha256_hash (tk_sha256_t* sha256, const uint8_t* data, uint32_t len);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| sha256 | tk\_sha256\_t* | sha256对象。 |
| data | const uint8\_t* | 数据。 |
| len | uint32\_t | 数据长度。 |
#### tk\_sha256\_init 函数
-----------------------

* 函数功能：

> <p id="tk_sha256_t_tk_sha256_init">初始化。

* 函数原型：

```
ret_t tk_sha256_init (tk_sha256_t* sha256);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| sha256 | tk\_sha256\_t* | sha256对象。 |
