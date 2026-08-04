/**
 * File:   object_orchestrator_test.cc
 * Brief:  object_orchestrator（对象编排器）单元测试。
 *
 * 覆盖：
 *   - can_exec 两态机（空闲 <-> 已执行）规则
 *   - 默认工作流按插入序执行、undo 按 LIFO 反序回滚
 *   - undo_stack 引用计数生命周期（销毁释放 + 弹出释放）
 *   - EXEC 分支开头 darray_clear 释放旧栈引用
 *   - 自定义工作流按条件跳过未就绪 cmd（设置页"只应用已变更项"用法）
 *   - 执行失败自动回滚已成功执行的 cmd
 *   - cmd_on_execed 仅记录 EXEC 事件（不记录 UNDO 事件）
 *   - 撤销期间的重入互斥（undo 不得重复撤销同一 cmd）
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
  bool_t undo_reexec_self; /* 为 TRUE 时 undo 会补偿性重执自身一次（仅一次） */
  bool_t undo_reexec_done; /* 内部：标记补偿重入是否已发生 */
  tk_object_t* undo_reexec_target; /* 非 NULL 时 undo 会补偿性重执该命令一次（仅一次） */
  const char* last_args;               /* 捕获 exec 时收到的 args 指针（用于验证 args 透传） */
  bool_t undo_reenter_done;           /* 内部：标记 undo 重入 orch UNDO 是否已发生 */
  tk_object_t* undo_reenter_orch;      /* 非 NULL 时 undo 会重入该 orchestrator 的 UNDO 一次（仅一次） */
  bool_t undo_reexec_orch_exec_done;   /* 内部：标记 undo 重入 orch EXEC 是否已发生 */
  tk_object_t* undo_reexec_orch_exec;  /* 非 NULL 时 undo 会重入该 orchestrator 的 EXEC 一次（仅一次） */
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
    cmd->last_args = args; /* 记录收到的 args，验证 orchestrator 是否透传 */
    if (cmd->log != NULL) {
      cmd->log->push_back(string(cmd->name) + ":exec");
    }
    return cmd->exec_fail ? RET_FAIL : RET_OK;
  } else if (tk_str_eq(name, TK_OBJECT_CMD_UNDO)) {
    cmd->undo_count++;
    if (cmd->log != NULL) {
      cmd->log->push_back(string(cmd->name) + ":undo");
    }
    /* 探针：模拟 undo 时补偿性重执另一个已注册命令（仅一次） */
    if (cmd->undo_reexec_target != NULL && !cmd->undo_reexec_done) {
      cmd->undo_reexec_done = TRUE;
      (void)tk_object_exec(cmd->undo_reexec_target, TK_OBJECT_CMD_EXEC, args);
    }
    /* 探针：模拟 undo 时重入 orchestrator 的 UNDO 一次（仅一次） */
    if (cmd->undo_reenter_orch != NULL && !cmd->undo_reenter_done) {
      cmd->undo_reenter_done = TRUE;
      (void)tk_object_exec(cmd->undo_reenter_orch, TK_OBJECT_CMD_UNDO, args);
    }
    /* 探针：模拟 undo 时重入 orchestrator 的 EXEC 一次（仅一次） */
    if (cmd->undo_reexec_orch_exec != NULL && !cmd->undo_reexec_orch_exec_done) {
      cmd->undo_reexec_orch_exec_done = TRUE;
      (void)tk_object_exec(cmd->undo_reexec_orch_exec, TK_OBJECT_CMD_EXEC, args);
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

/* 自定义工作流：仅执行已变更（can_exec=TRUE）的 cmd，未变更项跳过继续下一项；ctx 为访问计数器，验证 ctx 透传 */
static ret_t skip_unready_workflow_on_visit(void* ctx, const void* data) {
  tk_object_t* cmd = value_object((const value_t*)data);
  int* visited = (int*)ctx;
  return_value_if_fail(cmd != NULL, RET_BAD_PARAMS);
  if (visited != NULL) {
    (*visited)++;
  }
  if (!tk_object_can_exec(cmd, TK_OBJECT_CMD_EXEC, NULL)) {
    return RET_OK; /* 前置条件不满足（设置未变更）：跳过，继续下一个 */
  }
  return tk_object_exec(cmd, TK_OBJECT_CMD_EXEC, NULL);
}

static ret_t skip_unready_workflow(tk_object_t* cmds, const char* args, void* ctx) {
  (void)args;
  return_value_if_fail(cmds != NULL, RET_BAD_PARAMS);
  return tk_object_foreach_prop(cmds, skip_unready_workflow_on_visit, ctx);
}

/* 自定义工作流：对每个 cmd 触发一次 UNDO（不执行 EXEC），验证 UNDO 事件不入 undo_stack */
static ret_t undo_triggering_workflow_on_visit(void* ctx, const void* data) {
  tk_object_t* cmd = value_object((const value_t*)data);
  return_value_if_fail(cmd != NULL, RET_BAD_PARAMS);
  (void)ctx;
  return tk_object_exec(cmd, TK_OBJECT_CMD_UNDO, NULL);
}

static ret_t undo_triggering_workflow(tk_object_t* cmds, const char* args, void* ctx) {
  (void)args;
  (void)ctx;
  return tk_object_foreach_prop(cmds, undo_triggering_workflow_on_visit, NULL);
}

/* 自定义工作流：执行 c1 后立即重入 orchestrator 的 UNDO，模拟工作流中途触发回滚（ctx 须为 orch 自身） */
static ret_t undo_mid_batch_workflow(tk_object_t* cmds, const char* args, void* ctx) {
  tk_object_t* orch = (tk_object_t*)ctx;
  tk_object_t* c1 = tk_object_get_prop_object(cmds, "c1");
  return_value_if_fail(orch != NULL && c1 != NULL, RET_BAD_PARAMS);
  (void)tk_object_exec(c1, TK_OBJECT_CMD_EXEC, args);   /* 入栈 c1 */
  (void)tk_object_exec(orch, TK_OBJECT_CMD_UNDO, args); /* 重入 orch 的 UNDO */
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

/* 引用计数生命周期：
 *   - EXEC 后不 undo 直接销毁：on_destroy 释放 undo_stack 与 cmds 的引用 -> ref_count 回到 1。
 *   - UNDO 弹出 cmd 后释放栈中引用 -> ref_count 降 1。 */
TEST(ObjectOrchestrator, UndoStackRefcountLifecycle) {
  /* EXEC 后不 UNDO 直接销毁 */
  {
    bool_t destroyed = FALSE;
    tk_object_t* orch = object_orchestrator_create();
    tk_object_t* c1 = fake_cmd_create("c1", NULL, &destroyed);
    ASSERT_EQ(object_orchestrator_add_cmd(orch, "c1", c1), RET_OK);
    /* create(1) + cmds 哈希(1) = 2 */
    ASSERT_EQ(c1->ref_count, 2);
    ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_EXEC, NULL), RET_OK);
    /* cmd_on_execed 入栈并 tk_object_ref -> 3 */
    ASSERT_EQ(c1->ref_count, 3);
    /* 销毁（不 undo）：darray_deinit 释放栈引用 + cmds 释放哈希引用 -> 1 */
    TK_OBJECT_UNREF(orch);
    ASSERT_EQ(c1->ref_count, 1);
    ASSERT_EQ(destroyed, FALSE);
    TK_OBJECT_UNREF(c1);
    ASSERT_EQ(destroyed, TRUE);
  }
  /* UNDO 弹出 cmd 后释放栈引用 */
  {
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
}

/* 重复 EXEC 先清空旧栈引用再重新入栈 -> ref_count 不升 */
TEST(ObjectOrchestrator, ExecClearsPriorUndoStack) {
  bool_t destroyed = FALSE;
  tk_object_t* orch = object_orchestrator_create();
  tk_object_t* c1 = fake_cmd_create("c1", NULL, &destroyed);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c1", c1), RET_OK);

  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_EXEC, NULL), RET_OK);
  ASSERT_EQ(c1->ref_count, 3);
  ASSERT_EQ(FAKE_CMD(c1)->exec_count, 1);

  /* can_exec(EXEC)=FALSE，但直接 exec 仍会先清空旧栈再重跑 */
  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_EXEC, NULL), RET_OK);
  /* 旧栈引用已释放，重新入栈再加一次 -> 仍为 3 */
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

