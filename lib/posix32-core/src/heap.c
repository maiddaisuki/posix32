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
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "core-memory.h"
#include "core-winver.h"

/**
 * File Summary:
 *
 * This file defines functions which are wrappers for Windows Heap APIs.
 *
 * For example, function `HeapSummary` is only available since Windows NT 3.51
 * and is not available at all on Windows 9x systems, which means we cannot
 * reference it when `P32_WINNT < P32_WINNT_NT_3_51` or when Win9x support is
 * enabled.
 *
 * Depending on configuration, these wrappers either:
 *
 * - call a function directly; this means that function is always available
 *   according to `P32_WINNT` and `P32_WIN9X`
 *
 * - lookup whether a function is available at runtime, and if so, call it
 *   through a pointer; otherwise use a stub which simply fails
 *
 * See comments in this file for more details.
 */

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

#if P32_WINNT < P32_WINNT_NT_3_5
#define DYNAMIC_CHECKS

/**
 * Function type corresponding to `HeapLock`.
 */
typedef BOOL (WINAPI *FuncHeapLock) (HANDLE);

/**
 * Function type corresponding to `HeapUnlock`.
 */
typedef BOOL (WINAPI *FuncHeapUnlock) (HANDLE);

/**
 * Initialization thunk for `HeapLock`.
 */
static BOOL WINAPI P32InitHeapLock (HANDLE);

/**
 * Initialization thunk for `HeapUnlock`.
 */
static BOOL WINAPI P32InitHeapUnlock (HANDLE);

/**
 * Stub to use if `HeapLock` is not available.
 */
static BOOL WINAPI P32HeapLock (HANDLE);

/**
 * Stub to use if `HeapUnlock` is not available.
 */
static BOOL WINAPI P32HeapUnlock (HANDLE);
#endif /* P32_WINNT < Windows NT 3.5 */

#if P32_WINNT < P32_WINNT_NT_3_51 || P32_WIN9X
#define DYNAMIC_CHECKS

/**
 * Function type corresponding to `HeapSummary`.
 */
typedef BOOL (WINAPI *FuncHeapSummary) (HANDLE, DWORD, LPHEAP_SUMMARY);

/**
 * Initialization thunk for `HeapSummary`.
 */
static BOOL WINAPI P32InitHeapSummary (HANDLE, DWORD, LPHEAP_SUMMARY);

/**
 * Stub to use if `HeapSummary` is not available.
 */
static BOOL WINAPI P32HeapSummary (HANDLE, DWORD, LPHEAP_SUMMARY);
#endif /* P32_WINNT < Windows NT 3.51 || Win9x */

#if P32_WINNT < P32_WINNT_XP || P32_WIN9X
#define DYNAMIC_CHECKS

/**
 * Function type corresponding to `HeapSetInformation`.
 */
typedef BOOL (WINAPI *FuncHeapSetInformation) (HANDLE, HEAP_INFORMATION_CLASS, PVOID, SIZE_T);

/**
 * Initialization thunk for `HeapSetInformation`.
 */
static BOOL WINAPI P32InitHeapSetInformation (HANDLE, HEAP_INFORMATION_CLASS, PVOID, SIZE_T);

/**
 * Stub to use if `HeapSetInformation` is not available.
 */
static BOOL WINAPI P32HeapSetInformation (HANDLE, HEAP_INFORMATION_CLASS, PVOID, SIZE_T);
#endif /* P32_WINNT < Windows XP || Win9x */

#ifdef DYNAMIC_CHECKS
/**
 * Pointers to Windows Heap APIs which are looked up at runtime.
 */
