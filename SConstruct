import os
import awtk_config as awtk

APP_TOOLS = None
if awtk.TOOLS_NAME != '' :
  APP_TOOLS = [awtk.TOOLS_NAME]

DefaultEnvironment(TOOLS = APP_TOOLS,
  CCFLAGS = awtk.AWTK_CCFLAGS,
  LIBS = awtk.LIBS,
  LIBPATH = awtk.LIBPATH,
  CPPPATH = awtk.CPPPATH,
  LINKFLAGS = awtk.LINKFLAGS,
  TARGET_ARCH=awtk.TARGET_ARCH,
  OS_SUBSYSTEM_CONSOLE=awtk.OS_SUBSYSTEM_CONSOLE,
  OS_SUBSYSTEM_WINDOWS=awtk.OS_SUBSYSTEM_WINDOWS
)

SConscriptFiles=awtk.NANOVG_BACKEND_PROJS + [
  '3rd/cjson/SConscript',
  '3rd/nanovg/SConscript',
  '3rd/glad/SConscript',
  '3rd/gpinyin/SConscript', 
  '3rd/libunibreak/SConscript',
  '3rd/lz4/SConscript',
  '3rd/miniz/SConscript',
  '3rd/fribidi/SConscript',
  'src/SConscript',
  'src/conf_io/SConscript',
  'src/streams/SConscript',
  'src/compressors/SConscript',
  'src/ubjson/SConscript',
  'tools/common/SConscript', 
  'tools/theme_gen/SConscript', 
  'tools/font_gen/SConscript', 
  'tools/image_gen/SConscript', 
  'tools/image_resize/SConscript', 
  'tools/image_dither/SConscript',
  'tools/res_gen/SConscript', 
  'tools/str_gen/SConscript', 
  'tools/ui_gen/qt_to_xml/SConscript',
  'tools/ui_gen/xml_to_ui/SConscript',
  'tools/svg_gen/SConscript',
  'demos/SConscript', 
  'tests/SConscript'
  ] + awtk.OS_PROJECTS
  
os.environ['TK_ROOT'] = awtk.TK_ROOT;
os.environ['BIN_DIR'] = awtk.TK_BIN_DIR;
os.environ['LIB_DIR'] = awtk.TK_LIB_DIR;

SConscript(SConscriptFiles)

