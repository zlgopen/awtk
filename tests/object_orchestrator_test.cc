/**
 * File:   object_orchestrator_test.cc
 * Brief:  object_orchestrator（对象编排器）单元测试。
 *
 * 覆盖：
 *   - can_exec 两态机（空闲 <-> 已执行）规则
 *   - 默认工作流按插入序执行、undo 按 LIFO 反序回滚
 *   - on_destroy 释放 undo_stack 残留引用与分配器（Bug1 回归）
 *   - undo 弹出 cmd 后释放其引用（Bug2 回归）
 *   - EXEC 分支开头 slist_remove_all 释放旧栈引用
 *   - 自定义工作流替换默认实现并透传 ctx
 *   - cmd_on_execed 仅记录 exec 成功的 cmd（按 evt->result==RET_OK 过滤）
 */

#include "gtest/gtest.h"
#include "tkc/object.h"
#include "tkc/object_orchestrator.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

/**********************************************************************************
 * 一个最小化的“伪指令对象”，支持 TK_OBJECT_CMD_EXEC / TK_OBJECT_CMD_UNDO，
 * 记录 exec/undo 次数与顺序，可配置 exec 失败、can_exec 拒绝、销毁回调。
 **********************************************************************************/
typedef struct _fake_cmd_t {
  tk_object_t obj;
  const char* name;        /* 身份标识，用于顺序断言（指向字符串字面量） */
  int exec_count;
  int undo_count;
  bool_t exec_fail;        /* 为 TRUE 时 exec 返回 RET_FAIL */
  bool_t undo_fail;        /* 为 TRUE 时 undo 返回 RET_FAIL */
  bool_t can_exec_false;   /* 为 TRUE 时 can_exec("exec") 返回 FALSE */
  bool_t* destroyed;       /* 可选：销毁时置 TRUE 的外部标志 */
  vector<string>* log;     /* 可选：共享日志，记录 exec/undo 顺序 */
} fake_cmd_t;

static fake_cmd_t* fake_cmd_cast(tk_object_t* obj);
#define FAKE_CMD(obj) fake_cmd_cast(obj)

static bool_t fake_cmd_can_exec(tk_object_t* obj, const char* name, const char* args) {
  fake_cmd_t* cmd = FAKE_CMD(obj);
  return_value_if_fail(cmd != NULL, FALSE);
  (void)args;

  if (tk_str_eq(name, TK_OBJECT_CMD_EXEC)) {
    return !cmd->can_exec_false;
  } else if (tk_str_eq(name, TK_OBJECT_CMD_UNDO)) {
    return TRUE;
  }
  return FALSE;
}

static ret_t fake_cmd_exec(tk_object_t* obj, const char* name, const char* args) {
  fake_cmd_t* cmd = FAKE_CMD(obj);
  return_value_if_fail(cmd != NULL, RET_BAD_PARAMS);
  (void)args;

  if (tk_str_eq(name, TK_OBJECT_CMD_EXEC)) {
    cmd->exec_count++;
    if (cmd->log != NULL) {
      cmd->log->push_back(string(cmd->name) + ":exec");
    }
    return cmd->exec_fail ? RET_FAIL : RET_OK;
  } else if (tk_str_eq(name, TK_OBJECT_CMD_UNDO)) {
    cmd->undo_count++;
    if (cmd->log != NULL) {
      cmd->log->push_back(string(cmd->name) + ":undo");
    }
    return cmd->undo_fail ? RET_FAIL : RET_OK;
  }
  return RET_NOT_FOUND;
}

static ret_t fake_cmd_on_destroy(tk_object_t* obj) {
  fake_cmd_t* cmd = FAKE_CMD(obj);
  if (cmd != NULL && cmd->destroyed != NULL) {
    *cmd->destroyed = TRUE;
  }
  return RET_OK;
}