typedef struct HeapApi {
  pthread_once_t Init;

  /**
   * Functions `HeapLock` and `HeapUnlock` are available since Windows NT 3.5.
   * These functions are always available on Windows 9x systems.
   */
#if P32_WINNT < P32_WINNT_NT_3_5
  /**
   * Address of `HeapLock`.
   */
  FuncHeapLock PtrHeapLock;
  /**
   * Address of `HeapUnlock`.
   */
  FuncHeapUnlock PtrHeapUnlock;
#endif /* P32_WINNT < Windows NT 3.5 */

/**
 * Function `HeapSummary` is available since Windows NT 3.51.
 * This function is not available at all on Windows 9x systems.
 */
#if P32_WINNT < P32_WINNT_NT_3_51 || P32_WIN9X
  /**
   * Address of `HeapSummary`.
   */
  FuncHeapSummary PtrHeapSummary;
#endif /* P32_WINNT < Windows NT 3.51 || Win9x */

  /**
   * Function `HeapSetInformation` is available since Windows XP.
   * This function is not available at all on Windows 9x systems.
   */
#if P32_WINNT < P32_WINNT_XP || P32_WIN9X
  /**
   * Address of `HeapSetInformation`.
   */
  FuncHeapSetInformation PtrHeapSetInformation;
#endif /* P32_WINNT < Windows XP || Win9x */
} HeapApi;

/**
 * Windows Heap APIs.
 */
static HeapApi P32HeapApi = {
  .Init = PTHREAD_ONCE_INIT,

#if P32_WINNT < P32_WINNT_NT_3_5
  .PtrHeapLock   = P32InitHeapLock,
  .PtrHeapUnlock = P32InitHeapUnlock,
#endif /* P32_WINNT < Windows NT 3.5 */

#if P32_WINNT < P32_WINNT_NT_3_51 || P32_WIN9X
  .PtrHeapSummary = P32InitHeapSummary,
#endif /* P32_WINNT < Windows NT 3.51 || Win9x */

#if P32_WINNT < P32_WINNT_XP || P32_WIN9X
  .PtrHeapSetInformation = P32InitHeapSetInformation,
#endif /* P32_WINNT < Windows XP || Win9x */
};

/**
 * Initialize `P32HeapApi`.
 */
static void P32InitHeapApi (void) {
  HMODULE kernel32 = P32GetModuleHandle ("kernel32.dll");
  assert (kernel32 != NULL);

#if P32_WINNT < P32_WINNT_NT_3_5
  /**
   * Lookup `HeapLock` and `HeapUnlock`.
   */
  FuncHeapLock   ptrHeapLock   = NULL;
  FuncHeapUnlock ptrHeapUnlock = NULL;

  if (kernel32 != NULL) {
    if (P32_PLATFORM_CHECK (P32_WINNT_NT_3_5, WindowsNt3_5, P32_WIN9X_95, Windows95)) {
      ptrHeapLock = P32GetProcAddress (kernel32, HeapLock);
      assert (ptrHeapLock != NULL);
      ptrHeapUnlock = P32GetProcAddress (kernel32, HeapUnlock);
      assert (ptrHeapUnlock != NULL);
    }
  }

  assert ((ptrHeapLock == NULL) == (ptrHeapUnlock == NULL));

  if (ptrHeapLock != NULL && ptrHeapUnlock != NULL) {
    P32AtomicExchange (&P32HeapApi.PtrHeapLock, ptrHeapLock);
    P32AtomicExchange (&P32HeapApi.PtrHeapUnlock, ptrHeapUnlock);
  } else {
    P32AtomicExchange (&P32HeapApi.PtrHeapLock, P32HeapLock);
    P32AtomicExchange (&P32HeapApi.PtrHeapUnlock, P32HeapUnlock);
  }
#endif /* P32_WINNT < Windows NT 3.5 */

#if P32_WINNT < P32_WINNT_NT_3_51 || P32_WIN9X
  /**
   * Lookup `HeapSummary`.
   */
  FuncHeapSummary ptrHeapSummary = NULL;

  if (kernel32 != NULL) {
    if (P32_WINNT_CHECK (P32_WINNT_NT_3_51, WindowsNt3_51)) {
      ptrHeapSummary = P32GetProcAddress (kernel32, HeapSummary);
      assert (ptrHeapSummary != NULL);
    }
  }

  if (ptrHeapSummary != NULL) {
    P32AtomicExchange (&P32HeapApi.PtrHeapSummary, ptrHeapSummary);
  } else {
    P32AtomicExchange (&P32HeapApi.PtrHeapSummary, P32HeapSummary);
  }
#endif /* P32_WINNT < Windows NT 3.51 || Win9x */

#if P32_WINNT < P32_WINNT_XP || P32_WIN9X
  /**
   * Lookup `HeapSetInformation`.
   */
  FuncHeapSetInformation ptrHeapSetInformation = NULL;

  if (kernel32 != NULL) {
    if (P32_WINNT_CHECK (P32_WINNT_XP, WindowsNtXP)) {
      ptrHeapSetInformation = P32GetProcAddress (kernel32, HeapSetInformation);
      assert (ptrHeapSetInformation != NULL);
    }
  }

  if (ptrHeapSetInformation != NULL) {
    P32AtomicExchange (&P32HeapApi.PtrHeapSetInformation, ptrHeapSetInformation);
  } else {
    P32AtomicExchange (&P32HeapApi.PtrHeapSetInformation, P32HeapSetInformation);
  }
#endif /* P32_WINNT < Windows XP || Win9x */
}
#endif /* DYNAMIC_CHECKS */

