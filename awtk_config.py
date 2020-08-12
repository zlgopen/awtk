import os
import os.path
import platform
import shutil
from shutil import copyfile

TOOLS_PREFIX=''
OS_NAME = platform.system();
MACH = platform.machine();
ARCH = platform.architecture();
is32bit = (ARCH[0] == '32bit');

if is32bit:
  if MACH == 'i686' or MACH == 'i386' or MACH == 'x86':
    TARGET_ARCH='x86'
  else:
    TARGET_ARCH='arm'
else:
  TARGET_ARCH=''

print('MACH=' + MACH + ' ARCH=' + str(ARCH) + ' TARGET_ARCH=' + TARGET_ARCH)

def joinPath(root, subdir):
  return os.path.normpath(os.path.join(root, subdir))

TK_ROOT=os.path.dirname(os.path.normpath(os.path.abspath(__file__)))

WIN32_AWTK_RES=os.path.join(TK_ROOT, 'win32_res/awtk.res');

print('TK_ROOT: ' + TK_ROOT);

TK_SRC        = joinPath(TK_ROOT, 'src')
TK_BIN_DIR    = joinPath(TK_ROOT, 'bin')
TK_LIB_DIR    = joinPath(TK_ROOT, 'lib')
TK_3RD_ROOT   = joinPath(TK_ROOT, '3rd')
TK_TOOLS_ROOT = joinPath(TK_ROOT, 'tools')
TK_DEMO_ROOT  = joinPath(TK_ROOT, 'demos')
GTEST_ROOT    = joinPath(TK_ROOT, '3rd/gtest/googletest')
AWTK_STATIC_LIBS=['awtk_global', 'extwidgets', 'widgets', 'base', 'gpinyin', 'streams', 'conf_io', 'ubjson', 'compressors', 'fribidi', 'miniz', 'tkc', 'linebreak']

#INPUT_ENGINE='null'
#INPUT_ENGINE='spinyin'
#INPUT_ENGINE='t9'
#INPUT_ENGINE='t9ext'
INPUT_ENGINE='pinyin'

VGCANVAS='NANOVG'
if OS_NAME == 'Windows':
  TK_ROOT=TK_ROOT.replace('\\', '\\\\');
  NANOVG_BACKEND='GLES2'
else:
  NANOVG_BACKEND='GL3'

#VGCANVAS='CAIRO'
#NANOVG_BACKEND='GLES2'
#NANOVG_BACKEND='GLES3'
#NANOVG_BACKEND='AGG'
#NANOVG_BACKEND='AGGE'
#NANOVG_BACKEND='BGFX'

FRAME_BUFFER_FORMAT=''
if VGCANVAS == 'CAIRO':
  LCD='SDL_FB'
  FRAME_BUFFER_FORMAT='bgr565'
  #FRAME_BUFFER_FORMAT='bgra8888'
else:
  if NANOVG_BACKEND == 'AGGE' or NANOVG_BACKEND == 'AGG':
    LCD='SDL_FB'
    FRAME_BUFFER_FORMAT='bgr565'
    #FRAME_BUFFER_FORMAT='bgra8888'
  else:  
    LCD='SDL_GPU'
#LCD='SDL_FB_MONO'

NANOVG_BACKEND_LIBS=[];
NANOVG_BACKEND_PROJS=[];

NATIVE_WINDOW='sdl'
TOOLS_NAME = ''
# TOOLS_NAME = 'mingw'

COMMON_CCFLAGS=' -DTK_ROOT=\"\\\"'+TK_ROOT+'\\\"\" ' 
#COMMON_CCFLAGS=COMMON_CCFLAGS+' -DENABLE_PERFORMANCE_PROFILE=1 '
#COMMON_CCFLAGS=COMMON_CCFLAGS+' -DNATIVE_WINDOW_BORDERLESS=1 '
#COMMON_CCFLAGS=COMMON_CCFLAGS+' -DENABLE_MEM_LEAK_CHECK=1 '

