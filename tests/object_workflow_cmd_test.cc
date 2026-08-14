/**
 * File:   object_workflow_cmd_test.cc
 * Brief:  object_workflow_cmd（对象命令工作流）单元测试。
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
 *   - 自定义工作流执行期间动态创建/加入/执行指令并纳入撤销簿记（新委托设计）
 *   - copy_props 经 cmds_set_prop override 订阅 EXECED 观察者（修复旁路回归）
 *   - clone 语义：子命令按 ref 共享（非深 clone）、EVT_CMD_EXECED 订阅以新对象为 ctx
 *     重建、undo_stack 独立、外部观察契约不偷渡到克隆体
 */

#include "gtest/gtest.h"
#include "tkc/object.h"
#include "tkc/object_workflow_cmd.h"
#include "tkc/object_hash.h"

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
  const char* name; /* 身份标识，用于顺序断言（指向字符串字面量） */
  int exec_count;
  int undo_count;
  bool_t exec_fail;                  /* 为 TRUE 时 exec 返回 RET_FAIL */
  bool_t undo_fail;                  /* 为 TRUE 时 undo 返回 RET_FAIL */
  bool_t can_exec_false;             /* 为 TRUE 时 can_exec("exec") 返回 FALSE */
  bool_t undo_reexec_self;           /* 为 TRUE 时 undo 会补偿性重执自身一次（仅一次） */
  bool_t undo_reexec_done;           /* 内部：标记补偿重入是否已发生 */
  tk_object_t* undo_reexec_target;   /* 非 NULL 时 undo 会补偿性重执该命令一次（仅一次） */
  const char* last_args;             /* 捕获 exec 时收到的 args 指针（用于验证 args 透传） */
  bool_t undo_reenter_done;          /* 内部：标记 undo 重入 wfc UNDO 是否已发生 */
  tk_object_t* undo_reenter_wfc;     /* 非 NULL 时 undo 会重入该 workflow 的 UNDO 一次（仅一次） */
  bool_t undo_reexec_wfc_exec_done;  /* 内部：标记 undo 重入 wfc EXEC 是否已发生 */
  tk_object_t* undo_reexec_wfc_exec; /* 非 NULL 时 undo 会重入该 workflow 的 EXEC 一次（仅一次） */
  bool_t* destroyed;                 /* 可选：销毁时置 TRUE 的外部标志 */
  vector<string>* log;               /* 可选：共享日志，记录 exec/undo 顺序 */
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
    cmd->last_args = args; /* 记录收到的 args，验证 workflow 是否透传 */
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
    /* 探针：模拟 undo 时重入 workflow 的 UNDO 一次（仅一次） */
    if (cmd->undo_reenter_wfc != NULL && !cmd->undo_reenter_done) {
      cmd->undo_reenter_done = TRUE;
      (void)tk_object_exec(cmd->undo_reenter_wfc, TK_OBJECT_CMD_UNDO, args);
    }
    /* 探针：模拟 undo 时重入 workflow 的 EXEC 一次（仅一次） */
    if (cmd->undo_reexec_wfc_exec != NULL && !cmd->undo_reexec_wfc_exec_done) {
      cmd->undo_reexec_wfc_exec_done = TRUE;
      (void)tk_object_exec(cmd->undo_reexec_wfc_exec, TK_OBJECT_CMD_EXEC, args);
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

static ret_t skip_unready_workflow(tk_object_t* obj, const char* args, void* ctx) {
  (void)args;
  /* 新 API：workflow 自身委托 foreach_prop 到 cmds（cmds_foreach_prop override），
   * 直接对 obj 遍历即可，无需 object_workflow_cmd_get_cmds。 */
  return tk_object_foreach_prop(obj, skip_unready_workflow_on_visit, ctx);
}

/* 自定义工作流：对每个 cmd 触发一次 UNDO（不执行 EXEC），验证 UNDO 事件不入 undo_stack */
static ret_t undo_triggering_workflow_on_visit(void* ctx, const void* data) {
  tk_object_t* cmd = value_object((const value_t*)data);
  return_value_if_fail(cmd != NULL, RET_BAD_PARAMS);
  (void)ctx;
  return tk_object_exec(cmd, TK_OBJECT_CMD_UNDO, NULL);
}

static ret_t undo_triggering_workflow(tk_object_t* obj, const char* args, void* ctx) {
  (void)args;
  (void)ctx;
  /* 新 API：workflow 自身委托 foreach_prop 到 cmds */
  return tk_object_foreach_prop(obj, undo_triggering_workflow_on_visit, NULL);
}

/* 自定义工作流：执行 c1 后立即重入 workflow 的 UNDO，模拟工作流中途触发回滚。
 * 新 API 下首个参数 obj 即 workflow 自身（原 ctx 自指已冗余），
 * workflow 自身委托 get_prop 到 cmds，直接对 obj 取 c1 即可，无需 get_cmds。 */
static ret_t undo_mid_batch_workflow(tk_object_t* obj, const char* args, void* ctx) {
  tk_object_t* c1 = tk_object_get_prop_object(obj, "c1");
  return_value_if_fail(c1 != NULL, RET_BAD_PARAMS);
  (void)ctx;
  (void)tk_object_exec(c1, TK_OBJECT_CMD_EXEC, args);  /* 入栈 c1 */
  (void)tk_object_exec(obj, TK_OBJECT_CMD_UNDO, args); /* 重入 wfc 的 UNDO */
  return RET_OK;
}

/* 自定义工作流执行期间动态创建/加入/执行指令：
 * 体现新委托设计的便利——工作流中对 obj 调用 tk_object_set_prop_object 经 cmds_set_prop override
 * 自动订阅 EVT_CMD_EXECED 观察者，使该指令的后续 EXEC 被记入 undo_stack、可被 UNDO 回滚。
 * 创建引用经 ctx->cmd 转移给测试用于事后断言与释放（即便后续步骤失败也不泄漏）。 */
typedef struct _dynamic_cmd_ctx_t {
  tk_object_t* cmd; /* out：工作流现场创建的指令指针（创建引用转移给测试） */
  vector<string>* log;
} dynamic_cmd_ctx_t;

static ret_t dynamic_cmd_workflow(tk_object_t* obj, const char* args, void* ctx) {
  dynamic_cmd_ctx_t* info = (dynamic_cmd_ctx_t*)ctx;
  tk_object_t* cmd = NULL;
  ret_t ret = RET_OK;
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  /* 1) 工作流执行期间现场创建指令 */
  cmd = fake_cmd_create("dynamic", info->log);
  return_value_if_fail(cmd != NULL, RET_OOM);
  info->cmd = cmd; /* 创建引用转移给测试（即使后续失败也不泄漏） */
  /* 2) 经委托 set_prop 加入 cmds：cmds_set_prop override 自动订阅 EXECED 观察者 */
  ret = tk_object_set_prop_object(obj, "dynamic", cmd);
  return_value_if_fail(RET_OK == ret, ret);
  /* 3) 执行该指令：其 EXEC 事件经观察者记入 undo_stack */
  return tk_object_exec(cmd, TK_OBJECT_CMD_EXEC, args);
}

/**********************************************************************************
 * 用例
 **********************************************************************************/

/* can_exec 两态机：空闲(可EXEC/不可UNDO) -> 已执行(不可EXEC/可UNDO) -> 空闲 */
TEST(ObjectWorkflowCmd, CanExecState) {
  tk_object_t* wfc = object_workflow_cmd_create();
  ASSERT_TRUE(wfc != NULL);

  /* 空：无 cmd -> 新 API 不再检查 cmds_size：可 EXEC（空跑 no-op）、不可 UNDO */
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), TRUE);
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), FALSE);

  tk_object_t* c1 = fake_cmd_create("c1");
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c1", c1), RET_OK);

  /* 有 cmd 未执行：可 EXEC，不可 UNDO */
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), TRUE);
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), FALSE);

  ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), RET_OK);

  /* 已执行：不可 EXEC（undo_stack 非空），可 UNDO */
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), FALSE);
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), TRUE);

  ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), RET_OK);

  /* 回到空闲：可 EXEC，不可 UNDO */
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), TRUE);
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), FALSE);

  TK_OBJECT_UNREF(wfc);
  TK_OBJECT_UNREF(c1);
}

