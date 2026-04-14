/**
 * Copyright 2025-2026 Kirill Makurin
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#include "config-internal.h"
#endif

#include <assert.h>
#include <pthread.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "core-runtime.h"

#undef p32_terminate

/**
 * File Summary:
 *
 * This file defines various internal runtime-related functions.
 */

#if P32_IS_ARCH(P32_ARCH_X64)
#define P32_CONTEXT_PC(context) (context).Rip
#elif P32_IS_ARCH(P32_ARCH_X86)
#define P32_CONTEXT_PC(context) (context).Eip
#else
#define P32_CONTEXT_PC(context) (context).Pc
#endif

/*******************************************************************************
 * Structures, functions and macros used to lookup functions at runtime.
 */

/**
 * Windows 9x systems lack Unicode support; use `GetModuleHandleA` instead of
 * `GetModuleHandleW`.
 */
#if P32_WIN9X
#define P32GetModuleHandle(module) GetModuleHandleA (module)
#else
#define P32GetModuleHandle(module) GetModuleHandleW (TEXT (module))
#endif

/**
 * Convenience wrapper for `GetProcAddress`.
 */
#define P32GetProcAddress(module, func) (Func##func) (UINT_PTR) GetProcAddress (module, #func)

/**
 * Suppress warnings about conversion between data and function pointers with
 * picky compilers.
 */
#define F(ptr) (PVOID) (UINT_PTR) ptr

/**
 * Convenience wrapper for `InterlockedExchangePointer`.
 */
#define P32AtomicExchange(target, source) InterlockedExchangePointer ((void *volatile *) target, F (source))

#if P32_WINNT < P32_WINNT_WIN7
#define DYNAMIC_CHECKS

/**
 * Function type corresponding to `RaiseFailFastException`.
 */
typedef VOID (WINAPI *FuncRaiseFailFastException) (PEXCEPTION_RECORD, PCONTEXT, DWORD);
#endif /* P32_WINNT < Windows 7 */

#ifdef DYNAMIC_CHECKS
/**
 * Pointers to Windows APIs which are looked up at runtime.
 */
typedef struct RtApi {
  pthread_once_t Init;

  /**
   * Function `RaiseFailFastException` is available since Windows 7.
   */
#if P32_WINNT < P32_WINNT_WIN7
  FuncRaiseFailFastException PtrRaiseFailFastException;
#endif /* P32_WINNT < Windows 7 */
} RtApi;

/**
 * Windows APIs.
 */
static RtApi P32RtApi = {
  .Init = PTHREAD_ONCE_INIT,

#if P32_WINNT < P32_WINNT_WIN7
  .PtrRaiseFailFastException = NULL,
#endif /* P32_WINNT < Windows 7 */
};

/**
 * Initialize `P32RtApi`.
 *
 * NOTE: we cannot call functions `p32_platform`, `p32_winnt` and `p32_win9x`;
 *   they may call `p32_terminate` on error, which will result in a dead lock.
 */
static void P32RtApiInit (void) {
  HMODULE kernel32 = P32GetModuleHandle ("kernel32.dll");
  assert (kernel32 != NULL);

#if P32_WINNT < P32_WINNT_WIN7
  /**
   * Check for `RaiseFailFastException`.
   */
  FuncRaiseFailFastException ptrRaiseFailFastException = NULL;

  if (kernel32 != NULL) {
    ptrRaiseFailFastException = P32GetProcAddress (kernel32, RaiseFailFastException);
  }

  if (ptrRaiseFailFastException != NULL) {
    P32AtomicExchange (&P32RtApi.PtrRaiseFailFastException, ptrRaiseFailFastException);
  }
#endif /* P32_WINNT < Windows 7 */
}
#endif /* DYNAMIC_CHECKS */

#if P32_WINNT < P32_WINNT_WIN7
#define RaiseFailFastException P32RtApi.PtrRaiseFailFastException
#endif /* P32_WINNT < Windows 7 */

/*******************************************************************************
 * Implementation.
 *
 * RuntimeRaiseFatalException:
 *
 *   <= Windows Vista
 *
 *   Raises a non-continuable exception (`EXCEPTION_NONCONTINUABLE`).
 *   Such exception indicates that it is impossible to continue from the
 *   current state and will likely terminate the process.
 *
 *   If the raised exception was handled and did not terminate the process,
 *   call `TerminateProcess`.
 *
 *   >= Windows 7
 *
 *   Ues `RaiseFailFastException` to raise an exception which bypasses
 *   ALL exception handlers and always terminates the process.
 */

#if P32_WINNT < P32_WINNT_WIN7
/**
 * Function corresponding to `RuntimeRaiseFatalException`.
 */