static const struct fake_cmd_vtable_t : object_vtable_t {
  fake_cmd_vtable_t() {
    type = "fake_cmd_t";
    desc = "fake_cmd_t";
    size = sizeof(fake_cmd_t);
    is_collection = FALSE;
    on_destroy = fake_cmd_on_destroy;
    can_exec = fake_cmd_can_exec;
    exec = fake_cmd_exec;
  }
} s_fake_cmd_vtable;

static tk_object_t* fake_cmd_create(const char* name, vector<string>* log = NULL,
                                   bool_t* destroyed = NULL) {
  tk_object_t* obj = tk_object_create(&s_fake_cmd_vtable);
  fake_cmd_t* cmd = FAKE_CMD(obj);
  return_value_if_fail(cmd != NULL, NULL);
  cmd->name = name;
  cmd->log = log;
  cmd->destroyed = destroyed;
  return obj;
}

static fake_cmd_t* fake_cmd_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_fake_cmd_vtable, NULL);
  return (fake_cmd_t*)obj;
}

/* 自定义工作流：仅记录被调用与 ctx，不执行任何 cmd（用于验证替换默认工作流） */
static ret_t noop_workflow(tk_object_t* cmds, const char* args, void* ctx) {
  bool_t* called = (bool_t*)ctx;
  (void)cmds;
  (void)args;
  if (called != NULL) {
    *called = TRUE;
  }
  return RET_OK;
}

/**********************************************************************************
 * 用例
 **********************************************************************************/

/* can_exec 两态机：空闲(可EXEC/不可UNDO) -> 已执行(不可EXEC/可UNDO) -> 空闲 */
TEST(ObjectOrchestrator, CanExecState) {
  tk_object_t* orch = object_orchestrator_create();
  ASSERT_TRUE(orch != NULL);

  /* 空：无 cmd -> EXEC/UNDO 均不可 */
  ASSERT_EQ(tk_object_can_exec(orch, TK_OBJECT_CMD_EXEC, NULL), FALSE);
  ASSERT_EQ(tk_object_can_exec(orch, TK_OBJECT_CMD_UNDO, NULL), FALSE);

  tk_object_t* c1 = fake_cmd_create("c1");
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c1", c1), RET_OK);

  /* 有 cmd 未执行：可 EXEC，不可 UNDO */
  ASSERT_EQ(tk_object_can_exec(orch, TK_OBJECT_CMD_EXEC, NULL), TRUE);
  ASSERT_EQ(tk_object_can_exec(orch, TK_OBJECT_CMD_UNDO, NULL), FALSE);

  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_EXEC, NULL), RET_OK);

  /* 已执行：不可 EXEC（undo_stack 非空），可 UNDO */
  ASSERT_EQ(tk_object_can_exec(orch, TK_OBJECT_CMD_EXEC, NULL), FALSE);
  ASSERT_EQ(tk_object_can_exec(orch, TK_OBJECT_CMD_UNDO, NULL), TRUE);

  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_UNDO, NULL), RET_OK);

  /* 回到空闲：可 EXEC，不可 UNDO */
  ASSERT_EQ(tk_object_can_exec(orch, TK_OBJECT_CMD_EXEC, NULL), TRUE);
  ASSERT_EQ(tk_object_can_exec(orch, TK_OBJECT_CMD_UNDO, NULL), FALSE);

  TK_OBJECT_UNREF(orch);
  TK_OBJECT_UNREF(c1);
}

