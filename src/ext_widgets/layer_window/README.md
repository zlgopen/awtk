#  layer window

## 背景

有的平台支持多个 LCD layer，硬件能够快速将它们合并到一个 LCD 上。为了利用这个特性来提高性能，我们提供了一个 layer window 类型的窗口。

* 在 PC 上，它的行为其它窗口类似。
* 在支持多层的硬件上，它把内容显示到指定名称的层上，由于它有独立的脏矩形，它要求重绘时不会导致其它层重绘。

可以把某些不会频繁更新的内容放到主层上，而把另外需要频繁更新的内容放到一个或多个其它层上，这样，**频繁更新的内容**不会导致**不频繁更新**的内容重绘，从而提供性能。

## 使用方法

### 1. 编译

> 在 PC 上无需特殊修改。在实际的硬件中，要在配置文件中或工程中定义下面的宏：

```c
#define WITH_MULTI_LAYERS 1
```

### 2. XML

在 XML 中，使用其它类型的窗口基本一样，只是要注意以下事项：

* 不能启用窗口动画。
* 要指定 layer_name 属性，其名称要和注册时指定的名称一致。
* 和 popup 一样，要指定坐标和大小，坐标是屏幕绝对坐标，不是相对 layer 的坐标。

```
<layer_window x="20" y="60" w="280" h="400" layer_name="fast">

</layer_window>
```
> 请参考 layer_window.xml

### 3. 代码

* 初始化时注册 layer，以便在 layer window 可以通过名字找到该层。

```c
static canvas_t* s_canvas_fast = NULL;
#define create_fast_canvas(w, h) canvas_offline_create(w, h, BITMAP_FMT_RGBA8888)

static ret_t layers_init(void) {
  layer_t* layer = NULL;
  layer_manager_t* layer_manager = layer_manager_create();
  return_value_if_fail(layer_manager != NULL, RET_BAD_PARAMS);
  layer_manager_set(layer_manager);
  
  /*根据实际情况修改*/
  s_canvas_fast = create_fast_canvas(300, 460);

  layer = layer_create("fast", s_canvas_fast, 10, 10, 60); 
  assert(layer != NULL);
  layer_manager_add(layer_manager, layer);
  layer_start(layer);
  
  return RET_OK;
}
```

* 退出时销毁相关数据。

```c
static ret_t layers_deinit(void) {
  if(s_canvas_fast != NULL) {
    canvas_reset(s_canvas_fast);
  }
  layer_manager_destroy(layer_manager());
  layer_manager_set(NULL);

  return RET_OK;
}
```

> 完整实例请参考：demos/demo_layers.c
