## 控件动画

控件动画是一种很常见的动画，常用于入场动画、离场动画、装饰用户界面和吸引用户注意力等。

### 一、动画类型

AWTK目前支持的动画有：

* widget\_animator\_move。通过改变控件的位置形成动画效果。
* widget\_animator\_value。通过改变控件的值形成动画效果。
* widget\_animator\_opacity。通过改变控件的透明度形成动画效果。
* widget\_animator\_scale。通过改变控件的缩放比例形成动画效果(目前需要OpenGL)。
* widget\_animator\_rotation。通过改变控件的旋转角度形成动画效果(目前需要OpenGL)。

### 二、主要特色

* 支持定时(延迟)执行。
* 支持逆向模式。
* 支持重复模式(可指定次数)。
* 支持往返模式(可指定次数)。
* 支持多种插值算法(如加速和减速等)。
* 支持同一控件多个动画并行执行。
* 支持同一控件多个动画串行执行。

### 三、使用方法

使用控件动画时，一般需要创建动画、设置参数、设置模式和启动动画四个步骤。如：

```
  animator = widget_animator_move_create(image, 1000, delay, EASING_SIN_INOUT);
  widget_animator_move_set_params(animator, image->x, image->y, image->x + 100, image->y + 100);
  widget_animator_set_yoyo(animator, 10);
  widget_animator_start(animator);
```

> 可以为同一个控件创建多个动画，然后设置动画延迟执行时间，来实现并行或串行执行。

### 四、示例

完整示例请参考：demos/demo\_animator\_app.c

运行下列命令查看各种动画的效果：

```
./bin/demo_animator
```

### 五、其它
> 动画对象一般不需主动销毁，在动画结束或控件被销毁时自动销毁。
> 