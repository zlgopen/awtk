The Anti-Grain Geometry Project
A high quality rendering engine for C++
http://antigrain.com

Anti-Grain Geometry
Copyright (C) 2002-2005 Maxim Shemanarev (McSeem) 

Permission to copy, use, modify, sell and distribute this software 
is granted provided this copyright notice appears in all copies. 
This software is provided "as is" without express or implied
warranty, and with no claim as to its suitability for any purpose.

By default only the examples that do not require extra dependancies are built.
with the following commands:

cd (AGGDIRECTORY)/examples/X11
make clean
make

Some examples (freetype_test, trans_curve1_ft and trans_curve2_ft) require the Freetype Library, and can be built with:

make freetype

There is also an example (gpc_test) that requires the GPC library, build it with:

make gpc

Of course, to build ALL examples run:

make all

Alternatively you can also build the examples one by one, using the example name directly:

make aa_demo
make aa_test
...