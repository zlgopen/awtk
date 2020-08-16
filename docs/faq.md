# FAQ

#### 1.return\_value\_if\_fail 作为 AWTK 中使用率排第一的宏，它的功能、优点和注意事项都有哪些？

**功能**

* 主要用于对函数的参数或函数的返回值进行检查（这是防御性编程的手段之一）。

> return\_value\_if\_fail 这个宏并非是 AWTK 原创，而是从 GTK+（或者说 glib) 里拿来的。

**优点**

* 以简洁的方式对函数的参数或函数的返回值进行检查。

* Release 模式和 Debug 模式可以做不同的处理。

> 在参数出现错误时，悄无声息的返回一个错误码，其实是对调用者的纵容，很容易把错误隐藏起来。所以在 Debug 模式我们可以打出一条警告信息，甚至直接 assert 掉，这对于定位 BUG 非常有效。

**注意事项**

* 内部函数 (static) 一般不需要对参数进行检查。
* 只对异常的情况进行判断，对于正常的失败或无效参数，请不要使用本宏。
* 如果在返回之前，有资源需要释放，请不要用本宏。可以用 goto\_error\_if\_fail 跳到 error 出，释放资源后再返回。

---

#### 2. 每次在绘制图片前，都要调用 image\_manager\_load 去加载图片，这样做会不会很慢？有什么优点？

* 不会慢。因为 image\_manager 中有缓存，不会每次都去解码。

**优点**

* 缓存有助于多个控件共享同一张图片。

* 外面不保存对 bitmap 的引用，缓存管理更加灵活。比如，可以清除最近没有被渲染的图片（即使某个隐藏的窗口还在使用该图片）。

---

#### 3. 使用矢量字体，速度会慢吗？

* 几乎没有影响。因为有缓存，所以只需要渲染一次，之后和位图字体的并无不同。

---

#### 4. 在 16 位 LCD 上显示 PNG 图片效果很差，有什么办法吗？

* 如果是不透明的图片，可以将 PNG 转换成 JPG 文件，转换过程中启用 dithering 算法做平滑处理。

可以用 imagemagic 转换：
```
convert bg.png  -ordered-dither o8x8,32,64,32 bg.jpg
```
> 参考：http://www.imagemagick.org/Usage/quantize/

#### 5. 如何获取控件值？

获取控件的值有以下几种方式：

* 用 widget\_get\_value 函数获取（仅支持整数类型）。

* 用 wiget\_get\_prop 函数获取。

* 直接访问控件的属性。控件的属性如果标记为 readable，均可直接访问。如：

```
widget_t* slider = widget_lookup(win, "slider", TRUE);
double value = SLIDER(slider)->value;
```

> 直接访问控件属性时，需要用对应的宏（如上面的 SLIDER) 进行类型转换。

#### 6.Ubuntu 14 上无法启动，有什么办法吗？

Ubuntu 14 上的 OpenGL 有问题，请使用 AGGE 软件渲染。修改 awtk\_config.py：

```
NANOVG_BACKEND='AGGE'
```

#### 7. 如何实现半透明效果

* 在 style 中，使用 rgba 格式可以指定半透明填充颜色。如：

```xml
<normal     bg_color="rgba(200,200,200,0.1)" />
```

* 图片半透明。在制作图片时，使用 PNG 格式，保留 alpha 通道。

* 整个控件（包括子控件）半透明。可以使用函数 widget\_set\_opacity 设置不透明度。

```c
/**
 * @method widget_set_opacity
 * 设置控件的不透明度。
 * 
 *>在嵌入式平台，半透明效果会使性能大幅下降，请谨慎使用。
 * 
 * @param {widget_t*} widget 控件对象。
 * @param {uint8_t} opacity 不透明度（取值 0-255，0 表示完全透明，255 表示完全不透明）。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t widget_set_opacity(widget_t* widget, uint8_t opacity);
```

> opacity 会影响包括字体在内的全部元素，通常只适用于实现淡入淡出的动态效果。

#### 8. 如何定制缺省软键盘界面？

