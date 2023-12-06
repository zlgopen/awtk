import os
import sys
import json
import atexit
import shutil
import platform
import res_config
import compile_config
from SCons import Script, Environment

PLATFORM = platform.system()

SRT_SCONS_CONFIG_FUN = 'get_scons_config'
SRT_SCONS_CONFIG_SCRIPT = 'SCONS_CONFIG_SCRIPT'
SRT_SCONS_CONFIG_SCRIPT_ARGV = 'SCONS_CONFIG_SCRIPT_ARGV'

COMPILE_CONFIG = {
  'AWTK_ROOT' : { 'value' : None, 'desc' : ['awtk root'], 'help_info' : 'set link awtk root, AWTK_ROOT=XXXXX'},
  'LINUX_FB' : { 'value' : False, 'desc' : ['use linux\'s building'], 'help_info' : 'use linux\'s compile tools prefix building, value is true or false'},
  'MVVM_ROOT' : { 'value' : None, 'desc' : ['awtk\'s mvvm root'], 'help_info' : 'set link awtk\'s mvvm root, MVVM_ROOT=XXXXX'},
  'WITH_MVVM' : { 'value' : False, 'desc' : ['use mvvm'], 'help_info' : 'use mvvm\'s lib, value is true or false'},
  'WITH_JERRYSCRIPT' : { 'value' : False, 'desc' : ['use mvvm\'s js'], 'help_info' : 'use mvvm js\'s lib, value is true or false'},
  'WITH_IOTJS' : { 'value' : False, 'desc' : ['use mvvm\'s iotjs'], 'help_info' : 'use mvvm iotjs\'s lib, value is true or false'},
  'AWFLOW_ROOT' : { 'value' : None, 'desc' : ['awtk\'s awflow root'], 'help_info' : 'set link awtk\'s awflow root, AWFLOW_ROOT=XXXXX'},
  'WITH_AWFLOW' : { 'value' : False, 'desc' : ['use awflow'], 'help_info' : 'use awflow\'s lib, value is true or false'},
  SRT_SCONS_CONFIG_SCRIPT : { 'value' : None, 'save_file' : False, 'desc' : ['set script file path, this is script has {0}(COMPILE_CONFIG, ARGUMENTS, argv) function'.format(SRT_SCONS_CONFIG_FUN)], 'help_info' : 'set res config file path, this is script must has {0}(COMPILE_CONFIG, ARGUMENTS, argv) function, {0}\'s function return compile_config\'s class, CONFIG_SCRIPT=XXXXX'.format(SRT_SCONS_CONFIG_FUN)},
  SRT_SCONS_CONFIG_SCRIPT_ARGV : { 'value' : None, 'save_file' : False, 'desc' : ['value is {0}\s argv for script file '.format(SRT_SCONS_CONFIG_FUN)], 'help_info' : 'value is {0}\s argv for script file\'s {0}, SCONS_CONFIG_SCRIPT_ARGV=XXXXX'.format(SRT_SCONS_CONFIG_FUN)},
  'BUILD_DIR' : { 'value' : None, 'desc' : ['build dir, compile temp file dir'], 'help_info' : 'set build dir, save compile temp file dir or *.obj/.*o dir, BUILD_DIR=XXXXX'},
#   'APP_BIN_DIR' : { 'value' : None, 'desc' : ['build bin dir'], 'help_info' : 'set build bin dir, APP_BIN_DIR=XXXXX'},
#   'APP_LIB_DIR' : { 'value' : None, 'desc' : ['build lib dir'], 'help_info' : 'set build lib dir, APP_LIB_DIR=XXXXX'},
  'DEBUG' : { 'value' : None, 'desc' : ['awtk\'s compile is debug'], 'help_info' : 'awtk\'s compile is debug, value is true or false' },
  'PDB' : { 'value' : True, 'desc' : ['export pdb file'], 'help_info' : 'export pdb file, value is true or false' },
  'FLAGS' : { 'value' : None, 'desc' : ['compile flags'], 'help_info' : 'set compile\'s flags, so care of system and compile tools'},
  'LIBS' : { 'value' : [], 'desc' : ['compile libs'], 'help_info' : 'set compile\'s libs, so care of system and compile tools, use \',\' split muliple libraries '},
  'LIBPATH' : { 'value' : [], 'desc' : ['compile lib paths'], 'help_info' : 'set compile\'s lib paths, so care of system and compile tools, use \',\' split muliple librarie\'s paths '},
  'CPPPATH' : { 'value' : [], 'desc' : ['compile include paths'], 'help_info' : 'set compile\'s include paths, so care of system and compile tools, use \',\' split muliple include path '},
  'SHARED' : { 'value' : True, 'desc' : ['compile is SharedLibrary'], 'help_info' : 'app\'s compile is Shared Library, value is true or false' },
  'IDL_DEF' : { 'value' : True, 'desc' : ['compile build idl def file'], 'help_info' : 'app\'s compile build idl def file, value is true or false' },
  'LCD' : { 'value' : None, 'save_file' : False, 'desc' : ['app\'s lcd\'s size'], 'help_info' : 'app\'s lcd\'s size, value is [lcd_width]_[lcd_height], example is LCD=320_480' },
  'LANGUAGE' : { 'value' : None, 'save_file' : False, 'desc' : ['app\'s language'], 'help_info' : 'app\'s language, value is [country]_[language], example is LANGUAGE=zh_CH' },
  'FONT' : { 'value' : None, 'save_file' : False, 'desc' : ['app\'s font\'s name'], 'help_info' : 'app\'s font\'s name, FONT=XXXXX ' },
  'THEME' : { 'value' : None, 'save_file' : False, 'desc' : ['app\'s default\'s theme\'s name'], 'help_info' : 'app\'s default\'s theme\'s name, THEME=XXXXX ' },
  'RES_ROOT' : { 'value' : None, 'save_file' : False, 'desc' : ['app\'s res root'], 'help_info' : 'app\'s res root, RES_ROOT=XXXXX ' },
  'WIN32_RES' : { 'value' : None, 'desc' : ['app\'s win32 res path'], 'help_info' : 'app\'s win32 res path, WIN32_RES=XXXXX, value\'s default=\'awtk/win32_res/awtk.res\' ' },
}

