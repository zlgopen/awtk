import os
import atexit
import platform
import sys
sys.path.insert(0, './scripts')
import compile_config
compile_helper = compile_config.compile_helper()
compile_helper.try_load_default_config()
compile_helper.scons_user_sopt(ARGUMENTS)
compile_config.set_curr_config(compile_helper)

import awtk_config as awtk

APP_TOOLS = ['default']
awtk.scons_db_check_and_remove()

if awtk.TOOLS_NAME != '' :
  if awtk.TOOLS_NAME == 'mingw':
    APP_TOOLS = ['mingw']

awtk.genIdlAndDef();
DefaultEnvironment(TOOLS = APP_TOOLS,
  CCFLAGS = awtk.AWTK_CCFLAGS + awtk.BUILD_DEBUG_FLAG,
  LIBS = awtk.LIBS,
  LIBPATH = awtk.LIBPATH,
  CPPPATH = awtk.CPPPATH + [awtk.joinPath(awtk.TK_ROOT, 'res')],
  LINKFLAGS = awtk.LINKFLAGS + awtk.BUILD_DEBUG_LINKFLAGS,
  TARGET_ARCH=awtk.TARGET_ARCH,
  OS_SUBSYSTEM_CONSOLE=awtk.OS_SUBSYSTEM_CONSOLE,
  OS_SUBSYSTEM_WINDOWS=awtk.OS_SUBSYSTEM_WINDOWS
)

SConscriptFiles=awtk.NANOVG_BACKEND_PROJS + [
  '3rd/cjson/SConscript',
  '3rd/mbedtls/SConscript',
  '3rd/glad/SConscript',
  '3rd/gpinyin/SConscript', 
  '3rd/svgtiny/SConscript',
  '3rd/libunibreak/SConscript',
  '3rd/lz4/SConscript',
  '3rd/miniz/SConscript',
  '3rd/fribidi/SConscript',
  '3rd/nativefiledialog/SConscript',
  'src/SConscript',
  'src/csv/SConscript',
  'src/conf_io/SConscript',
  'src/hal/SConscript',
  'src/streams/SConscript',
  'src/compressors/SConscript',
  'src/ubjson/SConscript',
  'src/debugger/SConscript',
  'src/fscript_ext/SConscript',
  'src/xml/SConscript',
  'src/charset/SConscript',
  'src/romfs/SConscript',
  ]

os.environ['BUILD_TOOLS'] = str(compile_helper.get_value('BUILD_TOOLS', True))
if compile_helper.get_value('BUILD_TOOLS', True) :
  SConscriptFiles += [
    'src/streams/tools/SConscript',
    'tools/common/SConscript', 
    'tools/theme_gen/SConscript', 
    'tools/font_gen/SConscript', 
    'tools/image_gen/SConscript', 
    'tools/image_resize/SConscript', 
    'tools/image_dither/SConscript',
    'tools/res_gen/SConscript', 
    'tools/str_gen/SConscript', 
    'tools/ui_gen/xml_to_ui/SConscript',
    'tools/svg_gen/SConscript',
    'tools/api_doc_lint/SConscript',
    'tools/preview_ui/SConscript',
    'tools/fdb/SConscript',
    'tools/dltest/SConscript',
    'tools/ui_test/SConscript',
    'src/hal/tools/network_shell/SConscript',
  ]

if compile_helper.get_value('BUILD_DEMOS', True) :
  SConscriptFiles += ['demos/SConscript']

if compile_helper.get_value('BUILD_TESTS', True) :
  SConscriptFiles += ['tests/SConscript']

SConscriptFiles += awtk.OS_PROJECTS
os.environ['TK_ROOT'] = awtk.TK_ROOT;
os.environ['BIN_DIR'] = awtk.TK_BIN_DIR;
os.environ['LIB_DIR'] = awtk.TK_LIB_DIR;
os.environ['TARGET_OS'] = awtk.OS_NAME;
os.environ['AWTK_SHARED_LIBS'] = ';'.join(awtk.SHARED_LIBS);

SConscript(SConscriptFiles)


def compile_end() :
  compile_helper.save_last_compile_argv()
  compile_helper.output_compile_data(awtk.TK_ROOT)

atexit.register(compile_end)
