# 根据Json表生成harfbuzz信息表二进制bin文件
import os
import sys
import json
import struct
import uharfbuzz as hb
from collections import deque
from bidi.algorithm import get_display, get_base_level, get_empty_storage, get_embedding_levels, explicit_embed_and_overrides, resolve_weak_types, resolve_neutral_types, resolve_implicit_levels
import update_res_common as common

sys.stdout.reconfigure(encoding='utf-8')
user_json_infos = dict()

# ------------------------- 版本号 --------------------------
VERSION_MAGIC = 0x1

# ------------------------- 二进制相关宏定义 -----------------------
BIN_SEPARATOR = b"\0" # 分隔符
BIN_LINEBREAK = b"\n" # 换行符
BIN_EOF_CHECK = b"\xFE\xFE" # 文件结尾验证信息
BIN_DOUBLE_SEPARATOR = b"\xFC\xFC" # 双重分隔符
BIN_BEGIN_MAGIC = b"HARFBUZZ_INFO" # 文件验证信息
BIN_TABLE_INFO_MAGIC = b"TABLE_INFO" # 整形信息数据验证字符串
BIN_TABLE_INFO_SIZE_MAGIC = b"TABLE_INFO_SIZE" # 信息表总的信息数量验证字符串
# -----------------------------------------------------------------

# -------------------- 生成的整形数据二进制文件格式说明 ---------------------
"""
"HARFBUZZ_INFO"(文件头验证字符串)+'\0'+ 版本验证
"TABLE_INFO_SIZE" (获取数量验证字符串)+'\0'+ 字库数量 +'\n'
"TABLE_INFO"(获取字符位置信息 验证字符串)+ '\0'+'\n'
字库名称 + '\0' + 字库字符串数量 + '\n'
当前字符串 + '\0' + 0xFCFC(校验码)+ 当前字符串字模数量 + '\0' + 字符串字符数量 + '\0'
 遍历每个字符数据(gid cluster xoffset yoffset xadvance yadvance bidi_type)+ '\0'
 + '\n'
 + '\n'
0xFEFE(文件结尾校验码)

注意：bidi_type 为每个字模的 bidi 方向(1=LTR, 2=RTL)。
对于 RTL/LTR 混搭字符串，使用 python-bidi 拆分 bidi run，
对每个 run 按 harfbuzz shaping，最后按逻辑顺序合并结果。
"""
# -------------------------------------------------------------------------


# -------------------------- Bidi Run拆分(python-bidi) --------------------------
def bidi_get_runs(text):
    if len(text) == 0:
        return []

    storage = get_empty_storage()
    base_level = 0  # force LTR base direction to match VSCode/Notepad
    storage["base_level"] = base_level
    storage["base_dir"] = ("L", "R")[base_level]

    get_embedding_levels(text, storage)
    explicit_embed_and_overrides(storage)
    resolve_weak_types(storage)
    resolve_neutral_types(storage, False)
    resolve_implicit_levels(storage, False)

    storage_chars = storage["chars"]
    text_levels = [0] * len(text)
    si = 0
    ti = 0
    while ti < len(text):
        if si < len(storage_chars) and storage_chars[si]["ch"] == text[ti]:
            text_levels[ti] = storage_chars[si]["level"]
            si += 1
            ti += 1
        else:
            text_levels[ti] = text_levels[ti - 1] if ti > 0 else 0
            ti += 1

    runs = []
    start = 0
    cur_level = text_levels[0]
    for i in range(1, len(text_levels)):
        if text_levels[i] != cur_level:
            runs.append((start, i, text[start:i], "rtl" if (cur_level % 2 == 1) else "ltr"))
            start = i
            cur_level = text_levels[i]
    runs.append((start, len(text), text[start:], "rtl" if (cur_level % 2 == 1) else "ltr"))

    if len(runs) > 1:
        run_levels = [text_levels[r[0]] for r in runs]
        max_level = max(run_levels)
        for lev in range(max_level, 0, -1):
            i = 0
            while i < len(runs):
                if run_levels[i] >= lev:
                    j = i
                    while j < len(runs) and run_levels[j] >= lev:
                        j += 1
                    runs[i:j] = runs[i:j][::-1]
                    run_levels[i:j] = run_levels[i:j][::-1]
                    i = j
                else:
                    i += 1

    return runs


