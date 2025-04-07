# 如何配置窗口中的 Enter 和 Esc 按键响应机制

## 基本用法

所有的窗口类型都有 accept_button 和 cancel_button 的属性，该属性是用来配置该窗口响应的 Enter 和 Esc 按键。

| 属性          | 用法                       | 作用                                                    |
| ------------- | -------------------------- | ------------------------------------------------------- |
| accept_button | 设置一个 button 的控件名字 | 当该窗口按下 Enter 的时候，就相当于点击了该 button 控件 |
| cancel_button | 设置一个 button 的控件名字 | 当该窗口按下 Esc 的时候，就相当于点击了该 button 控件   |

AWTK 本身支持使用 Tab 按键的切换控件焦点，当切换到 edit 控件的时候，accept_button 对应的 button 控件就会设置为 focused 风格，如果切换到某个控件，该控件设置 accept_return 为 true 或者 widget->vt->return_key_to_activate 为 true 的话（表示控件接管 Enter 按键事件），accept_button 对应的 button 控件就不会设置为 focused 风格，同时 Enter 按钮也不会响应点击 accept_button 对应的 button 控件。

## mledit 控件

该控件的 accept_return 和 accept_tab 属性都是为 true，默认会接管键盘的 Enter 和 Tab 按键，所以此时如果在 mledit 中按下 Tab 按键只会给控件输入一个 \\t 字符，不会切换到其他控件中，如果按下 Enter 按键只会输入 \\n 字符，不会触发窗口的 accept_button 对应的 button 控件。

| 属性          | 作用                                                         |
| ------------- | ------------------------------------------------------------ |
| accept_return | true 的话，就表示 Enter 按键归为控件处理，false 的话，就表示控件不接管 Enter 按键的响应，该响应会触发窗口的 accept_button 对应的 button 控件 |
| accept_tab    | true 的话，就表示 Tab 按键归为控件处理，false 的话，就表示控件不接管 Tab 按键的响应，Tab 按键就会切换控件焦点 |

给 mledit 控件增加了两个快捷按钮：

1. Ctrl + Enter ：输入一个 \\n 字符
2. Ctrl + Tab ：输入一个 \\t 字符