/* 默认工作流按插入序执行；undo 从栈顶弹出 -> 反序回滚 */
TEST(ObjectWorkflowCmd, ExecUndoOrder) {
  vector<string> log;
  tk_object_t* wfc = object_workflow_cmd_create();
  tk_object_t* c1 = fake_cmd_create("c1", &log);
  tk_object_t* c2 = fake_cmd_create("c2", &log);
  tk_object_t* c3 = fake_cmd_create("c3", &log);
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c1", c1), RET_OK);
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c2", c2), RET_OK);
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c3", c3), RET_OK);

  ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), RET_OK);
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), TRUE);
  ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), RET_OK);

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

  TK_OBJECT_UNREF(wfc);
  TK_OBJECT_UNREF(c1);
  TK_OBJECT_UNREF(c2);
  TK_OBJECT_UNREF(c3);
}

/* 引用计数生命周期：
 *   - EXEC 后不 undo 直接销毁：on_destroy 释放 undo_stack 与 cmds 的引用 -> ref_count 回到 1。
 *   - UNDO 弹出 cmd 后释放栈中引用 -> ref_count 降 1。 */
TEST(ObjectWorkflowCmd, UndoStackRefcountLifecycle) {
  /* EXEC 后不 UNDO 直接销毁 */
  {
    bool_t destroyed = FALSE;
    tk_object_t* wfc = object_workflow_cmd_create();
    tk_object_t* c1 = fake_cmd_create("c1", NULL, &destroyed);
    ASSERT_EQ(tk_object_set_prop_object(wfc, "c1", c1), RET_OK);
    /* create(1) + cmds 哈希(1) = 2 */
    ASSERT_EQ(c1->ref_count, 2);
    ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), RET_OK);
    /* cmd_on_execed 入栈并 tk_object_ref -> 3 */
    ASSERT_EQ(c1->ref_count, 3);
    /* 销毁（不 undo）：darray_deinit 释放栈引用 + cmds 释放哈希引用 -> 1 */
    TK_OBJECT_UNREF(wfc);
    ASSERT_EQ(c1->ref_count, 1);
    ASSERT_EQ(destroyed, FALSE);
    TK_OBJECT_UNREF(c1);
    ASSERT_EQ(destroyed, TRUE);
  }
  /* UNDO 弹出 cmd 后释放栈引用 */
  {
    bool_t destroyed = FALSE;
    tk_object_t* wfc = object_workflow_cmd_create();
    tk_object_t* c1 = fake_cmd_create("c1", NULL, &destroyed);
    ASSERT_EQ(tk_object_set_prop_object(wfc, "c1", c1), RET_OK);
    ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), RET_OK);
    /* create(1) + cmds(1) + undo_stack(1) = 3 */
    ASSERT_EQ(c1->ref_count, 3);
    ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), RET_OK);
    /* 弹出并释放栈引用 -> 2（create + cmds） */
    ASSERT_EQ(c1->ref_count, 2);
    TK_OBJECT_UNREF(wfc);
    /* cmds 释放 -> 1（仅剩测试自持） */
    ASSERT_EQ(c1->ref_count, 1);
    ASSERT_EQ(destroyed, FALSE);
    TK_OBJECT_UNREF(c1);
    ASSERT_EQ(destroyed, TRUE);
  }
}

