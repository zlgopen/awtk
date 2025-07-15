# 如何让 json 文件和 object 之间相互转换？

## json 文件转 object
```c
void my_function(void) {
  /* json 的文件路径为 path/to/your/json/file.json */
  tk_object_t* my_object = conf_json_load_ex("file://path/to/your/json/file.json", FALSE, TRUE);

  ...使用 my_object, 通过 tk_object_get_prop 函数获取属性值

  tk_object_unref(my_object);
}
```

## object 转 json 文件
```c
static ret_t my_object_save_json(tk_object_t* obj, const char* url) {
  ret_t ret = RET_OK;
  tk_object_t* conf = NULL;
  return_value_if_fail(obj != NULL && url != NULL, RET_FAIL);

  conf = conf_json_create();
  return_value_if_fail(conf != NULL, RET_OOM);

  conf_doc_use_extend_type(conf_obj_get_doc(conf), TRUE);

  ret = tk_object_copy_props(conf, obj, TRUE);
  if (RET_OK == ret) {
    ret = conf_json_save_as(conf, url);
  }
  tk_object_unref(conf);

  return ret;
}

void my_function(void) {
  tk_object_t* my_object = object_hash_create();
  object_hash_set_keep_props_order(my_object, TRUE);

  ...通过 tk_object_set_prop 给 my_object 添加属性

  /* json 的文件路径为 path/to/your/json/file.json */
  my_object_save_json(my_object, "file://path/to/your/json/file.json");

  tk_object_unref(my_object);
}
```
