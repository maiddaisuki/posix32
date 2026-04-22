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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "core-crt.h"
#include "core-runtime.h"

/**
 * File Summary:
 *
 * This file contains functions to obtain information about CRT.
 */

#if P32_CRT == P32_UCRT
#ifdef _DEBUG
#define P32_CRT_LIBNAME "ucrtbased.dll"
#else
#define P32_CRT_LIBNAME "ucrtbase.dll"
#endif
#elif P32_CRT == P32_MSVCR120
#ifdef _DEBUG
#define P32_CRT_LIBNAME "msvcr120d.dll"
#else
#define P32_CRT_LIBNAME "msvcr120.dll"
#endif
#elif P32_CRT == P32_MSVCR110
#ifdef _DEBUG
#define P32_CRT_LIBNAME "msvcr110d.dll"
#else
#define P32_CRT_LIBNAME "msvcr110.dll"
#endif
#elif P32_CRT == P32_MSVCR100
#ifdef _DEBUG
#define P32_CRT_LIBNAME "msvcr100d.dll"
#else
#define P32_CRT_LIBNAME "msvcr100.dll"
#endif
#elif P32_CRT == P32_MSVCR90
#ifdef _DEBUG
#define P32_CRT_LIBNAME "msvcr90d.dll"
#else
#define P32_CRT_LIBNAME "msvcr90.dll"
#endif
#elif P32_CRT == P32_MSVCR80
#ifdef _DEBUG
#define P32_CRT_LIBNAME "msvcr80d.dll"
#else
#define P32_CRT_LIBNAME "msvcr80.dll"
#endif
#elif P32_CRT == P32_MSVCR71
#ifdef _DEBUG
#define P32_CRT_LIBNAME "msvcr71d.dll"
#else
#define P32_CRT_LIBNAME "msvcr71.dll"
#endif
#elif P32_CRT == P32_MSVCR70
#ifdef _DEBUG
#define P32_CRT_LIBNAME "msvcr70d.dll"
#else
#define P32_CRT_LIBNAME "msvcr70.dll"
#endif
#elif P32_CRT >= P32_MSVCRT42
#ifdef _DEBUG
#define P32_CRT_LIBNAME "msvcrtd.dll"
#else
#define P32_CRT_LIBNAME "msvcrt.dll"
#endif
#elif P32_CRT >= P32_MSVCRT40
#ifdef _DEBUG
/**
 * NOTE: msvcr40d.dll is the correct name.
 */
#define P32_CRT_LIBNAME "msvcr40d.dll"
#else
#define P32_CRT_LIBNAME "msvcrt40.dll"
#endif
#elif P32_CRT == P32_MSVCRT20
#ifdef _DEBUG
#error _DEBUG: msvcrt20.dll does not have debug version
#else
#define P32_CRT_LIBNAME "msvcrt20.dll"
#endif
#elif P32_CRT == P32_MSVCRT10
#ifdef _DEBUG
#error _DEBUG: msvcrt10.dll does not have debug version
#else
#define P32_CRT_LIBNAME "msvcrt10.dll"
#endif
#else
#ifdef _DEBUG
#error _DEBUG: crtdll.dll does not have debug version
#else
#define P32_CRT_LIBNAME "crtdll.dll"
#endif
#endif

#ifdef _DLL
static uintptr_t      P32CrtHandle     = 0;
static pthread_once_t P32CrtHandleOnce = PTHREAD_ONCE_INIT;

static void P32InitCrtHandle (void) {
  HANDLE crtHandle = GetModuleHandleW (TEXT (P32_CRT_LIBNAME));

  if (crtHandle == NULL) {
    p32_terminate (L"Failed to obtain handle to CRT (" TEXT (P32_CRT_LIBNAME) L").");
  }

  P32CrtHandle = (uintptr_t) crtHandle;
}
#endif /* _DLL */

uintptr_t p32_crt_handle (void) {
#ifdef _DLL
  pthread_once (&P32CrtHandleOnce, P32InitCrtHandle);
  return P32CrtHandle;
#else
  return 0;
#endif
}
