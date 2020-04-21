# 输入法引擎实现

目前只支持中文输入，如果有多语言输入需求，请找专业的输入法公司合作。

这里提供的输入引擎，主要用作参考，请自行优化和裁剪。

> 如果不启用输入法，可以不用加任何输入法引擎。

## 1.输入引擎介绍

### 1.1 null 输入引擎法

* 文件 input\_engine\_null.c   
* 宏：WITH\_IME\_NULL

> 如果不需要中文输入法，但又需要软键盘，请启用本引擎。

### 1.2 Goole 拼音输入引擎

* 文件：input\_engine\_pinyin.cpp
* 宏：WITH\_IME\_PINYIN

> 如果需要拼音输入法，推荐使用本引擎。

### 1.3 简单的拼音输入引擎

* 文件：input\_engine\_spinyin.c  
* 宏：WITH\_IME\_SPINYIN

> 优点是简单，如果不想用 Goole 拼音输入引擎，可以使用本引擎。

### 1.4 带触屏的 T9 输入法引擎

* 文件：input\_engine\_t9.c       
* 宏：WITH\_IME\_SPINYIN：
* 软键盘：
    * 缺省软键盘：demos/assets/default/raw/ui/kb\_default\_t9.xml
    * ascii 软键盘：demos/assets/default/raw/ui/kb\_ascii\_t9.xml

> 如果需要 T9 输入法，带有触摸屏，请使用本引擎。

### 1.5 不带触屏的 T9 输入法引擎

* 文件：input\_engine\_t9ext.c
* 宏：WITH\_IME\_T9EXT
* 软键盘：
    * 缺省软键盘：demos/assets/default/raw/ui/kb\_default\_t9ext.xml

> 如果需要 T9 输入法，而且没有触摸屏，使用外部硬键盘，请使用本引擎。

> 在PC上模拟测试时，可以用ESC键切换输入模式，在嵌入式平台可以重定义宏SWITCH\_INPUT\_MODE\_KEY来指定切换的按键。

## 2.数据生成

* t9\_en\_us.inc	由 tools/ime\_gen/to\_t9\_en.js  根据 english\_words.txt 生成。
* pinyin\_table.inc 由 tools/ime\_gen/to\_pinyin.js 根据 chinese\_words.txt 生成。	
* t9\_zh\_cn.inc	由 tools/ime\_gen/to\_t9\_zh.js 根据 chinese\_words.txt 生成。	
* t9ext\_zh\_cn.inc 由 tools/ime\_gen/to\_t9ext\_zh.js 根据 chinese\_words.txt 生成。	 

如果需要裁剪，请编辑相应的输入文件，并重新生成。

## 3. Scons 编译脚本

编辑 awtk\_config.py 中的 INPUT\_ENGINE，可以选择不同的输入法引擎。

```
#INPUT_ENGINE='null'
#INPUT_ENGINE='spinyin'
#INPUT_ENGINE='t9'
#INPUT_ENGINE='t9ext'
INPUT_ENGINE='pinyin'
```

## 4. 指定软键盘

软键盘必须和输入法引擎配套使用 (T9/T9EXT)，可以替换默认的软键盘 UI 文件，也可以在编辑器的属性中指定软键盘。

如：

```xml
<window anim_hint="htranslate" >
  <edit x="10" y="10" w="-20" h="30" keyboard="kb_default_t9ext" focused="true" />
  <edit x="10" y="50" w="-20" h="30" keyboard="kb_default_t9ext" focused="true" />
  <mledit x="10" y="90" w="-20" h="80" keyboard="kb_default_t9ext" focused="true" />
</window>
```

