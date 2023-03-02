#include "code_assist.h"
#include "tkc/log.h"

BEGIN_C_DECLS

typedef void (*log_hook_t)(void* ctx, log_level_t level, const char* s);
bool_t check_api_doc(code_assist_t* ca, const char* filename, log_hook_t hook, void* log_ctx, bool_t auto_fix);

/* 方便自动测试 */
typedef void (*auto_fix_hook_t)(const char* path, const char* buff, uint32_t size);
bool_t check_api_doc2(code_assist_t* ca, const char* filename, log_hook_t hook, void* log_ctx, bool_t auto_fix, auto_fix_hook_t fix_hook);

END_C_DECLS