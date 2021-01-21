# csv 文件

> 本目录的文件从 [awtk-csv-file](https://github.com/zlgopen/awtk-csv-file) 拷贝而来。如果需要修改，请先修改 [awtk-csv-file](https://github.com/zlgopen/awtk-csv-file)，再拷贝过来。

csv 为可选组件，需要自己包含头文件

```c
#include "csv/csv_file.h"
#include "csv/csv_file_object.h"
```

## 示例：

* 普通方式访问

```c
  const char* str = "11,12,13\n21,22,23";
  csv_file_t* csv = csv_file_create_with_buff(str, strlen(str), ',');
  ASSERT_EQ(csv_file_get_rows(csv), 2);
  ASSERT_EQ(csv_file_get_cols(csv), 3);

  ASSERT_STREQ(csv_file_get(csv, 0, 0), "11");
  ASSERT_STREQ(csv_file_get(csv, 0, 1), "12");
  ASSERT_STREQ(csv_file_get(csv, 0, 2), "13");

  ASSERT_STREQ(csv_file_get(csv, 1, 0), "21");
  ASSERT_STREQ(csv_file_get(csv, 1, 1), "22");
  ASSERT_STREQ(csv_file_get(csv, 1, 2), "23");

  csv_file_destroy(csv);
```

```c
TEST(csv_file, title) {
  const char* str = "aa,bb,cc\n11,12,13\n21,22,23";
  csv_file_t* csv = csv_file_create_with_buff(str, strlen(str), ',');
  ASSERT_EQ(csv_file_get_rows(csv), 3);
  ASSERT_EQ(csv_file_get_cols(csv), 3);

  ASSERT_STREQ(csv_file_get(csv, 0, 0), "aa");
  ASSERT_STREQ(csv_file_get(csv, 0, 1), "bb");
  ASSERT_STREQ(csv_file_get(csv, 0, 2), "cc");

  ASSERT_STREQ(csv_file_get(csv, 1, 0), "11");
  ASSERT_STREQ(csv_file_get(csv, 1, 1), "12");
  ASSERT_STREQ(csv_file_get(csv, 1, 2), "13");

  ASSERT_STREQ(csv_file_get(csv, 2, 0), "21");
  ASSERT_STREQ(csv_file_get(csv, 2, 1), "22");
  ASSERT_STREQ(csv_file_get(csv, 2, 2), "23");

  ASSERT_STREQ(csv_file_get_by_name(csv, 1, "aa"), "11");
  ASSERT_STREQ(csv_file_get_by_name(csv, 1, "bb"), "12");
  ASSERT_STREQ(csv_file_get_by_name(csv, 1, "cc"), "13");

  ASSERT_STREQ(csv_file_get_by_name(csv, 2, "aa"), "21");
  ASSERT_STREQ(csv_file_get_by_name(csv, 2, "bb"), "22");
  ASSERT_STREQ(csv_file_get_by_name(csv, 2, "cc"), "23");
  
  ASSERT_EQ(csv_file_get_col_of_name(csv, "aa"), 0);
  ASSERT_EQ(csv_file_get_col_of_name(csv, "bb"), 1);
  ASSERT_EQ(csv_file_get_col_of_name(csv, "cc"), 2);

  csv_file_destroy(csv);
}
```
* 以对象的方式访问：

```c
  const char* str = "aa,bb,cc\n11,12,13\n21,22,23";
  csv_file_t* csv = csv_file_create_with_buff(str, strlen(str), ',');
  object_t* obj = csv_file_object_create(csv);

  ASSERT_EQ(object_get_prop_int(obj, "#size", 0), 3); 
  ASSERT_STREQ(object_get_prop_str(obj, "[0].[0]"), "aa");
  ASSERT_STREQ(object_get_prop_str(obj, "[0].[1]"), "bb");
  ASSERT_STREQ(object_get_prop_str(obj, "[0].[2]"), "cc");
  
  ASSERT_STREQ(object_get_prop_str(obj, "[1].[0]"), "11");
  ASSERT_STREQ(object_get_prop_str(obj, "[1].[1]"), "12");
  ASSERT_STREQ(object_get_prop_str(obj, "[1].[2]"), "13");
  OBJECT_UNREF(obj);
```

## 具体用法请参考：
* https://github.com/zlgopen/awtk-csv-file/blob/master/tests/csv_file_test.cc
* https://github.com/zlgopen/awtk-csv-file/blob/master/tests/csv_file_object_test.cc
