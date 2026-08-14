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
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "core-atomic.h"
#include "core-debug.h"
#include "core-loader.h"
#include "core-winver.h"

/**
 * File Summary:
 *
 * This file defines internal portability wrappers for `LoadLibrary*`,
 * `GetModuleHandle*` and `FreeLibrary`.
 */

/**
 * If library is configured with Win9x support, always use ANSI APIs.
 *
 * Functions defined in this file are for internal use; it means that we can
 * ensure that passed strings are valid and contain only ASCII characters.
 */
#if P32_WIN9X
#define P32_AW(str) str##A
#define S           L"%hs"
#else
#define P32_AW(str) str##W
#define S           L"%s"
#endif

#undef LoadLibrary
#define LoadLibrary P32_AW (LoadLibrary)

#undef LoadLibraryEx
#define LoadLibraryEx P32_AW (LoadLibraryEx)

#undef GetModuleHandle
#define GetModuleHandle P32_AW (GetModuleHandle)

/*******************************************************************************
 * Structures, functions and macros used to lookup functions at runtime.
 */

#if P32_WINNT < P32_WINNT_XP || P32_WIN9X
#define DYNAMIC_CHECKS
#define DYNAMIC_IMPLEMENTATION

/**
 * Function type corresponding to `GetModuleHandleEx`.
 */
#define FuncGetModuleHandleEx P32_AW (FuncGetModuleHandleEx)
typedef BOOL (WINAPI *FuncGetModuleHandleEx) (DWORD, ModuleName, HMODULE *);
#endif /* P32_WINNT < Windows XP || P32_WIN9X */

#ifdef DYNAMIC_CHECKS
/**
 * Pointers to Windows Loader APIs which are looked up at runtime.
 */
typedef struct LoaderApi {
  pthread_once_t Init;

#if P32_WINNT < P32_WINNT_XP || P32_WIN9X
  FuncGetModuleHandleEx PtrGetModuleHandleEx;
#endif /* P32_WINNT < Windows XP || P32_WIN9X */
} LoaderApi;

/**
 * Windows Loader APIs.
 */
static LoaderApi P32LoaderApi = {
  .Init = PTHREAD_ONCE_INIT,

#if P32_WINNT < P32_WINNT_XP || P32_WIN9X
  .PtrGetModuleHandleEx = NULL,
#endif /* P32_WINNT < Windows XP || P32_WIN9X */
};

/**
 * Initialize `P32LoaderApi`.
 */
static void P32InitLoaderApi (void) {
  HMODULE kernel32 = GetModuleHandle (P32_MODULE_NAME ("kernel32.dll"));
  assert (kernel32 != NULL);

#if P32_WINNT < P32_WINNT_XP || P32_WIN9X
  /**
   * Lookup `GetModuleHandleEx`.
   */
  FuncGetModuleHandleEx ptrGetModuleHandleEx = NULL;

  if (kernel32 != NULL) {
    if (P32_WINNT_CHECK (P32_WINNT_XP, WindowsNtXP)) {
      ptrGetModuleHandleEx = p32_get_proc_address (kernel32, P32_AW (GetModuleHandleEx));
    }

    if (ptrGetModuleHandleEx != NULL) {
      p32_atomic_exchange_fpointer (&P32LoaderApi.PtrGetModuleHandleEx, ptrGetModuleHandleEx);
    }
  }
#endif /* P32_WINNT < Windows XP || P32_WIN9X */
}
#endif /* DYNAMIC_CHECKS */

#if P32_WINNT < P32_WINNT_XP || P32_WIN9X
#undef GetModuleHandleEx
#define GetModuleHandleEx P32LoaderApi.PtrGetModuleHandleEx
#else /* P32_WINNT >= Windows XP && !P32_WIN9X */
#undef GetModuleHandleEx
#define GetModuleHandleEx P32_AW (GetModuleHandleEx)
#endif /* P32_WINNT >= Windows XP && !P32_WIN9X */

/*******************************************************************************
 * Implementation.
 *
 * p32_load_library:
 *
 *  Windows 9x:
 *
 *    Implemented as a wrapper around `LoadLibrary`.
 *
 *  Windows NT:
 *
 *    Implemented as a wrapper around `LoadLibraryEx`. It uses different flags
 *    depending on Windows version the code is running on.
 *
 * p32_get_module_handle:
 *
 *  <= Windows 2000:
 *
 *    Implemented as a wrapper around `GetModuleHandle`.
 *
 *  >= Windows XP:
 *
 *    Implemented as a wrapper around `GetModuleHandleEx`.
 *
 * p32_free_library:
 *
 *  Simple wrapper around `FreeLibrary`.
 */

/**
 * Implementation for `p32_free_library`.
 */