/* 重复 EXEC 先清空旧栈引用再重新入栈 -> ref_count 不升 */
TEST(ObjectWorkflowCmd, ExecClearsPriorUndoStack) {
  bool_t destroyed = FALSE;
  tk_object_t* wfc = object_workflow_cmd_create();
  tk_object_t* c1 = fake_cmd_create("c1", NULL, &destroyed);
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c1", c1), RET_OK);

  ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), RET_OK);
  ASSERT_EQ(c1->ref_count, 3);
  ASSERT_EQ(FAKE_CMD(c1)->exec_count, 1);

  /* can_exec(EXEC)=FALSE，但直接 exec 仍会先清空旧栈再重跑 */
  ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), RET_OK);
  /* 旧栈引用已释放，重新入栈再加一次 -> 仍为 3 */
  ASSERT_EQ(c1->ref_count, 3);
  ASSERT_EQ(FAKE_CMD(c1)->exec_count, 2);

  ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), RET_OK);
  ASSERT_EQ(c1->ref_count, 2);

  TK_OBJECT_UNREF(wfc);
  ASSERT_EQ(c1->ref_count, 1);
  ASSERT_EQ(destroyed, FALSE);
  TK_OBJECT_UNREF(c1);
  ASSERT_EQ(destroyed, TRUE);
}

/* 设备设置页"应用"：theme/lang 已变更 -> 执行并入栈；network 未变更(can_exec=FALSE)
 * -> 自定义工作流跳过、不入栈。UNDO 按实际执行序 LIFO 回滚，仅撤销 theme/lang。
 * ctx 为访问计数器，兼验 set_runner 的 ctx 透传。 */
TEST(ObjectWorkflowCmd, CustomWorkflowSkipsUnreadyCmds) {
  vector<string> log;
  int visited = 0;
  tk_object_t* wfc = object_workflow_cmd_create();
  tk_object_t* c1 = fake_cmd_create("theme", &log);
  tk_object_t* c2 = fake_cmd_create("lang", &log);
  tk_object_t* c3 = fake_cmd_create("network", &log);
  FAKE_CMD(c3)->can_exec_false = TRUE; /* network 未变更 -> can_exec 拒绝 */
  ASSERT_EQ(tk_object_set_prop_object(wfc, "theme", c1), RET_OK);
  ASSERT_EQ(tk_object_set_prop_object(wfc, "lang", c2), RET_OK);
  ASSERT_EQ(tk_object_set_prop_object(wfc, "network", c3), RET_OK);
  ASSERT_EQ(object_workflow_cmd_set_runner(wfc, skip_unready_workflow, &visited), RET_OK);

  ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), RET_OK);
  /* ctx 透传：3 个 cmd 都被 on_visit 访问过 */
  ASSERT_EQ(visited, 3);
  /* theme/lang 执行并入栈；network 被跳过 */
  ASSERT_EQ(FAKE_CMD(c1)->exec_count, 1);
  ASSERT_EQ(FAKE_CMD(c2)->exec_count, 1);
  ASSERT_EQ(FAKE_CMD(c3)->exec_count, 0);
  /* 栈含 theme/lang -> 可 UNDO、不可 EXEC */
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), TRUE);
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), FALSE);

  ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), RET_OK);
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
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), TRUE);
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), FALSE);

  TK_OBJECT_UNREF(wfc);
  TK_OBJECT_UNREF(c1);
  TK_OBJECT_UNREF(c2);
  TK_OBJECT_UNREF(c3);
}

