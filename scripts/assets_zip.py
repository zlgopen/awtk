# -*- coding: utf-8 -*-
import os
import shutil
import sys
import zipfile

def main():
    # 获取源目录和目标 ZIP 文件路径
    src = sys.argv[1] if len(sys.argv) > 1 else 'res/assets'
    dst = sys.argv[2] if len(sys.argv) > 2 else 'assets.zip'
    create(src, dst)

def create(src, dst):
    print(f'SRC={src}')
    print(f'DST={dst}')

    # 检查源目录是否存在
    if not os.path.exists(src):
        print(f"{src} not exists")
        return

    print("preparing...")

    # 清理临时目录和目标文件
    temp_dir = 'temp'
    if os.path.exists(temp_dir):
        shutil.rmtree(temp_dir)
    if os.path.exists(dst):
        os.remove(dst)

    # 创建临时目录并复制源目录内容
    os.makedirs(temp_dir)
    shutil.copytree(src, os.path.join(temp_dir, 'assets'))

    # 清理 .inc 文件
    assets_dir = os.path.join(temp_dir, 'assets')
    inc_files_removed = []

    for root, dirs, files in os.walk(assets_dir):
        # 删除所有 .inc 文件
        for file in files:
            if file.endswith('.inc'):
                os.remove(os.path.join(root, file))
        
        # 删除特定目录和文件
        if 'inc' in dirs:
            print(f'clean {os.path.basename(root)}')
            shutil.rmtree(os.path.join(root, 'inc'))
        
        # 删除特定路径下的 XML 文件
        raw_ui_dir = os.path.join(root, 'raw', 'ui')
        raw_styles_dir = os.path.join(root, 'raw', 'styles')
        raw_strings_dir = os.path.join(root, 'raw', 'strings')
        
        if os.path.exists(raw_ui_dir):
            for xml_file in os.listdir(raw_ui_dir):
                if xml_file.endswith('.xml'):
                    os.remove(os.path.join(raw_ui_dir, xml_file))
        
        if os.path.exists(raw_styles_dir):
            for xml_file in os.listdir(raw_styles_dir):
                if xml_file.endswith('.xml'):
                    os.remove(os.path.join(raw_styles_dir, xml_file))
        
        if os.path.exists(raw_strings_dir):
            for xml_file in os.listdir(raw_strings_dir):
                if xml_file.endswith('.xml'):
                    os.remove(os.path.join(raw_strings_dir, xml_file))

    # 压缩 assets 目录
    with zipfile.ZipFile(dst, 'w', zipfile.ZIP_DEFLATED) as zipf:
        for root, dirs, files in os.walk(os.path.join(temp_dir, 'assets')):
            for file in files:
                filenameOutZip = os.path.join(root, file)
                filenameInZip = os.path.relpath(os.path.join(root, file), temp_dir);
                zipf.write(filenameOutZip, filenameInZip)

    # 清理临时目录
    shutil.rmtree(temp_dir)

    print(f"assets saved to {dst}:")
    print(os.path.getsize(dst), "bytes")  # 显示文件大小

if __name__ == "__main__":
    main()