# -------------------------- harfbuzz整形函数 --------------------------
class _ShapedGlyph:
    __slots__ = ('codepoint', 'cluster', 'x_offset', 'y_offset', 'x_advance', 'y_advance', 'bidi_type')
    def __init__(self, info, pos, cluster_offset, bidi_type):
        self.codepoint = info.codepoint
        self.cluster = info.cluster + cluster_offset
        self.x_offset = pos.x_offset
        self.y_offset = pos.y_offset
        self.x_advance = pos.x_advance
        self.y_advance = pos.y_advance
        self.bidi_type = bidi_type


def _shape_single_run(ttf_path, text, direction):
    try:
        blob = hb.Blob().from_file_path(ttf_path)
        buf = hb.Buffer()
        buf.add_str(text)
        face = hb.Face(blob)
        font = hb.Font(face)
    except:
        return None

    buf.direction = direction
    buf.guess_segment_properties()

    features = {"kern": True, "liga": False, "clig": False, "calt": False, "mark": True}
    hb.shape(font, buf, features)

    infos = buf.glyph_infos
    positions = buf.glyph_positions
    if infos and any(info.codepoint == 0 for info in infos):
        print(f"[ERROR] Missing glyphs for text: '{text}'.")
        return None

    return infos, positions


def _script_of_codepoint(ch):
    """用 uharfbuzz 自带的 HarfBuzz 脚本表取单字符脚本(与 C 端
    hb_unicode_script 同源)。guess_segment_properties 不需要 font,
    仅靠 buffer 内容即可推断脚本。"""
    b = hb.Buffer()
    b.add_str(ch)
    b.guess_segment_properties()
    return b.script


def split_runs_by_script(runs):
    """在 BIDI run 基础上按 Unicode 脚本再切分,使每个 run 单一脚本。
    Common/Inherited/Unknown 跟随当前 run,不触发切分。"""
    refined = []
    for (start, end, run_text, run_dir) in runs:
        seg_start = 0
        cur_script = None
        for i, ch in enumerate(run_text):
            sc = _script_of_codepoint(ch)
            if sc is None or sc in ('Zyyy', 'Zinh', 'Zzzz'):
                continue
            if cur_script is None:
                cur_script = sc
            elif sc != cur_script:
                refined.append((start + seg_start, start + i, run_text[seg_start:i], run_dir))
                seg_start = i
                cur_script = sc
        refined.append((start + seg_start, end, run_text[seg_start:], run_dir))
    return refined


def arabic_shape(ttf_path, text, direct=None):
    if direct is not None and direct != "":
        result = _shape_single_run(ttf_path, text, direct)
        if result is None:
            return None
        infos, positions = result
        bidi_type = 2 if direct == 'rtl' else 1
        glyphs = [_ShapedGlyph(info, pos, 0, bidi_type) for info, pos in zip(infos, positions)]
        print(f"[INFO] gen text: {text}, str_len: {len(text)} direction: {direct}, glyph_count: {len(glyphs)}")
        return glyphs

    runs = bidi_get_runs(text)
    if len(runs) == 0:
        return None
    runs = split_runs_by_script(runs)

    if len(runs) == 1:
        start, end, run_text, run_dir = runs[0]
        result = _shape_single_run(ttf_path, run_text, run_dir)
        if result is None:
            return None
        infos, positions = result
        bidi_type = 2 if run_dir == 'rtl' else 1
        glyphs = [_ShapedGlyph(info, pos, start, bidi_type) for info, pos in zip(infos, positions)]
        print(f"[INFO] gen text: {text}, str_len: {len(text)} direction: {run_dir}, glyph_count: {len(glyphs)}")
        return glyphs

    all_glyphs = []
    for start, end, run_text, run_dir in runs:
        result = _shape_single_run(ttf_path, run_text, run_dir)
        if result is None:
            print(f"[ERROR] Run shape failed for: '{run_text}' dir: {run_dir}")
            continue
        infos, positions = result
        bidi_type = 2 if run_dir == 'rtl' else 1
        for info, pos in zip(infos, positions):
            all_glyphs.append(_ShapedGlyph(info, pos, start, bidi_type))

    print(f"[INFO] gen text: {text}, str_len: {len(text)} runs: {len(runs)}, total_glyphs: {len(all_glyphs)}")
    return all_glyphs
