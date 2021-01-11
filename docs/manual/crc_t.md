## crc\_t
### 概述
crc相关函数。
----------------------------------
### 函数
<p id="crc_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#crc_t_tk_chksum">tk\_chksum</a> | 计算检验和。 |
| <a href="#crc_t_tk_chksum_byte">tk\_chksum\_byte</a> | 计算单子节的检验和。 |
| <a href="#crc_t_tk_crc16">tk\_crc16</a> | 计算crc16。 |
| <a href="#crc_t_tk_crc16_byte">tk\_crc16\_byte</a> | 计算单子节的crc16。 |
| <a href="#crc_t_tk_crc32">tk\_crc32</a> | 计算crc32。 |
| <a href="#crc_t_tk_crc32_byte">tk\_crc32\_byte</a> | 计算单子节的crc32。 |
#### tk\_chksum 函数
-----------------------

* 函数功能：

> <p id="crc_t_tk_chksum">计算检验和。

* 函数原型：

```
uint16_t tk_chksum (uint16_t sum, const uint8_t* data, int size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint16\_t | 返回计算结果。 |
| sum | uint16\_t | 初始值(一般用INITCHECKSUM) |
| data | const uint8\_t* | 数据。 |
| size | int | 数据长度。 |
#### tk\_chksum\_byte 函数
-----------------------

* 函数功能：

> <p id="crc_t_tk_chksum_byte">计算单子节的检验和。

* 函数原型：

```
uint16_t tk_chksum_byte (uint16_t sum, uint8_t data);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint16\_t | 返回计算结果。 |
| sum | uint16\_t | 初始值(一般用0，或者之前的值) |
| data | uint8\_t | 数据。 |
#### tk\_crc16 函数
-----------------------

* 函数功能：

> <p id="crc_t_tk_crc16">计算crc16。

* 函数原型：

```
uint16_t tk_crc16 (uint16_t crc, const uint8_t* data, int size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint16\_t | 返回计算结果。 |
| crc | uint16\_t | 初始值(一般用PPPINITFCS16) |
| data | const uint8\_t* | 数据。 |
| size | int | 数据长度。 |
#### tk\_crc16\_byte 函数
-----------------------

* 函数功能：

> <p id="crc_t_tk_crc16_byte">计算单子节的crc16。

* 函数原型：

```
uint16_t tk_crc16_byte (uint16_t crc, uint8_t data);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint16\_t | 返回计算结果。 |
| crc | uint16\_t | 初始值(一般用PPPINITFCS16，或者之前的值) |
| data | uint8\_t | 数据。 |
#### tk\_crc32 函数
-----------------------

* 函数功能：

> <p id="crc_t_tk_crc32">计算crc32。

* 函数原型：

```
uint32_t tk_crc32 (uint32_t crc, const uint8_t* data, int size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回计算结果。 |
| crc | uint32\_t | 初始值(一般用PPPINITFCS32) |
| data | const uint8\_t* | 数据。 |
| size | int | 数据长度。 |
#### tk\_crc32\_byte 函数
-----------------------

* 函数功能：

> <p id="crc_t_tk_crc32_byte">计算单子节的crc32。

* 函数原型：

```
uint32_t tk_crc32_byte (uint32_t crc, uint8_t data);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回计算结果。 |
| crc | uint32\_t | 初始值(一般用PPPINITFCS32，或者之前的值) |
| data | uint8\_t | 数据。 |
