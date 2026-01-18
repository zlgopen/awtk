#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import re
import sys
import os

def update_copyright_date(file_path, new_end_year):
    """
    更新文件中的版权日期
    
    Args:
        file_path: 要修改的文件路径
        new_end_year: 新的结束年份（字符串或整数）
    """
    if not os.path.exists(file_path):
        print(f"错误: 文件 '{file_path}' 不存在", file=sys.stderr)
        return False
    
    # 读取文件内容
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
    except UnicodeDecodeError:
        # 如果 UTF-8 解码失败，尝试其他编码
        try:
            with open(file_path, 'r', encoding='latin-1') as f:
                content = f.read()
        except Exception as e:
            print(f"错误: 无法读取文件 '{file_path}': {e}", file=sys.stderr)
            return False
    except Exception as e:
        print(f"错误: 无法读取文件 '{file_path}': {e}", file=sys.stderr)
        return False
    
    # 匹配 Copyright (c) 后面的日期
    # 支持以下格式：
    # - Copyright (c) 2022
    # - Copyright (c) 2018 - 2025
    # - Copyright (c) 2018-2025 (无空格)
    pattern = r'(Copyright\s+\(c\)\s+)(\d{4})(\s*-\s*)?(\d{4})?'
    
    def replace_copyright(match):
        prefix = match.group(1)  # "Copyright (c) "
        start_year = match.group(2)  # 起始年份
        dash = match.group(3)  # 可能存在的 "-" 和空格
        end_year = match.group(4)  # 可能存在的结束年份
        
        # 如果已经有结束年份，更新它
        if end_year:
            return f"{prefix}{start_year} - {new_end_year}"
        # 如果只有起始年份，添加结束年份
        else:
            return f"{prefix}{start_year} - {new_end_year}"
    
    # 替换所有匹配的版权信息
    new_content = re.sub(pattern, replace_copyright, content)
    
    # 如果内容有变化，写回文件
    if new_content != content:
        try:
            with open(file_path, 'w', encoding='utf-8') as f:
                f.write(new_content)
            print(f"成功更新文件: {file_path}")
            return True
        except Exception as e:
            print(f"错误: 无法写入文件 '{file_path}': {e}", file=sys.stderr)
            return False
    else:
        # 未找到版权信息不算错误，只是跳过
        return True

def main():
    if len(sys.argv) != 3:
        print("用法: python update_copyright_date.py <文件路径> <新的结束年份>", file=sys.stderr)
        print("示例: python update_copyright_date.py test.cpp 2026", file=sys.stderr)
        sys.exit(1)
    
    file_path = sys.argv[1]
    new_end_year = sys.argv[2]
    
    # 验证年份格式
    if not new_end_year.isdigit() or len(new_end_year) != 4:
        print(f"错误: 年份格式不正确，应为4位数字: {new_end_year}", file=sys.stderr)
        sys.exit(1)
    
    success = update_copyright_date(file_path, new_end_year)
    sys.exit(0 if success else 1)

if __name__ == '__main__':
    main()
