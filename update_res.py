#!/usr/bin/python

import os
import glob
import shutil
import platform
import copy

def joinPath(root, subdir):
  return os.path.normpath(os.path.join(root, subdir))

DPI='x1'
CWD=os.getcwd()
BIN_DIR=joinPath(CWD, 'bin')
APP_DIR=joinPath(CWD, 'demos')
INPUT_DIR=joinPath(APP_DIR, 'res/raw')
OUTPUT_DIR=joinPath(APP_DIR, 'res/inc')
RESOURCE_C=joinPath(APP_DIR, 'resource.c')

OS_NAME=platform.system()

def toTarget(name):
  if OS_NAME == 'Windows':
    return 'bin\\'+name+'.exe'
  else:
    return 'bin/'+name;
  
def toExe(name):
  if OS_NAME == 'Windows':
    return joinPath(BIN_DIR, name+'.exe')
  else:
    return joinPath(BIN_DIR, name)
	
def buildTools():
  os.system('scons '+toTarget('resgen'))
  os.system('scons '+toTarget('themegen'))
  os.system('scons '+toTarget('imagegen'))
  os.system('scons '+toTarget('xml_to_ui'))


def buildAll():
  os.system('scons')

def removeDir(path):
  if os.path.isdir(path):
     print('rmdir:' + path);
     shutil.rmtree(path)

def prepare():
  removeDir(OUTPUT_DIR)
  os.makedirs(joinPath(OUTPUT_DIR, 'theme'));
  os.makedirs(joinPath(OUTPUT_DIR, 'images'));
  os.makedirs(joinPath(OUTPUT_DIR, 'fonts'));
  os.makedirs(joinPath(OUTPUT_DIR, 'strings'));
  os.makedirs(joinPath(OUTPUT_DIR, 'ui'));

def themegen(raw, inc):
  print(toExe('themegen') + ' ' + joinPath(INPUT_DIR, raw) + ' ' + joinPath(OUTPUT_DIR, inc))
  os.system(toExe('themegen') + ' ' + joinPath(INPUT_DIR, raw) + ' ' + joinPath(OUTPUT_DIR, inc))

def themegen_bin(raw, bin):
  print(toExe('themegen') + ' ' + joinPath(INPUT_DIR, raw) + ' ' + joinPath(INPUT_DIR, bin) + ' bin')
  os.system(toExe('themegen') + ' ' + joinPath(INPUT_DIR, raw) + ' ' + joinPath(INPUT_DIR, bin) + ' bin')

def strgen(raw, inc):
  print(toExe('strgen') + ' ' + joinPath(INPUT_DIR, raw) + ' ' + joinPath(OUTPUT_DIR, inc))
  os.system(toExe('strgen') + ' ' + joinPath(INPUT_DIR, raw) + ' ' + joinPath(OUTPUT_DIR, inc))

def strgen_bin(raw, bin):
  print(toExe('strgen') + ' ' + joinPath(INPUT_DIR, raw) + ' ' + joinPath(INPUT_DIR, bin) + ' bin')
  os.system(toExe('strgen') + ' ' + joinPath(INPUT_DIR, raw) + ' ' + joinPath(INPUT_DIR, bin) + ' bin')

def resgen(raw, inc):
  os.system(toExe('resgen') + ' ' + joinPath(INPUT_DIR, raw) + ' ' + joinPath(OUTPUT_DIR, inc))

def fontgen(raw, text, inc, size):
  os.system(toExe('fontgen') + ' ' + joinPath(INPUT_DIR, raw) + ' ' + joinPath(INPUT_DIR, text) +' ' + joinPath(OUTPUT_DIR, inc) + ' ' + str(size))

def imagegen(raw, inc):
  print(toExe('imagegen') + ' ' + raw + ' ' + inc)
  os.system(toExe('imagegen') + ' ' + raw + ' ' + inc)
  inc=inc.replace('.data', '.res')
  resgen(raw, inc)

def xml_to_ui(raw, inc):
  os.system(toExe('xml_to_ui') + ' ' + raw + ' ' + inc)

def xml_to_ui_bin(raw, bin):
  os.system(toExe('xml_to_ui') + ' ' + raw + ' ' + bin + ' bin')

def gen_all_theme():
  for f in glob.glob(joinPath(INPUT_DIR, 'theme/*.xml')):
    inc=copy.copy(f);
    raw=copy.copy(f);
    bin=copy.copy(f);
    inc=inc.replace('.xml', '.data')
    inc=inc.replace(INPUT_DIR, OUTPUT_DIR)
    themegen(raw, inc)
    bin=bin.replace('.xml', '.bin')
    themegen_bin(raw, bin)