/* 设备设置页"应用"：theme/lang 已变更 -> 执行并入栈；network 未变更(can_exec=FALSE)
 * -> 自定义工作流跳过、不入栈。UNDO 按实际执行序 LIFO 回滚，仅撤销 theme/lang。
 * ctx 为访问计数器，兼验 set_workflow 的 ctx 透传。 */
TEST(ObjectOrchestrator, CustomWorkflowSkipsUnreadyCmds) {
  vector<string> log;
  int visited = 0;
  tk_object_t* orch = object_orchestrator_create();
  tk_object_t* c1 = fake_cmd_create("theme", &log);
  tk_object_t* c2 = fake_cmd_create("lang", &log);
  tk_object_t* c3 = fake_cmd_create("network", &log);
  FAKE_CMD(c3)->can_exec_false = TRUE; /* network 未变更 -> can_exec 拒绝 */
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "theme", c1), RET_OK);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "lang", c2), RET_OK);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "network", c3), RET_OK);
  ASSERT_EQ(object_orchestrator_set_workflow(orch, skip_unready_workflow, &visited), RET_OK);

  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_EXEC, NULL), RET_OK);
  /* ctx 透传：3 个 cmd 都被 on_visit 访问过 */
  ASSERT_EQ(visited, 3);
  /* theme/lang 执行并入栈；network 被跳过 */
  ASSERT_EQ(FAKE_CMD(c1)->exec_count, 1);
  ASSERT_EQ(FAKE_CMD(c2)->exec_count, 1);
  ASSERT_EQ(FAKE_CMD(c3)->exec_count, 0);
  /* 栈含 theme/lang -> 可 UNDO、不可 EXEC */
  ASSERT_EQ(tk_object_can_exec(orch, TK_OBJECT_CMD_UNDO, NULL), TRUE);
  ASSERT_EQ(tk_object_can_exec(orch, TK_OBJECT_CMD_EXEC, NULL), FALSE);

  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_UNDO, NULL), RET_OK);
  ASSERT_EQ(FAKE_CMD(c1)->undo_count, 1);
  ASSERT_EQ(FAKE_CMD(c2)->undo_count, 1);
  ASSERT_EQ(FAKE_CMD(c3)->undo_count, 0);

  /* LIFO：theme 先入栈、lang 后入栈 -> undo 序为 lang、theme */
  string actual;
  for (size_t i = 0; i < log.size(); i++) {
    actual += log[i] + ",";
  }
  ASSERT_EQ(actual, string("theme:exec,lang:exec,lang:undo,theme:undo,"));

  /* 回到空闲态：可 EXEC、不可 UNDO */
  ASSERT_EQ(tk_object_can_exec(orch, TK_OBJECT_CMD_EXEC, NULL), TRUE);
  ASSERT_EQ(tk_object_can_exec(orch, TK_OBJECT_CMD_UNDO, NULL), FALSE);

  TK_OBJECT_UNREF(orch);
  TK_OBJECT_UNREF(c1);
  TK_OBJECT_UNREF(c2);
  TK_OBJECT_UNREF(c3);
}

