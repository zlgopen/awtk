# Configure paths for libagg
# Kirill Smelkov 2005-10-23, based on freetype2.m4 by Marcelo Magallon

# AC_CHECK_LIBAGG([MINIMUM-VERSION [, ACTION-IF-FOUND [,ACTION-IF-NOT-FOUND]]])
# Test for libagg, and define LIBAGG_CFLAGS and LIBAGG_LIBS
#
AC_DEFUN([AC_CHECK_LIBAGG],
[
  # Get the cflags and libraries from pkg-config libagg ...
  AC_ARG_WITH([libagg],
    AS_HELP_STRING([--with-libagg=PREFIX],
                   [Prefix where libagg is installed (optional)]),
    [libagg_prefix="$withval"],
    [libagg_prefix=""])

  libagg_name=libagg
  if test "x$libagg_prefix" != "x"; then
    libagg_name="$libagg_prefix/lib/pkgconfig/libagg.pc"
  fi

  PKG_CHECK_MODULES([LIBAGG], "$libagg_name", success=yes, success=no)

  if test "x$success" = xyes; then
    ifelse([$2], , :, [$2])

    AC_SUBST([LIBAGG_CFLAGS])
    AC_SUBST([LIBAGG_LIBS])
  else
    ifelse([$3], , :, [$3])
  fi
])

# end of libagg.m4