static void P32LoaderFreeLibrary (uintptr_t module) {
  HMODULE moduleHandle = (HANDLE) module;
  assert (moduleHandle != NULL);

  if (!FreeLibrary (moduleHandle)) {
    p32_dbg_error (L"Failed to free library; module=<%p>.\n", moduleHandle);
  }
}

#if P32_WINNT < P32_WINNT_WIN8 || P32_WIN9X
#define DYNAMIC_IMPLEMENTATION

/**
 * Pointer type corresponding to `p32_load_library`.
 */
typedef uintptr_t (*FuncLoaderLoadLibrary) (ModuleName);

/**
 * Initialization thunk for `p32_load_library`.
 */
static uintptr_t P32InitLoaderLoadLibrary (ModuleName);

#if P32_WIN9X
/**
 * Implementation for `p32_load_library` using `LoadLibrary`.
 */
static uintptr_t P32LoaderLoadLibrary (ModuleName);
#endif /* Win9x */
#endif /* P32_WINNT < Windows 8 || P32_WIN9X */

/**
 * Implementation for `p32_load_library` using `LoadLibraryEx`.
 */
static uintptr_t P32LoaderLoadLibraryEx (ModuleName);

#if P32_WINNT < P32_WINNT_XP || P32_WIN9X
/**
 * Pointer type corresponding to `p32_get_module_handle`.
 */
typedef uintptr_t (*FuncLoaderGetModuleHandle) (ModuleName);

/**
 * Initialization thunk for `p32_get_module_handle`.
 */
static uintptr_t P32InitLoaderGetModuleHandle (ModuleName);

/**
 * Implementation for `p32_get_module_handle` using `GetModuleHandle`.
 */
static uintptr_t P32LoaderGetModuleHandle (ModuleName);
#endif /* P32_WINNT < Windows XP || P32_WIN9X */

/**
 * Implementation for `p32_get_module_handle` using `GetModuleHandleEx`.
 */
static uintptr_t P32LoaderGetModuleHandleEx (ModuleName);

#ifdef DYNAMIC_IMPLEMENTATION
/**
 * Pointers to implementation.
 */
typedef struct LoaderImpl {
  pthread_once_t Init;

#if P32_WINNT < P32_WINNT_WIN8 || P32_WIN9X
  /**
   * Flags to use with `LoadLibraryEx`.
   */
  uint32_t LoaderLoadLibraryFlags;
  /**
   * Implementation for `p32_load_library`.
   */
  FuncLoaderLoadLibrary PtrLoaderLoadLibrary;
#endif /* P32_WINNT < Windows 8 || P32_WIN9X */

#if P32_WINNT < P32_WINNT_XP || P32_WIN9X
  /**
   * Implementation for `p32_get_module_handle`.
   */
  FuncLoaderGetModuleHandle PtrLoaderGetModuleHandle;
#endif /* P32_WINNT < Windows XP || P32_WIN9X */
} LoaderImpl;

/**
 * Implementation.
 */
static LoaderImpl P32LoaderImpl = {
  .Init = PTHREAD_ONCE_INIT,

#if P32_WINNT < P32_WINNT_WIN8 || P32_WIN9X
  .LoaderLoadLibraryFlags = LOAD_WITH_ALTERED_SEARCH_PATH,
  .PtrLoaderLoadLibrary   = P32InitLoaderLoadLibrary,
#endif /* P32_WINNT < Windows 8 || P32_WIN9X */

#if P32_WINNT < P32_WINNT_XP || P32_WIN9X
  .PtrLoaderGetModuleHandle = P32InitLoaderGetModuleHandle,
#endif /* P32_WINNT < Windows XP || P32_WIN9X */
};

/**
 * Initialize `P32LoaderImpl`.
 */
static void P32InitLoaderImpl (void) {
#ifdef DYNAMIC_CHECKS
  pthread_once (&P32LoaderApi.Init, P32InitLoaderApi);
#endif

#if P32_WINNT < P32_WINNT_WIN8 || P32_WIN9X
  if (P32_WINNT_CHECK (P32_WINNT_WIN8, WindowsNt8)) {
    P32LoaderImpl.LoaderLoadLibraryFlags = LOAD_LIBRARY_SEARCH_DEFAULT_DIRS;
  } else {
    P32LoaderImpl.LoaderLoadLibraryFlags = LOAD_WITH_ALTERED_SEARCH_PATH;
  }
#endif /* P32_WINNT < Windows 8 || P32_WIN9X */

  if (!P32_WIN9X || p32_platform () == WindowsPlatformNt) {
    p32_atomic_exchange_fpointer (&P32LoaderImpl.PtrLoaderLoadLibrary, P32LoaderLoadLibraryEx);
#if P32_WIN9X
  } else {
    p32_atomic_exchange_fpointer (&P32LoaderImpl.PtrLoaderLoadLibrary, P32LoaderLoadLibrary);
#endif /* Win9x */
  }

#if P32_WINNT < P32_WINNT_XP || P32_WIN9X
  if (P32LoaderApi.PtrGetModuleHandleEx != NULL) {
    p32_atomic_exchange_fpointer (&P32LoaderImpl.PtrLoaderGetModuleHandle, P32LoaderGetModuleHandleEx);
  } else {
    p32_atomic_exchange_fpointer (&P32LoaderImpl.PtrLoaderGetModuleHandle, P32LoaderGetModuleHandle);
  }
#endif /* P32_WINNT < Windows XP || P32_WIN9X */
}
#endif /* DYNAMIC_IMPLEMENTATION */

