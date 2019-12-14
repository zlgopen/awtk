## rom\_t
### 概述
rom_file接口。

> 主要用于给google pinyin输入提供兼容的文件系统API。

示例：

```c
char rbuff[100];
size_t len = 0;
const char* buff = "hello world";
rom_file_t* f = rom_fopen_buff((const uint8_t*)buff, sizeof(buff));
len = rom_fread(rbuff, 2, 1, f);
rom_fclose(f);
```
----------------------------------
### 函数
<p id="rom_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#rom_t_rom_fclose">rom\_fclose</a> | 关闭文件。 |
| <a href="#rom_t_rom_feof">rom\_feof</a> | 判断是否到文件末尾。 |
| <a href="#rom_t_rom_fopen">rom\_fopen</a> | 打开文件。 |
| <a href="#rom_t_rom_fopen_buff">rom\_fopen\_buff</a> | 打开文件。 |
| <a href="#rom_t_rom_fread">rom\_fread</a> | 读取文件。 |
| <a href="#rom_t_rom_fseek">rom\_fseek</a> | 设置文件当前位置。 |
| <a href="#rom_t_rom_ftell">rom\_ftell</a> | 返回文件当前位置。 |
| <a href="#rom_t_rom_fwrite">rom\_fwrite</a> | 写文件。 |
#### rom\_fclose 函数
-----------------------

* 函数功能：

> <p id="rom_t_rom_fclose">关闭文件。

* 函数原型：

```
ret_t rom_fclose (rom_file_t* f);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| f | rom\_file\_t* | rom\_file\_t对象。 |
#### rom\_feof 函数
-----------------------

* 函数功能：

> <p id="rom_t_rom_feof">判断是否到文件末尾。

* 函数原型：

```
int rom_feof (rom_file_t* f);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int | 返回当前位置。 |
| f | rom\_file\_t* | rom\_file\_t对象。 |
#### rom\_fopen 函数
-----------------------

* 函数功能：

> <p id="rom_t_rom_fopen">打开文件。

* 函数原型：

```
ret_t rom_fopen (const char* name, const char* mode);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回不是NULL表示成功，否则表示失败。 |
| name | const char* | 文件名。 |
| mode | const char* | 打开方式。 |
#### rom\_fopen\_buff 函数
-----------------------

* 函数功能：

> <p id="rom_t_rom_fopen_buff">打开文件。

* 函数原型：

```
ret_t rom_fopen_buff (const uint8_t* data, uint32_t capacity);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| data | const uint8\_t* | 数据。 |
| capacity | uint32\_t | 容量。 |
#### rom\_fread 函数
-----------------------

* 函数功能：

> <p id="rom_t_rom_fread">读取文件。

* 函数原型：

```
size_t rom_fread (void* ptr, size_t size, size_t nitems, rom_file_t* f);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | size\_t | 返回实际读取的字节数。 |
| ptr | void* | 数据的缓冲区。 |
| size | size\_t | 缓冲区大小。 |
| nitems | size\_t | 元素的个数。 |
| f | rom\_file\_t* | rom\_file\_t对象。 |
#### rom\_fseek 函数
-----------------------

* 函数功能：

> <p id="rom_t_rom_fseek">设置文件当前位置。

* 函数原型：

```
int rom_fseek (rom_file_t* f, long offset, int whence);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int | 返回0表示成功。 |
| f | rom\_file\_t* | rom\_file\_t对象。 |
| offset | long | 偏移量。 |
| whence | int | 要从哪个位置开始偏移， SEEK\_SET表示从头开始，SEEK\_CUR表示从当前位置开始，SEEK\_END表示从文件末尾开始。 |
#### rom\_ftell 函数
-----------------------

* 函数功能：

> <p id="rom_t_rom_ftell">返回文件当前位置。

* 函数原型：

```
long rom_ftell (rom_file_t* f);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | long | 返回当前位置。 |
| f | rom\_file\_t* | rom\_file\_t对象。 |
#### rom\_fwrite 函数
-----------------------

* 函数功能：

> <p id="rom_t_rom_fwrite">写文件。
> 本函数什么也不做。

* 函数原型：

```
size_t rom_fwrite (void* ptr, size_t size, size_t nitems, rom_file_t* f);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | size\_t | 返回实际写的字节数。 |
| ptr | void* | 数据的缓冲区。 |
| size | size\_t | 缓冲区大小。 |
| nitems | size\_t | 元素的个数。 |
| f | rom\_file\_t* | rom\_file\_t对象。 |