COMMON_CCFLAGS=COMMON_CCFLAGS+' -DENABLE_CURSOR=1 '
COMMON_CCFLAGS=COMMON_CCFLAGS+' -DWITH_TEXT_BIDI=1 '
COMMON_CCFLAGS=COMMON_CCFLAGS+' -DWITH_DATA_READER_WRITER=1 '
COMMON_CCFLAGS=COMMON_CCFLAGS+' -DWITH_EVENT_RECORDER_PLAYER=1 '
COMMON_CCFLAGS=COMMON_CCFLAGS+' -DWITH_ASSET_LOADER -DWITH_FS_RES -DWITH_ASSET_LOADER_ZIP ' 
COMMON_CCFLAGS=COMMON_CCFLAGS+' -DSTBTT_STATIC -DSTB_IMAGE_STATIC -DWITH_STB_IMAGE '
COMMON_CCFLAGS=COMMON_CCFLAGS+' -DWITH_VGCANVAS -DWITH_UNICODE_BREAK -DWITH_DESKTOP_STYLE '
COMMON_CCFLAGS=COMMON_CCFLAGS+' -DSDL2 -DHAS_STD_MALLOC -DWITH_SDL -DHAS_STDIO -DHAVE_STDIO_H '

GRAPHIC_BUFFER='default'

#only for c compiler flags
COMMON_CFLAGS=''

if INPUT_ENGINE == 't9':
    COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_IME_T9 '
elif INPUT_ENGINE == 't9ext' :
    COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_IME_T9EXT'
elif INPUT_ENGINE == 'pinyin' :
    COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_IME_PINYIN '
elif INPUT_ENGINE == 'spinyin' :
    COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_IME_SPINYIN '
elif INPUT_ENGINE == 'null' :
    COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_IME_NULL '

if LCD == 'SDL_GPU':
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_NANOVG_GPU -DWITH_VGCANVAS_LCD'
  COMMON_CCFLAGS=COMMON_CCFLAGS+' -DWITH_STB_FONT '
elif LCD == 'SDL_FB_MONO':
  NANOVG_BACKEND='AGGE'
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_LCD_MONO -DWITH_NANOVG_SOFT '
  COMMON_CCFLAGS=COMMON_CCFLAGS+' -DWITH_FT_FONT '
else:
  COMMON_CCFLAGS=COMMON_CCFLAGS+' -DWITH_STB_FONT '
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_BITMAP_BGRA -DWITH_NANOVG_SOFT '
  if FRAME_BUFFER_FORMAT=='bgra8888':
    COMMON_CCFLAGS=COMMON_CCFLAGS+' -DWITH_FB_BGRA8888=1 ';
  else:
    COMMON_CCFLAGS=COMMON_CCFLAGS+' -DWITH_FB_BGR565=1';

if VGCANVAS == 'CAIRO':
  NANOVG_BACKEND_LIBS=['cairo', 'pixman'];
  NANOVG_BACKEND_PROJS=['3rd/cairo/SConscript', '3rd/pixman/SConscript'];
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_VGCANVAS_CAIRO -DHAVE_CONFIG_H -DCAIRO_WIN32_STATIC_BUILD '
else:
  if NANOVG_BACKEND == 'AGG':
    NANOVG_BACKEND_LIBS=['nanovg-agg', 'nanovg', 'agg'];
    NANOVG_BACKEND_PROJS=['3rd/agg/SConscript'];
    COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_NANOVG_AGG '
  elif NANOVG_BACKEND == 'AGGE':
    NANOVG_BACKEND_LIBS=['nanovg-agge', 'nanovg', 'agge'];
    NANOVG_BACKEND_PROJS=['3rd/agge/SConscript'];
    COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_NANOVG_AGGE '
  elif NANOVG_BACKEND == 'BGFX':
    NANOVG_BACKEND_LIBS=['nanovg-bgfx','nanovg', 'bgfx'];
    NANOVG_BACKEND_PROJS=['3rd/bgfx/SConscript'];
    COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_NANOVG_BGFX '
  elif NANOVG_BACKEND == 'GLES2':
    NANOVG_BACKEND_LIBS=['nanovg'];
    COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_NANOVG_GLES2 -DWITH_NANOVG_GL '
  elif NANOVG_BACKEND == 'GLES3':
    NANOVG_BACKEND_LIBS=['nanovg'];
    COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_NANOVG_GLES3 -DWITH_NANOVG_GL '
  else:
    NANOVG_BACKEND_LIBS=['nanovg'];
    COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DWITH_NANOVG_GL3 -DWITH_NANOVG_GL '

