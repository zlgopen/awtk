#!/bin/sh

./tools/gyp/gyp --depth=./ picasso.gyp --generator-output=proj
echo "Please change dir to \"proj\" and type \"make\" or \"make BUILDTYPE=Release\""
echo "...\n"		
