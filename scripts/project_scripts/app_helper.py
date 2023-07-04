import os
import sys
import shutil
import awtk_locator as locator
from SCons import Script

def Helper(ARGUMENTS):
    locator.init(ARGUMENTS)

    from app_helper_base import AppHelperBase
    return AppHelperBase(ARGUMENTS)


def parse_needed_file(helper, root, file):
    dst = helper.APP_BIN_DIR
    if isinstance(file, list):
        src = os.path.abspath(os.path.join(root, file[0]))
        if len(file) > 1:
            dst = os.path.abspath(os.path.join(helper.APP_BIN_DIR, file[1]))
    else:
        src = os.path.abspath(os.path.join(root, file))
    return src, dst


def clear_needed_files(helper, root, needed_files):
    for file in needed_files:
        src, dst = parse_needed_file(helper, root, file)

        if os.path.isfile(src):
            if os.path.isdir(dst):
                dst = os.path.join(dst, os.path.basename(src))
            if os.path.exists(dst):
                os.remove(dst)
                print('Removed {}'.format(os.path.relpath(dst, helper.APP_ROOT)))
        elif os.path.isdir(src) and os.path.isdir(dst):
            dirs = []
            for _root, _dirs, _files in os.walk(src):
                for _file in _files:
                    _dst = os.path.join(dst, os.path.relpath(os.path.join(_root, _file), src))
                    if os.path.exists(_dst):
                        os.remove(_dst)
                        print('Removed {}'.format(os.path.relpath(_dst, helper.APP_ROOT)))
                for _dir in _dirs:
                  dirs.append(os.path.join(dst, _dir))
            for _dir in dirs:
                try:
                    os.rmdir(_dir)
                    print('Removed {}'.format(os.path.relpath(_dir, helper.APP_ROOT)))
                except:
                    none
            if not os.path.relpath(helper.APP_BIN_DIR, dst) == '.':
                try:
                    os.rmdir(dst)
                    print('Removed {}'.format(os.path.relpath(dst, helper.APP_ROOT)))
                except:
                    none


def copy_needed_files(helper, root, needed_files):
    for file in needed_files:
        src, dst = parse_needed_file(helper, root, file)

        if os.path.isfile(src):
            if os.path.exists(src):
                if not os.path.exists(os.path.dirname(dst)):
                    os.makedirs(os.path.dirname(dst))
                shutil.copy(src, dst)
                print(src + '==>' + dst)
            else:
                print('[NeededFiles]: Not found {src}', src)
        elif os.path.isdir(src):
            if os.path.exists(src):
                for _root, _dirs, _files in os.walk(src):
                    for _file in _files:
                        _src = os.path.join(_root, _file)
                        _dst = os.path.join(dst, os.path.relpath(_src, src))
                        if not os.path.exists(os.path.dirname(_dst)):
                            os.makedirs(os.path.dirname(_dst))
                        shutil.copy(_src, _dst)
                print(src + '==>' + dst)
            else:
                print('[NeededFiles]: Not found {src}', src)


def prepare_depends_libs(ARGUMENTS, helper, libs):
    if ARGUMENTS.get('PREPARE_DEPENDS', '').lower().startswith('f'):
        return

    args = ' AWTK_ROOT=\"{}\"'.format(helper.AWTK_ROOT)
    if helper.MVVM_ROOT:
        args += ' MVVM_ROOT=\"{}\"'.format(helper.MVVM_ROOT)

    if 'APP_BIN_DIR' in ARGUMENTS:
        helper.APP_BIN_DIR = os.path.abspath(ARGUMENTS['APP_BIN_DIR'])
        args += ' APP_BIN_DIR=\"{}\"'.format(helper.APP_BIN_DIR.replace('\\', '/'))
        if not os.path.exists(helper.APP_BIN_DIR):
            os.makedirs(helper.APP_BIN_DIR)
    else:
        args += ' APP_BIN_DIR=\"{}\"'.format(os.path.abspath(helper.APP_BIN_DIR).replace('\\', '/'))

    for key in ARGUMENTS:
        if not key == 'AWTK_ROOT' and not key == 'MVVM_ROOT' and not key == 'APP_BIN_DIR' :
            if ' ' in ARGUMENTS[key]:
                args += ' {}=\"{}\"'.format(key, ARGUMENTS[key])
            else:
                args += ' {}={}'.format(key, ARGUMENTS[key])

    num_jobs_str = ''
    num_jobs = Script.GetOption('num_jobs')
    if num_jobs > 1:
        num_jobs_str = ' -j' + str(num_jobs)

    clean_str = ''
    if Script.GetOption('clean'):
        clean_str = ' -c '

    for lib in libs:
        if 'root' in lib and os.path.exists(lib['root'] + '/SConstruct'):
            cmd = 'cd ' + lib['root'] + ' && scons' + clean_str + num_jobs_str + args
            print('\n*******************************************************************************')
            print('[Dependencies]: {}'.format(lib['root']))
            print('*******************************************************************************\n')
            result = os.system(cmd)
            if not result == 0:
                sys.exit(result)

        if 'needed_files' in lib:
            if Script.GetOption('clean'):
                clear_needed_files(helper, lib['root'], lib['needed_files'])
            else:
                copy_needed_files(helper, lib['root'], lib['needed_files'])


def helper_has_func(helper, func_name):
    return False;


def helper_run_func(helper, func_name, argv):
    return None;

