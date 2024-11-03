import os
import sys
import glob
import json
import shutil

EXPORT_JSON = "export.json"


def join_path(root, subdir):
    return os.path.abspath(os.path.normpath(os.path.join(root, subdir)))


def abs_conf_files(group, app_root):
    for i in range(len(group)):
        group[i] = join_path(app_root, group[i])


def merge_config_files(group1, group2):
    for key in group2:
        group1.append(key)


def merge_config_flags(group1, group2):
    for key in group2:
        group1.append(key)


def merge_depend(config, root, depend, target_platform):
    filename = os.path.abspath(
        os.path.normpath(os.path.join(root, depend, EXPORT_JSON))
    )

    if os.path.exists(filename):
        depend_config = load_config_impl(filename, target_platform)
        if "sources" in depend_config:
            if "sources" not in config:
                config["sources"] = []
            merge_config_files(config["sources"], depend_config["sources"])
        if "includes" in depend_config:
            if "includes" not in config:
                config["includes"] = []
            merge_config_files(config["includes"], depend_config["includes"])
        if "cflags" in depend_config:
            if "cflags" not in config:
                config["cflags"] = []
            merge_config_flags(config["cflags"], depend_config["cflags"])
        if "cxxflags" in depend_config:
            if "cxxflags" not in config:
                config["cxxflags"] = []
            merge_config_flags(config["cxxflags"], depend_config["cxxflags"])


def merge_platform_config(config, root_dir, platform):
    if platform in config:
        for key in config[platform]:
            if key not in config:
                config[key] = config[platform][key]
            else:
                if isinstance(config[key], list):
                    config[key] = config[key] + config[platform][key]
                elif isinstance(config[key], dict):
                    config[key].update(config[platform][key])
                else:
                    config[key] = config[platform][key]
    if platform == "web":
        if "sources" in config:
            abs_conf_files(config["sources"], root_dir)

        if "includes" in config:
            abs_conf_files(config["includes"], root_dir)


def load_config_impl(filename, target_platform):
    config_root = os.path.dirname(filename)
    component_name = os.path.basename(config_root)
    config_name = os.path.basename(filename)

    with open(filename, "r") as f:
        config = json.load(f)
        merge_platform_config(config, config_root, target_platform)

        # depend itself
        if not config_name == EXPORT_JSON:
            merge_depend(
                config, os.path.dirname(config_root), component_name, target_platform
            )

        if "depends" in config:
            for key in config["depends"]:
                merge_depend(config, config_root, key, target_platform)

        return config


def load_app_config(filename, target_platform):
    config = load_config_impl(filename, target_platform)

    if "sources" in config:
        config["sources"] = list(set(config["sources"]))
    if "includes" in config:
        config["includes"] = list(set(config["includes"]))
    if "cflags" in config:
        config["cflags"] = list(set(config["cflags"]))
    if "cxxflags" in config:
        config["cxxflags"] = list(set(config["cxxflags"]))

    return config


def join_path(root, subdir):
    return os.path.abspath(os.path.normpath(os.path.join(root, subdir)))


AWTK_DIR = join_path(os.getcwd(), "../awtk")

print("AWTK_DIR:" + AWTK_DIR)


def show_usage():
    print("Usage: python create_proj.py app.json")
    sys.exit(0)


def mkdir_if_not_exist(fullpath):
    if os.path.exists(fullpath):
        print(fullpath + " exist.")
    else:
        os.makedirs(fullpath)


def file_read(name):
    with open(name) as infile:
        return infile.read()


def file_write(name, content):
    with open(name, "w") as outfile:
        outfile.write(content)


def file_replace(name, sfrom, sto):
    print("process:" + name)
    content = file_read(name)
    new_content = content.replace(sfrom, sto)
    file_write(name, new_content)


def copy_folder(src, dst, custom_ignore_function=None):
    print(src + "=>" + dst)
    if os.path.exists(dst):
        shutil.rmtree(dst)
    shutil.copytree(src, dst, ignore=custom_ignore_function)


def copy_folder_overwrite(src, dest, ignore=None):
    print(src + " => " + dest)
    if os.path.isdir(src):
        if not os.path.isdir(dest):
            os.makedirs(dest)
        files = os.listdir(src)
        if ignore is not None:
            ignored = ignore(src, files)
        else:
            ignored = set()
        for f in files:
            if f not in ignored:
                copy_folder_overwrite(
                    os.path.join(src, f), os.path.join(dest, f), ignore
                )
    else:
        shutil.copyfile(src, dest)


def copy_file(src, dst):
    print(src + "=>" + dst)
    mkdir_if_not_exist(os.path.dirname(dst))
    shutil.copy(src, dst)


def get_prefix_of_2path(s1, s2):
    i = 0
    l1 = len(s1)
    l2 = len(s2)
    n = min(l1, l2)
    for i in range(n):
        if s1[i] != s2[i]:
            s = s1[0:i]
            return len(os.path.dirname(s))

    return n


def copy_glob_files(src, srcdir, dstdir):
    print(src + " =>" + dstdir)
    files = glob.glob(src)
    for f in files:
        prefix = get_prefix_of_2path(f, srcdir)
        dst = join_path(dstdir, f[prefix + 1 :])
        print(f + " =>" + dst)
        copy_file(f, dst)


def file_rename(src, dst):
    if src != dst:
        os.rename(src, dst)
        print(src + " => " + dst)