OS_FLAGS=''
OS_LIBS=[]
OS_LIBPATH=[]
OS_CPPPATH=[]
OS_LINKFLAGS=''
OS_SUBSYSTEM_CONSOLE=''
OS_SUBSYSTEM_WINDOWS=''
OS_PROJECTS=[]
OS_WHOLE_ARCHIVE=''
if OS_NAME == 'Darwin':
  TOOLS_NAME = ''
  OS_FLAGS='-g -Wall -fPIC '
  OS_LIBS = ['stdc++', 'pthread', 'm', 'dl']
  OS_LINKFLAGS='-framework IOKit -framework Cocoa -framework QuartzCore -framework OpenGL -weak_framework Metal -weak_framework MetalKit'
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DHAS_SEM_OPEN '
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -D__APPLE__ -DHAS_PTHREAD -DMACOS '
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS  -DBGFX_CONFIG_RENDERER_METAL=1 '
  AWTK_DLL_DEPS_LIBS = AWTK_STATIC_LIBS + NANOVG_BACKEND_LIBS + ['SDL2', 'glad'] + OS_LIBS
  OS_WHOLE_ARCHIVE=' -all_load '

elif OS_NAME == 'Linux':
  TOOLS_NAME = ''
  OS_FLAGS='-g -Wall -fPIC ' 
  OS_LIBS = ['GL', 'gtk-3','gdk-3','Xext', 'X11', 'sndio','stdc++', 'pthread', 'm', 'dl', 'asound']
  COMMON_CFLAGS=COMMON_CFLAGS+' -std=gnu99 '
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DLINUX -DHAS_PTHREAD'
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DSDL_REAL_API -DSDL_TIMER_UNIX -DSDL_VIDEO_DRIVER_X11 -DSDL_VIDEO_DRIVER_X11_SUPPORTS_GENERIC_EVENTS '
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DSDL_AUDIO_DRIVER_SNDIO -DSDL_VIDEO_OPENGL_GLX -DSDL_VIDEO_RENDER_OGL '
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DSDL_LOADSO_DLOPEN -DSDL_VIDEO_OPENGL_EGL -DSDL_VIDEO_OPENGL_ES2 '
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DSDL_REAL_API -DSDL_HAPTIC_DISABLED -DSDL_SENSOR_DISABLED -DSDL_JOYSTICK_DISABLED '
  OS_PROJECTS=['3rd/SDL/SConscript']
  if TARGET_ARCH == 'x86':
    OS_FLAGS = OS_FLAGS + ' -U__FLT_EVAL_METHOD__ -D__FLT_EVAL_METHOD__=0 '
  else:
    OS_FLAGS = OS_FLAGS + ' -DWITH_64BIT_CPU '

  OS_LINKFLAGS=' -Wl,-rpath=' + os.path.abspath(TK_LIB_DIR) + ' '
  AWTK_DLL_DEPS_LIBS = NANOVG_BACKEND_LIBS + ['SDL2', 'glad'] + OS_LIBS
  OS_WHOLE_ARCHIVE =' -Wl,--whole-archive -lawtk_global -lextwidgets -lwidgets -lbase -lgpinyin -lstreams -lconf_io -lubjson -lcompressors -lfribidi -lminiz -ltkc -llinebreak -Wl,--no-whole-archive'