def set_compile_config(config) :
  global COMPILE_CONFIG
  COMPILE_CONFIG = config

def getTkcOnly():
    env = os.environ
    if 'TKC_ONLY' in env:
        return env['TKC_ONLY'] == 'True'
    else:
        return False

def join_path(root, sub):
    return os.path.abspath(os.path.join(root, sub))


def mkdir_if_not_exist(fullpath):
    if os.path.exists(fullpath):
        print(fullpath+' exist.')
    else:
        os.makedirs(fullpath)


def load_project_json(root, filename):
    content = None
    config_json = join_path(root, filename)
    if not os.path.exists(config_json) or filename == '':
        config_json = join_path(root, 'project.json')
        if not os.path.exists(config_json) :
            print(config_json + ' is not exists.')
            return
    content = res_config.res_config()
    content.load_file(config_json)
    return content

class AppHelperBase:
    def set_deps(self, DEPENDS_LIBS):
        self.DEPENDS_LIBS = DEPENDS_LIBS
        return self

    def set_src_dir(self, SRC_DIR):
        self.SRC_DIR = SRC_DIR
        return self

    def set_tkc_only(self):
        self.AWTK_LIBS = ['tkc']
        self.TKC_ONLY = True

        return self

    def set_libs(self, APP_LIBS):
        self.APP_LIBS = APP_LIBS
        return self

    def set_dll_def(self, DEF_FILE):
        self.DEF_FILE = DEF_FILE
        return self

    def set_dll_def_processor(self, processor):
        self.DEF_FILE_PROCESSOR = processor
        return self

    def set_ccflags(self, APP_CCFLAGS):
        self.APP_CCFLAGS = APP_CCFLAGS
        return self

    def set_cxxflags(self, APP_CXXFLAGS):
        self.APP_CXXFLAGS = APP_CXXFLAGS
        return self

    def add_deps(self, DEPENDS_LIBS):
        self.DEPENDS_LIBS += DEPENDS_LIBS
        return self

    def add_libs(self, APP_LIBS):
        self.APP_LIBS += APP_LIBS
        return self

    def add_platform_libs(self, plat, PLATFORM_LIBS):
        if plat == PLATFORM:
            self.PLATFORM_LIBS += PLATFORM_LIBS
        return self

    def add_libpath(self, APP_LIBPATH):
        self.APP_LIBPATH += APP_LIBPATH
        return self

    def add_platform_libpath(self, plat, APP_LIBPATH):
        if plat == PLATFORM:
            self.APP_LIBPATH += APP_LIBPATH
        return self

    def add_cpppath(self, APP_CPPPATH):
        self.APP_CPPPATH += APP_CPPPATH
        return self

    def add_platform_cpppath(self, plat, APP_CPPPATH):
        if plat == PLATFORM:
            self.APP_CPPPATH += APP_CPPPATH
        return self

    def add_ccflags(self, APP_CCFLAGS):
        self.APP_CCFLAGS += APP_CCFLAGS
        return self

    def add_cflags(self, APP_CFLAGS):
        self.APP_CFLAGS += APP_CFLAGS
        return self

    def add_platform_ccflags(self, plat, APP_CCFLAGS):
        if plat == PLATFORM:
            self.APP_CCFLAGS += APP_CCFLAGS
        return self

    def use_std_cxx(self, VERSION):
      if platform.system() == 'Windows':
        self.APP_CXXFLAGS += ' /std:c++'+str(VERSION)+' '
      else:
        self.APP_CXXFLAGS += ' -std=c++'+str(VERSION)+' '

    def add_cxxflags(self, APP_CXXFLAGS):
        self.APP_CXXFLAGS += APP_CXXFLAGS
        return self

    def add_platform_cxxflags(self, plat, APP_CXXFLAGS):
        if plat == PLATFORM:
            self.APP_CXXFLAGS += APP_CXXFLAGS
        return self

    def add_linkflags(self, APP_LINKFLAGS):
        self.APP_LINKFLAGS += APP_LINKFLAGS
        return self

    def add_platform_linkflags(self, plat, APP_LINKFLAGS):
        if plat == PLATFORM:
            self.APP_LINKFLAGS += APP_LINKFLAGS
        return self
        
    def root_get_scons_db_files(self, root):
        scons_db_files = []
        scons_db_filename = ".sconsign.dblite"

        for f in os.listdir(root):
            full_path = join_path(root, f)
            if os.path.isfile(full_path) and f == scons_db_filename:
                scons_db_files.append(full_path)
            elif os.path.isdir(full_path) and f != "." and f != "..":
                self.root_get_scons_db_files(full_path)
        return scons_db_files

    def check_and_remove_scons_db(self, root):
        scons_db_files = []
        scons_db_files = self.root_get_scons_db_files(root)

        if sys.version_info.major == 2:
            import cPickle as pickle
        else:
            import pickle

        for f in scons_db_files:
            try:
                with open(f, "rb") as fs:
                    pickle.load(fs)
                    fs.close()
            except ValueError as e:
                fs.close()
                os.remove(f)

    def SConscript(self, SConscriptFiles):
        if not self.BUILD_DIR:
            Script.SConscript(SConscriptFiles)
        else:
            env = Environment.Environment()
            env.Default(self.BUILD_DIR)
            for sc in SConscriptFiles:
                dir = os.path.dirname(sc)
                build_dir = os.path.join(self.BUILD_DIR, dir)
                Script.SConscript(sc, variant_dir=build_dir, duplicate=False)

    def get_curr_config(self) :
        return compile_config.get_curr_config()

    def get_complie_helper_by_script(self, ARGUMENTS, script_path, script_argv) :
        global COMPILE_CONFIG
        script_path = os.path.abspath(script_path)
        if os.path.exists(script_path) :
            import importlib
            script_dir = os.path.dirname(script_path)
            file_name = os.path.basename(script_path)
            module_name, ext = os.path.splitext(file_name)
            sys.path.insert(0, script_dir)
            script = importlib.import_module(module_name)
            sys.path.remove(script_dir)
            if hasattr(script, SRT_SCONS_CONFIG_FUN) :
                return script.get_scons_config(COMPILE_CONFIG, ARGUMENTS, script_argv)
            else :
                sys.exit(script_path + ' script not found get_res_config function')
        else :
            sys.exit('res_config_file sopt not found :' + script_path)

    def __init__(self, ARGUMENTS):
        global COMPILE_CONFIG
        global SRT_SCONS_CONFIG_SCRIPT
        global SRT_SCONS_CONFIG_SCRIPT_ARGV
        if SRT_SCONS_CONFIG_SCRIPT in ARGUMENTS :
            self.complie_helper = self.get_complie_helper_by_script(ARGUMENTS, ARGUMENTS[SRT_SCONS_CONFIG_SCRIPT], ARGUMENTS.get(SRT_SCONS_CONFIG_SCRIPT_ARGV, ''))
        else :
            self.complie_helper = compile_config.complie_helper()
            self.complie_helper.set_compile_config(COMPILE_CONFIG)
            self.complie_helper.scons_user_sopt(ARGUMENTS)
        compile_config.set_curr_config(self.complie_helper)

        APP_ROOT = os.path.normpath(os.getcwd())

        self.SRC_DIR = 'src'
        self.TKC_ONLY = getTkcOnly();
        self.ARGUMENTS = ARGUMENTS
        self.DEF_FILE = None
        self.DEF_FILE_PROCESSOR = None
        self.DEPENDS_LIBS = []
        self.GEN_IDL_DEF = True
        self.BUILD_SHARED = True
        self.LINUX_FB = self.complie_helper.get_value('LINUX_FB', False)
        self.AWTK_ROOT = self.getAwtkRoot()
        self.awtk = self.getAwtkConfig()
        self.AWTK_LIBS = self.awtk.LIBS
        self.AWTK_CFLAGS = self.awtk.CFLAGS
        self.AWTK_CCFLAGS = self.awtk.CCFLAGS
        self.APP_ROOT = APP_ROOT
        self.BUILD_DIR = self.complie_helper.get_value('BUILD_DIR', '')
        self.BIN_DIR = os.path.join(self.BUILD_DIR, 'bin')
        self.LIB_DIR = os.path.join(self.BUILD_DIR, 'lib')
        self.APP_BIN_DIR = os.path.join(APP_ROOT, self.BIN_DIR)
        self.APP_LIB_DIR = os.path.join(APP_ROOT, self.LIB_DIR)
        self.APP_SRC = os.path.join(APP_ROOT, 'src')
        self.APP_RES = os.path.join(APP_ROOT, 'res')
        self.APP_LIBS = self.complie_helper.get_value('LIBS', [])
        self.APP_LINKFLAGS = ''
        self.PLATFORM_LIBS = []
        self.APP_TOOLS = None
        self.WITH_JERRYSCRIPT = False
        self.WITH_IOTJS = False
        self.MVVM_ROOT = None
        self.AWFLOW_ROOT = None
        if self.LINUX_FB and 'DEBUG' not in ARGUMENTS :
            self.DEBUG = False
        else :
            self.DEBUG = self.complie_helper.get_value('DEBUG', True)

        self.parseArgs(self.awtk, ARGUMENTS)
        self.APP_CPPPATH = [self.APP_SRC, self.APP_RES] + self.complie_helper.get_value('CPPPATH', [])
        self.APP_LIBPATH = [self.APP_LIB_DIR, self.APP_BIN_DIR] + self.complie_helper.get_value('LIBPATH', [])

        mkdir_if_not_exist(self.APP_BIN_DIR)
        mkdir_if_not_exist(self.APP_LIB_DIR)

        os.environ['APP_SRC'] = self.APP_SRC
        os.environ['APP_ROOT'] = self.APP_ROOT
        os.environ['BIN_DIR'] = self.APP_BIN_DIR
        os.environ['LIB_DIR'] = self.APP_LIB_DIR
        os.environ['LINUX_FB'] = 'false'
        os.environ['BUILD_DIR'] = self.BUILD_DIR
        if self.LINUX_FB:
            os.environ['LINUX_FB'] = 'true'

        self.WITH_JERRYSCRIPT = self.complie_helper.get_value('WITH_JERRYSCRIPT', False)
        self.WITH_IOTJS = self.complie_helper.get_value('WITH_IOTJS', False)

        WITH_MVVM = self.complie_helper.get_value('WITH_MVVM', False)
        MVVM_ROOT = self.complie_helper.get_value('MVVM_ROOT', '')
        if WITH_MVVM or os.path.exists(MVVM_ROOT):
            os.environ['WITH_MVVM'] = 'true'
            if not os.path.exists(MVVM_ROOT):
                MVVM_ROOT = self.getMvvmRoot()
            self.MVVM_ROOT = MVVM_ROOT
            print("MVVM_ROOT: " + self.MVVM_ROOT)

        WITH_AWFLOW = self.complie_helper.get_value('WITH_AWFLOW', False)
        AWFLOW_ROOT = self.complie_helper.get_value('AWFLOW_ROOT', '')
        print(WITH_AWFLOW)
        if WITH_AWFLOW or os.path.exists(AWFLOW_ROOT):
            os.environ['WITH_AWFLOW'] = 'true'
            if not os.path.exists(AWFLOW_ROOT):
                AWFLOW_ROOT = self.getAwflowRoot()
            self.AWFLOW_ROOT = AWFLOW_ROOT
            print("AWFLOW_ROOT: " + self.AWFLOW_ROOT)
        
        if self.TKC_ONLY :
            self.set_tkc_only()

        print("AWTK_ROOT: " + self.AWTK_ROOT)
        print("TKC_ONLY: " + str(self.TKC_ONLY))
        print(ARGUMENTS)

    def getAwtkConfig(self):
        sys.path.insert(0, self.AWTK_ROOT)
        tmp_cwd = os.getcwd()
        os.chdir(self.AWTK_ROOT)
        compile_config.set_curr_app_root(tmp_cwd)
        tmp_complie_helper = compile_config.get_curr_config()
        compile_config.set_app_win32_res(tmp_complie_helper.get_value('WIN32_RES', None))
        compile_config.set_curr_config(None)
        import awtk_config as awtk
        os.chdir(tmp_cwd)
        compile_config.set_curr_config(tmp_complie_helper)
        return awtk

    def saveUsesSdkInfo(self):
        awtk = self.awtk
        release_id = ''
        filename = os.path.join(self.AWTK_ROOT, 'component.json')
        if os.path.exists(filename):
            with open(filename, 'r') as f:
                component_content = f.read()
                if len(component_content) > 0:
                    component_json = json.loads(component_content)
                    if 'release_id' in component_json:
                        release_id = component_json['release_id']
                        if sys.version_info < (3, 0):
                            release_id = release_id.encode('ascii')

        content = '{\n'
        content += '  "compileSDK": {\n'
        content += '    "awtk": {\n'
        content += '      "path": "' + \
            self.AWTK_ROOT.replace('\\', '/') + '",\n'
        content += '      "release_id": "' + release_id + '",\n'
        content += '      "nanovg_backend": "' + awtk.NANOVG_BACKEND + '"\n'
        content += '    }\n'
        content += '  }\n'
        content += '}'

        filename = os.path.join(self.APP_BIN_DIR, 'uses_sdk.json')
        if sys.version_info < (3, 0):
            with open(filename, 'w') as f:
                f.write(content)
        else:
            with open(filename, 'w', encoding='utf8') as f:
                f.write(content)

    def isBuildShared(self):
        return self.BUILD_SHARED

    def copyAwtkSharedLib(self):
        if self.TKC_ONLY:
            self.awtk.copySharedLib(self.AWTK_ROOT, self.APP_BIN_DIR, 'tkc')
        else:
            self.awtk.copySharedLib(self.AWTK_ROOT, self.APP_BIN_DIR, 'awtk')

        for iter in self.DEPENDS_LIBS:
            for so in iter['shared_libs']:
                src = os.path.join(iter['root'], self.BUILD_DIR)
                self.awtk.copySharedLib(src, self.APP_BIN_DIR, so)

    def cleanAwtkSharedLib(self):
        if self.TKC_ONLY:
            self.awtk.cleanSharedLib(self.APP_BIN_DIR, 'tkc')
        else:
            self.awtk.cleanSharedLib(self.APP_BIN_DIR, 'awtk')

        for iter in self.DEPENDS_LIBS:
            for so in iter['shared_libs']:
                self.awtk.cleanSharedLib(self.APP_BIN_DIR, so)
        shutil.rmtree(self.APP_BIN_DIR);
        shutil.rmtree(self.APP_LIB_DIR);

    def genIdlAndDef(self):
        if self.DEF_FILE:
            print(self.DEF_FILE)
        else:
            return
        defDirlist = ''
        if self.APP_ROOT != self.AWTK_ROOT:
            defDirlist += os.path.abspath(self.AWTK_ROOT + '/tools/idl_gen/idl.json') + ';'
        for defLib in self.DEPENDS_LIBS :
            tmp_filename = os.path.abspath(defLib["root"] + '/idl/idl.json')
            if not os.path.exists(tmp_filename) :
                tmp_filename = os.path.abspath(defLib["root"] + '/idl.json')
            defDirlist += tmp_filename + ';'

        idl_gen_tools = os.path.join(self.AWTK_ROOT, 'tools/idl_gen/index.js')
        dll_def_gen_tools = os.path.join(
            self.AWTK_ROOT, 'tools/dll_def_gen/index.js')

        cmd = 'node ' + '"' + idl_gen_tools + '"' + ' idl/idl.json ' + self.SRC_DIR
        if defDirlist != '' :
            cmd += ' "' + defDirlist + '" '
        if os.system(cmd) != 0:
            print('exe cmd: ' + cmd + ' failed.')

        cmd = 'node ' + '"' + dll_def_gen_tools + '"' + \
            ' idl/idl.json ' + self.DEF_FILE
        if os.system(cmd) != 0:
            print('exe cmd: ' + cmd + ' failed.')
        else:
            if self.DEF_FILE_PROCESSOR != None:
                self.DEF_FILE_PROCESSOR()

    def parseArgs(self, awtk, ARGUMENTS):
        APP_RES_ROOT = '../res'
        APP_THEME = 'default'
        LCD_WIDTH = '320'
        LCD_HEIGHT = '480'
        LCD_ORIENTATION = '0'
        APP_DEFAULT_FONT = 'default'
        APP_DEFAULT_LANGUAGE = 'zh'
        APP_DEFAULT_COUNTRY = 'CN'

        config = load_project_json(self.APP_ROOT, 'project.json')
        if config != None:
            APP_THEME = config.get_res_actived_theme()
            LCD_WIDTH = config.get_res_w(APP_THEME)
            LCD_HEIGHT = config.get_res_h(APP_THEME)
            APP_DEFAULT_LANGUAGE = config.get_res_language()
            APP_DEFAULT_COUNTRY = config.get_res_country()
            APP_RES_ROOT = config.get_res_res_root()
            LCD_ORIENTATION = config.get_res_lcd_orientation(APP_THEME)

        LCD = self.complie_helper.get_value('LCD', '')
        if len(LCD) > 0:
            wh = LCD.split('_')
            if len(wh) >= 1:
                LCD_WIDTH = wh[0]
            if len(wh) >= 2:
                LCD_HEIGHT = wh[1]

        FONT = self.complie_helper.get_value('FONT', '')
        if len(FONT) > 0:
            APP_DEFAULT_FONT = FONT

        APP_THEME = self.complie_helper.get_value('THEME', APP_THEME)

        LANGUAGE = self.complie_helper.get_value('LANGUAGE', '')
        if len(LANGUAGE) > 0:
            lan = LANGUAGE.split('_')
            if len(lan) >= 1:
                APP_DEFAULT_LANGUAGE = lan[0]
            if len(lan) >= 2:
                APP_DEFAULT_COUNTRY = lan[1]

        APP_RES_ROOT = self.complie_helper.get_value('RES_ROOT', APP_RES_ROOT)
        self.APP_RES = os.path.abspath(os.path.join(self.APP_BIN_DIR, APP_RES_ROOT))

        self.BUILD_SHARED = self.complie_helper.get_value('SHARED', False)
        self.GEN_IDL_DEF = self.complie_helper.get_value('IDL_DEF', True)
        
        if not self.LINUX_FB :
            if LCD_ORIENTATION == '90' or LCD_ORIENTATION == '270' :
                tmp = LCD_WIDTH;
                LCD_WIDTH = LCD_HEIGHT;
                LCD_HEIGHT = tmp;

        APP_CCFLAGS = ' -DLCD_WIDTH=' + LCD_WIDTH + ' -DLCD_HEIGHT=' + LCD_HEIGHT + ' '
        APP_CCFLAGS = APP_CCFLAGS + ' -DAPP_DEFAULT_FONT=\\\"' + APP_DEFAULT_FONT + '\\\" '
        APP_CCFLAGS = APP_CCFLAGS + ' -DAPP_THEME=\\\"' + APP_THEME + '\\\" '
        APP_CCFLAGS = APP_CCFLAGS + ' -DAPP_RES_ROOT=\"\\\"' + APP_RES_ROOT + '\\\"\" '
        APP_CCFLAGS = APP_CCFLAGS + ' -DAPP_DEFAULT_LANGUAGE=\\\"' + \
            APP_DEFAULT_LANGUAGE + '\\\" '
        APP_CCFLAGS = APP_CCFLAGS + ' -DAPP_DEFAULT_COUNTRY=\\\"' + \
            APP_DEFAULT_COUNTRY + '\\\" '
        APP_CCFLAGS = APP_CCFLAGS + ' -DAPP_ROOT=\"\\\"' + \
            self.APP_ROOT + '\\\"\" '

        self.APP_CFLAGS = ''
        self.APP_CCFLAGS = APP_CCFLAGS + self.complie_helper.get_value('FLAGS', '')
        self.APP_CXXFLAGS = ''

        if PLATFORM == 'Linux':
            self.APP_LINKFLAGS += ' -Wl,-rpath=' + self.APP_BIN_DIR + ' '

        if not self.isBuildShared() and hasattr(awtk, 'AWTK_CCFLAGS'):
            self.AWTK_CCFLAGS = awtk.AWTK_CCFLAGS

        if self.isBuildShared():
            if not self.TKC_ONLY :
                self.AWTK_LIBS = awtk.SHARED_LIBS
            self.APP_LIBPATH = [self.APP_BIN_DIR, self.APP_LIB_DIR]

        if hasattr(awtk, 'TOOLS_NAME') :
            if awtk.TOOLS_NAME != '':
                if awtk.TOOLS_NAME == 'mingw':
                    self.APP_TOOLS = ['mingw']
            elif awtk.TOOLS_NAME == '' and PLATFORM == 'Windows':
                APP_TOOLS = ['msvc', 'masm', 'mslink', "mslib"]

        os.environ['BUILD_SHARED'] = str(self.isBuildShared())
        if LCD_ORIENTATION == '90' or LCD_ORIENTATION == '270' :
            print(LCD_HEIGHT, LCD_WIDTH, "orentation_" + LCD_ORIENTATION)
        else :
            print(LCD_WIDTH, LCD_HEIGHT, "orentation_" + LCD_ORIENTATION)

    def prepare(self):
        if self.GEN_IDL_DEF and not self.LINUX_FB:
            self.genIdlAndDef()

        if self.isBuildShared():
            self.copyAwtkSharedLib()

        self.saveUsesSdkInfo()

    def getAwtkRoot(self):
        AWTK_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
        if self.LINUX_FB:
            AWTK_ROOT = os.path.join(os.path.dirname(AWTK_ROOT), 'awtk-linux-fb')
        return AWTK_ROOT

    def getMvvmRoot(self):
        MVVM_ROOT = os.path.join(os.path.dirname(self.getAwtkRoot()), 'awtk-mvvm')
        return MVVM_ROOT

    def getAwflowRoot(self):
        AWFLOW_ROOT = os.path.join(os.path.dirname(self.getAwtkRoot()), 'aw-flow')
        return AWFLOW_ROOT

    def call(self, DefaultEnvironment):
        awtk = self.awtk
        CPPPATH = awtk.CPPPATH + self.APP_CPPPATH
        LINKFLAGS = awtk.LINKFLAGS + self.APP_LINKFLAGS
        LIBS = self.AWTK_LIBS + self.PLATFORM_LIBS
        LIBPATH = self.APP_LIBPATH + awtk.LIBPATH
        CFLAGS = self.APP_CFLAGS + self.AWTK_CFLAGS
        CCFLAGS = self.APP_CCFLAGS + self.AWTK_CCFLAGS
        TARGET_ARCH = awtk.TARGET_ARCH
        APP_TOOLS = self.APP_TOOLS
        CXXFLAGS = self.APP_CXXFLAGS
        DEPENDS_LIBS = []
        if hasattr(awtk, 'OS_DEBUG') :
            OS_DEBUG = awtk.OS_DEBUG
        else :
            OS_DEBUG = self.DEBUG
        if hasattr(awtk, 'BUILD_DEBUG_FLAG') :
            BUILD_DEBUG_FLAG = awtk.BUILD_DEBUG_FLAG
        else :
            if hasattr(awtk, 'CC') :
                BUILD_DEBUG_FLAG = ' '
            else :
                BUILD_DEBUG_FLAG = '  /MDd -D_DEBUG -DDEBUG /DEBUG /Od '
        DEBUG = self.complie_helper.get_value('DEBUG', OS_DEBUG)
        self.APP_BIN_DIR = self.complie_helper.get_value('APP_BIN_DIR', os.path.join(self.APP_ROOT, self.BIN_DIR))
        self.APP_LIB_DIR = self.complie_helper.get_value('APP_LIB_DIR', os.path.join(self.APP_ROOT, self.LIB_DIR))

        if self.MVVM_ROOT:
            MVVM_3RD_ROOT = join_path(self.MVVM_ROOT, '3rd')
            if self.WITH_IOTJS:
                IOTJS_ROOT = os.path.join(MVVM_3RD_ROOT, 'iotjs')
                DEPENDS_LIBS += [{
                    'cxxflags': '-DWITH_MVVM -DWITH_JERRYSCRIPT -DWITH_IOTJS',
                    'cflags': '-DWITH_MVVM -DWITH_JERRYSCRIPT -DWITH_IOTJS',
                    'ccflags': '-DWITH_MVVM -DWITH_JERRYSCRIPT -DWITH_IOTJS',
                    'root' : self.MVVM_ROOT,
                    'shared_libs': ['mvvm'],
                    'static_libs': ['']
                }]
                CPPPATH += [
                    join_path(IOTJS_ROOT, 'deps/jerry/jerry-ext/include'),
                    join_path(IOTJS_ROOT, 'deps/jerry/jerry-core/include')
                ]
            elif self.WITH_JERRYSCRIPT:
                DEPENDS_LIBS += [{
                    'cxxflags': '-DWITH_MVVM -DWITH_JERRYSCRIPT',
                    'cflags': '-DWITH_MVVM -DWITH_JERRYSCRIPT',
                    'ccflags': '-DWITH_MVVM -DWITH_JERRYSCRIPT',
                    'root' : self.MVVM_ROOT,
                    'shared_libs': ['mvvm'],
                    'static_libs': ['']
                }]
                CPPPATH += [
                    join_path(MVVM_3RD_ROOT, 'jerryscript/jerryscript/jerry-ext/include'),
                    join_path(MVVM_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/include')
                ]
            else:
                DEPENDS_LIBS += [{
                    'cxxflags': '-DWITH_MVVM',
                    'cflags': '-DWITH_MVVM',
                    'ccflags': '-DWITH_MVVM',
                    'root' : self.MVVM_ROOT,
                    'shared_libs': ['mvvm'],
                    'static_libs': []
                }]

        if self.AWFLOW_ROOT:
            DEPENDS_LIBS += [{
                'cxxflags': '-DWITH_AWFLOW',
                'cflags': '-DWITH_AWFLOW',
                'ccflags': '-DWITH_AWFLOW',
                'root' : self.AWFLOW_ROOT,
                'shared_libs': ['awflow'],
                'static_libs': []
            }]

            if self.isBuildShared():
                src = join_path(self.AWFLOW_ROOT, 'bin/nodes')
                dst = join_path(self.APP_ROOT, 'bin/nodes')
                if os.path.exists(dst):
                    shutil.rmtree(dst)
                print(src + '==>' + dst)
                shutil.copytree(src, dst)

        self.DEPENDS_LIBS = DEPENDS_LIBS + self.DEPENDS_LIBS
        for depend in self.DEPENDS_LIBS:
            self.check_and_remove_scons_db(depend['root'])
        self.check_and_remove_scons_db(self.APP_ROOT)

        if self.TKC_ONLY:
            CCFLAGS += ' -DTKC_ONLY=1 '
        else:
            CCFLAGS += ' -DWITH_AWTK=1 '

        for iter in self.DEPENDS_LIBS:
            if 'shared_libs' in iter:
                LIBS = iter['shared_libs'] + LIBS
            if 'static_libs' in iter:
                LIBS = iter['static_libs'] + LIBS
            if 'cxxflags' in iter:
                CXXFLAGS += " " + iter['cxxflags'] + " "
            if 'cflags' in iter:
                CFLAGS += " " + iter['cflags'] + " "
            if 'ccflags' in iter:
                CCFLAGS += " " + iter['ccflags'] + " "

            if 'cpppath' in iter:
                for f in iter['cpppath']:
                    CPPPATH = CPPPATH + [join_path(iter['root'], f)]
            else:
                CPPPATH += [join_path(iter['root'], 'src')]

            if 'libpath' in iter:
                for f in iter['libpath']:
                    LIBPATH = LIBPATH + [join_path(iter['root'], f)]
            else:
                if self.isBuildShared():
                    LIBPATH += [join_path(iter['root'], self.BIN_DIR)]
                LIBPATH += [join_path(iter['root'], self.LIB_DIR)]
        LIBS = self.APP_LIBS + LIBS

        if hasattr(awtk, 'CC'):
            if DEBUG != OS_DEBUG:
                if DEBUG :
                    CCFLAGS += ' -g -O0 '
                else :
                    CCFLAGS += ' -Os '
            else :
                CCFLAGS += BUILD_DEBUG_FLAG

            env = DefaultEnvironment(
                ENV = os.environ,
                CC=awtk.CC,
                CXX=awtk.CXX,
                LD=awtk.LD,
                AR=awtk.AR,
                STRIP=awtk.STRIP,
                RANLIB=awtk.RANLIB,
                TOOLS=APP_TOOLS,
                CPPPATH=CPPPATH,
                LINKFLAGS=LINKFLAGS,
                LIBS=LIBS,
                LIBPATH=LIBPATH,
                CCFLAGS=CCFLAGS,
                CFLAGS=CFLAGS,
                CXXFLAGS=CXXFLAGS,
                TARGET_ARCH=TARGET_ARCH,
                OS_SUBSYSTEM_CONSOLE=awtk.OS_SUBSYSTEM_CONSOLE,
                OS_SUBSYSTEM_WINDOWS=awtk.OS_SUBSYSTEM_WINDOWS)
        else:
            if DEBUG != OS_DEBUG:
                if DEBUG :
                    CCFLAGS += ' -D_DEBUG -DDEBUG /DEBUG /MDd /Od '
                else :
                    CCFLAGS += ' -DNDEBUG /MD /O2 /Oi '

                if self.complie_helper.get_value('PDB', True) :
                    LINKFLAGS += ' /DEBUG '
            else :
                CCFLAGS += BUILD_DEBUG_FLAG
                LINKFLAGS += awtk.BUILD_DEBUG_LINKFLAGS

            env = DefaultEnvironment(
                TOOLS=APP_TOOLS,
                CPPPATH=CPPPATH,
                LINKFLAGS=LINKFLAGS,
                LIBS=LIBS,
                LIBPATH=LIBPATH,
                CFLAGS=CFLAGS,
                CCFLAGS=CCFLAGS,
                CXXFLAGS=CXXFLAGS,
                TARGET_ARCH=TARGET_ARCH,
                OS_SUBSYSTEM_CONSOLE=awtk.OS_SUBSYSTEM_CONSOLE,
                OS_SUBSYSTEM_WINDOWS=awtk.OS_SUBSYSTEM_WINDOWS)
        
        def variant_SConscript(env, SConscriptFiles):
            self.SConscript(SConscriptFiles)
        env.AddMethod(variant_SConscript, "SConscript")
    
        if not Script.GetOption('clean'):
            self.prepare()
        else:
            self.cleanAwtkSharedLib()

        return env