#if P32_WINNT < P32_WINNT_WIN8 || P32_WIN9X
#define LoaderLoadLibrary P32LoaderImpl.PtrLoaderLoadLibrary

static uintptr_t P32InitLoaderLoadLibrary (ModuleName moduleName) {
  pthread_once (&P32LoaderImpl.Init, P32InitLoaderImpl);
  return LoaderLoadLibrary (moduleName);
}

#if P32_WIN9X
static uintptr_t P32LoaderLoadLibrary (ModuleName moduleName) {
  HMODULE module = LoadLibrary (moduleName);

  if (module == NULL) {
    p32_dbg_warning (L"Failed to load library '" S L"'.\n", moduleName);
  }

  return (uintptr_t) module;
}
#endif /* Win9x */
#else  /* P32_WINNT >= Windows 8 && !P32_WIN9X */
#define LoaderLoadLibrary P32LoaderLoadLibraryEx
#endif /* P32_WINNT >= Windows 8 && !P32_WIN9X */

#if P32_WINNT > P32_WINNT_WIN10 || (P32_WINNT == P32_WINNT_WIN10 && P32_NTDDI >= NTDDI_WIN10_RS1)
#define LoaderLoadLibraryFlags (LOAD_LIBRARY_SEARCH_DEFAULT_DIRS | LOAD_LIBRARY_SAFE_CURRENT_DIRS)
#elif P32_WINNT >= P32_WINNT_WIN8 && !P32_WIN9X
#define LoaderLoadLibraryFlags (LOAD_LIBRARY_SEARCH_DEFAULT_DIRS)
#else /* P32_WINNT < Windows 8 */
#define LoaderLoadLibraryFlags P32LoaderImpl.LoaderLoadLibraryFlags
#endif /* P32_WINNT < Windows 8 */

static uintptr_t P32LoaderLoadLibraryEx (ModuleName moduleName) {
  HMODULE module = LoadLibraryEx (moduleName, NULL, LoaderLoadLibraryFlags);

  if (module == NULL) {
    p32_dbg_warning (L"Failed to load library '" S L"'.\n", moduleName);
  }

  return (uintptr_t) module;
}

#if P32_WINNT < P32_WINNT_XP || P32_WIN9X
#define LoaderGetModuleHandle P32LoaderImpl.PtrLoaderGetModuleHandle

static uintptr_t P32InitLoaderGetModuleHandle (ModuleName moduleName) {
  pthread_once (&P32LoaderImpl.Init, P32InitLoaderImpl);
  return LoaderGetModuleHandle (moduleName);
}

static uintptr_t P32LoaderGetModuleHandle (ModuleName moduleName) {
  HMODULE module = GetModuleHandle (moduleName);

  if (module == NULL) {
    p32_dbg_warning (L"Failed to obtain handle for '" S L"'.\n", moduleName);
  }

  return (uintptr_t) module;
}
#else /* P32_WINNT >= Windows XP && !P32_WIN9X */
#define LoaderGetModuleHandle P32LoaderGetModuleHandleEx
#endif /* P32_WINNT >= Windows XP && !P32_WIN9X */

static uintptr_t P32LoaderGetModuleHandleEx (ModuleName moduleName) {
  HMODULE module = NULL;

  if (!GetModuleHandleEx (GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, moduleName, &module)) {
    p32_dbg_warning (L"Failed to obtain handle for '" S L"'.\n", moduleName);
  }

  return (uintptr_t) module;
}

/*******************************************************************************
 * External Functions.
 */

uintptr_t p32_load_library (ModuleName moduleName) {
  return LoaderLoadLibrary (moduleName);
}

uintptr_t p32_get_module_handle (ModuleName moduleName) {
  return LoaderGetModuleHandle (moduleName);
}

void p32_free_library (uintptr_t module) {
  P32LoaderFreeLibrary (module);
}
