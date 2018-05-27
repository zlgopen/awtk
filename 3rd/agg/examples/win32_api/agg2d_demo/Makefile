#
# This makefile can be used to build a Win32 application under Cygwin
#

include ../../../Makefile.in.$(shell uname)

PROGNAME=agg2d_demo
OUTNAME=$(PROGNAME)
PLATFORM=win32

CXXFLAGS= $(AGGCXXFLAGS) -I../../../include \
-I../../../agg2d \
-I../../../font_win32_tt \
-I/usr/X11R6/include \
-L/usr/X11R6/lib \
-L../../../src \
$(PIXFMT)

LIBS = $(AGGLIBS) -lm -lgdi32

SRC=\
../../$(PROGNAME).cpp \
../../../agg2d/agg2d.cpp \
../../../src/platform/$(PLATFORM)/agg_platform_support.cpp \
../../../src/platform/$(PLATFORM)/agg_win32_bmp.cpp \
../../../font_win32_tt/agg_font_win32_tt.cpp

OBJ= $(SRC:.cpp=.o)

$(OUTNAME):	$(OBJ)
	$(CXX) $(CXXFLAGS)  $^ -o $(OUTNAME) $(LIBS)

clean:
	rm -f $(OUTNAME)
	rm -f ../../$(PROGNAME).o
	rm -f ../../../agg2d/agg2d.o
	rm -f ../../../src/platform/$(PLATFORM)/agg_platform_support.o
	rm -f ../../../src/platform/$(PLATFORM)/agg_win32_bmp.o
	rm -f gamma.txt
	rm -f gamma.bin

%.o:	%.cpp
	@echo \< $*.cpp \>
	$(CXX) -c $(CXXFLAGS) $*.cpp -o $@


