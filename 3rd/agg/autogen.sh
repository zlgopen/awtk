# autogen.sh
#
# invoke the auto* tools to create the configuration/build system

# build aclocal.m4
aclocal

# build config.h
autoheader

# build the configure script
autoconf

# set up libtool
libtoolize --force

# invoke automake
automake --foreign --add-missing --ignore-deps

# and finally invoke our new configure
./configure $*

# end