elif OS_NAME == 'Windows':
  if not os.path.exists(os.path.abspath(TK_BIN_DIR)) :
    os.makedirs(os.path.abspath(TK_BIN_DIR))
  if not os.path.exists(os.path.abspath(TK_LIB_DIR)) :
    os.makedirs(os.path.abspath(TK_LIB_DIR))
  if TOOLS_NAME == '' :
    OS_LIBS=['gdi32', 'user32','winmm.lib','imm32.lib','version.lib','shell32.lib','ole32.lib','Oleaut32.lib','Advapi32.lib','DelayImp.lib','psapi.lib']
    OS_FLAGS='-DWIN32 -D_WIN32 -DWINDOWS /EHsc -D_CONSOLE  /DEBUG /Od  /FS /Z7 /utf-8'
    if TARGET_ARCH == 'x86':
      OS_LINKFLAGS='/MACHINE:X86 /DEBUG \"' + WIN32_AWTK_RES + '\" '
      OS_SUBSYSTEM_CONSOLE='/SUBSYSTEM:CONSOLE,5.01  '
      OS_SUBSYSTEM_WINDOWS='/SUBSYSTEM:WINDOWS,5.01  '
      COMMON_CCFLAGS = COMMON_CCFLAGS + ' -D_WIN32 '
    else:
      OS_FLAGS = OS_FLAGS + ' -DWITH_64BIT_CPU '
      OS_LINKFLAGS='/MACHINE:X64 /DEBUG \"' + WIN32_AWTK_RES + '\" '
      OS_SUBSYSTEM_CONSOLE='/SUBSYSTEM:CONSOLE  '
      OS_SUBSYSTEM_WINDOWS='/SUBSYSTEM:WINDOWS  '
      COMMON_CCFLAGS = COMMON_CCFLAGS + ' -D_WIN64 '
    OS_WHOLE_ARCHIVE=' /DEF:"dllexports/awtk.def" '
    AWTK_DLL_DEPS_LIBS = AWTK_STATIC_LIBS + NANOVG_BACKEND_LIBS + ['SDL2', 'glad'] + OS_LIBS

  elif TOOLS_NAME == 'mingw' :
    OS_LIBS=['kernel32', 'gdi32', 'user32', 'winmm','imm32','version','shell32','ole32','Oleaut32','Advapi32','oleaut32','uuid','stdc++',"ws2_32"]
    OS_FLAGS='-DMINGW -DWINDOWS -D_CONSOLE -g -Wall'
    OS_LINKFLAGS=' -Wl,-rpath=' + os.path.abspath(TK_LIB_DIR) + ' '
    COMMON_CFLAGS=COMMON_CFLAGS+' -std=gnu99 '
    COMMON_CCFLAGS=COMMON_CCFLAGS+' -U__FLT_EVAL_METHOD__ -D__FLT_EVAL_METHOD__=0 -DUNICODE -DDECLSPEC=  ' 
    OS_WHOLE_ARCHIVE =' -Wl,--whole-archive -lawtk_global -lextwidgets -lwidgets -lbase -lgpinyin -lstreams -lconf_io -lubjson -lcompressors -lfribidi -lminiz -ltkc -llinebreak -Wl,--no-whole-archive'
    AWTK_DLL_DEPS_LIBS = AWTK_STATIC_LIBS + NANOVG_BACKEND_LIBS + ['SDL2', 'glad'] + OS_LIBS
    
  #OS_FLAGS='-DWIN32 -D_WIN32 -DWINDOWS /EHsc -D_CONSOLE  /DEBUG /Od  /FS /Z7 -D_DEBUG /MDd '
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DSDL_REAL_API -DSDL_HAPTIC_DISABLED -DSDL_SENSOR_DISABLED -DSDL_JOYSTICK_DISABLED '
  COMMON_CCFLAGS = COMMON_CCFLAGS + '-D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -D_HAS_EXCEPTIONS=0 -D_HAS_ITERATOR_DEBUGGING=0 -D_ITERATOR_DEBUG_LEVEL=0 -D_SCL_SECURE=0'
  COMMON_CCFLAGS = COMMON_CCFLAGS + '-D_SECURE_SCL=0 -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE '
  OS_PROJECTS=['3rd/SDL/SConscript']

  
CFLAGS=COMMON_CFLAGS
LINKFLAGS=OS_LINKFLAGS;
LIBPATH=[TK_LIB_DIR, TK_BIN_DIR] + OS_LIBPATH
CCFLAGS=OS_FLAGS + COMMON_CCFLAGS 
AWTK_CCFLAGS=OS_FLAGS + COMMON_CCFLAGS + ' -DWITH_WIDGET_TYPE_CHECK=1 '

STATIC_LIBS = AWTK_STATIC_LIBS + NANOVG_BACKEND_LIBS + ['SDL2', 'glad'] + OS_LIBS
SHARED_LIBS=['awtk'] + OS_LIBS;

