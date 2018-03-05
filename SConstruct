import os

LFTK_ROOT = os.path.normpath(os.getcwd())
LFTK_SRC = os.path.join(LFTK_ROOT, 'src')
LFTK_3RD_ROOT = os.path.join(LFTK_ROOT, '3rd')
LFTK_TOOLS_ROOT = os.path.join(LFTK_ROOT, 'tools')
GTEST_ROOT = os.path.join(LFTK_ROOT, '3rd/gtest/googletest')

if os.name == 'posix':
  OS_FLAGS='-g -Wall -DLUA_USE_POSIX -DLUA_COMPAT_MODULE'
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
  
os.environ['LFTK_ROOT'] = LFTK_ROOT;
os.environ['GTEST_ROOT'] = GTEST_ROOT;

DefaultEnvironment(CCFLAGS = OS_FLAGS + ' -DLFTK_ROOT=\\\"'+LFTK_ROOT+'\\\"', 
  CPPPATH = [LFTK_ROOT, LFTK_SRC, LFTK_3RD_ROOT, LFTK_TOOLS_ROOT, 
  ] + OS_CPPPATH, 
  LIBS=['lftk_base', 'main_loop', 'lcd', 'platform', 'SDL2'] + OS_LIBS,
  LINKFLAGS=OS_LINKFLAGS,
  OS_SUBSYSTEM_CONSOLE=OS_SUBSYSTEM_CONSOLE,
  OS_SUBSYSTEM_WINDOWS=OS_SUBSYSTEM_WINDOWS,
  LIBPATH=[os.path.join(LFTK_ROOT, 'src/base'), 
  os.path.join(LFTK_3RD_ROOT, 'lua'), 
  os.path.join(LFTK_ROOT, 'src/platforms'), 
  os.path.join(LFTK_ROOT, 'src/widgets'), 
  os.path.join(LFTK_ROOT, 'src/lcd'), 
  os.path.join(LFTK_ROOT, 'src/xml'), 
  os.path.join(LFTK_ROOT, 'src/font'), 
  os.path.join(LFTK_ROOT, 'src/main_loop'), 
  os.path.join(LFTK_ROOT, 'src/ui_loader'), 
  os.path.join(LFTK_ROOT, 'src/image_loader'), 
  os.path.join(LFTK_ROOT, 'tools/common'), 
  os.path.join(LFTK_ROOT, 'tools/font_gen'), 
  os.path.join(LFTK_ROOT, 'demos'), 
  os.path.join(LFTK_ROOT, 'tools/image_gen'), 
  os.path.join(LFTK_ROOT, 'tools/theme_gen')] + OS_LIBPATH)

SConscript([
  'src/platforms/SConscript', 
  'src/base/SConscript', 
  'src/lcd/SConscript', 
  'src/main_loop/SConscript', 
  'src/xml/SConscript', 
  'src/font/SConscript', 
  'src/ui_loader/SConscript', 
  'src/image_loader/SConscript', 
  'demos/SConscript', 
  'tools/common/SConscript', 
  'tools/theme_gen/SConscript', 
  'tools/font_gen/SConscript', 
  'tools/image_gen/SConscript', 
  'tests/SConscript',
  '3rd/lua/SConscript',
  'lua/SConscript'
  ])
