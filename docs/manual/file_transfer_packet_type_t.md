## file\_transfer\_packet\_type\_t
### 概述
文件传输包的类型。### 常量
<p id="file_transfer_packet_type_t_consts">

| 名称 | 说明 | 
| -------- | ------- | 
| FILE\_TRANSFER\_PACKET\_META | 文件元数据包(sender->receiver)。 |
| FILE\_TRANSFER\_PACKET\_DATA | 文件数据包(sender->receiver)。 |
| FILE\_TRANSFER\_PACKET\_ACK | 文件数据确认包(sender<-receiver)。 |
| FILE\_TRANSFER\_PACKET\_NACK | 文件数据错误确认包(sender<-receiver)。 |
| FILE\_TRANSFER\_PACKET\_REQ | 请求开始传输包(sender<-receiver)。 |