/* 执行失败自动回滚：c2 中段失败 -> 仅回滚已入栈的 c1，c2/c3 不受影响 */
TEST(ObjectWorkflowCmd, FailedExecAutoRollback) {
  vector<string> log;
  tk_object_t* wfc = object_workflow_cmd_create();
  tk_object_t* c1 = fake_cmd_create("c1", &log);
  tk_object_t* c2 = fake_cmd_create("c2", &log);
  tk_object_t* c3 = fake_cmd_create("c3", &log);
  FAKE_CMD(c2)->exec_fail = TRUE; /* c2 的 exec 返回 RET_FAIL，触发自动回滚 */
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c1", c1), RET_OK);
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c2", c2), RET_OK);
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c3", c3), RET_OK);

  /* 默认工作流遇非 OK 中止：c1 成功、c2 失败、c3 未执行；EXEC 返回非 OK */
  ASSERT_NE(tk_object_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), RET_OK);

  ASSERT_EQ(FAKE_CMD(c1)->exec_count, 1);
  ASSERT_EQ(FAKE_CMD(c2)->exec_count, 1);
  ASSERT_EQ(FAKE_CMD(c3)->exec_count, 0); /* c2 失败后工作流中止，c3 未执行 */

  /* 自动回滚：仅回滚已入栈的 c1（LIFO）；c2 失败未入栈故不被回滚 */
  ASSERT_EQ(FAKE_CMD(c1)->undo_count, 1);
  ASSERT_EQ(FAKE_CMD(c2)->undo_count, 0);
  ASSERT_EQ(FAKE_CMD(c3)->undo_count, 0);

  /* 回滚清空 undo_stack -> 回到空闲态：可 EXEC、不可 UNDO */
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), TRUE);
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), FALSE);

  string actual;
  for (size_t i = 0; i < log.size(); i++) {
    actual += log[i] + ",";
  }
  ASSERT_EQ(actual, string("c1:exec,c2:exec,c1:undo,"));

  TK_OBJECT_UNREF(wfc);
  TK_OBJECT_UNREF(c1);
  TK_OBJECT_UNREF(c2);
  TK_OBJECT_UNREF(c3);
}

/* 回滚失败在返回码中不可见：c1 施加后 undo 失败，c1 留在已施加未回滚状态。
 * EXEC 返回值与干净回滚相同，残余状态仅由 can_exec 暴露：可 UNDO、不可 EXEC。 */
TEST(ObjectWorkflowCmd, FailedRollbackInvisibleInReturnCode) {
  vector<string> log;
  tk_object_t* wfc = object_workflow_cmd_create();
  tk_object_t* c1 = fake_cmd_create("c1", &log);
  tk_object_t* c2 = fake_cmd_create("c2", &log);
  FAKE_CMD(c1)->undo_fail = TRUE; /* c1 的 undo 返回 RET_FAIL */
  FAKE_CMD(c2)->exec_fail = TRUE; /* c2 的 exec 失败 -> 触发自动回滚 */
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c1", c1), RET_OK);
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c2", c2), RET_OK);

  /* EXEC 返回 RET_FAIL，与干净回滚的返回值相同 */
  ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), RET_FAIL);

  ASSERT_EQ(FAKE_CMD(c1)->exec_count, 1); /* c1 已施加 */
  ASSERT_EQ(FAKE_CMD(c1)->undo_count, 1); /* c1 的 undo 尝试失败 */
  ASSERT_EQ(FAKE_CMD(c2)->exec_count, 1); /* c2 失败触发回滚 */
  ASSERT_EQ(FAKE_CMD(c2)->undo_count, 0);

  /* 残余状态：回滚失败后 c1 仍 applied -> 可 UNDO、不可 EXEC（干净回滚则相反） */
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), FALSE);
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), TRUE);

  TK_OBJECT_UNREF(wfc);
  TK_OBJECT_UNREF(c1);
  TK_OBJECT_UNREF(c2);
}

/* cmd_on_execed 只记录 EXEC 事件：工作流在 EXEC 阶段对每个 cmd 触发一次 UNDO，
 * 这些 UNDO 事件不得被记入 undo_stack。 */
TEST(ObjectWorkflowCmd, OnlyExecEventsRecordedForUndo) {
  tk_object_t* wfc = object_workflow_cmd_create();
  tk_object_t* c1 = fake_cmd_create("c1");
  tk_object_t* c2 = fake_cmd_create("c2");
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c1", c1), RET_OK);
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c2", c2), RET_OK);
  ASSERT_EQ(object_workflow_cmd_set_runner(wfc, undo_triggering_workflow, NULL), RET_OK);

  /* 工作流对每个 cmd 执行一次 UNDO（不执行 EXEC） */
  ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), RET_OK);

  ASSERT_EQ(FAKE_CMD(c1)->undo_count, 1);
  ASSERT_EQ(FAKE_CMD(c2)->undo_count, 1);
  ASSERT_EQ(FAKE_CMD(c1)->exec_count, 0);
  ASSERT_EQ(FAKE_CMD(c2)->exec_count, 0);

  /* UNDO 事件未被记录 -> undo_stack 为空 -> 不可 UNDO、可 EXEC */
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), FALSE);
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), TRUE);

  TK_OBJECT_UNREF(wfc);
  TK_OBJECT_UNREF(c1);
  TK_OBJECT_UNREF(c2);
}