# ------------------------------------------------------------------

# ------------------ 功能函数 -----------------------------------
def convert_line_break(text:str):
    new_text = ""
    for ch in text:
        if ch == "\n":
            new_text += "\\n"
        else:
            new_text += ch
    return new_text


def fix_path(output_file:str):
    abs_path = os.path.abspath(output_file)
    abs_path = os.path.dirname(abs_path)
    if not os.path.exists(abs_path):
        os.makedirs(abs_path)

# ---------------------------------------------------------------


# -------------------------- 写文件函数 --------------------------
def write_begin_magic(output_file) -> int:
    written_size = 0
    if output_file != None and not output_file.closed:
        pack_format = "=%ds %ds" % (len(BIN_BEGIN_MAGIC), len(BIN_SEPARATOR))
        out_bytes = struct.pack(pack_format, BIN_BEGIN_MAGIC, BIN_SEPARATOR)
        written_size += output_file.write(out_bytes)
    return written_size

def write_version_magic(output_file) -> int:
    written_size = 0
    if output_file != None and not output_file.closed:
        pack_format = "=i"
        out_bytes = struct.pack(pack_format, VERSION_MAGIC)
        written_size += output_file.write(out_bytes)
    return written_size

def write_table_info_size(output_file, info_size:int) -> int:
    written_size = 0
    if output_file != None and not output_file.closed:
        pack_format = "=%ds %ds i %ds" % (len(BIN_TABLE_INFO_SIZE_MAGIC), len(BIN_SEPARATOR), len(BIN_LINEBREAK))
        out_bytes = struct.pack(pack_format, BIN_TABLE_INFO_SIZE_MAGIC, BIN_SEPARATOR, info_size, BIN_LINEBREAK)
        written_size += output_file.write(out_bytes)
    return written_size