/* 执行失败自动回滚：c2 中段失败 -> 仅回滚已入栈的 c1，c2/c3 不受影响 */
TEST(ObjectOrchestrator, FailedExecAutoRollback) {
  vector<string> log;
  tk_object_t* orch = object_orchestrator_create();
  tk_object_t* c1 = fake_cmd_create("c1", &log);
  tk_object_t* c2 = fake_cmd_create("c2", &log);
  tk_object_t* c3 = fake_cmd_create("c3", &log);
  FAKE_CMD(c2)->exec_fail = TRUE; /* c2 的 exec 返回 RET_FAIL，触发自动回滚 */
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c1", c1), RET_OK);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c2", c2), RET_OK);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c3", c3), RET_OK);

  /* 默认工作流遇非 OK 中止：c1 成功、c2 失败、c3 未执行；EXEC 返回非 OK */
  ASSERT_NE(tk_object_exec(orch, TK_OBJECT_CMD_EXEC, NULL), RET_OK);

  ASSERT_EQ(FAKE_CMD(c1)->exec_count, 1);
  ASSERT_EQ(FAKE_CMD(c2)->exec_count, 1);
  ASSERT_EQ(FAKE_CMD(c3)->exec_count, 0); /* c2 失败后工作流中止，c3 未执行 */

  /* 自动回滚：仅回滚已入栈的 c1（LIFO）；c2 失败未入栈故不被回滚 */
  ASSERT_EQ(FAKE_CMD(c1)->undo_count, 1);
  ASSERT_EQ(FAKE_CMD(c2)->undo_count, 0);
  ASSERT_EQ(FAKE_CMD(c3)->undo_count, 0);

  /* 回滚清空 undo_stack -> 回到空闲态：可 EXEC、不可 UNDO */
  ASSERT_EQ(tk_object_can_exec(orch, TK_OBJECT_CMD_EXEC, NULL), TRUE);
  ASSERT_EQ(tk_object_can_exec(orch, TK_OBJECT_CMD_UNDO, NULL), FALSE);

  string actual;
  for (size_t i = 0; i < log.size(); i++) {
    actual += log[i] + ",";
  }
  ASSERT_EQ(actual, string("c1:exec,c2:exec,c1:undo,"));

  TK_OBJECT_UNREF(orch);
  TK_OBJECT_UNREF(c1);
  TK_OBJECT_UNREF(c2);
  TK_OBJECT_UNREF(c3);
}

