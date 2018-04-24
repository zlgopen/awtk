#!/bin/sh

echo n | libtoolize --copy --force
aclocal
autoconf
autoheader
automake --add-missing 