typedef void (*FuncRuntimeRaiseFatalException) (PCONTEXT);

/**
 * `RuntimeRaiseFatalException` implementation using `RaiseException`.
 */
static void P32RaiseNonContinuableException (PCONTEXT context);

/**
 * Initialization thunk for `RuntimeRaiseFatalException`.
 */
static void P32RuntimeRaiseFatalExceptionInit (PCONTEXT context);
#endif /* P32_WINNT < Windows 7 */

/**
 * `RuntimeRaiseFatalException` implementation using `RaiseFailFastException`.
 */
static void P32RaiseFailFastException (PCONTEXT context);

#ifdef DYNAMIC_CHECKS
/**
 * Pointers to implementation.
 */
typedef struct RuntimeApi {
  pthread_once_t Init;

#if P32_WINNT < P32_WINNT_WIN7
  /**
   * Implementation for `RuntimeRaiseFatalException`.
   */
  FuncRuntimeRaiseFatalException PtrRuntimeRaiseFatalException;
#endif /* P32_WINNT < Windows 7 */
} RuntimeApi;

/**
 * Implementation.
 */
static RuntimeApi P32RuntimeApi = {
  .Init = PTHREAD_ONCE_INIT,

#if P32_WINNT < P32_WINNT_WIN7
  .PtrRuntimeRaiseFatalException = P32RuntimeRaiseFatalExceptionInit,
#endif /* P32_WINNT < Windows 7 */
};

/**
 * Initialize `P32RuntimeApi`.
 */
static void P32RuntimeApiInit (void) {
  pthread_once (&P32RtApi.Init, P32RtApiInit);

#if P32_WINNT < P32_WINNT_WIN7
  if (P32RtApi.PtrRaiseFailFastException != NULL) {
    P32AtomicExchange (&P32RuntimeApi.PtrRuntimeRaiseFatalException, P32RaiseFailFastException);
  } else {
    P32AtomicExchange (&P32RuntimeApi.PtrRuntimeRaiseFatalException, P32RaiseNonContinuableException);
  }
#endif /* P32_WINNT < Windows 7 */
}
#endif /* DYNAMIC_CHECKS */

#if P32_WINNT < P32_WINNT_WIN7
#define RuntimeRaiseFatalException P32RuntimeApi.PtrRuntimeRaiseFatalException

static void P32RaiseNonContinuableException (PCONTEXT context) {
  RaiseException (STATUS_FATAL_APP_EXIT, EXCEPTION_NONCONTINUABLE, 0, NULL);

  _RPTW0 (_CRT_ERROR, L"Non-continuable exception failed to terminate process.\n");

  /**
   * If non-continuable exception was handled, terminate process the old way.
   */
  TerminateProcess (GetCurrentProcess (), STATUS_FATAL_APP_EXIT);

  _RPTW0 (_CRT_ERROR, L"TerminateProcess failed to terminate process.\n");

  return;
  UNREFERENCED_PARAMETER (context);
}

static void P32RuntimeRaiseFatalExceptionInit (PCONTEXT context) {
  pthread_once (&P32RuntimeApi.Init, P32RuntimeApiInit);
  RuntimeRaiseFatalException (context);
}
#else /* P32_WINNT >= Windows 7 */
#define RuntimeRaiseFatalException P32RaiseFailFastException
#endif /* P32_WINNT >= Windows 7 */

static void P32RaiseFailFastException (PCONTEXT context) {
  EXCEPTION_RECORD exceptionRecord = {0};

  exceptionRecord.ExceptionCode    = STATUS_FATAL_APP_EXIT;
  exceptionRecord.ExceptionAddress = (PVOID) P32_CONTEXT_PC (*context);

  RaiseFailFastException (&exceptionRecord, context, 0);
}

/*******************************************************************************
 * External Functions.
 */

#ifdef LIBPOSIX32_TEST
/**
 * Terminate handler set with `p32_terminate_handler`.
 */
static TerminateHandler P32TerminateHandler = NULL;

void p32_terminate_handler (TerminateHandler handler) {
  P32AtomicExchange (&P32TerminateHandler, handler);
}
#endif

void p32_terminate (const wchar_t *message, void *context) {
  _RPTW1 (_CRT_ERROR, L"%s\n", message);

#ifdef LIBPOSIX32_TEST
  if (P32TerminateHandler != NULL) {
    P32TerminateHandler ();
  }
#endif

  /**
   * Send `message` to debugger (if present) before terminating.
   */
  OutputDebugStringW (message);

  /**
   * Terminate the process.
   */
  RuntimeRaiseFatalException (context);

  /**
   * We should never ever get here.
   */
  _exit (STATUS_FATAL_APP_EXIT);
}