/* 默认工作流按插入序执行；undo 从栈顶弹出 -> 反序回滚 */
TEST(ObjectOrchestrator, ExecUndoOrder) {
  vector<string> log;
  tk_object_t* orch = object_orchestrator_create();
  tk_object_t* c1 = fake_cmd_create("c1", &log);
  tk_object_t* c2 = fake_cmd_create("c2", &log);
  tk_object_t* c3 = fake_cmd_create("c3", &log);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c1", c1), RET_OK);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c2", c2), RET_OK);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c3", c3), RET_OK);

  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_EXEC, NULL), RET_OK);
  ASSERT_EQ(tk_object_can_exec(orch, TK_OBJECT_CMD_UNDO, NULL), TRUE);
  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_UNDO, NULL), RET_OK);

  string actual;
  for (size_t i = 0; i < log.size(); i++) {
    actual += log[i] + ",";
  }
  ASSERT_EQ(actual, string("c1:exec,c2:exec,c3:exec,c3:undo,c2:undo,c1:undo,"));

  ASSERT_EQ(FAKE_CMD(c1)->exec_count, 1);
  ASSERT_EQ(FAKE_CMD(c2)->exec_count, 1);
  ASSERT_EQ(FAKE_CMD(c3)->exec_count, 1);
  ASSERT_EQ(FAKE_CMD(c1)->undo_count, 1);
  ASSERT_EQ(FAKE_CMD(c2)->undo_count, 1);
  ASSERT_EQ(FAKE_CMD(c3)->undo_count, 1);

  TK_OBJECT_UNREF(orch);
  TK_OBJECT_UNREF(c1);
  TK_OBJECT_UNREF(c2);
  TK_OBJECT_UNREF(c3);
}

/* Bug1 回归：EXEC 后不 undo 直接销毁，on_destroy 必须 slist_deinit(undo_stack)
 * 释放栈中引用，并释放 cmds 中的引用 -> ref_count 回到 1（仅剩测试自持的引用） */
TEST(ObjectOrchestrator, DestroyReleasesUndoStackRefs) {
  bool_t destroyed = FALSE;
  tk_object_t* orch = object_orchestrator_create();
  tk_object_t* c1 = fake_cmd_create("c1", NULL, &destroyed);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c1", c1), RET_OK);

  /* create(1) + cmds 哈希(1) = 2 */
  ASSERT_EQ(c1->ref_count, 2);
  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_EXEC, NULL), RET_OK);
  /* cmd_on_execed 入栈并 tk_object_ref -> 3 */
  ASSERT_EQ(c1->ref_count, 3);

  /* 销毁（不 undo）：slist_deinit 释放栈引用 + cmds 释放哈希引用 -> 1 */
  TK_OBJECT_UNREF(orch);
  ASSERT_EQ(c1->ref_count, 1);
  ASSERT_EQ(destroyed, FALSE);

  TK_OBJECT_UNREF(c1);
  ASSERT_EQ(destroyed, TRUE);
}

/* Bug2 回归：UNDO 弹出 cmd 后必须释放栈中持有的引用 -> ref_count 降 1 */
TEST(ObjectOrchestrator, UndoReleasesPoppedCmdRef) {
  bool_t destroyed = FALSE;
  tk_object_t* orch = object_orchestrator_create();
  tk_object_t* c1 = fake_cmd_create("c1", NULL, &destroyed);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c1", c1), RET_OK);
  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_EXEC, NULL), RET_OK);
  /* create(1) + cmds(1) + undo_stack(1) = 3 */
  ASSERT_EQ(c1->ref_count, 3);

  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_UNDO, NULL), RET_OK);
  /* 弹出并释放栈引用 -> 2（create + cmds） */
  ASSERT_EQ(c1->ref_count, 2);

  TK_OBJECT_UNREF(orch);
  /* cmds 释放 -> 1（仅剩测试自持） */
  ASSERT_EQ(c1->ref_count, 1);
  ASSERT_EQ(destroyed, FALSE);

  TK_OBJECT_UNREF(c1);
  ASSERT_EQ(destroyed, TRUE);
}

