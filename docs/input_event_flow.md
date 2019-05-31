# 输入事件处理流程

本文介绍一下 AWTK 中 Pointer 事件和 Key 事件的处理流程。

## 一、事件处理的三个阶段

### 第一阶段：子控件处理前。

控件内部实现时，可以重载 on\_event\_before\_children 处理这个阶段的事件。

应用程序开发者可以注册 XXX\_BEFORE\_CHILDREN 之类的事件处理这个阶段的事件。这些事件包括：

* EVT\_POINTER\_DOWN\_BEFORE\_CHILDREN
* EVT\_POINTER\_MOVE\_BEFORE\_CHILDREN
* EVT\_POINTER\_UP\_BEFORE\_CHILDREN
* EVT\_KEY\_DOWN\_BEFORE\_CHILDREN
* EVT\_KEY\_UP\_BEFORE\_CHILDREN

### 第二阶段：子控件处理。

子控件处理时同样遵循这个三个阶段。

### 第三阶段：子控件处理后。

控件内部实现时，可以重载 on\_event 处理这个阶段的事件。

应用程序开发者可以注册正常的 pointer 事件和 key 事件处理这个阶段的事件。这些事件包括：

* EVT\_POINTER\_DOWN
* EVT\_POINTER\_MOVE
* EVT\_POINTER\_UP
* EVT\_KEY\_DOWN
* EVT\_KEY\_UP

## 二、事件终止

如果在某个阶段，处理事件后，不希望后面阶段继续处理，让事件处理函数返回 RET\_STOP 即可。