# 写每一个字符串对应的整形信息
def write_table_info(output_file, ttfs_dict:dict) -> int:
    written_size = 0
    if output_file == None or output_file.closed:
        return 0

    pack_format = "=%ds %ds %ds" % (len(BIN_TABLE_INFO_MAGIC), len(BIN_SEPARATOR), len(BIN_LINEBREAK))
    out_bytes = struct.pack(pack_format, BIN_TABLE_INFO_MAGIC, BIN_SEPARATOR, BIN_LINEBREAK)
    written_size += output_file.write(out_bytes)

    out_bytes = ""
    pack_format = ""

    # 遍历每一个字符串
    for ttf_path, texts in ttfs_dict.items():
        texts_len = 0
        glyphs_list = list()
        cur_str_list = list()

        if (not os.path.isabs(ttf_path)):
            cur_ttf = os.path.join(os.path.dirname(output_file.name), ttf_path)
        else:
            cur_ttf = ttf_path
        for cur_str in texts:
            # harfbuzz整形
            if cur_str.__class__.__name__ == "list":
                glyphs = arabic_shape((cur_ttf if cur_ttf != (None or "") else ttfs[0]), cur_str[0], cur_str[1])
                if glyphs == None:
                    print(f"[ERROR] gen text: {cur_str} failed.")
                    continue
                cur_str_list.append(cur_str[0])
            else:
                glyphs = arabic_shape((cur_ttf if cur_ttf != (None or "") else ttfs[0]), cur_str)
                if glyphs == None:
                    print(f"[ERROR] gen text: {cur_str} failed.")
                    continue
                cur_str_list.append(cur_str)

            glyphs_list.append(glyphs)
            texts_len += 1

        file_name = os.path.splitext(os.path.basename(ttf_path))
        pack_format = "=%ds %ds i %ds" % (len(file_name[0].encode()), len(BIN_SEPARATOR), len(BIN_LINEBREAK))
        out_bytes = struct.pack(pack_format, file_name[0].encode(), BIN_SEPARATOR, texts_len, BIN_LINEBREAK)
        written_size += output_file.write(out_bytes)
        out_bytes = ""
        pack_format = ""

        for cur_str, glyphs in zip(cur_str_list, glyphs_list):
            pack_format = "=%ds %ds 2s I %ds I %ds" % (len(cur_str.encode()), len(BIN_SEPARATOR), len(BIN_SEPARATOR), len(BIN_SEPARATOR))
            out_bytes = struct.pack(pack_format, cur_str.encode(), BIN_SEPARATOR, BIN_DOUBLE_SEPARATOR, len(glyphs), BIN_SEPARATOR, len(cur_str), BIN_SEPARATOR)
            written_size += output_file.write(out_bytes)
            out_bytes = ""
            pack_format = ""
            for g in glyphs:
                pack_format = "=i i i i i i i %ds" % (len(BIN_SEPARATOR))
                out_bytes = struct.pack(pack_format, g.codepoint, g.cluster, g.x_offset, g.y_offset, g.x_advance, g.y_advance, g.bidi_type, BIN_SEPARATOR)
                written_size += output_file.write(out_bytes)
                out_bytes = ""
                pack_format = ""

            written_size += output_file.write(BIN_LINEBREAK)
        written_size += output_file.write(BIN_LINEBREAK)

    return written_size

def write_end_magic(output_file) -> int:
    written_size = 0
    if output_file != None and not output_file.closed:
        pack_format = "=%ds" % (len(BIN_EOF_CHECK))
        out_bytes = struct.pack(pack_format, BIN_EOF_CHECK)
        written_size += output_file.write(out_bytes)
    return written_size

# -----------------------------------------------------------------

def gen_harfbuzz_bin(ttfs_dict:dict, output_file:str):
    written_size = 0
    ttfs_len = len(ttfs_dict)
    if ttfs_len <= 0:
        return

    fix_path(output_file)

    with open(output_file, "wb") as output:
        written_size += write_begin_magic(output)
        written_size += write_version_magic(output)
        written_size += write_table_info_size(output, ttfs_len)
        written_size += write_table_info(output, ttfs_dict)
        written_size += write_end_magic(output)
        output.close()
        print(f"Total write: {written_size}")

    return

def load_ttf_and_text_from_json(json_path:str):
    with open(json_path, encoding="utf-8") as js_file:
        file_contents = js_file.read()
        print(f"json name {js_file.name}")
    js_info = json.loads(file_contents)

    for cur_id in js_info:
        cur_ttf = js_info[cur_id]
        user_json_infos[cur_id] = cur_ttf

def uharfbuzz_gen_bin(file_path:str, output_file:str = "_s_harfbuzz_data_info.bin"):
    ttf_list = list()
    text_ttf_dict = dict()

    load_ttf_and_text_from_json(file_path)

    for cur_id in user_json_infos:
        cur_ttf = user_json_infos[cur_id]
        cur_texts = cur_ttf["text"]
        text_list = list()
        for cur_text in cur_ttf["text"]:
            text_list.append(cur_text)
        text_ttf_dict[cur_ttf["ttf_path"]] = text_list

    for key, value in text_ttf_dict.items():
        value.sort(key = lambda x: (len(x), x))

    gen_harfbuzz_bin(text_ttf_dict, output_file)
