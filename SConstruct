import os

LFTK_ROOT = os.path.normpath(os.getcwd())
LFTK_SRC = os.path.join(LFTK_ROOT, 'src')
LFTK_3RD_ROOT = os.path.join(LFTK_ROOT, '3rd')
LFTK_TOOLS_ROOT = os.path.join(LFTK_ROOT, 'tools')
GTEST_ROOT = os.path.join(LFTK_ROOT, '3rd/gtest/googletest')
BIN_DIR=os.path.join(LFTK_ROOT, 'bin')
LIB_DIR=os.path.join(LFTK_ROOT, 'lib')

if os.name == 'posix':
  OS_FLAGS='-g -Wall -DLUA_USE_POSIX -DLUA_COMPAT_MODULE -DHAS_STD_MALLOC'
  OS_LINKFLAGS=''
  OS_LIBPATH=[]
  OS_CPPPATH=[]
  OS_LIBS=['stdc++', 'pthread', 'm']
  OS_SUBSYSTEM_CONSOLE=''
  OS_SUBSYSTEM_WINDOWS=''
  
elif os.name == 'nt':
  OS_FLAGS='-DWIN32 -DWINDOWS /EHsc -D_CONSOLE  /DEBUG /INCREMENTA -DUNICODE -D_UNICODE -DLUA_COMPAT_MODULE'
  OS_LINKFLAGS='/MACHINE:X64 '
  OS_LIBPATH=[LFTK_3RD_ROOT+'/SDL2-2.0.7/lib/x64']
  OS_CPPPATH=[LFTK_3RD_ROOT+'/SDL2-2.0.7/']
  OS_LIBS=[]
  OS_SUBSYSTEM_CONSOLE='/SUBSYSTEM:CONSOLE  '
  OS_SUBSYSTEM_WINDOWS='/SUBSYSTEM:WINDOWS  '
  
os.environ['BIN_DIR'] = BIN_DIR;
os.environ['LIB_DIR'] = LIB_DIR;
os.environ['LFTK_ROOT'] = LFTK_ROOT;
os.environ['GTEST_ROOT'] = GTEST_ROOT;

CCFLAGS=OS_FLAGS + ' -DLFTK_ROOT=\\\"'+LFTK_ROOT+'\\\" -DWITH_STB_IMAGE -DWITH_STB_FONT -DSDL2'
DefaultEnvironment(CCFLAGS = CCFLAGS, 
  CPPPATH = [LFTK_ROOT, LFTK_SRC, LFTK_3RD_ROOT, os.path.join(LFTK_3RD_ROOT, 'agg-2.5/include'), LFTK_TOOLS_ROOT] + OS_CPPPATH, 
  LIBS=['lftk', 'agg', 'SDL2'] + OS_LIBS,
  LINKFLAGS=OS_LINKFLAGS,
  OS_SUBSYSTEM_CONSOLE=OS_SUBSYSTEM_CONSOLE,
  OS_SUBSYSTEM_WINDOWS=OS_SUBSYSTEM_WINDOWS,
  LIBPATH=[os.path.join(LFTK_ROOT, 'lib')] + OS_LIBPATH)

SConscript([
  'src/SConscript',
  'tools/common/SConscript', 
  'tools/theme_gen/SConscript', 
  'tools/font_gen/SConscript', 
  'tools/image_gen/SConscript', 
  'tools/res_gen/SConscript', 
  'tools/ui_gen/xml_to_ui/SConscript', 
  'tools/ui_gen/qt_to_xml/SConscript', 
  'tools/ui_gen/rc_to_xml/SConscript',
  'demos/SConscript', 
  'tests/SConscript',
  '3rd/lua/SConscript',
  '3rd/agg-2.5/SConscript',
  'lua/SConscript'
  ])
