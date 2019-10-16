# FAQ

#### 1.return\_value\_if\_fail作为AWTK中使用率排第一的宏，它的功能、优点和注意事项都有哪些？

**功能**

* 主要用于对函数的参数或函数的返回值进行检查(这是防御性编程的手段之一)。

> return\_value\_if\_fail这个宏并非是AWTK原创，而是从GTK+(或者说glib)里拿来的。

**优点**

* 以简洁的方式对函数的参数或函数的返回值进行检查。

* Release模式和Debug模式可以做不同的处理。

> 在参数出现错误时，悄无声息的返回一个错误码，其实是对调用者的纵容，很容易把错误隐藏起来。所以在Debug模式我们可以打出一条警告信息，甚至直接assert掉，这对于定位BUG非常有效。

**注意事项**

* 内部函数(static)一般不需要对参数进行检查。
* 只对异常的情况进行判断，对于正常的失败或无效参数，请不要使用本宏。
* 如果在返回之前，有资源需要释放，请不要用本宏。可以用goto\_error\_if\_fail跳到error出，释放资源后再返回。

---

#### 2.每次在绘制图片前，都要调用image\_manager\_load去加载图片，这样做会不会很慢？有什么优点？

* 不会慢。因为image\_manager中有缓存，不会每次都去解码。

**优点**

* 缓存有助于多个控件共享同一张图片。

* 外面不保存对bitmap的引用，缓存管理更加灵活。比如，可以清除最近没有被渲染的图片(即使某个隐藏的窗口还在使用该图片)。

---

#### 3.使用矢量字体，速度会慢吗？

* 几乎没有影响。因为有缓存，所以只需要渲染一次，之后和位图字体的并无不同。

---

#### 4.在16位LCD上显示PNG图片效果很差，有什么办法吗？

* 如果是不透明的图片，可以将PNG转换成JPG文件，转换过程中启用dithering算法做平滑处理。

可以用imagemagic转换：
```
convert bg.png  -ordered-dither o8x8,32,64,32 bg.jpg
```
> 参考：http://www.imagemagick.org/Usage/quantize/


#### 5.如何获取控件值？

获取控件的值有以下几种方式：

* 用widget\_get\_value函数获取(仅支持整数类型)。

* 用wiget\_get\_prop函数获取。

* 直接访问控件的属性。控件的属性如果标记为readable，均可直接访问。如：

```
widget_t* slider = widget_lookup(win, "slider", TRUE);
double value = SLIDER(slider)->value;
```

> 直接访问控件属性时，需要用对应的宏(如上面的SLIDER)进行类型转换。


#### 6.Ubuntu 14上无法启动，有什么办法吗？

Ubuntu 14上的OpenGL有问题，请使用AGGE软件渲染。修改awtk\_config.py：

```
NANOVG_BACKEND='AGGE'
```

#### 7.如何实现半透明效果

* 在style中，使用rgba格式可以指定半透明填充颜色。如：

```xml
<normal     bg_color="rgba(200,200,200,0.1)" />
```

* 图片半透明。在制作图片时，使用PNG格式，保留alpha通道。

* 整个控件(包括子控件)半透明。可以使用函数widget\_set\_opacity设置不透明度。

```c
/**
 * @method widget_set_opacity
 * 设置控件的不透明度。
 * 
 *>在嵌入式平台，半透明效果会使性能大幅下降，请谨慎使用。
 * 
 * @param {widget_t*} widget 控件对象。
 * @param {uint8_t} opacity 不透明度(取值0-255，0表示完全透明，255表示完全不透明)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_opacity(widget_t* widget, uint8_t opacity);
```

> opacity会影响包括字体在内的全部元素，通常只适用于实现淡入淡出的动态效果。
