## network\_interface\_t
### 概述
网卡接口。
----------------------------------
### 函数
<p id="network_interface_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#network_interface_t_network_interface_create">network\_interface\_create</a> | 网卡接口创建函数。 |
| <a href="#network_interface_t_network_interface_destroy">network\_interface\_destroy</a> | 网卡接口销毁函数。 |
| <a href="#network_interface_t_network_interface_disable">network\_interface\_disable</a> | 网卡接口禁能函数。 |
| <a href="#network_interface_t_network_interface_enable">network\_interface\_enable</a> | 网卡接口使能函数。 |
| <a href="#network_interface_t_network_interface_get_ipaddr">network\_interface\_get\_ipaddr</a> | 网卡接口获取IP地址。 |
| <a href="#network_interface_t_network_interface_get_macaddr">network\_interface\_get\_macaddr</a> | 网卡接口获取MAC地址。 |
| <a href="#network_interface_t_network_interface_get_quality">network\_interface\_get\_quality</a> | 网卡接口获取通信质量。 |
| <a href="#network_interface_t_network_interface_get_status">network\_interface\_get\_status</a> | 网卡接口获取状态。 |
| <a href="#network_interface_t_network_interface_set_dhcp">network\_interface\_set\_dhcp</a> | 网卡接口设置DNS。 |
| <a href="#network_interface_t_network_interface_set_dns">network\_interface\_set\_dns</a> | 网卡接口设置DNS。 |
| <a href="#network_interface_t_network_interface_set_gateway">network\_interface\_set\_gateway</a> | 网卡接口设置DNS。 |
| <a href="#network_interface_t_network_interface_set_ipaddr">network\_interface\_set\_ipaddr</a> | 网卡接口设置IP地址。 |
| <a href="#network_interface_t_network_interface_set_wifi_ap">network\_interface\_set\_wifi\_ap</a> | 网卡接口设置WIFI AP模式。 |
| <a href="#network_interface_t_network_interface_set_wifi_sta">network\_interface\_set\_wifi\_sta</a> | 网卡接口设置WIFI STA模式。 |
#### network\_interface\_create 函数
-----------------------

* 函数功能：

> <p id="network_interface_t_network_interface_create">网卡接口创建函数。

* 函数原型：

```
network_interface_t network_interface_create (char * interface, type network_interface_type_t);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | network\_interface\_t | 网卡接口对象。 |
| interface | char * | 网卡名。 |
| network\_interface\_type\_t | type | 网卡接口类型。 |
#### network\_interface\_destroy 函数
-----------------------

* 函数功能：

> <p id="network_interface_t_network_interface_destroy">网卡接口销毁函数。

* 函数原型：

```
void network_interface_destroy (network_interface_t * interface);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void | 。 |
| interface | network\_interface\_t * | 网卡对象。 |
#### network\_interface\_disable 函数
-----------------------

* 函数功能：

> <p id="network_interface_t_network_interface_disable">网卡接口禁能函数。

* 函数原型：

```
ret_t network_interface_disable (network_interface_t * interface);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | RET\_OK表示成功，否则失败。 |
| interface | network\_interface\_t * | 网卡对象。 |
#### network\_interface\_enable 函数
-----------------------

* 函数功能：

> <p id="network_interface_t_network_interface_enable">网卡接口使能函数。

* 函数原型：

```
ret_t network_interface_enable (network_interface_t * interface);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | RET\_OK表示成功，否则失败。 |
| interface | network\_interface\_t * | 网卡对象。 |
#### network\_interface\_get\_ipaddr 函数
-----------------------

* 函数功能：

> <p id="network_interface_t_network_interface_get_ipaddr">网卡接口获取IP地址。

* 函数原型：

```
char * network_interface_get_ipaddr (network_interface_t * interface);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char * | 返回IP地址成功，NULL失败。 |
| interface | network\_interface\_t * | 网卡对象。 |
#### network\_interface\_get\_macaddr 函数
-----------------------

* 函数功能：

> <p id="network_interface_t_network_interface_get_macaddr">网卡接口获取MAC地址。

* 函数原型：

```
char * network_interface_get_macaddr (network_interface_t * interface);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char * | 返回MAC地址成功，NULL失败。 |
| interface | network\_interface\_t * | 网卡对象。 |
#### network\_interface\_get\_quality 函数
-----------------------

