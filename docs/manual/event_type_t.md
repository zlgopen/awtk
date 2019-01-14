## event\_type\_t
### 概述
 类型常量定义。

### 常量
<p id="event_type_t_consts">

| 名称 | 说明 | 
| -------- | ------- | 
| EVT\_NONE | 无效事件名称。 |
| EVT\_POINTER\_DOWN | 指针按下事件名(pointer\_event\_t)。 |
| EVT\_POINTER\_DOWN\_ABORT | 取消前一个指针按下事件名(pointer\_event\_t)。 |
| EVT\_POINTER\_MOVE | 指针移动事件名(pointer\_event\_t)。 |
| EVT\_POINTER\_UP | 指针抬起事件名(pointer\_event\_t)。 |
| EVT\_WHEEL | 滚轮事件名(pointer\_event\_t)。 |
| EVT\_CONTEXT\_MENU | 右键/长按弹出上下文菜单的事件名(pointer\_event\_t)。 |
| EVT\_POINTER\_ENTER | 指针进入事件名(pointer\_event\_t)。 |
| EVT\_POINTER\_LEAVE | 指针离开事件名(pointer\_event\_t)。 |
| EVT\_LONG\_PRESS | 长按事件名(pointer\_event\_t)。 |
| EVT\_CLICK | 点击事件名(pointer\_event\_t)。 |
| EVT\_FOCUS | 得到焦点事件名(event\_t)。 |
| EVT\_BLUR | 失去焦点事件名(event\_t)。 |
| EVT\_KEY\_DOWN | 键按下事件名(key\_event\_t)。 |
| EVT\_KEY\_REPEAT | 按键repeat事件名(key\_event\_t)。 |
| EVT\_KEY\_UP | 键抬起事件名(key\_event\_t)。 |
| EVT\_WILL\_MOVE | 即将移动Widget的事件名(event\_t)。 |
| EVT\_MOVE | 移动Widget的事件名(event\_t)。 |
| EVT\_WILL\_RESIZE | 即将调整Widget大小的事件名(event\_t)。 |
| EVT\_RESIZE | 调整Widget大小的事件名(event\_t)。 |
| EVT\_WILL\_MOVE\_RESIZE | 即将调整Widget大小/位置的事件名(event\_t)。 |
| EVT\_MOVE\_RESIZE | 调整Widget大小/位置的事件名(event\_t)。 |
| EVT\_VALUE\_WILL\_CHANGE | 控件的值即将改变的事件名(event\_t)。 |
| EVT\_VALUE\_CHANGED | 控件的值改变的事件名(event\_t)。 |
| EVT\_VALUE\_CHANGING | 控件的值持续改变(如编辑器正在编辑)的事件名(event\_t)。 |
| EVT\_PAINT | 绘制的事件名(paint\_event\_t)。 |
| EVT\_BEFORE\_PAINT | 即将绘制的事件名(paint\_event\_t)。 |
| EVT\_AFTER\_PAINT | 绘制完成的事件名(paint\_event\_t)。 |
| EVT\_LOCALE\_CHANGED | locale改变的事件(event\_t)。 |
| EVT\_ANIM\_START | 控件动画开始事件(event\_t)。 |
| EVT\_ANIM\_STOP | 控件动画被主动停止的事件(event\_t)。 |
| EVT\_ANIM\_PAUSE | 控件动画被暂停的事件(event\_t)。 |
| EVT\_ANIM\_ONCE | 控件动画yoyo/repeat时，完成一次的事件(event\_t)。 |
| EVT\_ANIM\_END | 控件动画完成事件(event\_t)。 |
| EVT\_WINDOW\_LOAD | 窗口加载完成事件(event\_t)。 |
| EVT\_WINDOW\_WILL\_OPEN | 窗口即将打开事件(event\_t)。
 如果有窗口动画，在窗口动画开始前触发。如果没有窗口动画，在窗口被加载后的下一次循环中触发。 |
| EVT\_WINDOW\_OPEN | 窗口打开事件(event\_t)。
 如果有窗口动画，在窗口动画完成时触发。如果没有窗口动画，在窗口被加载后的下一次循环中触发。 |
| EVT\_WINDOW\_CLOSE | 窗口关闭事件。 |
| EVT\_REQUEST\_CLOSE\_WINDOW | 请求关闭窗口的事件(event\_t)。 |
| EVT\_TOP\_WINDOW\_CHANGED | 顶层窗口改变的事件(window\_event\_t)。 |
| EVT\_IM\_COMMIT | 输入法提交输入的文本事件(im\_commit\_event\_t)。 |
| EVT\_IM\_SHOW\_CANDIDATES | 输入法请求显示候选字事件(im\_candidates\_event\_t)。 |
| EVT\_IM\_ACTION | 软键盘Action点击事件(event\_t)。 |
| EVT\_IM\_ACTION\_INFO | 请求更新软键盘上的Action按钮的信息(im\_action\_button\_info\_event\_t)。 |
| EVT\_DRAG\_START | 开始拖动(event\_t)。 |
| EVT\_DRAG | 拖动(event\_t)。 |
| EVT\_DRAG\_END | 结束拖动(event\_t)。 |
| EVT\_REQ\_START | event queue其它请求编号起始值。 |
| EVT\_USER\_START | 用户定义事件起始值。 |