/* 回滚失败在返回码中不可见：c1 施加后 undo 失败，c1 留在已施加未回滚状态。
 * EXEC 返回值与干净回滚相同，残余状态仅由 can_exec 暴露：可 UNDO、不可 EXEC。 */
TEST(ObjectOrchestrator, FailedRollbackInvisibleInReturnCode) {
  vector<string> log;
  tk_object_t* orch = object_orchestrator_create();
  tk_object_t* c1 = fake_cmd_create("c1", &log);
  tk_object_t* c2 = fake_cmd_create("c2", &log);
  FAKE_CMD(c1)->undo_fail = TRUE; /* c1 的 undo 返回 RET_FAIL */
  FAKE_CMD(c2)->exec_fail = TRUE; /* c2 的 exec 失败 -> 触发自动回滚 */
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c1", c1), RET_OK);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c2", c2), RET_OK);

  /* EXEC 返回 RET_FAIL，与干净回滚的返回值相同 */
  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_EXEC, NULL), RET_FAIL);

  ASSERT_EQ(FAKE_CMD(c1)->exec_count, 1); /* c1 已施加 */
  ASSERT_EQ(FAKE_CMD(c1)->undo_count, 1); /* c1 的 undo 尝试失败 */
  ASSERT_EQ(FAKE_CMD(c2)->exec_count, 1); /* c2 失败触发回滚 */
  ASSERT_EQ(FAKE_CMD(c2)->undo_count, 0);

  /* 残余状态：回滚失败后 c1 仍 applied -> 可 UNDO、不可 EXEC（干净回滚则相反） */
  ASSERT_EQ(tk_object_can_exec(orch, TK_OBJECT_CMD_EXEC, NULL), FALSE);
  ASSERT_EQ(tk_object_can_exec(orch, TK_OBJECT_CMD_UNDO, NULL), TRUE);

  TK_OBJECT_UNREF(orch);
  TK_OBJECT_UNREF(c1);
  TK_OBJECT_UNREF(c2);
}

