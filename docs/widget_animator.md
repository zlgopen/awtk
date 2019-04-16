## 控件动画

控件动画是一种很常见的动画，常用于入场动画、离场动画、装饰用户界面和吸引用户注意力等。

### 一、动画类型

AWTK目前支持的动画有：

* move：通过改变控件的位置形成动画效果。
* value：通过改变控件的值形成动画效果。
* opacity：通过改变控件的透明度形成动画效果。
* scale：通过改变控件的缩放比例形成动画效果(目前需要vgcanvas)。
* rotation：通过改变控件的旋转角度形成动画效果(目前需要vgcanvas)。
* 其它任何数值型的属性。如x/y/w/h等等。

### 二、主要特色

* 支持逆向模式。
* 支持停止和暂停。
* 支持定时(延迟)执行。
* 支持重复模式(可指定次数)。
* 支持往返模式(可指定次数)。
* 支持多种插值算法(如加速和减速等)。
* 支持同一控件多个动画并行执行。
* 支持同一控件多个动画串行执行。
* 支持时间倍率，让时间变快和变慢。
* 支持按名称去开始、暂停、停止和销毁动画。

### 三、使用方法

#### 1.方法一：通过函数widget\_create\_animator。

示例：

1.创建动画

```
widget_create_animator(image1, "opacity(to=0, duration=500, yoyo_times=1000)");
```

2.暂停指定的动画

```
widget_pause_animator(image1, "opacity");
```

3.暂停全部动画

```
widget_pause_animator(NULL, NULL);
```

4.恢复/启动全部动画

```
widget_start_animator(NULL, NULL);
```
 
> 在缺省情况下，动画创建后自动启动，完成后自动销毁。可以指定auto\_start=false禁止创建后自动启动，指定参数auto_destroy=false禁止完成时自动销毁(控件销毁时仍然会自动销毁)。

动画创建、播放、暂停、停止、设置时间倍率和销毁等相关函数如下：

```
/**
 * @method widget_create_animator
 * 创建动画。
 * 除非指定auto_start=false，动画创建后自动启动。
 * 除非指定auto_destroy=false，动画播放完成后自动销毁。
 * 参数的格式类似函数调用, 如:
 * opacity(from=0, to=255, yoyo_times=1000, duration=1000)
 * move(x_from=10, x_to=100, y_from=10, y_to=100, duration=1000)
 * 参考：https://github.com/zlgopen/awtk/blob/master/docs/widget_animator.md
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} animation 动画参数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_create_animator(widget_t* widget, const char* animation);

/**
 * @method widget_start_animator
 * 播放动画。
 * 1.widget为NULL时，播放所有名称为name的动画。
 * 2.name为NULL时，播放所有widget相关的动画。
 * 3.widget和name均为NULL，播放所有动画。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 动画名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_start_animator(widget_t* widget, const char* name);

/**
 * @method widget_set_animator_time_scale
 * 设置动画的时间倍率，<0: 时间倒退，<1: 时间变慢，>1 时间变快。
 * 1.widget为NULL时，设置所有名称为name的动画的时间倍率。
 * 2.name为NULL时，设置所有widget相关的动画的时间倍率。
 * 3.widget和name均为NULL，设置所有动画的时间倍率。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} name 动画名称。
 * @param {float_t} time_scale 时间倍率。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_set_animator_time_scale(widget_t* widget, const char* name, float_t time_scale);

/**
 * @method widget_pause_animator
 * 暂停动画。
 * 1.widget为NULL时，暂停所有名称为name的动画。
 * 2.name为NULL时，暂停所有widget相关的动画。
 * 3.widget和name均为NULL，暂停所有动画。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {char*} name 动画名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_pause_animator(widget_t* widget, const char* name);

/**
 * @method widget_stop_animator
 * 停止动画(控件的相应属性回归原位)。
 * 1.widget为NULL时，停止所有名称为name的动画。
 * 2.name为NULL时，停止所有widget相关的动画。
 * 3.widget和name均为NULL，停止所有动画。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {char*} name 动画名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_stop_animator(widget_t* widget, const char* name);

/**
 * @method widget_destroy_animator
 * 销毁动画。
 * 1.widget为NULL时，销毁所有名称为name的动画。
 * 2.name为NULL时，销毁所有widget相关的动画。
 * 3.widget和name均为NULL，销毁所有动画。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {char*} name 动画名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_destroy_animator(widget_t* widget, const char* name);
```

#### 2.方法二：在XML中指定

animation参数的格式，类似与函数调用。多个参数可以用『;』分隔。如：

```
 <button name="inc_value" text="Inc" animation="opacity(from=0, to=255, yoyo_times=1000, duration=1000)"/>
 <button name="dec_value" text="Dec" animation="move(x_from=10, x_to=100, y_from=10, y_to=100, duration=1000);opacity(from=0, to=255, yoyo_times=1000, duration=1000)"/>
```

> 此方法最为简单，后续可以在程序中通过动画名字，对动画进行启动和暂停等控制。

#### 3.方法三：直接使用(不再推荐)

使用控件动画时，一般需要创建动画、设置参数、设置模式和启动动画四个步骤。如：

```
  animator = widget_animator_move_create(image, 1000, delay, EASING_SIN_INOUT);
  widget_animator_move_set_params(animator, image->x, image->y, image->x + 100, image->y + 100);
  widget_animator_set_yoyo(animator, 10);
  widget_animator_start(animator);
```

> 此方法比较麻烦，不再推荐使用。


### 四、可用参数

#### 1.公共参数

* name 动画名称(缺省为动画的类型如move)。
* delay 延迟启动时间(毫秒)。
* duration 时长(毫秒)。
* easing 插值算法(见后面描述)。
* yoyo\_times 往返的次数(x2)，为0视为永久播放。
* repeat\_times 重复的次数，为0视为永久播放。
* auto\_start 创建后自动启动(缺省为true)。
* auto\_destroy 完成后自动销毁(缺省为true)。

> 以下参数的缺省值为控件当前对应的值。

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

#### 7. 其它数值型属性动画的参数
* from 起始值。
* to 结束值。

### 五、插值算法名称(easing)

请参考：[插值算法名称](easing.md)

### 六、完整示例

完整示例请参考：demos/demo\_animator\_app.c

运行下列命令查看各种动画的效果：

```
./bin/demo_animator
```

### 五、其它
 1.动画对象一般不需主动销毁，在动画结束或控件被销毁时自动销毁。
 
 2.如使用控件的入场动画，一般不要启用窗口动画。

