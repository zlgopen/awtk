/**
 * stb_textedit_undo.inc —— AWTK 对 stb_textedit 撤销分组的修改
 *
 * 只由 3rd/stb/stb_textedit.h 在 STB_TEXTEDIT_IMPLEMENTATION 段内包含，不要单独包含。
 *
 * 分组规则对齐 VS Code（monaco editor 的 cursorTypeEditOperations.ts）：
 *   - 连续输入：遇到半角空格开始新分组，空格归到它后面那一段；连续的空格单独一段，
 *     空格之后回到普通字符时再断一次；换行单独一段；其它字符一律不断组；
 *   - 连续删除：同方向一直合并成一条，跨行删除单独一段；
 *   - 一次粘贴：单独成段；
 *   - 选区被输入/粘贴替换：删除选区与紧随其后的第一个内容合成一步，之后再按输入规则
 *     继续分组（选中一段打 “hello world” 会分成 “hello” 和 “ world” 两段）。
 */

/* 上游的 stb_text_createundo 在本文件之后定义，这里先声明 */
static STB_TEXTEDIT_CHARTYPE* stb_text_createundo(STB_TexteditState* state, int pos, int insert_len,
                                                  int delete_len);

typedef enum _stb_text_merge_t {
  STB_TEXT_MERGE_NONE = 0,     /* 不可合并：上一条是替换/设置文本等记录 */
  STB_TEXT_MERGE_TYPING,       /* 连续输入普通字符（对应 TypingOther） */
  STB_TEXT_MERGE_SPACE_FIRST,  /* 输入的第一个空格（对应 TypingFirstSpace） */
  STB_TEXT_MERGE_SPACE_MORE,   /* 连续输入的空格（对应 TypingConsecutiveSpace） */
  STB_TEXT_MERGE_PASTE,        /* 一次粘贴 */
  STB_TEXT_MERGE_BACKSPACE,    /* 连续退格删除 */
  STB_TEXT_MERGE_DELETE,       /* 连续 Delete 向后删除 */
} stb_text_merge_t;

/* 结束当前撤销分组：撤销/重做、光标或选区变化、整段命令之后调用 */
inline static void stb_text_end_undo_group(STB_TexteditState* state) {
  state->can_merge = (unsigned char)STB_TEXT_MERGE_NONE;
  state->undo_changed = 0;
  state->replace_group = 0;
}

/* 标记当前分组是“选区被替换”的记录：删除选区之后调用 */
inline static void stb_text_start_replace_group(STB_TexteditState* state) {
  state->replace_group = 1;
}

/* 判断是否是输入类分组 */
inline static int stb_text_is_typing(stb_text_merge_t kind) {
  return kind == STB_TEXT_MERGE_TYPING || kind == STB_TEXT_MERGE_SPACE_FIRST ||
         kind == STB_TEXT_MERGE_SPACE_MORE;
}

/* 本次输入的操作类型：只有单独一个半角空格才有独立状态（对齐 VS Code 的 getTypingOperation） */
inline static stb_text_merge_t stb_text_typing_op(STB_TEXTEDIT_STRING* str, int where, int length,
                                                  stb_text_merge_t prev) {
  if (length != 1 || STB_TEXTEDIT_GETCHAR(str, where) != ' ') return STB_TEXT_MERGE_TYPING;

  if (prev == STB_TEXT_MERGE_SPACE_FIRST || prev == STB_TEXT_MERGE_SPACE_MORE)
    return STB_TEXT_MERGE_SPACE_MORE;

  return STB_TEXT_MERGE_SPACE_FIRST;
}

/* 本次插入是否要另起一组（对齐 VS Code 的 shouldPushStackElementBetween） */
inline static int stb_text_insert_break(STB_TexteditState* state, stb_text_merge_t op,
                                        StbUndoRecord* r) {
  if (r == NULL) return 1;

  /* 上一条不是输入类记录（粘贴、删除、设置文本），另起一组 */
  if (!stb_text_is_typing(state->can_merge)) return 1;

  /* 非输入类内容（粘贴）不与输入合并 */
  if (!stb_text_is_typing(op)) return 1;

  /* 空格总是开始新分组：“hello| world” */
  if (op == STB_TEXT_MERGE_SPACE_FIRST) return 1;

  /* 连续空格之后回到普通字符再断一次：“abc  |d”，但“abc |d”不断 */
  return op == STB_TEXT_MERGE_TYPING && state->can_merge == STB_TEXT_MERGE_SPACE_MORE;
}

