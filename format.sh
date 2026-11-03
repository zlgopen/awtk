#!/bin/bash

#查找clang-format：环境变量CLANG_FORMAT -> PATH -> VS Code的C/C++插件等常见安装位置
. "`dirname "$0"`/find_clang_format.sh"

CLANG_FORMAT=`find_clang_format`
if [ -z "$CLANG_FORMAT" ]; then
  echo "clang-format not found, please install clang-format or set CLANG_FORMAT"
  exit 1
fi

echo "using $CLANG_FORMAT"

find src tools demos -name \*.c -exec "$CLANG_FORMAT" -i {} \;
find src tools demos -name \*.h -exec "$CLANG_FORMAT" -i {} \;
find src tools demos -name \*.cc -exec "$CLANG_FORMAT" -i {} \;
find src tools demos -name \*.cpp -exec "$CLANG_FORMAT" -i {} \;

for f in tests/*.cpp tests/*.c tests/*.cc
do
  echo "clang-format -i $f"
  "$CLANG_FORMAT" -i $f
done
