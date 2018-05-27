AC_INIT(src/agg_arc.cpp) # give me a source file, any source file...
AC_CANONICAL_TARGET
AC_CONFIG_HEADERS(include/config.h)
AM_INIT_AUTOMAKE(agg, 2.4.0)


dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_ISC_POSIX
AM_C_PROTOTYPES
if test "x$U" != "x"; then
  AC_MSG_ERROR(Compiler not ANSI compliant)
fi
AM_PROG_LIBTOOL
AC_PROG_INSTALL

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

AC_ARG_ENABLE(examples,
    AC_HELP_STRING([--enable-examples],[Antigrain examples]))
AM_CONDITIONAL(ENABLE_EXAMPLES,test x$enable_examples != xno)

AC_ARG_ENABLE(ctrl,
    AC_HELP_STRING([--enable-ctrl],[a gui libray used in examples]))

AC_ARG_ENABLE(platform,
    AC_HELP_STRING([--enable-platform],[portable platform layers]))

if test x$enable_examples != xno  ; then 
  enable_platform="yes"
fi
if test x$enable_platform != xno  ; then 
  enable_ctrl="yes"
fi
AM_CONDITIONAL(ENABLE_CTRL,test x$enable_ctrl != xno)

# used as platform library in examples:
# todo, make the PREFERED_PLATFORM selectable, after the set of possible 
# Platforms to link the examples have been evaluated.
PREFERED_PLATFORM=X11
case "$host" in
  *darwin* )
    OSX_LIBS="-framework Carbon -framework QuickTime"
    OSX_CFLAGS="-I/System/Library/Frameworks/Carbon.framework/Headers -I/System/Library/Frameworks/QuickTime.framework/Headers "
    AC_SUBST(OSX_CFLAGS)
    AC_SUBST(OSX_LIBS)
    osx_host=yes
    PREFERED_PLATFORM=mac
  ;;
dnl ####  Check if we are compiling for win32 #####
  *mingw*)
    win32_host=yes
    WINDOWS_LIBS=-lgdi32
    WINDOWS_CFLAGS=
    AC_SUBST(WINDOWS_CFLAGS)
    AC_SUBST(WINDOWS_LIBS)
    PREFERED_PLATFORM=win32
  ;;
esac
AM_CONDITIONAL(ENABLE_WIN32,[test x$win32_host = xyes -a x$enable_platform != xno ])
AM_CONDITIONAL(ENABLE_OSX,[test x$osx_host = xyes -a x$enable_platform != xno ])
dnl then enable font_win32tt
AC_ARG_ENABLE(win32tt,
    AC_HELP_STRING([--enable-win32tt],[Win32 TrueType font support library]),
    enable_tt=$enable_win32tt,
    enable_tt=$win32_host)
AM_CONDITIONAL(ENABLE_WIN32_TT, test x$enable_tt = xyes )

dnl #########  Check for FT2: #####################
ft_enabled=""
PKG_CHECK_MODULES([FREETYPE],
    freetype2,
    [ft_enabled="yes"],
    AC_MSG_WARN([*** Freetype2 not found! Building without font library.])
    )
AC_ARG_ENABLE(freetype,
    AC_HELP_STRING([--enable-freetype],[freetype font support library]),
    ft_enabled=$enable_freetype)

AM_CONDITIONAL(ENABLE_FT,[test xyes = x$ft_enabled])
dnl ###############################################


dnl #########  Ask for GPC: #######################
AC_ARG_ENABLE(gpc,
    AC_HELP_STRING([--enable-gpc],[gpc polygon clipper library]) ) 

AM_CONDITIONAL(ENABLE_GPC,[test xyes = x$enable_gpc])
dnl ###############################################




dnl #########  Check for SDL: #####################
dnl the sdl script pollutes our global values:
temp_LIBS="$LIBS"
temp_CFLAGS="$CFLAGS"
temp_CXXFLAGS="$CXXFLAGS"
sdl_enabled=""
SDL_VERSION=1.2.0
AM_PATH_SDL($SDL_VERSION,
    [sdl_enabled="yes"],
    AC_MSG_WARN([*** SDL version $SDL_VERSION not found! Omitting sdl layer.])
    )
dnl ### Restore old values
CFLAGS=$temp_CFLAGS
CXXFLAGS=$temp_CXXFLAGS
LIBS=$temp_LIBS
dnl ### the sdl script already does that:
dnl AC_SUBST(SDL_CFLAGS)
dnl AC_SUBST(SDL_LIBS)
AM_CONDITIONAL(ENABLE_SDL,[test xyes = x$sdl_enabled -a xno != x$enable_platform -a x$win32_host != xyes])
dnl ###############################################


dnl #########  Checking for X11: ##################
AC_PATH_X
if test "$no_x" = "yes"; then
  AC_MSG_WARN([*** X11 not found! Omitting X11 layer.])
fi
AM_CONDITIONAL(ENABLE_X11,[test x$no_x = x -a xno != x$enable_platform -a x$win32_host != xyes])
AC_SUBST(x_includes)
AC_SUBST(x_libraries)
dnl ###############################################

dnl Settung up library version
AGG_LIB_VERSION="2:4:0"
dnl     current-´ / /
dnl    revision--´ /
dnl         age---´
dnl Update the version information only immediately before a public release of antigrain
dnl If the library source code has changed, increment revision (c:r:a becomes c:r+1:a). 
dnl If any interfaces have been added, removed, or changed since the last update, 
dnl      increment current, and set revision to 0. 
dnl If any interfaces have been added since the last public release, then increment age. 
dnl If any interfaces have been removed since the last public release, then set age to 0.

AC_SUBST(AGG_LIB_VERSION)
AC_SUBST(PREFERED_PLATFORM)


AC_OUTPUT(
   Makefile
   libagg.pc
   gpc/Makefile
   font_freetype/Makefile
   font_win32_tt/Makefile
   src/Makefile
   src/ctrl/Makefile
   src/platform/Makefile
   src/platform/X11/Makefile
   src/platform/sdl/Makefile
   src/platform/mac/Makefile
   src/platform/win32/Makefile
   src/platform/BeOS/Makefile
   src/platform/AmigaOS/Makefile
   include/Makefile
   include/ctrl/Makefile
   include/util/Makefile
   include/platform/Makefile
   examples/Makefile
) 