/* 记录被合并/延长之后，更新“编辑之后”的光标位置 */
inline static void stb_text_update_record_after(StbUndoRecord* r) {
  r->after_cursor = r->where + r->delete_length;
  r->after_select_start = r->after_cursor;
  r->after_select_end = r->after_cursor;
}

/* 用实时光标/选区更新最近一条记录的“编辑之后”状态，由调用方在每个编辑动作结束后调用 */
inline static void stb_text_update_undo_after(STB_TexteditState* state) {
  StbUndoState* s = &state->undostate;
  StbUndoRecord* r = NULL;

  if (!state->undo_changed || s->undo_point == 0) return;

  state->undo_changed = 0;
  r = &s->undo_rec[s->undo_point - 1];
  r->after_cursor = state->cursor;
  r->after_select_start = state->select_start;
  r->after_select_end = state->select_end;
}

/* 取最近一条撤销记录（仅在历史末尾、且没有可重做的记录时才允许合并） */
inline static StbUndoRecord* stb_text_peek_undo(STB_TexteditState* state) {
  StbUndoState* s = &state->undostate;

  if (s->undo_point == 0 || s->redo_point != STB_TEXTEDIT_UNDOSTATECOUNT) return NULL;
  return &s->undo_rec[s->undo_point - 1];
}

/* 连续输入/粘贴时合并到上一条记录，避免每个字符占一条历史 */
static int stb_text_merge_insert(STB_TexteditState* state, int where, int length) {
  StbUndoRecord* r = stb_text_peek_undo(state);

  if (r == NULL || r->insert_length != 0 || r->delete_length == 0) return 0;

  /* 只合并在上一条记录末尾紧接着输入的字符；光标移走后位置不再相邻，自然断开 */
  if (r->where + r->delete_length != where) return 0;

  /* 一条记录太长时，撤销就腾不出字符缓冲来保存重做所需的字符，分组到此为止 */
  if (state->undostate.undo_char_point + r->delete_length + length >= STB_TEXTEDIT_UNDOCHARCOUNT)
    return 0;

  r->delete_length += length;
  stb_text_update_record_after(r);

  return 1;
}

/* 连续删除时合并到上一条删除记录，避免每个字符占一条历史 */
static int stb_text_merge_delete(STB_TEXTEDIT_STRING* str, STB_TexteditState* state, int where,
                                 int length, stb_text_merge_t kind) {
  StbUndoState* s = &state->undostate;
  StbUndoRecord* r = stb_text_peek_undo(state);
  int i;

  /* 只有同方向的连续删除才合并：退格和 Delete 是不同的命令 */
  if (kind == STB_TEXT_MERGE_NONE || state->can_merge != kind || r == NULL) return 0;
  if (r->delete_length != 0 || r->insert_length == 0 || r->char_storage < 0) return 0;
  if (s->undo_char_point + length > STB_TEXTEDIT_UNDOCHARCOUNT) return 0;

  if (kind == STB_TEXT_MERGE_BACKSPACE && where + length == r->where) {
    /* Backspace：新删的字符在已存字符之前，先把它插到已存字符前面 */
    STB_TEXTEDIT_CHARTYPE* base = &s->undo_char[r->char_storage];

    STB_TEXTEDIT_memmove(
        base + length, base,
        (size_t)((s->undo_char_point - r->char_storage) * sizeof(STB_TEXTEDIT_CHARTYPE)));
    for (i = 0; i < length; ++i) base[i] = STB_TEXTEDIT_GETCHAR(str, where + i);
    r->where = where;
    r->insert_length += length;
    s->undo_char_point += length;
    stb_text_update_record_after(r);
    return 1;
  }

  if (kind == STB_TEXT_MERGE_DELETE && where == r->where) {
    /* Delete：新删的字符接在已存字符之后 */
    for (i = 0; i < length; ++i)
      s->undo_char[s->undo_char_point + i] = STB_TEXTEDIT_GETCHAR(str, where + i);
    r->insert_length += length;
    s->undo_char_point += length;
    stb_text_update_record_after(r);
    return 1;
  }

  return 0;
}