/* cmd_on_execed 只记录 EXEC 事件：工作流在 EXEC 阶段对每个 cmd 触发一次 UNDO，
 * 这些 UNDO 事件不得被记入 undo_stack。 */
TEST(ObjectOrchestrator, OnlyExecEventsRecordedForUndo) {
  tk_object_t* orch = object_orchestrator_create();
  tk_object_t* c1 = fake_cmd_create("c1");
  tk_object_t* c2 = fake_cmd_create("c2");
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c1", c1), RET_OK);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c2", c2), RET_OK);
  ASSERT_EQ(object_orchestrator_set_workflow(orch, undo_triggering_workflow, NULL), RET_OK);

  /* 工作流对每个 cmd 执行一次 UNDO（不执行 EXEC） */
  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_EXEC, NULL), RET_OK);

  ASSERT_EQ(FAKE_CMD(c1)->undo_count, 1);
  ASSERT_EQ(FAKE_CMD(c2)->undo_count, 1);
  ASSERT_EQ(FAKE_CMD(c1)->exec_count, 0);
  ASSERT_EQ(FAKE_CMD(c2)->exec_count, 0);

  /* UNDO 事件未被记录 -> undo_stack 为空 -> 不可 UNDO、可 EXEC */
  ASSERT_EQ(tk_object_can_exec(orch, TK_OBJECT_CMD_UNDO, NULL), FALSE);
  ASSERT_EQ(tk_object_can_exec(orch, TK_OBJECT_CMD_EXEC, NULL), TRUE);

  TK_OBJECT_UNREF(orch);
  TK_OBJECT_UNREF(c1);
  TK_OBJECT_UNREF(c2);
}

/* 自动回滚窗口内的重入：c1 的 undo 补偿性重执 c3（已注册、因 c2 失败未执行）一次。
 * 重入的 EXEC 被记入仍注册的 undo_stack，c1 仍须只被撤销一次。 */
TEST(ObjectOrchestrator, AutoRollbackReentryPopsWrongCmd) {
  vector<string> log;
  tk_object_t* orch = object_orchestrator_create();
  tk_object_t* c1 = fake_cmd_create("c1", &log);
  tk_object_t* c2 = fake_cmd_create("c2", &log);
  tk_object_t* c3 = fake_cmd_create("c3", &log);
  FAKE_CMD(c1)->undo_reexec_target = c3; /* c1 的 undo 补偿性重执 c3 一次 */
  FAKE_CMD(c2)->exec_fail = TRUE;         /* c2 失败 -> 触发自动回滚 */
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c1", c1), RET_OK);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c2", c2), RET_OK);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c3", c3), RET_OK);

  ASSERT_NE(tk_object_exec(orch, TK_OBJECT_CMD_EXEC, NULL), RET_OK);

  /* 核心不变量：c1 必须恰好被撤销 1 次。 */
  EXPECT_EQ(FAKE_CMD(c1)->undo_count, 1) << "原命令 c1 被重复撤销";
  /* 确认补偿性重执已发生：c3 被重执一次 */
  EXPECT_EQ(FAKE_CMD(c3)->exec_count, 1);

  /* 回滚后栈被清空 -> 回到空闲态 */
  ASSERT_EQ(tk_object_can_exec(orch, TK_OBJECT_CMD_EXEC, NULL), TRUE);
  ASSERT_EQ(tk_object_can_exec(orch, TK_OBJECT_CMD_UNDO, NULL), FALSE);

  TK_OBJECT_UNREF(orch);
  TK_OBJECT_UNREF(c1);
  TK_OBJECT_UNREF(c2);
  TK_OBJECT_UNREF(c3);
}

