import os
import platform

OS_NAME=platform.system()
TK_ROOT = os.path.normpath(os.getcwd())
TK_SRC = os.path.join(TK_ROOT, 'src')
TK_3RD_ROOT = os.path.join(TK_ROOT, '3rd')
TK_TOOLS_ROOT = os.path.join(TK_ROOT, 'tools')
GTEST_ROOT = os.path.join(TK_ROOT, '3rd/gtest/googletest')
BIN_DIR=os.path.join(TK_ROOT, 'bin')
LIB_DIR=os.path.join(TK_ROOT, 'lib')

if OS_NAME == 'Windows':
  LCD='NANOVG'
  TK_ROOT=TK_ROOT.replace('\\', '\\\\');
  print(TK_ROOT)
else:
  LCD='SDL'
  LCD='NANOVG'
  VGCANVAS='AGG'
  VGCANVAS='PICASSO'

INPUT_METHOD='native'
INPUT_METHOD='default'
INPUT_ENGINE='null'
INPUT_ENGINE='pinyin'

FRAME_BUFFER_FORMAT='rgba8888'
FRAME_BUFFER_FORMAT='rgb565'

COMMON_CCFLAGS=' -DTK_ROOT=\\\"'+TK_ROOT+'\\\" -DHAS_STD_MALLOC -DSDL2 -DWITH_FS_RES -DHAS_STDIO -DWITH_DESKTOP_STYLE'
COMMON_CCFLAGS=COMMON_CCFLAGS+' -DLUA_COMPAT_MODULE -DSTBTT_STATIC -DSTB_IMAGE_STATIC -DWITH_STB_IMAGE -DWITH_STB_FONT -DWITH_VGCANVAS -DWITH_UNICODE_BREAK'

if FRAME_BUFFER_FORMAT=='rgba8888':
  COMMON_CCFLAGS=COMMON_CCFLAGS+' -DWITH_FB_8888=1';
else:
  COMMON_CCFLAGS=COMMON_CCFLAGS+' -DWITH_FB_565=1';

if INPUT_METHOD == 'native':
  COMMON_CCFLAGS=COMMON_CCFLAGS+' -DWITH_NATIVE_IM=1';
else:
  COMMON_CCFLAGS=COMMON_CCFLAGS+' -DWITH_DEFAULT_IM=1';

if LCD == 'NANOVG':
  VGCANVAS='NANOVG'
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_NANOVG -DWITH_GL3 -DWITH_VGCANVAS_LCD'
else:
  if VGCANVAS == 'AGG':
    COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_AGG'
  elif VGCANVAS == 'PICASSO':
    COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_PICASSO -DIMAGE_RGBA'
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_BITMAP_RGB565'

os.environ['LCD'] = LCD
os.environ['VGCANVAS'] =VGCANVAS 
os.environ['BIN_DIR'] = BIN_DIR;
os.environ['LIB_DIR'] = LIB_DIR;
os.environ['TK_ROOT'] = TK_ROOT;
os.environ['GTEST_ROOT'] = GTEST_ROOT;
os.environ['INPUT_METHOD'] = INPUT_METHOD;
os.environ['INPUT_ENGINE'] = INPUT_ENGINE;
os.environ['FRAME_BUFFER_FORMAT'] = FRAME_BUFFER_FORMAT;

OS_LIBPATH=[]
OS_CPPPATH=[]
OS_FLAGS='-g -Wall'
OS_SUBSYSTEM_CONSOLE=''
OS_SUBSYSTEM_WINDOWS=''
OS_LINKFLAGS=''
OS_LIBS=['SDL2', 'glad', 'stdc++', 'pthread', 'm']

if OS_NAME == 'Darwin':
  OS_LINKFLAGS='-framework OpenGL'
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DLUA_USE_POSIX -D__APPLE__ -DHAS_PTHREAD -DMACOS'
  if VGCANVAS == 'PICASSO':
    OS_LIBS = ['freetype'] + OS_LIBS
    COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DENABLE_FREE_TYPE2=1 -DFONT_FILE_NAME=\\\"'+TK_ROOT+'/demos/res/raw/fonts/default_ttf.ttf\\\"'
  OS_LIBS = ['picasso', 'agg'] + OS_LIBS

