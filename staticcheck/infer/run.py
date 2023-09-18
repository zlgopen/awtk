import os
import sys
sys.path.append("../common")
import awtk_files as awtk;

CPPFLAGS = 'run -- clang -c -DHAS_STD_MALLOC -DNDEBUG -DWITH_INFERCHECK '
awtk.run('infer', CPPFLAGS, awtk.getInferFiles())
