import os
import platform
import awtk_config as awtk

APP_TOOLS = None
OS_NAME = platform.system()
awtk.scons_db_check_and_remove()

if awtk.TOOLS_NAME != '' :
  if awtk.TOOLS_NAME == 'mingw':
    APP_TOOLS = ['mingw']
elif awtk.TOOLS_NAME == '' and OS_NAME == 'Windows':
    APP_TOOLS = ['msvc', 'masm', 'mslink', "mslib"]

awtk.genIdlAndDef();
DefaultEnvironment(TOOLS = APP_TOOLS,
  CCFLAGS = awtk.AWTK_CCFLAGS,
  LIBS = awtk.LIBS,
  LIBPATH = awtk.LIBPATH,
  CPPPATH = awtk.CPPPATH + [awtk.joinPath(awtk.TK_ROOT, 'res')],
  LINKFLAGS = awtk.LINKFLAGS,
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
  'src/streams/tools/SConscript',
  'src/compressors/SConscript',
  'src/ubjson/SConscript',
  'src/debugger/SConscript',
  'src/fscript_ext/SConscript',
  'src/xml/SConscript',
  'src/charset/SConscript',
  'src/romfs/SConscript',
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
  'demos/SConscript', 
  'tests/SConscript',
  'src/hal/tools/network_shell/SConscript',
  ] + awtk.OS_PROJECTS
  
os.environ['TK_ROOT'] = awtk.TK_ROOT;
os.environ['BIN_DIR'] = awtk.TK_BIN_DIR;
os.environ['LIB_DIR'] = awtk.TK_LIB_DIR;
os.environ['AWTK_SHARED_LIBS'] = ';'.join(awtk.SHARED_LIBS);

SConscript(SConscriptFiles)

