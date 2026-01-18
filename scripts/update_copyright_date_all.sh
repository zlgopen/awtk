#!/bin/bash

# 批量更新指定目录下所有 .c/.h/.cpp 文件的版权日期

# 获取脚本所在目录
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PYTHON_SCRIPT="${SCRIPT_DIR}/update_copyright_date.py"

# 检查参数
if [ $# -lt 1 ]; then
    echo "用法: $0 <目录> [年份]"
    echo "示例: $0 src 2026"
    echo "示例: $0 src  # 使用当前年份"
    exit 1
fi

TARGET_DIR="$1"
YEAR="${2:-$(date +%Y)}"

# 验证目录是否存在
if [ ! -d "$TARGET_DIR" ]; then
    echo "错误: 目录 '$TARGET_DIR' 不存在" >&2
    exit 1
fi

# 验证 Python 脚本是否存在
if [ ! -f "$PYTHON_SCRIPT" ]; then
    echo "错误: Python 脚本 '$PYTHON_SCRIPT' 不存在" >&2
    exit 1
fi

# 验证年份格式
if ! [[ "$YEAR" =~ ^[0-9]{4}$ ]]; then
    echo "错误: 年份格式不正确，应为4位数字: $YEAR" >&2
    exit 1
fi

echo "开始更新目录 '$TARGET_DIR' 下的版权日期为 $YEAR"
echo ""

# 创建临时文件存储文件列表
TEMP_FILE=$(mktemp)
trap "rm -f '$TEMP_FILE'" EXIT

# 查找所有 .c、.h、.cpp 文件并保存到临时文件
find "$TARGET_DIR" -type f \( -name "*.c" -o -name "*.h" -o -name "*.cpp" \) > "$TEMP_FILE"

# 统计变量
TOTAL=0
UPDATED=0
SKIPPED=0
FAILED=0

# 处理每个文件
while IFS= read -r file; do
    TOTAL=$((TOTAL + 1))
    
    # 捕获 Python 脚本的输出
    OUTPUT=$(python3 "$PYTHON_SCRIPT" "$file" "$YEAR" 2>&1)
    EXIT_CODE=$?
    
    if [ $EXIT_CODE -eq 0 ]; then
        # 检查输出中是否包含"成功更新"
        if echo "$OUTPUT" | grep -q "成功更新"; then
            UPDATED=$((UPDATED + 1))
            echo "✓ $file"
        else
            SKIPPED=$((SKIPPED + 1))
            # 不显示跳过的文件，减少输出噪音
        fi
    else
        FAILED=$((FAILED + 1))
        echo "✗ $file"
        echo "  $OUTPUT" >&2
    fi
done < "$TEMP_FILE"

echo ""
echo "完成！"
echo "总计文件: $TOTAL"
echo "成功更新: $UPDATED"
if [ $SKIPPED -gt 0 ]; then
    echo "跳过(无版权信息): $SKIPPED"
fi
if [ $FAILED -gt 0 ]; then
    echo "更新失败: $FAILED"
fi