/* [已删] 重入 EXEC 的 busy 拦截不变量已由 ExecDuringUndoCrossOpHoleDoubleUndoes 覆盖，不再单列用例。 */

/* args 透传：orchestrator 的 EXEC args 应经默认工作流透传到每个 cmd 的 exec。 */
TEST(ObjectOrchestrator, ArgsForwardedToCmds) {
  tk_object_t* orch = object_orchestrator_create();
  tk_object_t* c1 = fake_cmd_create("c1");
  tk_object_t* c2 = fake_cmd_create("c2");
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c1", c1), RET_OK);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c2", c2), RET_OK);

  /* 默认工作流应把 orchestrator 的 args 透传给 cmd 的 exec */
  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_EXEC, "hello"), RET_OK);

  ASSERT_TRUE(FAKE_CMD(c1)->last_args != NULL)
      << "args 未透传给 c1";
  ASSERT_TRUE(FAKE_CMD(c2)->last_args != NULL)
      << "args 未透传给 c2";
  EXPECT_STREQ(FAKE_CMD(c1)->last_args, "hello");
  EXPECT_STREQ(FAKE_CMD(c2)->last_args, "hello");

  TK_OBJECT_UNREF(orch);
  TK_OBJECT_UNREF(c1);
  TK_OBJECT_UNREF(c2);
}

/* 工作流驱动的 drain：EXEC 期间工作流重入 UNDO，验证 c1 至多被撤销 1 次。 */
TEST(ObjectOrchestrator, WorkflowDrivenUndoDuringExecMispops) {
  vector<string> log;
  tk_object_t* orch = object_orchestrator_create();
  tk_object_t* c1 = fake_cmd_create("c1", &log);
  tk_object_t* c3 = fake_cmd_create("c3", &log);
  FAKE_CMD(c1)->undo_reexec_target = c3; /* c1 的 undo 补偿性重执 c3 一次 */
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c1", c1), RET_OK);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c3", c3), RET_OK);
  /* 自定义工作流：exec c1 后立即重入 orch UNDO（ctx=orch 自指）*/
  ASSERT_EQ(object_orchestrator_set_workflow(orch, undo_mid_batch_workflow, orch), RET_OK);

  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_EXEC, NULL), RET_OK);

  /* 核心不变量：c1 至多被撤销 1 次（不得双重撤销）。 */
  EXPECT_LE(FAKE_CMD(c1)->undo_count, 1)
      << "工作流驱动 drain 期间 c1 被双重撤销";

  TK_OBJECT_UNREF(orch);
  TK_OBJECT_UNREF(c1);
  TK_OBJECT_UNREF(c3);
}

/* cmd 的 undo 重入 orch UNDO：验证 c2 至多被撤销 1 次（内层 drain 不得重复撤销）。 */
TEST(ObjectOrchestrator, CmdUndoReentersOrchUndoDoubleUndoes) {
  vector<string> log;
  tk_object_t* orch = object_orchestrator_create();
  tk_object_t* c1 = fake_cmd_create("c1", &log);
  tk_object_t* c2 = fake_cmd_create("c2", &log);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c1", c1), RET_OK);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c2", c2), RET_OK);
  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_EXEC, NULL), RET_OK); /* 栈=[c1,c2] */
  /* c2 的 undo 重入 orch UNDO 一次 */
  FAKE_CMD(c2)->undo_reenter_orch = orch;

  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_UNDO, NULL), RET_OK);

  /* 核心不变量：c2 至多被撤销 1 次（不得双重撤销）。 */
  EXPECT_LE(FAKE_CMD(c2)->undo_count, 1)
      << "cmd undo 重入 orch UNDO 致 c2 双重撤销";
  EXPECT_EQ(FAKE_CMD(c1)->undo_count, 1);

  TK_OBJECT_UNREF(orch);
  TK_OBJECT_UNREF(c1);
  TK_OBJECT_UNREF(c2);
}

