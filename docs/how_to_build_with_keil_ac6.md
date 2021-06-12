## 如何使用keil的AC6工具链编译

有些平台必须使用AC6工具链编译，但是使用AC6编译会有些问题，请先做下列修改：

1. 在C/C++(AC6)设置中，指定Language C为gnu99
2. 在C/C++(AC6)设置中，从Misc Controls中去掉 --gnu
3. 选中.cpp的文件，右键打开"Options for file xxx.cpp"菜单，File Type选择“C++ Source file”
4. 在AC6中有些语法变化，比如__attribute__((at(0x08041000)))是无效的，请改成 __attribute__((section(".ARM.__AT_0x08041000")))。

