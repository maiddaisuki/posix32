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

#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "crt-internal.h"

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

#ifdef LIBPOSIX32_TEST
/**
 * Terminate handler set with `p32_terminate_handler`.
 */
static TerminateHandler P32TerminateHandler = NULL;

void p32_terminate_handler (TerminateHandler handler) {
  InterlockedExchangePointer ((volatile PVOID *) &P32TerminateHandler, (PVOID) (UINT_PTR) handler);
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

#if _WIN32_WINNT >= _WIN32_WINNT_WIN7
  EXCEPTION_RECORD exceptionRecord = {0};

  exceptionRecord.ExceptionCode    = STATUS_FATAL_APP_EXIT;
  exceptionRecord.ExceptionAddress = (PVOID) P32_CONTEXT_PC (*(CONTEXT *) context);

  RaiseFailFastException (&exceptionRecord, context, 0);
#else
  RaiseException (STATUS_FATAL_APP_EXIT, EXCEPTION_NONCONTINUABLE, 0, NULL);
#endif

  _RPTW0 (_CRT_ERROR, L"Runtime: non-continuable exception failed to terminate process.\n");

  /**
   * Exception with EXCEPTION_NONCONTINUABLE flag should always terminate the
   * process and we should never get here, but let's be a little paranoid.
   */
  TerminateProcess (GetCurrentProcess (), STATUS_FATAL_APP_EXIT);

  _RPTW0 (_CRT_ERROR, L"Runtime: call to TerminateProcess failed to terminate process.\n");

  /**
   * We should never ever get here.
   */
  _exit (STATUS_FATAL_APP_EXIT);
  UNREFERENCED_PARAMETER (context);
}
