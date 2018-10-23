## 控件动画

控件动画是一种很常见的动画，常用于入场动画、离场动画、装饰用户界面和吸引用户注意力等。

### 一、动画类型

AWTK目前支持的动画有：

* widget\_animator\_move：通过改变控件的位置形成动画效果。
* widget\_animator\_value：通过改变控件的值形成动画效果。
* widget\_animator\_opacity：通过改变控件的透明度形成动画效果。
* widget\_animator\_scale：通过改变控件的缩放比例形成动画效果(目前需要vgcanvas)。
* widget\_animator\_rotation：通过改变控件的旋转角度形成动画效果(目前需要vgcanvas)。

### 二、主要特色

* 支持定时(延迟)执行。
* 支持逆向模式。
* 支持重复模式(可指定次数)。
* 支持往返模式(可指定次数)。
* 支持多种插值算法(如加速和减速等)。
* 支持同一控件多个动画并行执行。
* 支持同一控件多个动画串行执行。

### 三、使用方法

#### 1.方法一：直接使用

使用控件动画时，一般需要创建动画、设置参数、设置模式和启动动画四个步骤。如：

```
  animator = widget_animator_move_create(image, 1000, delay, EASING_SIN_INOUT);
  widget_animator_move_set_params(animator, image->x, image->y, image->x + 100, image->y + 100);
  widget_animator_set_yoyo(animator, 10);
  widget_animator_start(animator);
```

> 此方法比较麻烦，但比较灵活，可以主动停止。

#### 2.方法二：通过函数widget\_set\_animation指定。

animation参数的格式，类似与函数调用。多个参数可以用『;』分隔。如：

```
widget_set_animation(image, "opacity(from=0, to=255, yoyo_times=1000, duration=1000)");
```

> 此方法比较简单，但缺乏灵活性，没法主动停止。

#### 3.方法三：在XML中指定

animation参数的格式，类似与函数调用。多个参数可以用『;』分隔。如：

```
 <button name="inc_value" text="Inc" animation="opacity(from=0, to=255, yoyo_times=1000, duration=1000)"/>
 <button name="dec_value" text="Dec" animation="move(x_from=10, x_to=100, y_from=10, y_to=100, duration=1000);opacity(from=0, to=255, yoyo_times=1000, duration=1000)"/>
```

> 此方法最为简单，但缺乏灵活性，没法主动停止，也没法动态提供参数。

### 四、可用参数

#### 1.公共参数

* delay 延迟启动时间(毫秒)。
* duration 时长(毫秒)。
* yoyo\_times 往返的次数(x2)，为0视为永久播放。
* repeat\_times 重复的次数，为0视为永久播放。

#### 2. widget\_animator\_move动画的参数
* x\_from x起始位置。
* y\_from y起始位置。
* x\_to x结束位置。
* y\_to y结束位置。

#### 3. widget\_animator\_value动画的参数
* from 起始值。
* to 结束值。

#### 4. widget\_animator\_opacity动画的参数
* from 起始值(0-255)。
* to 结束值(0-255)。

#### 5. widget\_animator\_scale动画的参数
* x\_from x方向缩放起始值。
* y\_from y方向缩放起始值。
* x\_to x方向缩放结束值。
* y\_to y方向缩放结束值。

#### 6. widget\_animator\_rotation动画的参数
* from 起始值(弧度)。
* to 结束值(弧度)。

### 五、示例

完整示例请参考：demos/demo\_animator\_app.c

运行下列命令查看各种动画的效果：

```
./bin/demo_animator
```

### 五、其它
> 1.动画对象一般不需主动销毁，在动画结束或控件被销毁时自动销毁。
> 
> 2.如使用控件的入场动画，一般不要启用窗口动画。