/* 自动回滚窗口内的重入：c1 的 undo 补偿性重执 c3（已注册、因 c2 失败未执行）一次。
 * 重入的 EXEC 被记入仍注册的 undo_stack，c1 仍须只被撤销一次。 */
TEST(ObjectWorkflowCmd, AutoRollbackReentryPopsWrongCmd) {
  vector<string> log;
  tk_object_t* wfc = object_workflow_cmd_create();
  tk_object_t* c1 = fake_cmd_create("c1", &log);
  tk_object_t* c2 = fake_cmd_create("c2", &log);
  tk_object_t* c3 = fake_cmd_create("c3", &log);
  FAKE_CMD(c1)->undo_reexec_target = c3; /* c1 的 undo 补偿性重执 c3 一次 */
  FAKE_CMD(c2)->exec_fail = TRUE;        /* c2 失败 -> 触发自动回滚 */
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c1", c1), RET_OK);
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c2", c2), RET_OK);
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c3", c3), RET_OK);

  ASSERT_NE(tk_object_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), RET_OK);

  /* 核心不变量：c1 必须恰好被撤销 1 次。 */
  EXPECT_EQ(FAKE_CMD(c1)->undo_count, 1) << "原命令 c1 被重复撤销";
  /* 确认补偿性重执已发生：c3 被重执一次 */
  EXPECT_EQ(FAKE_CMD(c3)->exec_count, 1);

  /* 回滚后栈被清空 -> 回到空闲态 */
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), TRUE);
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), FALSE);

  TK_OBJECT_UNREF(wfc);
  TK_OBJECT_UNREF(c1);
  TK_OBJECT_UNREF(c2);
  TK_OBJECT_UNREF(c3);
}

/* [已删] 重入 EXEC 的 busy 拦截不变量已由 ExecDuringUndoCrossOpHoleDoubleUndoes 覆盖，不再单列用例。 */

/* args 透传：workflow 的 EXEC args 应经默认工作流透传到每个 cmd 的 exec。 */
TEST(ObjectWorkflowCmd, ArgsForwardedToCmds) {
  tk_object_t* wfc = object_workflow_cmd_create();
  tk_object_t* c1 = fake_cmd_create("c1");
  tk_object_t* c2 = fake_cmd_create("c2");
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c1", c1), RET_OK);
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c2", c2), RET_OK);

  /* 默认工作流应把 workflow 的 args 透传给 cmd 的 exec */
  ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_EXEC, "hello"), RET_OK);

  ASSERT_TRUE(FAKE_CMD(c1)->last_args != NULL) << "args 未透传给 c1";
  ASSERT_TRUE(FAKE_CMD(c2)->last_args != NULL) << "args 未透传给 c2";
  EXPECT_STREQ(FAKE_CMD(c1)->last_args, "hello");
  EXPECT_STREQ(FAKE_CMD(c2)->last_args, "hello");

  TK_OBJECT_UNREF(wfc);
  TK_OBJECT_UNREF(c1);
  TK_OBJECT_UNREF(c2);
}

/* 工作流驱动的 drain：EXEC 期间工作流重入 UNDO，验证 c1 至多被撤销 1 次。 */
TEST(ObjectWorkflowCmd, WorkflowDrivenUndoDuringExecMispops) {
  vector<string> log;
  tk_object_t* wfc = object_workflow_cmd_create();
  tk_object_t* c1 = fake_cmd_create("c1", &log);
  tk_object_t* c3 = fake_cmd_create("c3", &log);
  FAKE_CMD(c1)->undo_reexec_target = c3; /* c1 的 undo 补偿性重执 c3 一次 */
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c1", c1), RET_OK);
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c3", c3), RET_OK);
  /* 自定义工作流：exec c1 后立即重入 wfc UNDO（ctx=wfc 自指）*/
  ASSERT_EQ(object_workflow_cmd_set_runner(wfc, undo_mid_batch_workflow, wfc), RET_OK);

  ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), RET_OK);

  /* 核心不变量：c1 至多被撤销 1 次（不得双重撤销）。 */
  EXPECT_LE(FAKE_CMD(c1)->undo_count, 1) << "工作流驱动 drain 期间 c1 被双重撤销";

  TK_OBJECT_UNREF(wfc);
  TK_OBJECT_UNREF(c1);
  TK_OBJECT_UNREF(c3);
}

/* cmd 的 undo 重入 wfc UNDO：验证 c2 至多被撤销 1 次（内层 drain 不得重复撤销）。 */
TEST(ObjectWorkflowCmd, CmdUndoReentersWfcUndoDoubleUndoes) {
  vector<string> log;
  tk_object_t* wfc = object_workflow_cmd_create();
  tk_object_t* c1 = fake_cmd_create("c1", &log);
  tk_object_t* c2 = fake_cmd_create("c2", &log);
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c1", c1), RET_OK);
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c2", c2), RET_OK);
  ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), RET_OK); /* 栈=[c1,c2] */
  /* c2 的 undo 重入 wfc UNDO 一次 */
  FAKE_CMD(c2)->undo_reenter_wfc = wfc;

  ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), RET_OK);

  /* 核心不变量：c2 至多被撤销 1 次（不得双重撤销）。 */
  EXPECT_LE(FAKE_CMD(c2)->undo_count, 1) << "cmd undo 重入 wfc UNDO 致 c2 双重撤销";
  EXPECT_EQ(FAKE_CMD(c1)->undo_count, 1);

  TK_OBJECT_UNREF(wfc);
  TK_OBJECT_UNREF(c1);
  TK_OBJECT_UNREF(c2);
}

