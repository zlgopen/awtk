## event\_type\_t
### 概述
类型常量定义。
### 常量
<p id="event_type_t_consts">

| 名称 | 说明 | 
| -------- | ------- | 
| EVT\_POINTER\_DOWN | 指针按下事件名(pointer\_event\_t)。 |
| EVT\_POINTER\_DOWN\_BEFORE\_CHILDREN | 指针按下事件名，在子控件处理之前触发(pointer\_event\_t)。 |
| EVT\_POINTER\_MOVE | 指针移动事件名(pointer\_event\_t)。 |
| EVT\_POINTER\_MOVE\_BEFORE\_CHILDREN | 指针移动事件名，在子控件处理之前触发(pointer\_event\_t)。 |
| EVT\_POINTER\_UP | 指针抬起事件名(pointer\_event\_t)。 |
| EVT\_POINTER\_UP\_BEFORE\_CHILDREN | 指针抬起事件名，在子控件处理之前触发(pointer\_event\_t)。 |
| EVT\_WHEEL | 滚轮事件名(wheel\_event\_t)。 |
| EVT\_WHEEL\_BEFORE\_CHILDREN | 鼠标滚轮事件名，在子控件处理之前触发(wheel\_event\_t)。 |
| EVT\_POINTER\_DOWN\_ABORT | 取消前一个指针按下事件名(pointer\_event\_t)。 |
| EVT\_CONTEXT\_MENU | 右键/长按弹出上下文菜单的事件名(pointer\_event\_t)。 |
| EVT\_POINTER\_ENTER | 指针进入事件名(pointer\_event\_t)。 |
| EVT\_POINTER\_LEAVE | 指针离开事件名(pointer\_event\_t)。 |
| EVT\_LONG\_PRESS | 长按事件名(pointer\_event\_t)。 |
| EVT\_CLICK | 点击事件名(pointer\_event\_t)。 |
| EVT\_FOCUS | 得到焦点事件名(event\_t)。 |
| EVT\_BLUR | 失去焦点事件名(event\_t)。 |
| EVT\_KEY\_DOWN | 键按下事件名(key\_event\_t)。 |
| EVT\_KEY\_DOWN\_BEFORE\_CHILDREN | 键按下事件名，在子控件处理之前触发(key\_event\_t)。 |
| EVT\_KEY\_REPEAT | 按键repeat事件名(key\_event\_t)。 |
| EVT\_KEY\_UP | 键抬起事件名(key\_event\_t)。 |
| EVT\_KEY\_UP\_BEFORE\_CHILDREN | 键抬起事件名，在子控件处理之前触发(key\_event\_t)。 |
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
| EVT\_PAINT\_DONE | 绘制完成(canvas状态已经恢复)的事件名(paint\_event\_t)。 |
| EVT\_LOCALE\_CHANGED | locale改变的事件(event\_t)。 |
| EVT\_ANIM\_START | 控件动画开始事件(event\_t)。 |
| EVT\_ANIM\_STOP | 控件动画被主动停止的事件(event\_t)。 |
| EVT\_ANIM\_PAUSE | 控件动画被暂停的事件(event\_t)。 |
| EVT\_ANIM\_ONCE | 控件动画yoyo/repeat时，完成一次的事件(event\_t)。 |
| EVT\_ANIM\_END | 控件动画完成事件(event\_t)。 |
| EVT\_WINDOW\_LOAD | 窗口加载完成事件(event\_t)。 |
| EVT\_WIDGET\_LOAD | 控件加载完成事件(event\_t)。 |
| EVT\_WINDOW\_WILL\_OPEN | 窗口即将打开事件(event\_t)。如果有窗口动画，在窗口动画开始前触发。如果没有窗口动画，在窗口被加载后的下一次循环中触发。 |
| EVT\_WINDOW\_OPEN | 窗口打开事件(event\_t)。如果有窗口动画，在窗口动画完成时触发。如果没有窗口动画，在窗口被加载后的下一次循环中触发。 |
| EVT\_WINDOW\_TO\_BACKGROUND | 窗口被切换到后台事件(event\_t)。打开新窗口时，当前窗口被切换到后台时，对当前窗口触发本事件。 |
| EVT\_WINDOW\_TO\_FOREGROUND | 窗口被切换到前台事件(event\_t)。关闭当前窗口时，前一个窗口被切换到前台时，对前一个窗口触发本事件。 |
| EVT\_WINDOW\_CLOSE | 窗口关闭事件。 |
| EVT\_REQUEST\_CLOSE\_WINDOW | 请求关闭窗口的事件(event\_t)。 |
| EVT\_TOP\_WINDOW\_CHANGED | 顶层窗口改变的事件(window\_event\_t)。 |
| EVT\_IM\_COMMIT | 输入法提交输入的文本事件(im\_commit\_event\_t)。 |
| EVT\_IM\_PREEDIT | 进入预编辑状态(event\_t)。 |
| EVT\_IM\_PREEDIT\_CONFIRM | 确认预编辑内容，退出预编辑状态(event\_t)。 |
| EVT\_IM\_PREEDIT\_ABORT | 删除预编辑内容，退出预编辑状态event\_t)。 |
| EVT\_IM\_SHOW\_CANDIDATES | 输入法请求显示候选字事件(im\_candidates\_event\_t)。 |
| EVT\_IM\_SHOW\_PRE\_CANDIDATES | 输入法请求显示预候选字事件(im\_candidates\_event\_t)。 |
| EVT\_IM\_LANG\_CHANGED | 输入法语言改变事件(event\_t)。 |
| EVT\_IM\_ACTION | 软键盘Action点击事件(event\_t)。 |
| EVT\_IM\_ACTION\_INFO | 请求更新软键盘上的Action按钮的信息(im\_action\_button\_info\_event\_t)。 |
| EVT\_DRAG\_START | 开始拖动(event\_t)。 |
| EVT\_DRAG | 拖动(event\_t)。 |
| EVT\_DRAG\_END | 结束拖动(event\_t)。 |
| EVT\_RESET | Reset(event\_t)。 |
| EVT\_SCREEN\_SAVER | 在指定的时间内(WITH\_SCREEN\_SAVER\_TIME)，没有用户输入事件，由窗口管理器触发。 |
| EVT\_LOW\_MEMORY | 内存不足(event\_t)。 |
| EVT\_OUT\_OF\_MEMORY | 内存耗尽(event\_t)。 |
| EVT\_ORIENTATION\_WILL\_CHANGED | 屏幕即将旋转(event\_t)。 |
| EVT\_ORIENTATION\_CHANGED | 屏幕旋转(event\_t)。 |
| EVT\_WIDGET\_CREATED | 控件创建事件(event\_t)。 |
| EVT\_REQUEST\_QUIT\_APP | 请求退出应用程序事件。点击原生窗口关闭按钮时，通过窗口管理器触发，注册该事件并返回RET\_STOP，可以阻止窗口关闭。 |
| EVT\_THEME\_CHANGED | 主题变化(event\_t)。 |
| EVT\_WIDGET\_ADD\_CHILD | 控件加载新的子控件(event\_t)。 |
| EVT\_WIDGET\_REMOVE\_CHILD | 控件移除子控件(event\_t)。 |
| EVT\_SCROLL\_START | scroll view开始滚动(event\_t)。 |
| EVT\_SCROLL | scroll view滚动(event\_t)。 |
| EVT\_SCROLL\_END | scroll view结束滚动(event\_t)。 |
| EVT\_REQ\_START | event queue其它请求编号起始值。 |
| EVT\_USER\_START | 用户定义事件起始值。 |
| EVT\_NONE | 无效事件名称。 |
| EVT\_PROP\_WILL\_CHANGE | 对象的属性即将改变的事件名(prop\_change\_event\_t)。 |
| EVT\_PROP\_CHANGED | 对象的属性改变的事件名(prop\_change\_event\_t)。 |
| EVT\_CMD\_WILL\_EXEC | 对象即将执行命令(cmd\_exec\_event\_t)。 |
| EVT\_CMD\_EXECED | 对象完成执行命令(cmd\_exec\_event\_t)。 |
| EVT\_CMD\_CAN\_EXEC | 对象命令是否能执行指定的命令(cmd\_exec\_event\_t)。 |
| EVT\_ITEMS\_WILL\_CHANGE | 即将增加和删除集合中的项目(event\_t)。 |
| EVT\_ITEMS\_CHANGED | 完成增加和删除集合中的项目(event\_t)。 |
| EVT\_PROPS\_CHANGED | 对象的属性改变的事件名(props\_event\_t)。 |
| EVT\_PROGRESS | 进度状态(progress\_event\_t)。 |
| EVT\_DONE | 完成(done\_event\_t)。 |
| EVT\_ERROR | 错误(error\_event\_t)。 |
| EVT\_DESTROY | 对象销毁事件名(event\_t)。 |
