/**
 * Copyright 2026 Kirill Makurin
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
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "core-atomic.h"
#include "core-charset.h"
#include "core-debug.h"
#include "core-loader.h"
#include "core-runtime.h"
#include "core-winver.h"

/**
 * File Summary:
 *
 * The file defines function which are portability wrappers for
 * Windows Debug APIs.
 *
 * In addition, this file also defines helper functions to assist testing
 * the library; see `core-debug.h` for more details on such functions.
 */

/*******************************************************************************
 * Structures, functions and macros used to lookup functions at runtime.
 */

#if P32_WINNT < P32_WINNT_NT_3_51 || (P32_WIN9X && P32_WIN9X < P32_WIN9X_98)
#define DYNAMIC_CHECKS

/**
 * Function type corresponding to `IsDebuggerPresent`.
 */
typedef BOOL (WINAPI *FuncIsDebuggerPresent) (VOID);

/**
 * Initialization thunk for `IsDebuggerPresent`.
 */
static BOOL WINAPI P32InitIsDebuggerPresent (VOID);

/**
 * Stub to use if `IsDebuggerPresent` is not available.
 */
static BOOL WINAPI P32IsDebuggerPresent (VOID);
#endif /* P32_WINNT < Windows NT 3.51 || P32_WIN9X < Windows 98 */

#ifdef DYNAMIC_CHECKS
/**
 * Pointers to Windows Debug APIs which are looked up at runtime.
 */
typedef struct DebugApi {
  pthread_once_t Init;

#if P32_WINNT < P32_WINNT_NT_3_51 || (P32_WIN9X && P32_WIN9X < P32_WIN9X_98)
  /**
   * Function `IsDebuggerPresent` is available since Windows NT 3.51 and
   * Windows 98.
   */
  FuncIsDebuggerPresent PtrIsDebuggerPresent;
#endif /* P32_WINNT < Windows NT 3.51 || P32_WIN9X < Windows 98 */
} DebugApi;

/**
 * Windows Debug APIs.
 */
static DebugApi P32DebugApi = {
  .Init = PTHREAD_ONCE_INIT,

#if P32_WINNT < P32_WINNT_NT_3_51 || (P32_WIN9X && P32_WIN9X < P32_WIN9X_98)
  .PtrIsDebuggerPresent = P32InitIsDebuggerPresent,
#endif /* P32_WINNT < Windows NT 3.51 || P32_WIN9X < Windows 98 */
};

/**
 * Init `P32DebugApi`.
 */
static void P32InitDebugApi (void) {
  uintptr_t kernel32 = p32_get_module_handle (P32_MODULE_NAME ("kernel32.dll"));
  assert (kernel32 != 0);

#if P32_WINNT < P32_WINNT_NT_3_51 || (P32_WIN9X && P32_WIN9X < P32_WIN9X_98)
  /**
   * Lookup `IsDebuggerPresent`.
   */
  FuncIsDebuggerPresent ptrIsDebuggerPresent = NULL;

  if (kernel32 != 0) {
    if (P32_PLATFORM_CHECK (P32_WINNT_NT_3_51, WindowsNt3_51, P32_WIN9X_98, Windows98)) {
      ptrIsDebuggerPresent = p32_get_proc_address (kernel32, IsDebuggerPresent);
      assert (ptrIsDebuggerPresent != NULL);
    }
  }

  if (ptrIsDebuggerPresent != NULL) {
    p32_atomic_exchange_fpointer (&P32DebugApi.PtrIsDebuggerPresent, ptrIsDebuggerPresent);
  } else {
    p32_atomic_exchange_fpointer (&P32DebugApi.PtrIsDebuggerPresent, P32IsDebuggerPresent);
  }
#endif /* P32_WINNT < Windows NT 3.51 || P32_WIN9X < Windows 98 */
}
#endif /*DYNAMIC_CHECKS */

#if P32_WINNT < P32_WINNT_NT_3_51 || (P32_WIN9X && P32_WIN9X < P32_WIN9X_98)
#define IsDebuggerPresent P32DebugApi.PtrIsDebuggerPresent

static BOOL WINAPI P32InitIsDebuggerPresent (VOID) {
  pthread_once (&P32DebugApi.Init, P32InitDebugApi);
  return IsDebuggerPresent ();
}

static BOOL WINAPI P32IsDebuggerPresent (VOID) {
  return FALSE;
}
#endif /* P32_WINNT < Windows NT 3.51 || P32_WIN9X < Windows 98 */

/*******************************************************************************
 * Implementation.
 *
 * p32_dbg_output_string:
 *
 *  A portability wrapper for `OutputDebugString` functions.
 */

#if P32_WIN9X
#define DYNAMIC_IMPLEMENTATION

