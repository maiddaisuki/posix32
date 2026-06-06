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

#include "core-atomic.h"
#include "core-heap.h"
#include "core-loader.h"
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
  uintptr_t kernel32 = p32_get_module_handle (P32_MODULE_NAME ("kernel32.dll"));
  assert (kernel32 != 0);

#if P32_WINNT < P32_WINNT_NT_3_5
  /**
   * Lookup `HeapLock` and `HeapUnlock`.
   */
  FuncHeapLock   ptrHeapLock   = NULL;
  FuncHeapUnlock ptrHeapUnlock = NULL;

  if (kernel32 != 0) {
    if (P32_PLATFORM_CHECK (P32_WINNT_NT_3_5, WindowsNt3_5, P32_WIN9X_95, Windows95)) {
      ptrHeapLock = p32_get_proc_address (kernel32, HeapLock);
      assert (ptrHeapLock != NULL);
      ptrHeapUnlock = p32_get_proc_address (kernel32, HeapUnlock);
      assert (ptrHeapUnlock != NULL);
    }
  }

  assert ((ptrHeapLock == NULL) == (ptrHeapUnlock == NULL));

  if (ptrHeapLock != NULL && ptrHeapUnlock != NULL) {
    p32_atomic_exchange_fpointer (&P32HeapApi.PtrHeapLock, ptrHeapLock);
    p32_atomic_exchange_fpointer (&P32HeapApi.PtrHeapUnlock, ptrHeapUnlock);
  } else {
    p32_atomic_exchange_fpointer (&P32HeapApi.PtrHeapLock, P32HeapLock);
    p32_atomic_exchange_fpointer (&P32HeapApi.PtrHeapUnlock, P32HeapUnlock);
  }
#endif /* P32_WINNT < Windows NT 3.5 */

#if P32_WINNT < P32_WINNT_NT_3_51 || P32_WIN9X
  /**
   * Lookup `HeapSummary`.
   */
  FuncHeapSummary ptrHeapSummary = NULL;

  if (kernel32 != 0) {
    if (P32_WINNT_CHECK (P32_WINNT_NT_3_51, WindowsNt3_51)) {
      ptrHeapSummary = p32_get_proc_address (kernel32, HeapSummary);
      assert (ptrHeapSummary != NULL);
    }
  }

  if (ptrHeapSummary != NULL) {
    p32_atomic_exchange_fpointer (&P32HeapApi.PtrHeapSummary, ptrHeapSummary);
  } else {
    p32_atomic_exchange_fpointer (&P32HeapApi.PtrHeapSummary, P32HeapSummary);
  }
#endif /* P32_WINNT < Windows NT 3.51 || Win9x */

#if P32_WINNT < P32_WINNT_XP || P32_WIN9X
  /**
   * Lookup `HeapSetInformation`.
   */
  FuncHeapSetInformation ptrHeapSetInformation = NULL;

  if (kernel32 != 0) {
    if (P32_WINNT_CHECK (P32_WINNT_XP, WindowsNtXP)) {
      ptrHeapSetInformation = p32_get_proc_address (kernel32, HeapSetInformation);
      assert (ptrHeapSetInformation != NULL);
    }
  }

  if (ptrHeapSetInformation != NULL) {
    p32_atomic_exchange_fpointer (&P32HeapApi.PtrHeapSetInformation, ptrHeapSetInformation);
  } else {
    p32_atomic_exchange_fpointer (&P32HeapApi.PtrHeapSetInformation, P32HeapSetInformation);
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
 * Internal Functions.
 */

/**
 * All CRTs prior to UCRT do not support %zu.
 */
#ifdef _WIN64
#define ZU L"%I64u"
#else /* _WIN32 */
#define ZU L"%lu"
#endif /* _WIN32 */

#ifdef LIBPOSIX32_TEST

#ifdef _DEBUG
#define P32HeapDbgMsg(format, ...) _CrtDbgReportW (_CRT_WARN, NULL, 0, NULL, format __VA_OPT__ (, ) __VA_ARGS__)
#define P32HeapDbgErr(format, ...) _CrtDbgReportW (_CRT_ERROR, NULL, 0, NULL, format __VA_OPT__ (, ) __VA_ARGS__)
#else
#define P32HeapDbgMsg(format, ...) fwprintf (stdout, format __VA_OPT__ (, ) __VA_ARGS__)
#define P32HeapDbgErr(format, ...) fwprintf (stderr, format __VA_OPT__ (, ) __VA_ARGS__)
#endif

#else

#define P32HeapDbgMsg(...) (void) 0
#define P32HeapDbgErr(...) (void) 0

#endif

#ifdef LIBPOSIX32_TEST
/**
 * Print heap summary for `heap`.
 */
static void P32HeapPrintSummary (uintptr_t heap) {
  HANDLE       heapHandle  = (HANDLE) heap;
  HEAP_SUMMARY heapSummary = {0};

  heapSummary.cb = sizeof (heapSummary);

  if (HeapSummary (heapHandle, 0, &heapSummary)) {
    P32HeapDbgMsg (L"Summary of heap <%p>:\n", heapHandle);
    P32HeapDbgMsg (L"  MaxReserve: " ZU " (" ZU "KiB)\n", heapSummary.cbMaxReserve, heapSummary.cbMaxReserve / 1024);
    P32HeapDbgMsg (L"  Reserved:   " ZU " (" ZU "KiB)\n", heapSummary.cbReserved, heapSummary.cbReserved / 1024);
    P32HeapDbgMsg (L"  Committed:  " ZU " (" ZU "KiB)\n", heapSummary.cbCommitted, heapSummary.cbCommitted / 1024);
    P32HeapDbgMsg (L"  Allocated:  " ZU " (" ZU "KiB)\n", heapSummary.cbAllocated, heapSummary.cbAllocated / 1024);
  }
}
#endif

/*******************************************************************************
 * External Functions.
 */

uintptr_t p32_heap_create (uint32_t createFlags, uint32_t flags, size_t initialSize, size_t maxSize) {
  /**
   * Low Fragmentation Heap and `HEAP_NO_SERIALIZE` are not compatible.
   *
   * Normally, if you create a heap with `HEAP_NO_SERIALIZE` flag,
   * you would just be unable to enable Low Fragmentation Heap afterwards.
   *
   * However, we fail completely to signal an inconsistency.
   */
  if ((createFlags & P32_HEAP_CREATE_LFH) && (flags & HEAP_NO_SERIALIZE)) {
    return 0;
  }

  HANDLE heapHandle = HeapCreate (flags, initialSize, maxSize);

  if (heapHandle == NULL) {
    return 0;
  }

  uintptr_t heap = (uintptr_t) heapHandle;

  /**
   * Enable Low Fragmentation Heap if requested.
   */
  if (createFlags & P32_HEAP_CREATE_LFH) {
    p32_heap_low_fragmentation (heap);
  }

  /**
   * Enable Terminate on Corruption feature if requested.
   */
  if (createFlags & P32_HEAP_CREATE_TERMINATE_ON_CORRUPTION) {
    p32_heap_terminate_on_corruption (heap);
  }

  return heap;
}

bool p32_heap_destroy (uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

#ifdef LIBPOSIX32_TEST
  P32HeapPrintSummary (heap);
#endif

  return HeapDestroy (heapHandle);
}

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
