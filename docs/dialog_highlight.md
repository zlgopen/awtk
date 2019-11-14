# 对话框高亮策略

## 一、介绍

在弹出除对话框时，通常希望让背景窗口变暗或者变模糊，以突显当前的对话框的重要性。我们把让背景窗口变暗或者变模糊的方法称为对话框高亮策略。并把对话框高亮策略抽象成接口，开发者可以自己实现特殊效果的高亮策略，也可以使用缺省的高亮策略。

缺省的对话框高亮策略是让背景窗口变暗，可以是静态的(固定alpha)，也可以是动态的。动态的高亮策略在对话框打开的动画过程中，背景窗口逐渐变暗，这有更好的视觉效果，但也需要更多的计算开销，开发者可以根据实际情况选用静态还是动态效果。

## 二、使用方法

通过对话框的hightlight属性来指定对话框的高亮策略，"default"表示缺省的对话框的高亮策略。如：

使用静态效果：

```
<dialog anim_hint="popdown" highlight="default(alpha=40)"  
  x="0" y="0" w="100%" h="160">
```

使用动态效果：

```
<dialog anim_hint="popup" highlight="default(start_alpha=0, end_alpha=80)" 
  x="0" y="bottom" w="100%" h="160">
```



## 三、参数说明

参数其格式为[类似函数调用的参数格式](func_call_params_format.md)。 缺省的对话框高亮策略支持以下几个参数：

* start_alpha 起始alpha值(0-255)。
* end_alpha 结束alpha值(0-255)。
* alpha 相当于将start\_alpha和end\_alpha设置为同一个值。

> 当alpha=0时，显示效果没有变化，但相当于了缓存背景窗口的绘制到图片中，从而提高显示效率，在特殊情况下可以酌情使用。

## 四、自定义对话框高亮策略

开发者可以自定义高亮策略，自定义的高亮策略可以像内置高亮策略一样，可以用以上方法在XML文件中启用。


1.实现dialog\_highlighter\_t接口。如：

```
static const dialog_highlighter_vtable_t s_dialog_highlighter_default_vt = { 
    .type = "dialog_highlighter_default_t",
    .desc = "dialog_highlighter_default_t",
    .size = sizeof(dialog_highlighter_default_t),
    .prepare = dialog_highlighter_default_prepare,
    .draw = dialog_highlighter_default_draw};

dialog_highlighter_t* dialog_highlighter_default_create(object_t* args) {
  value_t v;
  dialog_highlighter_t* h = dialog_highlighter_create(&s_dialog_highlighter_default_vt);
  dialog_highlighter_default_t* dh = (dialog_highlighter_default_t*)h;
  return_value_if_fail(h != NULL, NULL);

  dh->end_alpha = 0;
  dh->start_alpha = 0;

...

  return h;
}
```

2.注册到dialog\_highlighter\_factory工厂。如：

```
  dialog_highlighter_factory_register(f, DIALOG_HIGHLIGHTER_DEFAULT,
                                      dialog_highlighter_default_create);
```

> 完整示例请参考：https://github.com/zlgopen/awtk/tree/master/src/dialog_highlighters