/* 选区被输入替换：把上一条“选区删除”记录就地改造成替换记录，撤销时一步恢复 */
static int stb_text_merge_replace(STB_TexteditState* state, int where, int length) {
  StbUndoRecord* r = stb_text_peek_undo(state);

  if (r == NULL || r->insert_length == 0 || r->char_storage < 0) return 0;

  /* 只有紧接在这条记录末尾的输入才并进来 */
  if (r->where + r->delete_length != where) return 0;

  /* 撤销时要保存重做所需的字符，单条记录不能超过字符缓冲 */
  if (state->undostate.undo_char_point + r->delete_length + length >= STB_TEXTEDIT_UNDOCHARCOUNT)
    return 0;

  r->delete_length += length;
  stb_text_update_record_after(r);

  return 1;
}

static void stb_text_makeundo_insert(STB_TEXTEDIT_STRING* str, STB_TexteditState* state, int where,
                                     int length, stb_text_merge_t kind) {
  StbUndoRecord* r = stb_text_peek_undo(state);
  stb_text_merge_t op = kind;
  int is_newline = 0;
  int merged = 0;

  if (kind == STB_TEXT_MERGE_TYPING) {
    op = stb_text_typing_op(str, where, length, state->can_merge);
    is_newline = length == 1 && STB_TEXTEDIT_GETCHAR(str, where) == STB_TEXTEDIT_NEWLINE;
  }

  /* 选区刚被替换：第一个插入内容归属同一步；换行之后后面的输入另起一组 */
  if (state->replace_group && r != NULL && r->delete_length == 0 &&
      stb_text_merge_replace(state, where, length)) {
    if (is_newline) {
      stb_text_end_undo_group(state);
    } else {
      state->can_merge = (unsigned char)op;
    }
    state->undo_changed = 1;
    return;
  }

  /* 换行是独立的一步，后面的输入另起一组（对齐 VS Code 的 EnterOperation） */
  if (is_newline) {
    stb_text_createundo(state, where, 0, length);
    stb_text_end_undo_group(state);
    state->undo_changed = 1;
    return;
  }

  if (!stb_text_insert_break(state, op, r)) {
    merged = state->replace_group ? stb_text_merge_replace(state, where, length)
                                  : stb_text_merge_insert(state, where, length);
    if (merged) {
      state->can_merge = (unsigned char)op;
      state->undo_changed = 1;
      return;
    }
  }

  stb_text_createundo(state, where, 0, length);
  state->can_merge = (unsigned char)op;
  state->replace_group = 0;
  state->undo_changed = 1;
}

static void stb_text_makeundo_delete(STB_TEXTEDIT_STRING* str, STB_TexteditState* state, int where,
                                     int length) {
  int i;
  int multiline = 0;
  stb_text_merge_t kind;
  STB_TEXTEDIT_CHARTYPE* p;

  /* 按删除位置和光标位置判断方向：退格删光标左侧，Delete 删光标右侧，两者是不同命令 */
  if (where + length == state->cursor) {
    kind = STB_TEXT_MERGE_BACKSPACE;
  } else if (where == state->cursor) {
    kind = STB_TEXT_MERGE_DELETE;
  } else {
    /* 选区删除等与方向无关，能否合并由调用方决定 */
    kind = STB_TEXT_MERGE_NONE;
  }

  /* 跨行删除本身是一步完整操作，不与相邻删除合并（对齐 VS Code） */
  for (i = 0; i < length && !multiline; ++i) {
    multiline = STB_TEXTEDIT_GETCHAR(str, where + i) == STB_TEXTEDIT_NEWLINE;
  }

  if (!multiline && stb_text_merge_delete(str, state, where, length, kind)) {
    state->undo_changed = 1;
    return;
  }

  p = stb_text_createundo(state, where, length, 0);
  if (p) {
    for (i = 0; i < length; ++i) p[i] = STB_TEXTEDIT_GETCHAR(str, where + i);
  }

  state->can_merge = (unsigned char)kind;
  state->replace_group = 0;
  state->undo_changed = 1;
}

static void stb_text_makeundo_replace(STB_TEXTEDIT_STRING* str, STB_TexteditState* state, int where,
                                      int old_length, int new_length) {
  int i;
  STB_TEXTEDIT_CHARTYPE* p = stb_text_createundo(state, where, old_length, new_length);
  if (p) {
    for (i = 0; i < old_length; ++i) p[i] = STB_TEXTEDIT_GETCHAR(str, where + i);
  }
  /* 替换（覆写输入、整体设置文本）不参与后续输入的合并 */
  stb_text_end_undo_group(state);
}