软键盘可以根据自己的情况进行调整，可以修改 design/default/ui/kb_default.xml。

* 修改 active 的值可以改变初始页面。

```xml
 <pages x="0" y="bottom" w="100%" h="-28" active="4">
```

* 修改 s 的值可以调正按钮之间的间距，嵌入式系统 s=1 就好了。

```xml
children_layout="default(r=4,c=1,s=2,m=2)"
```

* 修改 h 的值可以调整键盘的高度

```xml
<keyboard theme="keyboard" x="0" y="bottom" w="100%" h="40%">
```

* 中英文按钮切换的图片最好自己设计一个。

修改之后，需要更新资源的脚本：

```
python scripts/update_res.py ui
```

#### 9. 如何查看应用程序占用了多少内存？

tk\_mem\_stat 函数可以获取内存的使用情况，也可以直接调用 tk\_mem\_dump 显示内存使用情况。

tk\_mem\_dump 函数的实现如下：

```c
void tk_mem_dump(void) {
  mem_stat_t s = tk_mem_stat();
  log_debug("used: %d bytes %d blocks\n", s.used_bytes, s.used_block_nr);
}
```

#### 10. 如何确定是否是内存不够导致运行速度变慢？

内存不够时系统确实有可能变慢，内存不够时会清除部分或全部解码的图片缓存，下次使用这些图片时会重新解码。

可以在 mem.c 中的函数 tk\_mem\_on\_out\_of\_memory 里设置断点，内存耗尽时会调用这个函数释放缓存。如果这个函数被调用，说明内存不够。

#### 11. 如何降低内存开销？

* 减少同时打开窗口的个数。

* 根据实际情况 (lcd 的格式） 定义 WITH\_BITMAP_BGR565/WITH\_BITMAP\_RGB565 把不透明的图片解码成 16 位。

#### 12. 如何关闭 log_debug 打印的调试信息。

log\_set\_log\_level 函数可设置 log 的级别，用它可以关闭低级别的 log 信息。

```c
/**
 * @method log_set_log_level
 *
 * 设置 log 的级别。
 *
 * @param {log_level_t} log_level log 的级别。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t log_set_log_level(log_level_t log_level);
```

在 awtk_main.inc 中有如下代码：

```c
#ifdef NDEBUG
  log_set_log_level(LOG_LEVEL_INFO);
#else
  log_set_log_level(LOG_LEVEL_DEBUG);
#endif /*NDEBUG*/
```

如果使用 awtk_main.inc 作为应用程序的入口，定义 NDEBUG 也关闭 debug 级别的调试信息。

#### 13. 应用程序在 Windows 的手持设备中运行，如何去掉窗口的标题栏？

在 awtk\_config.py 中定义宏 NATIVE\_WINDOW\_BORDERLESS，重新编译即可：

```python
COMMON_CCFLAGS=COMMON_CCFLAGS+' -DNATIVE_WINDOW_BORDERLESS=1 '
```

#### 14. 子控件处理了事件，不希望父控件继续处理，怎么处理呢？
   
让事件处理函数返回 RET\_STOP，AWTK 不再调用后续事件处理函数。

#### 15. 如何去掉不需要的控件，以节省 flash 空间

因为控件注册了，即使没有使用，编译器也会把相关代码编译进去。所以只有去掉控件的注册代码，才能优化掉不必要的代码。

* 去掉不必要的基本控件，可以编辑 src/widgets/widgets.c 

比如去掉 overlay 窗口，可以注释掉下面的这行代码：

```
  widget_factory_register(f, WIDGET_TYPE_OVERLAY, overlay_create);
```

* 去掉不必要的扩展控件，可以编辑 src/ext_widgets/ext_widgets.c

* 从 assets/default/raw/styles/default.xml 中去掉不必要的。

* 去掉不必要的图片。一些控件去掉了，相应的图片也没有必要了。

#### 16. 内存不够，内部flash不够，如何加载大字体文件？

请参考：[自定义字体加载器：加载部分字体](https://github.com/zlgopen/awtk-custom-font-loader)