/* EXEC 分支开头的 slist_remove_all 释放旧栈引用后重新入栈 -> ref_count 不升 */
TEST(ObjectOrchestrator, ExecClearsPriorUndoStack) {
  bool_t destroyed = FALSE;
  tk_object_t* orch = object_orchestrator_create();
  tk_object_t* c1 = fake_cmd_create("c1", NULL, &destroyed);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c1", c1), RET_OK);

  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_EXEC, NULL), RET_OK);
  ASSERT_EQ(c1->ref_count, 3);
  ASSERT_EQ(FAKE_CMD(c1)->exec_count, 1);

  /* can_exec(EXEC) 现为 FALSE，但直接 exec(EXEC) 必须先清空旧栈再重跑 */
  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_EXEC, NULL), RET_OK);
  /* 旧栈引用被 slist_remove_all 释放，cmd_on_execed 再加一次 -> 仍为 3 */
  ASSERT_EQ(c1->ref_count, 3);
  ASSERT_EQ(FAKE_CMD(c1)->exec_count, 2);

  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_UNDO, NULL), RET_OK);
  ASSERT_EQ(c1->ref_count, 2);

  TK_OBJECT_UNREF(orch);
  ASSERT_EQ(c1->ref_count, 1);
  ASSERT_EQ(destroyed, FALSE);
  TK_OBJECT_UNREF(c1);
  ASSERT_EQ(destroyed, TRUE);
}

/* 自定义工作流替换默认实现并透传 ctx；不执行任何 cmd -> undo_stack 为空 */
TEST(ObjectOrchestrator, CustomWorkflowReplacesDefault) {
  bool_t called = FALSE;
  vector<string> log;
  tk_object_t* orch = object_orchestrator_create();
  tk_object_t* c1 = fake_cmd_create("c1", &log);
  tk_object_t* c2 = fake_cmd_create("c2", &log);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c1", c1), RET_OK);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c2", c2), RET_OK);
  ASSERT_EQ(object_orchestrator_set_workflow(orch, noop_workflow, &called), RET_OK);

  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_EXEC, NULL), RET_OK);

  /* 自定义工作流被调用（ctx 透传成功），且没有执行任何 cmd */
  ASSERT_EQ(called, TRUE);
  ASSERT_TRUE(log.empty());
  /* 没有 cmd 入栈 -> 不可 UNDO */
  ASSERT_EQ(tk_object_can_exec(orch, TK_OBJECT_CMD_UNDO, NULL), FALSE);

  TK_OBJECT_UNREF(orch);
  TK_OBJECT_UNREF(c1);
  TK_OBJECT_UNREF(c2);
}

/* exec 返回 RET_FAIL 的 cmd 不会被 cmd_on_execed 记录入栈 -> undo 不会回滚它 */
TEST(ObjectOrchestrator, FailedExecNotRecordedForUndo) {
  vector<string> log;
  tk_object_t* orch = object_orchestrator_create();
  tk_object_t* c1 = fake_cmd_create("c1", &log);
  tk_object_t* c2 = fake_cmd_create("c2", &log);
  tk_object_t* c3 = fake_cmd_create("c3", &log);
  FAKE_CMD(c3)->exec_fail = TRUE; /* c3 的 exec 返回 RET_FAIL */
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c1", c1), RET_OK);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c2", c2), RET_OK);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c3", c3), RET_OK);

  /* 默认工作流遇非 OK 会中止，整体 EXEC 返回非 OK */
  ASSERT_NE(tk_object_exec(orch, TK_OBJECT_CMD_EXEC, NULL), RET_OK);

  /* 三个 cmd 都被访问并 exec 过 */
  ASSERT_EQ(FAKE_CMD(c1)->exec_count, 1);
  ASSERT_EQ(FAKE_CMD(c2)->exec_count, 1);
  ASSERT_EQ(FAKE_CMD(c3)->exec_count, 1);

  /* 但只有成功的 c1/c2 入栈；undo 按 LIFO 反序回滚它们 */
  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_UNDO, NULL), RET_OK);
  ASSERT_EQ(FAKE_CMD(c1)->undo_count, 1);
  ASSERT_EQ(FAKE_CMD(c2)->undo_count, 1);
  ASSERT_EQ(FAKE_CMD(c3)->undo_count, 0);

  string actual;
  for (size_t i = 0; i < log.size(); i++) {
    actual += log[i] + ",";
  }
  ASSERT_EQ(actual, string("c1:exec,c2:exec,c3:exec,c2:undo,c1:undo,"));

  TK_OBJECT_UNREF(orch);
  TK_OBJECT_UNREF(c1);
  TK_OBJECT_UNREF(c2);
  TK_OBJECT_UNREF(c3);
}
