#
# This makefile can be used to build a Win32 application under Cygwin
#

include ../../../Makefile.in.$(shell uname)

PROGNAME=gpc_test
OUTNAME=$(PROGNAME)
PLATFORM=win32

CXXFLAGS= $(AGGCXXFLAGS) -I../../../include -I../../../gpc \
-I/usr/X11R6/include \
-L/usr/X11R6/lib \
-L../../../src \
$(PIXFMT)

LIBS = $(AGGLIBS) -lm -lgdi32

SRC=\
../../$(PROGNAME).cpp \
../../make_arrows.cpp \
../../make_gb_poly.cpp \
../../../src/platform/$(PLATFORM)/agg_platform_support.cpp \
../../../src/platform/$(PLATFORM)/agg_win32_bmp.cpp

OBJ= $(SRC:.cpp=.o)

$(OUTNAME):	$(OBJ)
	$(CXX) $(CXXFLAGS)  $^ -o $(OUTNAME) $(LIBS)

clean:
	rm -f $(OUTNAME)
	rm -f ../../*.o
	rm -f ../../../src/platform/$(PLATFORM)/agg_platform_support.o
	rm -f gamma.txt
	rm -f gamma.bin

%.o:	%.cpp
	@echo \< $*.cpp \>
	$(CXX) -c $(CXXFLAGS) $*.cpp -o $@


