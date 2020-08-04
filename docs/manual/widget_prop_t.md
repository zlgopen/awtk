## widget\_prop\_t
### 概述
控件的属性。
### 常量
<p id="widget_prop_t_consts">

| 名称 | 说明 | 
| -------- | ------- | 
| WIDGET\_PROP\_EXEC | 用于执行某些特殊的命令（比如控制动画的启停），主要是方便MVVM通过属性来控制动画。 |
| WIDGET\_PROP\_X | X坐标。 |
| WIDGET\_PROP\_Y | Y坐标。 |
| WIDGET\_PROP\_W | 宽度。 |
| WIDGET\_PROP\_H | 高度。 |
| WIDGET\_PROP\_INPUTING | inputing。 |
| WIDGET\_PROP\_CARET\_X | caret x。 |
| WIDGET\_PROP\_CARET\_Y | caret y。 |
| WIDGET\_PROP\_DIRTY\_RECT\_TOLERANCE | 脏矩形超出控件本身大小的最大范围。 |
| WIDGET\_PROP\_BIDI | bidi type(rtl,ltr,auto,wrtl,wltr,lro,rlo)。 |
| WIDGET\_PROP\_CANVAS | Canvas。 |
| WIDGET\_PROP\_LOCALIZE\_OPTIONS | Canvas。 |
| WIDGET\_PROP\_NATIVE\_WINDOW | Native Window。 |
| WIDGET\_PROP\_HIGHLIGHT | dialog highlight。 |
| WIDGET\_PROP\_BAR\_SIZE | slider中的bar的的宽度或高度。 |
| WIDGET\_PROP\_OPACITY | 不透明度。 |
| WIDGET\_PROP\_MIN\_W | 最小宽度。 |
| WIDGET\_PROP\_MAX\_W | 最大宽度。 |
| WIDGET\_PROP\_CHILDREN\_LAYOUT | 子控件布局参数。 |
| WIDGET\_PROP\_LAYOUT | 子控件布局参数(过时)。 |
| WIDGET\_PROP\_SELF\_LAYOUT | 控件布局参数。 |
| WIDGET\_PROP\_LAYOUT\_W | layout宽度。 |
| WIDGET\_PROP\_LAYOUT\_H | layout高度。 |
| WIDGET\_PROP\_VIRTUAL\_W | 虚拟宽度。 |
| WIDGET\_PROP\_VIRTUAL\_H | 虚拟高度。 |
| WIDGET\_PROP\_NAME | 名称。 |
| WIDGET\_PROP\_TYPE | 类型。 |
| WIDGET\_PROP\_CLOSABLE | 是否可以关闭。 |
| WIDGET\_PROP\_POINTER\_CURSOR | 鼠标指针。 |
| WIDGET\_PROP\_VALUE | 值。 |
| WIDGET\_PROP\_LENGTH | 长度。 |
| WIDGET\_PROP\_TEXT | 文本。 |
| WIDGET\_PROP\_TR\_TEXT | 待翻译文本。 |
| WIDGET\_PROP\_STYLE | style。 |
| WIDGET\_PROP\_ENABLE | 是否启用。 |
| WIDGET\_PROP\_FEEDBACK | 是否启用按键音等反馈。 |
| WIDGET\_PROP\_FLOATING | 是否启用floating布局。 |
| WIDGET\_PROP\_MARGIN | 边距。 |
| WIDGET\_PROP\_SPACING | 间距。 |
| WIDGET\_PROP\_LEFT\_MARGIN | 左边距。 |
| WIDGET\_PROP\_RIGHT\_MARGIN | 右边距。 |
| WIDGET\_PROP\_TOP\_MARGIN | 顶边距。 |
| WIDGET\_PROP\_BOTTOM\_MARGIN | 底边距。 |
| WIDGET\_PROP\_STEP | 步长。 |
| WIDGET\_PROP\_VISIBLE | 是否可见。 |
| WIDGET\_PROP\_SENSITIVE | 是否接受用户事件。 |
| WIDGET\_PROP\_ANIMATION | 控件动画。 |
| WIDGET\_PROP\_ANIM\_HINT | 窗口动画。 |
| WIDGET\_PROP\_FULLSCREEN | 窗口设置为全部大小。 |
| WIDGET\_PROP\_DISABLE\_ANIM | 禁用窗口动画。 |
| WIDGET\_PROP\_OPEN\_ANIM\_HINT | 打开窗口时的动画。 |
| WIDGET\_PROP\_CLOSE\_ANIM\_HINT | 关闭窗口时的动画。 |
| WIDGET\_PROP\_MIN | 最小值。 |
| WIDGET\_PROP\_ACTION\_TEXT | 软键盘上action按钮的文本。 |
| WIDGET\_PROP\_TIPS | 提示信息。 |
| WIDGET\_PROP\_TR\_TIPS | 需要翻译的提示信息。 |
| WIDGET\_PROP\_INPUT\_TYPE | 输入类型。 |
| WIDGET\_PROP\_KEYBOARD | 自定义软键盘资源名称。 |
| WIDGET\_PROP\_DEFAULT\_FOCUSED\_CHILD | 缺省获得焦点的子控件(可用控件名或类型)。 |
| WIDGET\_PROP\_READONLY | 只读模式。 |
| WIDGET\_PROP\_PASSWORD\_VISIBLE | 密码是否可见。 |
| WIDGET\_PROP\_ACTIVE | 是否处于active状态。 |
| WIDGET\_PROP\_VERTICAL | 是否为垂直模式。 |
| WIDGET\_PROP\_SHOW\_TEXT | 是否显示文本。 |
| WIDGET\_PROP\_XOFFSET | X方向的偏移。 |
| WIDGET\_PROP\_YOFFSET | Y方向的偏移。 |
| WIDGET\_PROP\_ALIGN\_V | 垂直对齐模式。 |
| WIDGET\_PROP\_ALIGN\_H | 水平对齐模式。 |
| WIDGET\_PROP\_AUTO\_PLAY | 是否自动播放或指定播放的时间。 |
| WIDGET\_PROP\_LOOP | 是否循环播放或循环播放的次数。 |
| WIDGET\_PROP\_AUTO\_FIX | 是否启用自动更正功能。 |
| WIDGET\_PROP\_SELECT\_NONE\_WHEN\_FOCUSED | 编辑器在获得焦点时是否不选中文本。 |
| WIDGET\_PROP\_OPEN\_IM\_WHEN\_FOCUSED | 编辑器在获得焦点时是否打开输入法。 |
| WIDGET\_PROP\_X\_MIN | X最小值。 |
| WIDGET\_PROP\_X\_MAX | X最大值。 |
| WIDGET\_PROP\_Y\_MIN | Y最小值。 |
| WIDGET\_PROP\_Y\_MAX | Y最大值。 |
| WIDGET\_PROP\_MAX | 最大值。 |
| WIDGET\_PROP\_GRAB\_KEYS | 让窗口管理器直接把按键发给自己。 |
| WIDGET\_PROP\_ROW | 行数或每行的高度。 |
| WIDGET\_PROP\_STATE\_FOR\_STYLE | 控件状态。 |
| WIDGET\_PROP\_THEME | 窗口主题名称。 |
| WIDGET\_PROP\_STAGE | window stage |
| WIDGET\_PROP\_IMAGE\_MANAGER | 图片管理器。 |
| WIDGET\_PROP\_ASSETS\_MANAGER | 资源管理器。 |
| WIDGET\_PROP\_LOCALE\_INFO | locale\_info。 |
| WIDGET\_PROP\_FONT\_MANAGER | 字体管理器。 |
| WIDGET\_PROP\_THEME\_OBJ | 窗口的主题对象。 |
| WIDGET\_PROP\_DEFAULT\_THEME\_OBJ | 缺省的主题对象。 |
| WIDGET\_PROP\_ITEM\_WIDTH | 项的宽度。 |
| WIDGET\_PROP\_ITEM\_HEIGHT | 项的高度。 |
| WIDGET\_PROP\_DEFAULT\_ITEM\_HEIGHT | 项的缺省高度。 |
| WIDGET\_PROP\_XSLIDABLE | X方向是否可拖动。 |
| WIDGET\_PROP\_YSLIDABLE | Y方向是否可拖动。 |
| WIDGET\_PROP\_REPEAT | 重复次数。 |
| WIDGET\_PROP\_ENABLE\_LONG\_PRESS | 是否启用长按。 |
| WIDGET\_PROP\_ANIMATABLE | 是否启用动画。 |
| WIDGET\_PROP\_AUTO\_HIDE\_SCROLL\_BAR | 是否自动隐藏滚动条。 |
| WIDGET\_PROP\_IMAGE | 图片名称。 |
| WIDGET\_PROP\_FORMAT | 显示格式。 |
| WIDGET\_PROP\_DRAW\_TYPE | 图片绘制类型。 |
| WIDGET\_PROP\_SELECTABLE | 是否可选择。 |
| WIDGET\_PROP\_CLICKABLE | 是否可点击。 |
| WIDGET\_PROP\_SCALE\_X | X方向缩放比例。 |
| WIDGET\_PROP\_SCALE\_Y | Y方向缩放比例。 |
| WIDGET\_PROP\_ANCHOR\_X | x锚点。 |
| WIDGET\_PROP\_ANCHOR\_Y | y锚点。 |
| WIDGET\_PROP\_ROTATION | 选中角度(幅度) |
| WIDGET\_PROP\_COMPACT | 紧凑模式。 |
| WIDGET\_PROP\_SCROLLABLE | 是否支持滚动。 |
| WIDGET\_PROP\_ICON | 图标名称。 |
| WIDGET\_PROP\_OPTIONS | 选项集合。 |
| WIDGET\_PROP\_SELECTED | 是否被选中。 |
| WIDGET\_PROP\_CHECKED | 是否被勾选。 |
| WIDGET\_PROP\_ACTIVE\_ICON | active状态下的图标。 |
| WIDGET\_PROP\_LOAD\_UI | 动态加载UI名字。 |
| WIDGET\_PROP\_OPEN\_WINDOW | 要打开窗口的名称。 |
| WIDGET\_PROP\_SELECTED\_INDEX | 被选中项的索引。 |
| WIDGET\_PROP\_CLOSE\_WHEN\_CLICK | 点击窗口时关闭窗口。 |
| WIDGET\_PROP\_CLOSE\_WHEN\_CLICK\_OUTSIDE | 点击窗口外部时关闭窗口。 |
| WIDGET\_PROP\_LINE\_GAP | 行间距。 |
| WIDGET\_PROP\_BG\_COLOR | 背景颜色(仅仅使用于color tile)。 |
| WIDGET\_PROP\_BORDER\_COLOR | 边框颜色(仅仅使用于color tile)。 |
| WIDGET\_PROP\_DELAY | 延迟时间(毫秒) |
| WIDGET\_PROP\_IS\_KEYBOARD | 是否为键盘。 |
| WIDGET\_PROP\_FOCUSED | 是否为焦点控件。 |
| WIDGET\_PROP\_FOCUS | (过时请用focused)。 |
| WIDGET\_PROP\_FOCUSABLE | 是否支持焦点停留。 |
| WIDGET\_PROP\_WITH\_FOCUS\_STATE | 是否支持焦点状态(如果希望style支持焦点状态，但有不希望焦点停留，可用本属性)。 |
| WIDGET\_PROP\_MOVE\_FOCUS\_PREV\_KEY | 将焦点移到前一个的键值。 |
| WIDGET\_PROP\_MOVE\_FOCUS\_NEXT\_KEY | 将焦点移到后一个的键值。 |
| WIDGET\_PROP\_MOVE\_FOCUS\_UP\_KEY | 将焦点向上移动的键值。 |
| WIDGET\_PROP\_MOVE\_FOCUS\_DOWN\_KEY | 将焦点向下移动的键值。 |
| WIDGET\_PROP\_MOVE\_FOCUS\_LEFT\_KEY | 将焦点向左移动的键值。 |
| WIDGET\_PROP\_MOVE\_FOCUS\_RIGHT\_KEY | 将焦点向右移动的键值。 |
