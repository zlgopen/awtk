Anti-Grain Geometry (AGG) - Version 2.5
A high quality rendering engine for C++
Copyright (C) 2002-2006 Maxim Shemanarev
Contact: mcseem@antigrain.com
         mcseemagg@yahoo.com
         http://antigrain.com

AGG is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

AGG is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with AGG; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, 
MA 02110-1301, USA.



---------------------------------

Use automake to build the library.

If automake is not available you still can use the old make.
There is a very simple Makefile that can be used. Note that
if you use automake it will overwrite Makefile.

---------------------------------

If building on AmigaOS 4.0 or higher type the following for
instructions on what targets are available.
 make -f Makefile.AmigaOS

To just build and install AGG into the standard AmigaOS SDK
ready for use type:
 make -f Makefile.AmigaOS install

If you just want to build one demo (e.g. lion) use:
 make -f Makefile.AmigaOS bin/lion

If you have any questions about the AmigaOS port please
contact Steven Solie (ssolie@telus.net) for help.

---------------------------------

To build all examples using SDL (Mac or Linux) just type:

cd /examples/sdl
make

Individual examples can be built with

make aa_test

In the same way the native Carbon examples can be built with

cd /examples/macosx_carbon
make

In both cases the static library will be built (if it was not already) 
from the existing global Makefile in /src/.

The Makefiles for both SDL and Carbon will also attempt to download the 
required .bmp files if they are not found in the system for a given 
example. If the files could not be fetched (wget) the user will receive 
a message explaining where to download the samples from (sphere.bmp, 
etc.)  Since all programs reside in the same directory there is no need 
to duplicate the .bmp files for each program that needs to use them.

---------------------------------