/* EXEC-during-UNDO 重入：c1 的 undo 重入 orch EXEC，验证 c1 至多撤销 1 次、整批不重复执行。 */
TEST(ObjectOrchestrator, ExecDuringUndoCrossOpHoleDoubleUndoes) {
  vector<string> log;
  tk_object_t* orch = object_orchestrator_create();
  tk_object_t* c1 = fake_cmd_create("c1", &log);
  tk_object_t* c2 = fake_cmd_create("c2", &log);
  FAKE_CMD(c1)->undo_reexec_orch_exec = orch; /* c1 的 undo 重入 orch EXEC 一次 */
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c1", c1), RET_OK);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c2", c2), RET_OK);
  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_EXEC, NULL), RET_OK); /* 栈=[c1,c2] */

  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_UNDO, NULL), RET_OK);

  /* 核心不变量：c1 至多被撤销 1 次。 */
  EXPECT_LE(FAKE_CMD(c1)->undo_count, 1)
      << "EXEC-during-UNDO 期间 c1 被双重撤销";
  EXPECT_EQ(FAKE_CMD(c1)->exec_count, 1);
  EXPECT_EQ(FAKE_CMD(c2)->exec_count, 1);
  EXPECT_EQ(FAKE_CMD(c2)->undo_count, 1);

  TK_OBJECT_UNREF(orch);
  TK_OBJECT_UNREF(c1);
  TK_OBJECT_UNREF(c2);
}

/* ===== 已知残余（框架级，非 object_orchestrator.c 可修）=====
 * EVT_CMD_EXECED/WILL_EXEC 观察者重入 EXEC 会绕过 busy 守卫（派发点归 object.c）。
 * 下方 DISABLED 用例锁定该不变量，待 object.c 增加守卫后启用即可通过。 */
typedef struct _reentry_probe_t {
  tk_object_t* orch;
  bool_t done;
} reentry_probe_t;

static ret_t execed_probe_handler(void* ctx, event_t* e) {
  reentry_probe_t* p = (reentry_probe_t*)ctx;
  if (!p->done) {
    p->done = TRUE;
    (void)tk_object_exec(p->orch, TK_OBJECT_CMD_EXEC, NULL);
  }
  return RET_OK;
}

/* DISABLED：框架级残余（见上方注释）。期望 EXECED 观察者重入 EXEC 被拒绝，
 * 整批 exec/undo 各 1 次；待 object.c 增加守卫后移除前缀即可通过。 */
TEST(ObjectOrchestrator, DISABLED_ExternalExecedObserverReentryMustNotDoubleExec) {
  vector<string> log;
  tk_object_t* orch = object_orchestrator_create();
  tk_object_t* c1 = fake_cmd_create("c1", &log);
  tk_object_t* c2 = fake_cmd_create("c2", &log);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c1", c1), RET_OK);
  ASSERT_EQ(object_orchestrator_add_cmd(orch, "c2", c2), RET_OK);

  reentry_probe_t p = {orch, FALSE};
  uint32_t id = emitter_on(EMITTER(orch), EVT_CMD_EXECED, execed_probe_handler, &p);

  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_EXEC, NULL), RET_OK);
  emitter_off(EMITTER(orch), id);

  ASSERT_EQ(tk_object_exec(orch, TK_OBJECT_CMD_UNDO, NULL), RET_OK);

  /* 期望：整批仅被执行一次、撤销一次，无孤儿施加。 */
  EXPECT_EQ(FAKE_CMD(c1)->exec_count, 1);
  EXPECT_EQ(FAKE_CMD(c1)->undo_count, 1);
  EXPECT_EQ(FAKE_CMD(c2)->exec_count, 1);
  EXPECT_EQ(FAKE_CMD(c2)->undo_count, 1);

  TK_OBJECT_UNREF(orch);
  TK_OBJECT_UNREF(c1);
  TK_OBJECT_UNREF(c2);
}
