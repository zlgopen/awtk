import os
import sys
sys.path.append("../common")
import awtk_files as awtk;

CPPFLAGS = '-DHAS_STD_MALLOC -DNDEBUG --enable=warning  --enable=performance -DWITH_CPPCHECK '

awtk.run('cppcheck', CPPFLAGS, awtk.getCppCheckFiles())
