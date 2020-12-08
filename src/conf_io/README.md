# conf-io

为应用程序提供一个统一访问各个配置文件的API，目前支持INI/JSON/UBJSON。

## 注意 

conf-io是可选组件，需要自己包含头文件，否则64位数据类型会被截断成32位的数据。

```c
#include "conf_io/app_conf.h"
```