* 函数功能：

> <p id="network_interface_t_network_interface_get_quality">网卡接口获取通信质量。

* 函数原型：

```
int network_interface_get_quality (network_interface_t * interface);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int | 对于无线网络返回信号RSSI的绝对值， 对于有线网络，返回10、100、1000M带宽。 |
| interface | network\_interface\_t * | 网卡对象。 |
#### network\_interface\_get\_status 函数
-----------------------

* 函数功能：

> <p id="network_interface_t_network_interface_get_status">网卡接口获取状态。

* 函数原型：

```
int network_interface_get_status (network_interface_t * interface);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int | 0表示未连接，1表示连接成功，-1表示操作失败。 |
| interface | network\_interface\_t * | 网卡对象。 |
#### network\_interface\_set\_dhcp 函数
-----------------------

* 函数功能：

> <p id="network_interface_t_network_interface_set_dhcp">网卡接口设置DNS。

* 函数原型：

```
ret_t network_interface_set_dhcp (network_interface_t * interface);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | RET\_OK表示成功，否则失败。 |
| interface | network\_interface\_t * | 网卡对象。 |
#### network\_interface\_set\_dns 函数
-----------------------

* 函数功能：

> <p id="network_interface_t_network_interface_set_dns">网卡接口设置DNS。

* 函数原型：

```
ret_t network_interface_set_dns (network_interface_t * interface, char * dns);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | RET\_OK表示成功，否则失败。 |
| interface | network\_interface\_t * | 网卡对象。 |
| dns | char * | 网卡dns。 |
#### network\_interface\_set\_gateway 函数
-----------------------

* 函数功能：

> <p id="network_interface_t_network_interface_set_gateway">网卡接口设置DNS。

* 函数原型：

```
ret_t network_interface_set_gateway (network_interface_t * interface, char * gateway, char * dev);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | RET\_OK表示成功，否则失败。 |
| interface | network\_interface\_t * | 网卡对象。 |
| gateway | char * | 网卡网关。 |
| dev | char * | 出口网卡设备名。 |
#### network\_interface\_set\_ipaddr 函数
-----------------------

* 函数功能：

> <p id="network_interface_t_network_interface_set_ipaddr">网卡接口设置IP地址。

* 函数原型：

```
ret_t network_interface_set_ipaddr (network_interface_t * interface, char * ipaddr, char * netmask);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | RET\_OK表示成功，否则失败。 |
| interface | network\_interface\_t * | 网卡对象。 |
| ipaddr | char * | 网卡IP。 |
| netmask | char * | 网卡MASK。 |
#### network\_interface\_set\_wifi\_ap 函数
-----------------------

* 函数功能：

> <p id="network_interface_t_network_interface_set_wifi_ap">网卡接口设置WIFI AP模式。

* 函数原型：

```
ret_t network_interface_set_wifi_ap (network_interface_t * interface, char * essid, char * passwd, network_interface_wifi_auth_t   auth, uint8_t  channel, network_interface_wifi_freq_t   freq, uint8_t  hidden);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | RET\_OK表示成功，否则失败。 |
| interface | network\_interface\_t * | 网卡对象。 |
| essid | char * | wifi帐号 。 |
| passwd | char * | wifi密码。 |
| auth | network\_interface\_wifi\_auth\_t   | wifi认证方式。 |
| channel | uint8\_t  | wifi通道。 |
| freq | network\_interface\_wifi\_freq\_t   | wifi频段。 |
| hidden | uint8\_t  | wifi是否隐藏。 |
#### network\_interface\_set\_wifi\_sta 函数
-----------------------

* 函数功能：

> <p id="network_interface_t_network_interface_set_wifi_sta">网卡接口设置WIFI STA模式。

* 函数原型：

```
ret_t network_interface_set_wifi_sta (network_interface_t * interface, char * essid, char * passwd, network_interface_wifi_auth_t   auth);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | RET\_OK表示成功，否则失败。 |
| interface | network\_interface\_t * | 网卡对象。 |
| essid | char * | wifi帐号 。 |
| passwd | char * | wifi密码。 |
| auth | network\_interface\_wifi\_auth\_t   | wifi认证方式。 |