def gen_all_image(): 
  for f in glob.glob(joinPath(INPUT_DIR, 'images/'+DPI+'/*.*')):
    inc=copy.copy(f);
    raw=copy.copy(f);
    basename=os.path.basename(inc);
    inc=joinPath(OUTPUT_DIR, 'images/'+basename);
    inc=inc.replace('.png', '.data')
    inc=inc.replace('.jpg', '.data')
    imagegen(raw, inc)

def gen_all_ui():
  for f in glob.glob(joinPath(INPUT_DIR, 'ui/*.xml')):
    inc=copy.copy(f);
    raw=copy.copy(f);
    bin=copy.copy(f);
    inc=inc.replace('.xml', '.data')
    inc=inc.replace(INPUT_DIR, OUTPUT_DIR)
    xml_to_ui(raw, inc)
    bin=bin.replace('.xml', '.bin')
    xml_to_ui_bin(raw, bin)

def gen_all_font():
  resgen('fonts/default.ttf', 'fonts/default.res');
  resgen('fonts/default.mini.ttf', 'fonts/default.mini.res');
  fontgen('fonts/default.ttf', 'fonts/text.txt', 'fonts/default.data', 18);

def gen_all_string():
  strgen('strings/strings.xml', 'strings');
  strgen_bin('strings/strings.xml', 'strings');

def gen_all():
  gen_all_string()
  gen_all_font()
  gen_all_image()
  gen_all_ui()
  gen_all_theme()

def writeResult(str):
  fd = os.open(RESOURCE_C, os.O_RDWR|os.O_CREAT|os.O_TRUNC)
  os.write(fd, str)
  os.close(fd)

def genIncludes(files):
  str1 = ""
  for f in files:
    incf = copy.copy(f);
    incf=incf.replace(APP_DIR, ".");
    incf=incf.replace('\\', '/');
    incf=incf.replace('./', '');
    str1 += '#include "'+incf+'"\n'

  return str1

def gen_res_c():
  result = '#include "tk.h"\n'
  result += '#include "base/resource_manager.h"\n'

  result += '#ifndef WITH_FS_RES\n'
  files=glob.glob(joinPath(OUTPUT_DIR, 'strings/*.data')) \
    + glob.glob(joinPath(OUTPUT_DIR, 'theme/*.data')) \
    + glob.glob(joinPath(OUTPUT_DIR, 'ui/*.data')) 

  result += genIncludes(files);

  result += "#ifdef WITH_STB_IMAGE\n"
  files=glob.glob(joinPath(OUTPUT_DIR, 'images/*.res')) 
  result += genIncludes(files)
  result += "#else\n"
  files=glob.glob(joinPath(OUTPUT_DIR, 'images/*.data')) 
  result += genIncludes(files)
  result += '#endif/*WITH_STB_IMAGE*/\n'

  result += "#ifdef WITH_STB_FONT\n"
  result += "#ifdef WITH_MINI_FONT\n"
  files=glob.glob(joinPath(OUTPUT_DIR, 'fonts/default.mini.res')) 
  result += genIncludes(files)
  result += "#else/*WITH_MINI_FONT*/\n"
  files=glob.glob(joinPath(OUTPUT_DIR, 'fonts/default.res')) 
  result += genIncludes(files)
  result += '#endif/*WITH_MINI_FONT*/\n'
  result += "#else/*WITH_STB_FONT*/\n"
  files=glob.glob(joinPath(OUTPUT_DIR, 'fonts/*.data')) 
  result += genIncludes(files)
  result += '#endif/*WITH_STB_FONT*/\n'

  result += '#endif/*WITH_FS_RES*/\n'

  result += '\n';
  result += 'ret_t resource_init(void) {\n'
  result += '  resource_manager_t* rm = resource_manager();\n\n'
  result += ''

  result += '#ifdef WITH_FS_RES\n'
  result += '  resource_manager_load(rm, RESOURCE_TYPE_THEME, "default");\n'
  result += '  resource_manager_load(rm, RESOURCE_TYPE_FONT, "default");\n'
  result += '#else\n'

  files=glob.glob(joinPath(OUTPUT_DIR, '**/*.data'))
  for f in files:
    incf = copy.copy(f);
    basename = incf.replace(OUTPUT_DIR, '.');
    basename = basename.replace('\\', '/');
    basename = basename.replace('/fonts/', '/font/');
    basename = basename.replace('/images/', '/image/');
    basename = basename.replace('./', '');
    basename = basename.replace('/', '_');
    basename = basename.replace('.data', '');
    result += '  resource_manager_add(rm, '+basename+');\n'
  result += '#endif\n'

  result += '\n'
  result += '  tk_init_resources();\n'
  result += '  return RET_OK;\n'
  result += '}\n'
  writeResult(result);

def run():
  buildTools()
  prepare()
  gen_all()
  gen_res_c()
  buildAll()

run()