#if P32_WINNT < P32_WINNT_NT_3_5
#define HeapLock   P32HeapApi.PtrHeapLock
#define HeapUnlock P32HeapApi.PtrHeapUnlock

static BOOL WINAPI P32InitHeapLock (HANDLE heapHandle) {
  pthread_once (&P32HeapApi.Init, P32InitHeapApi);
  return HeapLock (heapHandle);
}

static BOOL WINAPI P32HeapLock (HANDLE heapHandle) {
  SetLastError (ERROR_CALL_NOT_IMPLEMENTED);
  return FALSE;
  UNREFERENCED_PARAMETER (heapHandle);
}

static BOOL WINAPI P32InitHeapUnlock (HANDLE heapHandle) {
  pthread_once (&P32HeapApi.Init, P32InitHeapApi);
  return HeapUnlock (heapHandle);
}

static BOOL WINAPI P32HeapUnlock (HANDLE heapHandle) {
  SetLastError (ERROR_CALL_NOT_IMPLEMENTED);
  return FALSE;
  UNREFERENCED_PARAMETER (heapHandle);
}
#endif /* P32_WINNT < Windows NT 3.5 */

#if P32_WINNT < P32_WINNT_NT_3_51 || P32_WIN9X
#define HeapSummary P32HeapApi.PtrHeapSummary

static BOOL WINAPI P32InitHeapSummary (HANDLE heapHandle, DWORD flags, LPHEAP_SUMMARY heapSummary) {
  pthread_once (&P32HeapApi.Init, P32InitHeapApi);
  return HeapSummary (heapHandle, flags, heapSummary);
}

static BOOL WINAPI P32HeapSummary (HANDLE heapHandle, DWORD flags, LPHEAP_SUMMARY heapSummary) {
  SetLastError (ERROR_CALL_NOT_IMPLEMENTED);
  return FALSE;
  UNREFERENCED_PARAMETER (heapHandle);
  UNREFERENCED_PARAMETER (flags);
  UNREFERENCED_PARAMETER (heapSummary);
}
#endif /* P32_WINNT < Windows NT 3.51 || Win9x */

#if P32_WINNT < P32_WINNT_XP || P32_WIN9X
#define HeapSetInformation P32HeapApi.PtrHeapSetInformation

static BOOL WINAPI P32InitHeapSetInformation (HANDLE heap, HEAP_INFORMATION_CLASS info, PVOID data, SIZE_T dataSize) {
  pthread_once (&P32HeapApi.Init, P32InitHeapApi);
  return HeapSetInformation (heap, info, data, dataSize);
}