elif OS_NAME == 'Linux':
  OS_LIBS = ['GL'] + OS_LIBS + ['dl']
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DLUA_USE_POSIX -DLINUX -DHAS_PTHREAD'
  if VGCANVAS == 'PICASSO':
    OS_LIBS = ['freetype'] + OS_LIBS
    COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DENABLE_FREE_TYPE2=1 -DFONT_FILE_NAME=\\\"'+TK_ROOT+'/demos/res/raw/fonts/default_ttf.ttf\\\"'

elif OS_NAME == 'Windows':
  OS_LIBS=['SDL2', 'glad']
  OS_FLAGS='-DWIN32 -D_WIN32 -DWINDOWS /EHsc -D_CONSOLE  /DEBUG -DUNICODE -D_UNICODE /Od /ZI'
  OS_LINKFLAGS='/MACHINE:X64 /DEBUG'
  OS_LIBPATH=[TK_3RD_ROOT+'/SDL2-2.0.7/lib/x64']
  OS_CPPPATH=[TK_3RD_ROOT+'/SDL2-2.0.7/']
  OS_SUBSYSTEM_CONSOLE='/SUBSYSTEM:CONSOLE  '
  OS_SUBSYSTEM_WINDOWS='/SUBSYSTEM:WINDOWS  '
  
LIBS=['awtk', 'gpinyin', 'awtk', 'linebreak', 'nanovg'] + OS_LIBS

CCFLAGS=OS_FLAGS + COMMON_CCFLAGS 
CPPPATH=[TK_ROOT, 
  TK_SRC, 
  TK_3RD_ROOT, 
  os.path.join(TK_SRC, 'ext_widgets'), 
  os.path.join(TK_3RD_ROOT, 'nanovg/src'), 
  os.path.join(TK_3RD_ROOT, 'agg/include'), 
  os.path.join(TK_3RD_ROOT, 'gpinyin/include'), 
  os.path.join(TK_3RD_ROOT, 'picasso/src'), 
  os.path.join(TK_3RD_ROOT, 'picasso/build'), 
  os.path.join(TK_3RD_ROOT, 'picasso/include'), 
  os.path.join(TK_3RD_ROOT, 'picasso/src/gfx'), 
  os.path.join(TK_3RD_ROOT, 'picasso/src/include'), 
  os.path.join(TK_3RD_ROOT, 'freetype2/include'), 
  os.path.join(TK_3RD_ROOT, 'libunibreak/src'), 
  TK_TOOLS_ROOT] + OS_CPPPATH

DefaultEnvironment(CCFLAGS = CCFLAGS, 
  CPPPATH = CPPPATH,
  LIBS=LIBS,
  LINKFLAGS=OS_LINKFLAGS,
  OS_SUBSYSTEM_CONSOLE=OS_SUBSYSTEM_CONSOLE,
  OS_SUBSYSTEM_WINDOWS=OS_SUBSYSTEM_WINDOWS,
  LIBPATH=[os.path.join(TK_ROOT, 'lib')] + OS_LIBPATH)

SConscriptFiles=[
  '3rd/nanovg/SConscript',
  '3rd/glad/SConscript',
  '3rd/lua/SConscript',
  '3rd/agg/SConscript', 
  '3rd/gpinyin/SConscript', 
  '3rd/picasso/SConscript',
  '3rd/libunibreak/SConscript',
  'src/SConscript',
  'tools/common/SConscript', 
  'tools/theme_gen/SConscript', 
  'tools/font_gen/SConscript', 
  'tools/image_gen/SConscript', 
  'tools/image_resize/SConscript', 
  'tools/res_gen/SConscript', 
  'tools/str_gen/SConscript', 
  'tools/ui_gen/xml_to_ui/SConscript',
  'demos/SConscript', 
  'tests/SConscript',
  'lua/SConscript'
  ]
  
SConscript(SConscriptFiles)

