#!/bin/bash

. "`dirname "$0"`/find_clang_format.sh"

CLANG_FORMAT=`find_clang_format`
if [ -z "$CLANG_FORMAT" ]; then
  echo "clang-format not found, please install clang-format or set CLANG_FORMAT"
  exit 1
fi

#不带参数时格式化整个工程，带参数时只格式化指定的文件或目录(如: ./format.sh src/base)
if [ $# -eq 0 ]; then
  TARGETS="src tools demos tests"
else
  TARGETS="$@"
fi

FILES=()
while IFS= read -r f
do
  FILES+=("$f")
done < <(find $TARGETS \( -name '*.c' -o -name '*.h' -o -name '*.cc' -o -name '*.cpp' -o -name '*.inc' \) 2>/dev/null)

TOTAL=${#FILES[@]}
echo "using $CLANG_FORMAT"
echo "formatting $TOTAL files ..."

#一次传多个文件，避免每个文件都启动一次clang-format(Windows上启动一次要0.6~0.9秒)
BATCH=50
start=0
while [ $start -lt $TOTAL ]
do
  batch=("${FILES[@]:$start:$BATCH}")
  end=$((start + ${#batch[@]}))

  #先列出本批要格式化的文件，再一次性格式化
  k=$start
  while [ $k -lt $end ]
  do
    printf "[%d/%d] %s\n" "$((k + 1))" "$TOTAL" "${FILES[$k]}"
    k=$((k + 1))
  done

  "$CLANG_FORMAT" -i "${batch[@]}"
  start=$end
done

echo "done. $TOTAL files formatted"