/* EXEC-during-UNDO 重入：c1 的 undo 重入 wfc EXEC，验证 c1 至多撤销 1 次、整批不重复执行。 */
TEST(ObjectWorkflowCmd, ExecDuringUndoCrossOpHoleDoubleUndoes) {
  vector<string> log;
  tk_object_t* wfc = object_workflow_cmd_create();
  tk_object_t* c1 = fake_cmd_create("c1", &log);
  tk_object_t* c2 = fake_cmd_create("c2", &log);
  FAKE_CMD(c1)->undo_reexec_wfc_exec = wfc; /* c1 的 undo 重入 wfc EXEC 一次 */
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c1", c1), RET_OK);
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c2", c2), RET_OK);
  ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), RET_OK); /* 栈=[c1,c2] */

  ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), RET_OK);

  /* 核心不变量：c1 至多被撤销 1 次。 */
  EXPECT_LE(FAKE_CMD(c1)->undo_count, 1) << "EXEC-during-UNDO 期间 c1 被双重撤销";
  EXPECT_EQ(FAKE_CMD(c1)->exec_count, 1);
  EXPECT_EQ(FAKE_CMD(c2)->exec_count, 1);
  EXPECT_EQ(FAKE_CMD(c2)->undo_count, 1);

  TK_OBJECT_UNREF(wfc);
  TK_OBJECT_UNREF(c1);
  TK_OBJECT_UNREF(c2);
}

/* 自定义工作流执行期间动态创建/加入/执行指令，验证新委托设计的撤销簿记：
 * 工作流对 obj 调用 tk_object_set_prop_object 经 cmds_set_prop override 自动订阅 EXECED
 * 观察者 -> 现场创建的指令执行后其 EXEC 事件被记入 undo_stack -> UNDO 可回滚。
 * 兼证便利性：无需 get_cmds，对 workflow 直接增指令即获一致的撤销簿记。 */
TEST(ObjectWorkflowCmd, DynamicCmdAddedDuringWorkflowIsRecordedForUndo) {
  vector<string> log;
  dynamic_cmd_ctx_t info = {NULL, &log};
  tk_object_t* wfc = object_workflow_cmd_create();
  ASSERT_TRUE(wfc != NULL);
  ASSERT_EQ(object_workflow_cmd_set_runner(wfc, dynamic_cmd_workflow, &info), RET_OK);

  /* 初始无指令：可 EXEC、不可 UNDO */
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), TRUE);
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), FALSE);

  /* EXEC：工作流现场创建 dynamic、加入 cmds、执行之 -> 记入 undo_stack */
  ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), RET_OK);

  tk_object_t* dyn = info.cmd;
  ASSERT_TRUE(dyn != NULL);
  /* 指令确已执行 */
  ASSERT_EQ(FAKE_CMD(dyn)->exec_count, 1);
  /* 创建引用(1) + cmds 哈希(1) + undo_stack(1) = 3 */
  ASSERT_EQ(dyn->ref_count, 3);
  /* workflow 委托 #size 到 cmds -> 已注册 1 条指令 */
  ASSERT_EQ(tk_object_get_prop_uint32(wfc, TK_OBJECT_PROP_SIZE, 0), 1u);
  /* 已记入 undo_stack -> 可 UNDO、不可 EXEC */
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), FALSE);
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), TRUE);

  /* UNDO：弹出 dynamic 并撤销 */
  ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), RET_OK);
  ASSERT_EQ(FAKE_CMD(dyn)->undo_count, 1);
  /* 弹出释放栈引用 -> 创建(1) + cmds(1) = 2 */
  ASSERT_EQ(dyn->ref_count, 2);
  /* 回到空闲态：可 EXEC、不可 UNDO（cmds 仍保留 dynamic 注册） */
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), TRUE);
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), FALSE);

  /* 日志：先 exec 后 undo */
  string actual;
  for (size_t i = 0; i < log.size(); i++) {
    actual += log[i] + ",";
  }
  ASSERT_EQ(actual, string("dynamic:exec,dynamic:undo,"));

  /* 释放：先 wfc（cmds 释放 dyn 引用 -> 1），再 dyn 的创建引用 -> 销毁 */
  TK_OBJECT_UNREF(wfc);
  ASSERT_EQ(dyn->ref_count, 1);
  TK_OBJECT_UNREF(dyn);
}

/* copy_props 须经 cmds_set_prop override 订阅 EXECED 观察者（修复旁路回归）：
 * 修复前 copy_props 直写 cmds_base 跳过 cmds_set_prop -> 复制的命令 EXEC 不入 undo_stack。
 * 锁定：copy_props 加入的命令 EXEC 后被记入 undo_stack，UNDO 可回滚。 */
