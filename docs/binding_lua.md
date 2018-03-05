# LFTK脚本绑定实现原理 - lua

脚本化是[LFTK](https://github.com/xianjimli/lftk)的一个基本功能。[LFTK](https://github.com/xianjimli/lftk)计划支持嵌入式系统中常用的脚本，如lua、python和jerryscript。脚本绑定说简单也简单，说复杂也复杂，采用FFI(calling external C functions)和SWIG绑定一些简单的函数是很方便的，但绑定一个复杂的GUI系统还是很有挑战的。之所以不采用通用的工具，而是实现自己的代码产生器，主要有以下考虑：

* 我使用FFI和SWIG的经验很有限，觉得有些功能不太好实现，至少目前我还不知道实现这些功能的方法和思路。

* 担心FFI和SWIG的运行环境的可移植性。在Windows、Linux和Macos上肯定是没有问题的，但是在嵌入式系统，特别是RTOS和裸系统中，支持像动态库，甚至文件系统可能都是困难的。

* 对jerryscript没有支持。jerryscript是三星开源的javascript实现，在嵌入式系统中用得比较多，通用的绑定机制对jerryscript没有支持。

* 代码风格。不同语言有不同的代码风格，特别是命名风格，直接把C言语的风格映射过去，对于开发者不太友好。FFI和SWIG都需要做额外的工作才能实现这个功能。

* LFTK是完全采用面向对象设计，并用C实现面向对象编程的。而通用的绑定机制对此并不友好，实现起来非常困难。
 
* 自定义的代码产生器并不复杂，而且具有更大的灵活性。 

基于以上这些原因，我决定自己实现[LFTK](https://github.com/xianjimli/lftk)的脚本绑定机制。它的实现原理如下：用特定格式的API注释来描述要脚本化的API，用一个名为gen_idl的工具把注释提取出来生成JSON的接口描述文件，然后用不同的代码产生器生成各种语言的绑定：

![1](images/lftk_binding.png) 

## 注释格式

我采用了类型[jsduck](https://github.com/senchalabs/jsduck)的API注释格式，但是jsduck并不支持C语言的数据类型，完全要兼容jsduck也不可能。

### 一、类的注释

示例：

```
/**
 * @class progress_bar_t 
 * @parent widget_t
 * @scriptable
 * 进度条控件。
 */
```

里面说明了类的名称、基类的名称和该类型是否可以脚本化。

### 二、类成员变量注释

示例：

```
  /** 
   * @property {uint8_t} value 
   * @readonly
   * 进度条的值[0-100]。
   */
```

里面说明了成员变量的类型、名称和只读性等信息。

### 三、函数的注释

示例：

```
/**
 * @method progress_bar_set_value
 * 设置进度条的进度。
 * @param {widget_t*} widget 控件对象。
 * @param {uint8_t}  value 进度
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
```

里面说明了函数的名称、参数和返回值。缺省为前一个类的成员函数，也可以用@global把它标识为一个全局函数。构造函数和析构可能会有不同的处理，所以用@constructor和deconstructor分别标识出来。有的函数自动产生绑定代码比较困难，此时可以用@custom标识为手写绑定代码。

### 四、枚举的注释

示例：

```
/**
 * @enum align_v_t
 * @scriptable
 * 垂直对齐的常量定义。
 */
typedef enum _align_v_t {
  /**
   * @const ALIGN_V_NONE
   * 无效对齐方式。
   */
  ALIGN_V_NONE= 0,
  /**
   * @const ALIGN_V_MIDDLE
   * 居中对齐。
   */
  ALIGN_V_MIDDLE,
  /**
   * @const ALIGN_V_TOP
   * 顶部对齐。
   */
  ALIGN_V_TOP,
  /** 
   * @const ALIGN_V_BOTTOM
   * 底部对齐。
   */
  ALIGN_V_BOTTOM
}align_v_t;
```

里面定义了枚举的名称和各个值。


## 代码产生器

代码产生器本身很简单，有兴趣的朋友可以帮忙Review一下。

* JSON格式的代码产生器：tools/idl_gen/gen_idl.js

* Lua的绑定代码产生器：tools/idl_gen/gen_lua.js

## lua绑定

对lua绑定也是花了一些，由于对lua不熟悉，还特意买了两本书，翻了一下有些帮助，后来还是靠阅读lua的代码解决了问题。这里做个笔记，方便有需要的朋友参考：


### 一、全局函数的绑定

示例：

* 1.实现wrap函数

```
static int wrap_lftk_quit(lua_State* L) {
  ret_t ret = 0;
  ret = (ret_t)lftk_quit();

  lua_pushnumber(L,(lua_Number)(ret));

  return 1;
}
```

* 2.注册

```
  lua_pushcfunction(L, wrap_lftk_quit);
  lua_setglobal(L, "lftk_quit");
```

### 二、构造函数的绑定

* 1.实现wrap函数。构造函数的wrap函数并无多大差别，只是最后要调用lftk\_newuserdata创建一个userdata对象，并关联metatable。我开始用的lua\_pushlightuserdata函数，后来发现lua里所用的ligthuserdata都是用的一个metatable，修改一个对象的metatable，其它类型的对象的metatable也被修改了，我感觉这种做法并不合理。

```
static int wrap_button_create(lua_State* L) {
  widget_t* ret = NULL;
  widget_t* parent = (widget_t*)lftk_checkudata(L, 1, "widget_t");
  xy_t x = (xy_t)luaL_checkinteger(L, 2);
  xy_t y = (xy_t)luaL_checkinteger(L, 3);
  wh_t w = (wh_t)luaL_checkinteger(L, 4);
  wh_t h = (wh_t)luaL_checkinteger(L, 5);
  ret = (widget_t*)button_create(parent, x, y, w, h);

  return lftk_newuserdata(L, ret, "/button_t/widget_t", "lftk.button_t");
}
```

* 2.注册
```
static void button_t_init(lua_State* L) {
  static const struct luaL_Reg static_funcs[] = {
    {"create", wrap_button_create},
    {NULL, NULL}
  };
...
  luaL_openlib(L, "Button", static_funcs, 0);
}
```

### 三、成员函数的绑定

### 四、设置属性的函数的绑定

### 五、获取属性的函数的绑定

### 六、枚举的绑定

### 七、回调函数的处理

## lua示例

```
function application_init()
  local win = Window.create(nil, 0, 0, 0, 0)
  local inc = Button.create(win, 10, 5, 80, 30) 
  inc:set_text(to_wstr('Inc'));
 
  local dec = Button.create(win, 100, 5, 80, 30);
  dec:set_text(to_wstr('Dec'));
  
  local progress_bar = ProgressBar.create(win, 10, 80, 168, 30);
  progress_bar:set_value(40);
  
  inc:on(EventType.EVT_CLICK, function(evt) 
    local e = PointerEvent.cast(evt);
    progress_bar:set_value(progress_bar.value + 10);
    print('on inc click:(' .. tostring(e.x) .. ' ' .. tostring(e.y) .. ')')
  end);
  
  dec:on(EventType.EVT_CLICK, function(evt) 
    local e = PointerEvent.cast(evt);
    progress_bar:set_value(progress_bar.value - 10);
    print('on dec click:(' .. tostring(e.x) .. ' ' .. tostring(e.y) .. ')')
  end);
end

application_init()
```
