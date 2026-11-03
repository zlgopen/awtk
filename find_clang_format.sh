#!/bin/bash

#source后调用find_clang_format取路径，也可以直接执行本文件输出路径
#查找顺序：环境变量CLANG_FORMAT -> PATH -> VS Code的C/C++插件等常见安装位置 -> PATH里带版本号的名字

#在指定的目录中查找clang-format(Linux/macOS不带后缀，Windows带.exe)
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

#从网络查询clang-format的最新主版本号(带超时)，查不到(超时/无网络/无curl和wget)时返回空
function latest_clang_format_version() {
  url=https://api.github.com/repos/llvm/llvm-project/releases/latest
  timeout=${CLANG_FORMAT_NET_TIMEOUT:-2}
  resp=""

  if [ -n "`command -v curl`" ]; then
    resp=`curl -fsSL --connect-timeout $timeout --max-time $timeout "$url" 2>/dev/null`
  elif [ -n "`command -v wget`" ]; then
    resp=`wget -q -T $timeout -t 1 -O - "$url" 2>/dev/null`
  fi

  #响应里的tag_name形如"llvmorg-21.1.4"，取主版本号21
  echo "$resp" | sed -n 's/.*"tag_name"[ ]*:[ ]*"llvmorg-\([0-9][0-9]*\)\..*/\1/p' | head -n 1
}

#1. 环境变量CLANG_FORMAT
function find_clang_format_from_env() {
  if [ -n "$CLANG_FORMAT" ]; then
    echo "$CLANG_FORMAT"
  fi
}

#2. PATH里的名字
function find_clang_format_in_path() {
  command -v clang-format 2>/dev/null
}

#3. 常见安装位置
function find_clang_format_in_common_dirs() {
  local f=""

  case "`uname -s`" in
    MINGW*|MSYS*|CYGWIN*)
      f=`find_clang_format_in "$HOME/.vscode/extensions"/*/LLVM/bin \
        "/c/Program Files/LLVM/bin" \
        "/c/Program Files (x86)/LLVM/bin" \
        "/c/Program Files/Microsoft Visual Studio"/*/*/VC/Tools/Llvm/x64/bin`
      ;;
    Darwin)
      f=`find_clang_format_in "$HOME/.vscode/extensions"/*/LLVM/bin \
        /opt/homebrew/opt/llvm/bin \
        /usr/local/opt/llvm/bin`
      #Xcode命令行工具里的clang-format只能通过xcrun访问
      if [ -z "$f" ] && command -v xcrun > /dev/null 2>&1; then
        f=`xcrun -f clang-format 2>/dev/null`
      fi
      ;;
    *)
      f=`find_clang_format_in "$HOME/.vscode/extensions"/*/LLVM/bin \
        /usr/lib/llvm-*/bin`
      ;;
  esac

  if [ -n "$f" ]; then
    echo "$f"
  fi
}

#4. PATH里带版本号的名字(如clang-format-18)，从新到旧找
#   上界优先用CLANG_FORMAT_MAX_VERSION，其次联网查最新版本，都拿不到就用50
function find_clang_format_with_version() {
  local v="${CLANG_FORMAT_MAX_VERSION}"

  if [ -z "$v" ]; then
    v=`latest_clang_format_version`
  fi

  if [ -z "$v" ]; then
    v=50
  fi

  while [ $v -ge 10 ]
  do
    if [ -n "`command -v clang-format-$v`" ]; then
      command -v clang-format-$v
      return
    fi
    v=$((v - 1))
  done
}

#依次尝试上面的步骤，找到即返回
function find_clang_format() {
  local f=""

  f=`find_clang_format_from_env`

  if [ -z "$f" ]; then
    f=`find_clang_format_in_path`
  fi

  if [ -z "$f" ]; then
    f=`find_clang_format_in_common_dirs`
  fi

  if [ -z "$f" ]; then
    f=`find_clang_format_with_version`
  fi

  if [ -n "$f" ]; then
    echo "$f"
  fi
}

#直接执行本文件时，输出找到的clang-format路径
if [ "$0" = "$BASH_SOURCE" ]; then
  find_clang_format
fi