TEST(ObjectWorkflowCmd, CopyPropsCmdRecordedForUndo) {
  tk_object_t* wfc = object_workflow_cmd_create();
  tk_object_t* src = object_hash_create();
  tk_object_t* c1 = fake_cmd_create("c1");
  ASSERT_EQ(tk_object_set_prop_object(src, "c1", c1), RET_OK);

  ASSERT_EQ(tk_object_copy_props(wfc, src, TRUE), RET_OK);
  ASSERT_EQ(tk_object_get_prop_uint32(wfc, TK_OBJECT_PROP_SIZE, 0), 1u);

  /* EXEC -> 观察者记录入栈（修复前：未订 -> can_exec(UNDO)=FALSE、ref 不增） */
  ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), RET_OK);
  ASSERT_EQ(FAKE_CMD(c1)->exec_count, 1);
  ASSERT_EQ(c1->ref_count, 4); /* create + src + cmds + undo_stack */
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), TRUE);

  /* UNDO -> 弹出并撤销（修复前：栈空 -> undo_count=0） */
  ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), RET_OK);
  ASSERT_EQ(FAKE_CMD(c1)->undo_count, 1);

  TK_OBJECT_UNREF(wfc);
  ASSERT_EQ(c1->ref_count, 2);
  TK_OBJECT_UNREF(src);
  ASSERT_EQ(c1->ref_count, 1);
  TK_OBJECT_UNREF(c1);
}

/* object_hash_clone 的 extra_data 处理回归（#4 修复 + L312 已修复）。
 * 带 extra_data 的 object_hash 克隆后：
 *   (a) 克隆的 extra_data_size 应保留（#4：object_hash_clone 现传入 extra_data_size）；
 *   (b) 源的 extra_data 内容不得被克隆覆盖。
 * 注：get_cmds 已从公开 API 移除（workflow 自身委托 prop 操作到 cmds），clone-cmds 路径经
 * workflow 不可达，故此回归直接在 object_hash 层验证（workflow 即其 extra_data 消费者）。
 *
 * 历史：object_hash_clone 曾把 memcpy 方向写反（`memcpy(o+1,dupo+1,...)`，dest=源 src=克隆），
 * 把克隆清零的 extra_data 覆盖回源，破坏源反向指针；已修复为 `memcpy(dupo+1,o+1,...)`
 * （拷源到克隆，源不动）。本用例锁定该不变量：克隆后源的 extra_data 哨兵保持不变。 */
TEST(ObjectWorkflowCmd, ObjectHashCloneExtraDataNotCorrupted) {
  const uint32_t kExtra = sizeof(void*);
  object_hash_create_opt_t opt = {}; /* C++ 聚合零初始化（POD + 位域） */
  opt.enable_path = FALSE;
  opt.keep_props_order = TRUE;
  opt.extra_data_size = kExtra;
  tk_object_t* hash = object_hash_create_with_opt(&opt);
  ASSERT_TRUE(hash != NULL);
  ASSERT_EQ(OBJECT_HASH(hash)->extra_data_size, kExtra);

  /* 在源的 extra_data 槽位写入哨兵（模拟 workflow 反向指针），取实变量地址避免 int→ptr 转换警告 */
  int sentinel_storage = 0;
  void* sentinel = &sentinel_storage;
  void** slot = (void**)((uint8_t*)hash + hash->vt->size);
  *slot = sentinel;

  /* 克隆：object_hash_clone（override 无 .clone → tk_object_clone(base_obj=hash)） */
  tk_object_t* clone = tk_object_clone(hash);
  ASSERT_TRUE(clone != NULL);
  ASSERT_EQ(OBJECT_HASH(clone)->extra_data_size, kExtra); /* #4：槽位大小保留 */

  /* L312 回归：源的 extra_data 不得被克隆覆盖（克隆把源的 extra_data 拷给克隆，源应不动） */
  EXPECT_EQ(*slot, sentinel)
      << "object_hash_clone 的 memcpy 方向若写反会把克隆清零的 extra_data 覆盖回源";

  TK_OBJECT_UNREF(clone);
  TK_OBJECT_UNREF(hash);
}

/* clone 语义锁定：object_workflow_cmd_clone（src/tkc/object_workflow_cmd.c:245）
 *   (1) 子命令按 ref 共享（非深 clone）：两端 get_prop_object 同指针；
 *       fake_cmd 无 clone 槽——若深 clone 会返回 NULL、prop 丢失，反证不深拷。
 *   (2) undo_stack 独立：原 EXEC 不使 clone 可 UNDO（未继承历史）。
 *   (3) EVT_CMD_EXECED 订阅以新对象为 ctx 重建：clone 自身 EXEC 后 c1 的
 *       EXECED 回调入 clone 栈（can_exec UNDO→TRUE），不误入原对象栈。
 *   (4) clone EXEC 不波及原对象 undo 栈：各自 UNDO 各撤销一次。
 *   (5) 新对象自身 emitter 为空：原对象上的外部观察者不被偷渡到克隆体。 */
static ret_t clone_test_external_observer(void* ctx, event_t* e) {
  (void)e;
  (*(int*)ctx)++;
  return RET_OK;
}