static BOOL WINAPI P32HeapSetInformation (HANDLE heap, HEAP_INFORMATION_CLASS info, PVOID data, SIZE_T dataSize) {
  SetLastError (ERROR_CALL_NOT_IMPLEMENTED);
  return FALSE;
  UNREFERENCED_PARAMETER (heap);
  UNREFERENCED_PARAMETER (info);
  UNREFERENCED_PARAMETER (data);
  UNREFERENCED_PARAMETER (dataSize);
}
#endif /* P32_WINNT < Windows XP || Win9x */

/*******************************************************************************
 * External Functions.
 */

bool p32_heap_lock (uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;
  return HeapLock (heapHandle);
}

bool p32_heap_unlock (uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;
  return HeapUnlock (heapHandle);
}

bool p32_heap_summary (uintptr_t heap, uint32_t flags, void *data) {
  HANDLE         heapHandle  = (HANDLE) heap;
  LPHEAP_SUMMARY heapSummary = (LPHEAP_SUMMARY) data;

  assert (heapSummary != NULL);
  assert (heapSummary->cb == sizeof (HEAP_SUMMARY));

  return HeapSummary (heapHandle, flags, heapSummary);
}

bool p32_heap_set_information (uintptr_t heap, uint32_t infoClass, void *data, size_t dataSize) {
  HANDLE heapHandle = (HANDLE) heap;
  return HeapSetInformation (heapHandle, infoClass, data, dataSize);
}

bool p32_heap_low_fragmentation (uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;
  ULONG  lfh        = 2;
  return HeapSetInformation (heapHandle, HeapCompatibilityInformation, &lfh, sizeof (lfh));
}

bool p32_heap_terminate_on_corruption (uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;
  return HeapSetInformation (heapHandle, HeapEnableTerminationOnCorruption, NULL, 0);
}

#ifdef LIBPOSIX32_TEST
/**
 * Very old 32-bit CRTs do not support %zu; use %lu instead.
 */
#if P32_IS_ARCH(P32_ARCH_X64 | P32_ARCH_ARM64 | P32_ARCH_ARM32)
#define ZU L"%zu"
#else
#define ZU L"%lu"
#endif

void p32_heap_print_summary (uintptr_t heap) {
  HANDLE       heapHandle  = (HANDLE) heap;
  HEAP_SUMMARY heapSummary = {0};

  heapSummary.cb = sizeof (heapSummary);

  if (HeapSummary (heapHandle, 0, &heapSummary)) {
#ifdef _DEBUG_
    _RPTW1 (_CRT_WARN, L"Summary of heap <%p>:\n", heapHandle);
    _RPTW2 (_CRT_WARN, L"  MaxReserve: %zu (%zuKiB)\n", heapSummary.cbMaxReserve, heapSummary.cbMaxReserve / 1024);
    _RPTW2 (_CRT_WARN, L"  Reserved:   %zu (%zuKiB)\n", heapSummary.cbReserved, heapSummary.cbReserved / 1024);
    _RPTW2 (_CRT_WARN, L"  Committed:  %zu (%zuKiB)\n", heapSummary.cbCommitted, heapSummary.cbCommitted / 1024);
    _RPTW2 (_CRT_WARN, L"  Allocated:  %zu (%zuKiB)\n", heapSummary.cbAllocated, heapSummary.cbAllocated / 1024);
#else
    fwprintf (stderr, L"Summary of heap <%p>:\n", heapHandle);
    fwprintf (stderr, L"  MaxReserve: " ZU " (" ZU "KiB)\n", heapSummary.cbMaxReserve, heapSummary.cbMaxReserve / 1024);
    fwprintf (stderr, L"  Reserved:   " ZU " (" ZU "KiB)\n", heapSummary.cbReserved, heapSummary.cbReserved / 1024);
    fwprintf (stderr, L"  Committed:  " ZU " (" ZU "KiB)\n", heapSummary.cbCommitted, heapSummary.cbCommitted / 1024);
    fwprintf (stderr, L"  Allocated:  " ZU " (" ZU "KiB)\n", heapSummary.cbAllocated, heapSummary.cbAllocated / 1024);
#endif
  }
}
#endif
