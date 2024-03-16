# 如何为指定的键设置长按时间

有时候可能需要为不同的按键指定不同的长按时间，比如长按 1 重启系统，长按 2 重置系统，为了避免误操作，重置系统的长按时间可以设置的长一些。这时可以使用`input_device_status_set_key_long_press_time`函数来设置。

示例：

```c
    widget_t* wm = window_manager();
    input_device_status_t* ids = window_manager_get_input_device_status(wm);
    input_device_status_set_key_long_press_time(ids, TK_KEY_1, 2000);
    input_device_status_set_key_long_press_time(ids, TK_KEY_2, 10000);
```    