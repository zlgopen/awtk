import os
import platform

OS_NAME=platform.system()
LFTK_ROOT = os.path.normpath(os.getcwd())
LFTK_SRC = os.path.join(LFTK_ROOT, 'src')
LFTK_3RD_ROOT = os.path.join(LFTK_ROOT, '3rd')
LFTK_TOOLS_ROOT = os.path.join(LFTK_ROOT, 'tools')
GTEST_ROOT = os.path.join(LFTK_ROOT, '3rd/gtest/googletest')
BIN_DIR=os.path.join(LFTK_ROOT, 'bin')
LIB_DIR=os.path.join(LFTK_ROOT, 'lib')

LCD='NANOVG'
LCD='SDL'

os.environ['LCD'] = LCD
os.environ['BIN_DIR'] = BIN_DIR;
os.environ['LIB_DIR'] = LIB_DIR;
os.environ['LFTK_ROOT'] = LFTK_ROOT;
os.environ['GTEST_ROOT'] = GTEST_ROOT;

COMMON_CCFLAGS=' -DLFTK_ROOT=\\\"'+LFTK_ROOT+'\\\" -DLUA_COMPAT_MODULE -DHAS_STD_MALLOC -DSTBTT_STATIC -DSTB_IMAGE_STATIC'

if LCD == 'NANOVG':
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_NANOVG -DWITH_GL3'
else:
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_STB_IMAGE -DWITH_STB_FONT -DSDL2'

OS_LIBPATH=[]
OS_CPPPATH=[]
OS_FLAGS='-g -Wall'
OS_SUBSYSTEM_CONSOLE=''
OS_SUBSYSTEM_WINDOWS=''
OS_LINKFLAGS=''
OS_LIBS=['SDL2', 'glad', 'stdc++', 'pthread', 'm']

if OS_NAME == 'Darwin':
  OS_LINKFLAGS='-framework OpenGL'
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DLUA_USE_POSIX -D__APPLE__'
  print("Macos"); 

elif OS_NAME == 'Linux':
  OS_LIBS = ['GL'] + OS_LIBS + ['dl']
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DLUA_USE_POSIX -DLINUX'
  print("Linux"); 

elif OS_NAME == 'Windows':
  OS_LIBS=['SDL2', 'glad']
  OS_FLAGS='-DWIN32 -D_WIN32 -DWINDOWS /EHsc -D_CONSOLE  /DEBUG /INCREMENTA -DUNICODE -D_UNICODE /Od /ZI'
  OS_LINKFLAGS='/MACHINE:X64 /DEBUG'
  OS_LIBPATH=[LFTK_3RD_ROOT+'/SDL2-2.0.7/lib/x64']
  OS_CPPPATH=[LFTK_3RD_ROOT+'/SDL2-2.0.7/']
  OS_SUBSYSTEM_CONSOLE='/SUBSYSTEM:CONSOLE  '
  OS_SUBSYSTEM_WINDOWS='/SUBSYSTEM:WINDOWS  '
  
LIBS=['lftk', 'agg', 'nanovg'] + OS_LIBS

CCFLAGS=OS_FLAGS + COMMON_CCFLAGS 
CPPPATH=[LFTK_ROOT, LFTK_SRC, LFTK_3RD_ROOT, os.path.join(LFTK_3RD_ROOT, 'nanovg/src'), os.path.join(LFTK_3RD_ROOT, 'agg-2.5/include'), LFTK_TOOLS_ROOT] + OS_CPPPATH

DefaultEnvironment(CCFLAGS = CCFLAGS, 
  CPPPATH = CPPPATH,
  LIBS=LIBS,
  LINKFLAGS=OS_LINKFLAGS,
  OS_SUBSYSTEM_CONSOLE=OS_SUBSYSTEM_CONSOLE,
  OS_SUBSYSTEM_WINDOWS=OS_SUBSYSTEM_WINDOWS,
  LIBPATH=[os.path.join(LFTK_ROOT, 'lib')] + OS_LIBPATH)

SConscript([
  '3rd/agg-2.5/SConscript',
  '3rd/nanovg/SConscript',
  '3rd/glad/SConscript',
  'src/SConscript',
  'tools/common/SConscript', 
  'tools/theme_gen/SConscript', 
  'tools/font_gen/SConscript', 
  'tools/image_gen/SConscript', 
  'tools/res_gen/SConscript', 
  'tools/ui_gen/xml_to_ui/SConscript',
  'demos/SConscript', 
  'tests/SConscript',
  '3rd/lua/SConscript',
  'lua/SConscript'
  ])