LIBS=STATIC_LIBS

CPPPATH=[TK_ROOT, 
  TK_SRC, 
  TK_3RD_ROOT, 
  joinPath(TK_SRC, 'ext_widgets'), 
  joinPath(TK_3RD_ROOT, 'fribidi'), 
  joinPath(TK_3RD_ROOT, 'pixman'), 
  joinPath(TK_3RD_ROOT, 'cairo'),
  joinPath(TK_3RD_ROOT, 'bgfx/bgfx/include'), 
  joinPath(TK_3RD_ROOT, 'bgfx/bx/include'), 
  joinPath(TK_3RD_ROOT, 'bgfx/bimg/include'), 
  joinPath(TK_3RD_ROOT, 'agge'), 
  joinPath(TK_3RD_ROOT, 'agg/include'), 
  joinPath(TK_3RD_ROOT, 'nanovg'), 
  joinPath(TK_3RD_ROOT, 'nanovg/gl'), 
  joinPath(TK_3RD_ROOT, 'nanovg/base'), 
  joinPath(TK_3RD_ROOT, 'nanovg/agge'), 
  joinPath(TK_3RD_ROOT, 'nanovg/bgfx'), 
  joinPath(TK_3RD_ROOT, 'SDL/src'), 
  joinPath(TK_3RD_ROOT, 'SDL/include'), 
  joinPath(TK_3RD_ROOT, 'agge/src'), 
  joinPath(TK_3RD_ROOT, 'agge/include'), 
  joinPath(TK_3RD_ROOT, 'gpinyin/include'), 
  joinPath(TK_3RD_ROOT, 'libunibreak'), 
  joinPath(TK_3RD_ROOT, 'gtest/googletest'), 
  joinPath(TK_3RD_ROOT, 'gtest/googletest/include'), 
  TK_TOOLS_ROOT] + OS_CPPPATH

os.environ['LCD'] = LCD
os.environ['TK_ROOT'] = TK_ROOT
os.environ['CCFLAGS'] = CCFLAGS;
os.environ['VGCANVAS'] = VGCANVAS 
os.environ['TOOLS_NAME'] = TOOLS_NAME;
os.environ['GTEST_ROOT'] = GTEST_ROOT;
os.environ['TK_3RD_ROOT'] = TK_3RD_ROOT;
os.environ['INPUT_ENGINE'] = INPUT_ENGINE;
os.environ['NANOVG_BACKEND'] = NANOVG_BACKEND;
os.environ['NATIVE_WINDOW'] = NATIVE_WINDOW;
os.environ['GRAPHIC_BUFFER'] = GRAPHIC_BUFFER;
os.environ['FRAME_BUFFER_FORMAT'] = FRAME_BUFFER_FORMAT;
os.environ['OS_WHOLE_ARCHIVE'] = OS_WHOLE_ARCHIVE;
os.environ['AWTK_DLL_DEPS_LIBS'] = ';'.join(AWTK_DLL_DEPS_LIBS)
os.environ['STATIC_LIBS'] = ';'.join(STATIC_LIBS)

os.environ['WITH_AWTK_SO'] = 'true'
os.environ['AWTK_CCFLAGS'] = AWTK_CCFLAGS;

def has_custom_cc():
    return False

def copySharedLib(src, dst, name):
  if OS_NAME == 'Darwin':
    src = os.path.join(src, 'bin/lib'+name+'.dylib')
  elif OS_NAME == 'Linux':
    src = os.path.join(src, 'bin/lib'+name+'.so')
  elif OS_NAME == 'Windows':
    src = os.path.join(src, 'bin/'+name+'.dll')
  else:
    print('not support ' + OS_NAME)
    return
	
  src = os.path.normpath(src);
  dst = os.path.normpath(dst);
  if not os.path.exists(src):
    print('Can\'t find ' + src + '. Please build '+name+'before!')
  else:
    if not os.path.exists(dst):
        os.makedirs(dst)
    shutil.copy(src, dst)
    print(src + '==>' + dst);

def isBuildShared():
  return 'WITH_AWTK_SO' in os.environ and os.environ['WITH_AWTK_SO'] == 'true'

