/**
 * File:   cond_var.c
 * Author: AWTK Develop Team
 * Brief:  cond_var
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2019-05-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/cond_var.h"

#ifdef WIN32

#ifdef _MSC_VER

#include <synchapi.h>

#elif defined(__GNUC__)

#include <winbase.h>

typedef PVOID SRWLOCK, *PSRWLOCK;

void WINAPI InitializeSRWLock(PSRWLOCK);
void WINAPI AcquireSRWLockExclusive(PSRWLOCK);
void WINAPI AcquireSRWLockShared(PSRWLOCK);
void WINAPI ReleaseSRWLockExclusive(PSRWLOCK);
void WINAPI ReleaseSRWLockShared(PSRWLOCK);

typedef PVOID CONDITION_VARIABLE, *PCONDITION_VARIABLE;

void WINAPI InitializeConditionVariable(PCONDITION_VARIABLE);
BOOL WINAPI SleepConditionVariableCS(PCONDITION_VARIABLE, PCRITICAL_SECTION, DWORD);
BOOL WINAPI SleepConditionVariableSRW(PCONDITION_VARIABLE, PSRWLOCK, DWORD, ULONG);
void WINAPI WakeAllConditionVariable(PCONDITION_VARIABLE);
void WINAPI WakeConditionVariable(PCONDITION_VARIABLE);

#else
#error not found Condition Variable
#endif

struct _tk_cond_var_t {
  bool_t inited;
  bool_t has_signal;
  CRITICAL_SECTION mutex;
  CONDITION_VARIABLE cond;
};

tk_cond_var_t* tk_cond_var_create(void) {
  tk_cond_var_t* cond_var = TKMEM_ZALLOC(tk_cond_var_t);
  return_value_if_fail(cond_var != NULL, NULL);

  cond_var->inited = TRUE;
  InitializeCriticalSection(&(cond_var->mutex));
  InitializeConditionVariable(&(cond_var->cond));

  return cond_var;
}

ret_t tk_cond_var_wait(tk_cond_var_t* cond_var, uint32_t timeout_ms) {
  return_value_if_fail(cond_var != NULL && cond_var->inited, RET_BAD_PARAMS);

  EnterCriticalSection(&(cond_var->mutex));
  while (!cond_var->has_signal) {
    SleepConditionVariableCS(&(cond_var->cond), &(cond_var->mutex), timeout_ms);
  }
  cond_var->has_signal = FALSE;
  LeaveCriticalSection(&(cond_var->mutex));

  return RET_OK;
}

ret_t tk_cond_var_awake(tk_cond_var_t* cond_var) {
  return_value_if_fail(cond_var != NULL && cond_var->inited, RET_BAD_PARAMS);

  EnterCriticalSection(&(cond_var->mutex));
  cond_var->has_signal = TRUE;
  LeaveCriticalSection(&(cond_var->mutex));
  WakeConditionVariable(&(cond_var->cond));

  return RET_OK;
}

ret_t tk_cond_var_destroy(tk_cond_var_t* cond_var) {
  return_value_if_fail(cond_var != NULL && cond_var->inited, RET_BAD_PARAMS);

  memset(cond_var, 0x00, sizeof(tk_cond_var_t));

  TKMEM_FREE(cond_var);

  return RET_OK;
}

#else
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif /*_POSIX_C_SOURCE*/

#include <pthread.h>
#include <sys/time.h>

struct _tk_cond_var_t {
  bool_t inited;
  bool_t has_signal;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
};

tk_cond_var_t* tk_cond_var_create(void) {
  tk_cond_var_t* cond_var = TKMEM_ZALLOC(tk_cond_var_t);
  return_value_if_fail(cond_var != NULL, NULL);

  cond_var->inited = TRUE;
  pthread_mutex_init(&(cond_var->mutex), NULL);
  pthread_cond_init(&(cond_var->cond), NULL);

  return cond_var;
}

ret_t tk_cond_var_wait(tk_cond_var_t* cond_var, uint32_t timeout_ms) {
  return_value_if_fail(cond_var != NULL && cond_var->inited, RET_BAD_PARAMS);

  pthread_mutex_lock(&(cond_var->mutex));
  while (!cond_var->has_signal) {
    struct timespec ts;
    struct timeval now;

    gettimeofday(&now, NULL);
    ts.tv_sec = now.tv_sec + timeout_ms / 1000;
    ts.tv_nsec = now.tv_usec * 1000 + timeout_ms % 1000;

    pthread_cond_timedwait(&(cond_var->cond), &(cond_var->mutex), &ts);
  }
  cond_var->has_signal = FALSE;
  pthread_mutex_unlock(&(cond_var->mutex));

  return RET_OK;
}

ret_t tk_cond_var_awake(tk_cond_var_t* cond_var) {
  return_value_if_fail(cond_var != NULL && cond_var->inited, RET_BAD_PARAMS);

  pthread_mutex_lock(&(cond_var->mutex));
  cond_var->has_signal = TRUE;
  pthread_mutex_unlock(&(cond_var->mutex));
  pthread_cond_signal(&(cond_var->cond));

  return RET_OK;
}

ret_t tk_cond_var_destroy(tk_cond_var_t* cond_var) {
  return_value_if_fail(cond_var != NULL && cond_var->inited, RET_BAD_PARAMS);

  pthread_cond_destroy(&cond_var->cond);
  pthread_mutex_destroy(&(cond_var->mutex));
  memset(cond_var, 0x00, sizeof(tk_cond_var_t));

  TKMEM_FREE(cond_var);

  return RET_OK;
}
#endif /*PTHREAD*/