def files_replace(files, app_root_dst, app_full_name, app_name, vendor):
    for f in files:
        filename = join_path(app_root_dst, f)
        print("process " + filename)
        file_replace(filename, "org.zlgopen.awtktemplate", app_full_name)
        file_replace(filename, "awtktemplate", app_name)
        file_replace(filename, "AwtkTemplate", app_name)
        file_replace(filename, "awtkvendor", vendor)


def files_replace_with_config(files, app_root_dst, config):
    app_full_name = config_get_app_full_name(config)
    app_name = config_get_app_name(config)
    vendor = config_get_app_vendor(config)

    files_replace(files, app_root_dst, app_full_name, app_name, vendor)


def custom_ignore_objs(directory, files):
    # 返回一个包含要忽略的文件名的集合
    return {file for file in files if file.endswith((".o", ".obj"))}


def copy_awtk_files(app_root_dst):
    awtk_src = join_path(AWTK_DIR, "src")
    awtk_src_to = join_path(app_root_dst, "src")
    copy_folder(awtk_src, awtk_src_to, custom_ignore_objs)

    awtk_3rd = join_path(AWTK_DIR, "3rd")
    awtk_3rd_to = join_path(app_root_dst, "3rd")
    copy_folder(awtk_3rd, awtk_3rd_to, custom_ignore_objs)


def copy_app_sources(config, app_sources_dst, app_root_src):
    sources = config_get_sources(config)

    for f in sources:
        sfrom = join_path(app_root_src, f)
        if sfrom.find("*") >= 0:
            sto = app_sources_dst
            copy_glob_files(sfrom, app_root_src, sto)
        else:
            sto = join_path(app_sources_dst, f)
            copy_file(sfrom, sto)


def copy_app_assets(config, app_assets_dst, app_root_src):
    assets_dir = config["assets"]
    themes = config_get_themes(config)
    for t in themes:
        print("copy theme:")
        src = join_path(app_root_src, assets_dir + "/" + t + "/raw")
        dst = join_path(app_assets_dst, t + "/raw")
        copy_folder(src, dst)


def to_string(input):
    if isinstance(input, list):
        return " ".join(input)
    return str(input)


def update_cmake_file(config, filename):
    includes = config_get_includes(config)
    sincludes = (
        "${APP_SOURCE_DIR}/res\n ${APP_SOURCE_DIR}/src\n  ${APP_SOURCE_DIR}/3rd\n"
    )
    for f in includes:
        sincludes += "  ${APP_SOURCE_DIR}/" + f + "\n"
    print("process " + filename)

    cflags = to_string(config_get_cflags(config))
    defines = to_string(config_get_defines(config))
    cppflags = to_string(config_get_cppflags(config))
    file_replace(filename, "EXTRA_CFLAGS", cflags)
    file_replace(filename, "EXTRA_DEFINES", defines)
    file_replace(filename, "EXTRA_CPPFLAGS", cppflags)
    file_replace(filename, "EXTRA_INCLUDES", sincludes)


def config_get_app_icon(config):
    if "icon" in config:
        return config["icon"]
    else:
        return "design/default/images/xx/icon.png"


def config_get_app_vendor(config):
    return config["vendor"]


def config_get_app_full_name(config):
    return config["app_name"]


def config_get_app_name(config):
    app_full_name = config_get_app_full_name(config)
    items = app_full_name.split(".")
    return items[2]


def config_get_sources(config):
    return config["sources"]


def config_get_includes(config):
    if "includes" in config:
        return config["includes"]
    else:
        return []


def config_get_themes(config):
    if "themes" in config:
        return config["themes"]
    else:
        return ["default"]


def config_get_cflags(config):
    if "cflags" in config:
        return config["cflags"]
    else:
        return ""


def config_get_defines(config):
    if "defines" in config:
        return config["defines"]
    else:
        return ""


def config_get_cppflags(config):
    if "cppflags" in config:
        return config["cppflags"]
    else:
        return ""


def config_get_plugins(config):
    if "plugins" in config:
        return config["plugins"]
    else:
        return []


def load_config(filename, platform):
    config = load_app_config(filename, platform)
    app_full_name = config_get_app_full_name(config)
    items = app_full_name.split(".")

    if len(items) != 3:
        print("invalid app name")
        sys.exit(0)

    if len(config_get_sources(config)) < 1:
        print("no sources")
        sys.exit(0)

    return config


def show_usage():
    print("Usage: python create_proj.py app.json")
    sys.exit(0)


def create_assets_zip(app_root_src, app_root_dst, zip_file):
    CREATE_ASSETS_ZIP = os.path.abspath(
        os.path.normpath(AWTK_DIR + "/scripts/create_assets_zip.sh")
    )
    if not os.path.exists(CREATE_ASSETS_ZIP):
        print("ERROR: " + CREATE_ASSETS_ZIP + " not found!")
        sys.exit(0)

    ASSETS_DIR = join_path(app_root_src, "res/assets")
    ASSETS_ZIP = join_path(app_root_dst, zip_file)

    print("ASSETS_DIR:" + ASSETS_DIR)
    print("ASSETS_ZIP:" + ASSETS_ZIP)

    if os.path.exists(ASSETS_ZIP):
        os.remove(ASSETS_ZIP)
    CMD_CREATE_ASSETS_ZIP = (
        CREATE_ASSETS_ZIP + ' "' + ASSETS_DIR + '" "' + ASSETS_ZIP + '"'
    )
    print("CMD_CREATE_ASSETS_ZIP:" + CMD_CREATE_ASSETS_ZIP)

    os.system(CMD_CREATE_ASSETS_ZIP)
