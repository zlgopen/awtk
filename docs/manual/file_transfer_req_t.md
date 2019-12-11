## file\_transfer\_req\_t
### 概述
请求开始传输包(sender<-receiver)。
----------------------------------
### 属性
<p id="file_transfer_req_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#file_transfer_req_t_block_size">block\_size</a> | uint32\_t | 一次传输包的大小。 |
| <a href="#file_transfer_req_t_filename">filename</a> | char* | 请求传输的文件名。 |
| <a href="#file_transfer_req_t_productid">productid</a> | char* | 请求方的产品ID(有时可用于发送方发送不同的固件，暂时没用) |
| <a href="#file_transfer_req_t_type">type</a> | uint32\_t | 包的类型。 |
#### block\_size 属性
-----------------------
> <p id="file_transfer_req_t_block_size">一次传输包的大小。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 是 |
#### filename 属性
-----------------------
> <p id="file_transfer_req_t_filename">请求传输的文件名。

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 是 |
#### productid 属性
-----------------------
> <p id="file_transfer_req_t_productid">请求方的产品ID(有时可用于发送方发送不同的固件，暂时没用)

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 是 |
#### type 属性
-----------------------
> <p id="file_transfer_req_t_type">包的类型。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 是 |