/**
 * Function type corresponding to `p32_dbg_output_string`.
 */
typedef void (*FuncDbgOutputString) (const wchar_t *);

/**
 * Initialization thunk for `p32_dbg_output_string`
 */
void P32InitDbgOutputString (const wchar_t *);

/**
 * Implementation for `p32_dbg_output_string` using `OutputDebugStringA`.
 */
void P32DbgOutputStringAnsi (const wchar_t *);
#endif /* Win9x */

/**
 * Implementation for `p32_dbg_output_string` using `OutputDebugStringW`.
 */
void P32DbgOutputStringUnicode (const wchar_t *);

#ifdef DYNAMIC_IMPLEMENTATION
/**
 * Pointers to implementation.
 */
typedef struct DbgImpl {
  pthread_once_t Init;

#ifdef P32_WIN9X
  /**
   * `Charset` structure for active ANSI code page.
   *
   * We cache this information in order to avoid redundant calls to
   * `p32_charset_info` from `P32DbgOutputStringAnsi`.
   */
  Charset ActiveCharset;
  /**
   * Implementation for `p32_dbg_output_string`.
   */
  FuncDbgOutputString PtrDbgOutputString;
#endif /* Win9x */
} DbgImpl;

/**
 * Implementation.
 */
static DbgImpl P32DbgImpl = {
  .Init = PTHREAD_ONCE_INIT,

#if P32_WIN9X
  .PtrDbgOutputString = P32InitDbgOutputString,
#endif /* Win9x */
};

/**
 * Initialize `P32DbgImpl`.
 */
static void P32InitDbgImpl (void) {
#ifdef DYNAMIC_CHECKS
  pthread_once (&P32DebugApi.Init, P32InitDebugApi);
#endif

#if P32_WIN9X
  if (p32_platform () == WindowsPlatformNt) {
    p32_atomic_exchange_fpointer (&P32DbgImpl.PtrDbgOutputString, P32DbgOutputStringUnicode);
  } else {
    P32DbgImpl.ActiveCharset.CodePage = GetACP ();

    if (!p32_charset_info (&P32DbgImpl.ActiveCharset)) {
      p32_terminate_safely (L"Failed to obtain information about active ANSI code page.");
    }

    p32_atomic_exchange_fpointer (&P32DbgImpl.PtrDbgOutputString, P32DbgOutputStringAnsi);
  }
#endif /* Win9x */
}
#endif /* DYNAMIC_IMPLEMENTATION */

#if P32_WIN9X
#define DbgOutputString P32DbgImpl.PtrDbgOutputString

void P32InitDbgOutputString (const wchar_t *string) {
  pthread_once (&P32DbgImpl.Init, P32InitDbgImpl);
  DbgOutputString (string);
}

void P32DbgOutputStringAnsi (const wchar_t *string) {
  /**
   * Convert `string` to active ANSI code page.
   */
  char *str = NULL;

  CharsetConversionRequest conversionRequest = {0};

  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_MALLOC;
  conversionRequest.Charset   = &P32DbgImpl.ActiveCharset;
  conversionRequest.Input.W   = string;
  conversionRequest.Output.A  = &str;

  if (p32_charset_wcstombs (&conversionRequest, 0) == -1) {
    return;
  }

  OutputDebugStringA (str);
  free (str);
}
#else /* No Win9x support */
#define DbgOutputString P32DbgOutputStringUnicode
#endif /* No Win9x support */

void P32DbgOutputStringUnicode (const wchar_t *string) {
  OutputDebugStringW (string);
}

/*******************************************************************************
 * External Functions.
 */

bool p32_dbg_is_debugger_present (void) {
  return IsDebuggerPresent ();
}

void p32_dbg_output_string (const wchar_t *string) {
  DbgOutputString (string);
}

/*******************************************************************************
 * Helper functions used by test version of the library and unit tests.
 */

#ifdef LIBPOSIX32_TEST
void p32_dbg_write_message (DbgMessageType type, const wchar_t *filename, int line, const wchar_t *format, ...) {
  va_list va;
  va_start (va, format);

  /**
   * Message destination.
   */
  FILE *stream = NULL;

  if (type == DbgMessageInfo) {
    stream = stdout;
  } else if (type == DbgMessageWarning) {
    stream = stderr;
  } else if (type == DbgMessageError) {
    stream = stderr;
  }

#if P32_CRT >= P32_MSVCR80
  _lock_file (stream);
#endif

  if (filename != NULL) {
    fwprintf (stream, L"%s:%d: ", filename, line);
  }

  vfwprintf (stream, format, va);

#if P32_CRT >= P32_MSVCR80
  _unlock_file (stream);
#endif

  va_end (va);
}
#endif
