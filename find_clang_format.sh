#!/bin/bash

#查找clang-format的路径
#查找顺序：环境变量CLANG_FORMAT -> PATH -> VS Code的C/C++插件等常见安装位置
#用法：
# 1. source本文件后调用find_clang_format(结果输出到标准输出)
# 2. 直接执行本文件(输出找到的路径，找不到时没有输出)

#在指定的目录中查找clang-format(Linux/macOS下的名字是clang-format，Windows下是clang-format.exe)
function find_clang_format_in() {
  for dir in "$@"
  do
    if [ -x "$dir/clang-format" ]; then
      echo "$dir/clang-format"
      return
    fi

    if [ -x "$dir/clang-format.exe" ]; then
      echo "$dir/clang-format.exe"
      return
    fi
  done
}

function find_clang_format() {
  if [ -n "$CLANG_FORMAT" ]; then
    echo "$CLANG_FORMAT"
    return
  fi

  if [ -n "`command -v clang-format`" ]; then
    command -v clang-format
    return
  fi

  #没有放到PATH中时，按平台到几个常见的安装位置找一找
  case "`uname -s`" in
    MINGW*|MSYS*|CYGWIN*)
      find_clang_format_in "$HOME/.vscode/extensions"/*/LLVM/bin \
        "/c/Program Files/LLVM/bin" \
        "/c/Program Files (x86)/LLVM/bin" \
        "/c/Program Files/Microsoft Visual Studio"/*/*/VC/Tools/Llvm/x64/bin
      ;;
    Darwin)
      find_clang_format_in "$HOME/.vscode/extensions"/*/LLVM/bin \
        /opt/homebrew/opt/llvm/bin \
        /usr/local/opt/llvm/bin
      ;;
    *)
      find_clang_format_in "$HOME/.vscode/extensions"/*/LLVM/bin \
        /usr/lib/llvm-*/bin
      ;;
  esac
}

#直接执行本文件时，输出找到的clang-format路径
if [ "$0" = "$BASH_SOURCE" ]; then
  find_clang_format
fi
