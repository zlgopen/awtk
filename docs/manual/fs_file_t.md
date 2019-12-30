## fs\_file\_t
### 概述
文件接口。

示例：

```c
int32_t ret = 0;
const char* file_name = "test.txt";
int32_t len = file_get_size(file_name);
uint8_t* buff = (uint8_t*)TKMEM_ALLOC(len + 1);
return_value_if_fail(buff != NULL, NULL);

fs_file_t* fp = fs_open_file(os_fs(), file_name, "rb");
if (fp != NULL) {
ret = fs_file_read(fp, buff, len);
fs_file_close(fp);
}
```
----------------------------------
