# LCD显示的实现

目前支持3类不同的LCD实现。

* 基于寄存器实现：lcd\_reg.inc 
* 基于framebuffer实现：lcd\_mem.inc  
* 基于vgcanvas实现(有GPU时才启用)：lcd\_vgcanvas.inc

> lcd\_mem\*.c/.h均由gen.sh根据template下的模板自动生成。支持新的格式可以修改gen.sh，并运行gen.sh。

> gen.sh是bash脚本，Windows下可在git bash下运行。