TEST(ObjectWorkflowCmd, CloneSharesCmdsRebuildsSubscriptionIndependentUndoStack) {
  vector<string> log;
  tk_object_t* wfc = object_workflow_cmd_create();
  tk_object_t* c1 = fake_cmd_create("c1", &log);
  ASSERT_TRUE(wfc != NULL && c1 != NULL);
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c1", c1), RET_OK);

  /* 克隆（fake_cmd 无 clone 槽：反证 workflow clone 不深拷子命令） */
  tk_object_t* clone = tk_object_clone(wfc);
  ASSERT_TRUE(clone != NULL);

  /* (1) 子命令按 ref 共享：两端取到同一指针（非深 clone） */
  ASSERT_EQ(tk_object_get_prop_object(clone, "c1"), c1);

  /* (5) 外部观察契约不偷渡：wfc 上挂观察者，后续对 clone EXEC 不触发它 */
  int ext_count = 0;
  emitter_on(EMITTER(wfc), EVT_CMD_EXECED, clone_test_external_observer, &ext_count);

  /* (2) undo_stack 独立：原 EXEC 后 wfc 可 UNDO，clone 仍不可 */
  ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), RET_OK);
  ASSERT_EQ(tk_object_can_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), TRUE);
  ASSERT_EQ(tk_object_can_exec(clone, TK_OBJECT_CMD_UNDO, NULL), FALSE);
  ASSERT_EQ(ext_count, 1); /* wfc 被 EXEC → 触发其上的外部观察者 */

  /* (3) 订阅以新对象为 ctx 重建：clone 自身 EXEC 后 c1 EXECED 入 clone 栈，
   *     且只 emit 到 clone 的空 emitter，不触发 wfc 上的外部观察者。 */
  ASSERT_EQ(tk_object_exec(clone, TK_OBJECT_CMD_EXEC, NULL), RET_OK);
  ASSERT_EQ(tk_object_can_exec(clone, TK_OBJECT_CMD_UNDO, NULL), TRUE);
  ASSERT_EQ(ext_count, 1);                /* clone EXECED 未偷渡到 wfc 的外部观察者 */
  ASSERT_EQ(FAKE_CMD(c1)->exec_count, 2); /* wfc 1 + clone 1 */

  /* (4) 各自 UNDO 一次，互不波及：c1 共被撤销两次 */
  ASSERT_EQ(tk_object_exec(clone, TK_OBJECT_CMD_UNDO, NULL), RET_OK);
  ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), RET_OK);
  ASSERT_EQ(FAKE_CMD(c1)->undo_count, 2);

  TK_OBJECT_UNREF(clone);
  TK_OBJECT_UNREF(wfc);
  TK_OBJECT_UNREF(c1);
}

/* ===== 已知残余（框架级，非 object_workflow_cmd.c 可修）=====
 * EVT_CMD_EXECED/WILL_EXEC 观察者重入 EXEC 会绕过 busy 守卫（派发点归 object.c）。
 * 下方 DISABLED 用例锁定该不变量，待 object.c 增加守卫后启用即可通过。 */
typedef struct _reentry_probe_t {
  tk_object_t* wfc;
  bool_t done;
} reentry_probe_t;

static ret_t execed_probe_handler(void* ctx, event_t* e) {
  reentry_probe_t* p = (reentry_probe_t*)ctx;
  if (!p->done) {
    p->done = TRUE;
    (void)tk_object_exec(p->wfc, TK_OBJECT_CMD_EXEC, NULL);
  }
  return RET_OK;
}

/* DISABLED：框架级残余（见上方注释）。期望 EXECED 观察者重入 EXEC 被拒绝，
 * 整批 exec/undo 各 1 次；待 object.c 增加守卫后移除前缀即可通过。 */
TEST(ObjectWorkflowCmd, DISABLED_ExternalExecedObserverReentryMustNotDoubleExec) {
  vector<string> log;
  tk_object_t* wfc = object_workflow_cmd_create();
  tk_object_t* c1 = fake_cmd_create("c1", &log);
  tk_object_t* c2 = fake_cmd_create("c2", &log);
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c1", c1), RET_OK);
  ASSERT_EQ(tk_object_set_prop_object(wfc, "c2", c2), RET_OK);

  reentry_probe_t p = {wfc, FALSE};
  uint32_t id = emitter_on(EMITTER(wfc), EVT_CMD_EXECED, execed_probe_handler, &p);

  ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_EXEC, NULL), RET_OK);
  emitter_off(EMITTER(wfc), id);

  ASSERT_EQ(tk_object_exec(wfc, TK_OBJECT_CMD_UNDO, NULL), RET_OK);

  /* 期望：整批仅被执行一次、撤销一次，无孤儿施加。 */
  EXPECT_EQ(FAKE_CMD(c1)->exec_count, 1);
  EXPECT_EQ(FAKE_CMD(c1)->undo_count, 1);
  EXPECT_EQ(FAKE_CMD(c2)->exec_count, 1);
  EXPECT_EQ(FAKE_CMD(c2)->undo_count, 1);

  TK_OBJECT_UNREF(wfc);
  TK_OBJECT_UNREF(c1);
  TK_OBJECT_UNREF(c2);
}
