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
#include <stdint.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef LIBPOSIX32_DLL
#define _DECL_DLLMAIN
#include <process.h>
#else
#include <pthread.h>
#endif

#include "crt-internal.h"

/**
 * File Summary:
 *
 * This file defines functions to manipulate TLS.
 */

/**
 * TLS index used by the library.
 */
static DWORD P32Tls = TLS_OUT_OF_INDEXES;

#ifndef LIBPOSIX32_DLL
/**
 * Allocate TLS index.
 */
static void P32TlsAlloc (void) {
  P32Tls = TlsAlloc ();

  if (P32Tls == TLS_OUT_OF_INDEXES) {
    p32_terminate (L"TLS: failed to allocate TLS index.");
  }
}
#endif

/**
 * Get TLS index.
 */
static uint32_t P32TlsIndex (void) {
#ifndef LIBPOSIX32_DLL
  /**
   * Use `pthread_once` instead of `DllMain` to allocate TLS.
   */
  static pthread_once_t P32TlsOnce = PTHREAD_ONCE_INIT;
  pthread_once (&P32TlsOnce, P32TlsAlloc);
#endif

  return P32Tls;
}

/**
 * Initialize current thread's TLS.
 */
static ThreadStorage *P32InitTls (uint32_t tlsIndex) {
  HANDLE heapHandle = HeapCreate (HEAP_GENERATE_EXCEPTIONS, 4096, 0);

  if (heapHandle == NULL) {
    _RPTW0 (_CRT_ERROR, L"TLS: failed to create private heap.\n");
    return NULL;
  }

  /**
   * Request Low-Fragmentation Heap.
   */
  ULONG lfh = 2;

  HeapSetInformation (heapHandle, HeapCompatibilityInformation, &lfh, sizeof (ULONG));

  /**
   * Request termination if heap corruption has occured.
   */
  HeapSetInformation (heapHandle, HeapEnableTerminationOnCorruption, NULL, 0);

  /**
   * TODO: is there any performance gain from calling `HeapLock`?
   */
  HeapLock (heapHandle);

  /**
   * Allocate TLS structure.
   */
  ThreadStorage *tls = HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, sizeof (ThreadStorage));

  if (tls == NULL) {
    p32_terminate (L"TLS: failed to allocate storage.");
  }

  tls->Heap = (uintptr_t) heapHandle;

  if (!TlsSetValue (tlsIndex, tls)) {
    p32_terminate (L"TLS: failed to set.");
  }

  return tls;
}

/**
 * Free current thread's TLS.
 *
 * This function dallocated TLS structure itself and destroys its private heap.
 * When this function is called, all data stored in the TLS must be freed.
 */
static void P32FreeTls (ThreadStorage *tls, uint32_t tlsIndex) {
  HANDLE heapHandle = (HANDLE) tls->Heap;
  tls->Heap         = 0;

  if (!HeapFree (heapHandle, 0, tls)) {
    p32_terminate (L"TLS: failed to deallocate storage.");
  }

#if defined(LIBPOSIX32_TEST) && defined(_DEBUG)
  HEAP_SUMMARY heapSummary = {0};
  heapSummary.cb           = sizeof (heapSummary);

  if (HeapSummary (heapHandle, 0, &heapSummary)) {
    _RPTW1 (_CRT_WARN, L"TLS: heap <%p> is about to be destroyed.\n", heapHandle);
    _RPTW1 (_CRT_WARN, L"  cbAllocated=%zu\n", heapSummary.cbAllocated);
    _RPTW1 (_CRT_WARN, L"  cbCommitted=%zu\n", heapSummary.cbCommitted);
    _RPTW1 (_CRT_WARN, L"  cbMaxReserve=%zu\n", heapSummary.cbMaxReserve);
    _RPTW1 (_CRT_WARN, L"  cbReserved=%zu\n", heapSummary.cbReserved);
  }
#endif

  /**
   * TODO: is there any performance gain from calling `HeapLock`?
   */
  HeapUnlock (heapHandle);

  if (!HeapDestroy (heapHandle)) {
    p32_terminate (L"TLS: failed to destroy private heap.");
  }

  if (!TlsSetValue (tlsIndex, NULL)) {
    p32_terminate (L"TLS: failed to set.");
  }
}

#ifdef LIBPOSIX32_DLL
/**
 * Destroy current thread's TLS.
 */
static void P32DestroyTls (void) {
  uint32_t       tlsIndex = P32TlsIndex ();
  ThreadStorage *tls      = TlsGetValue (tlsIndex);

  if (tls == NULL && GetLastError () != ERROR_SUCCESS) {
    p32_terminate (L"TLS: failed to access.");
  }

  /**
   * Current thread's TLS was not initialized.
   */
  if (tls == NULL) {
    return;
  }

  /**
   * Destroy thread locale.
   */
  p32_destroy_thread_locale (tls);

  /**
   * Free TLS.
   */
  P32FreeTls (tls, tlsIndex);
}

BOOL APIENTRY DllMain (HANDLE Module, DWORD Reason, LPVOID Reserved) {
  switch (Reason) {
    /**
     * DLL is being loaded.
     */
    case DLL_PROCESS_ATTACH: {
      P32Tls = TlsAlloc ();

      if (P32Tls == TLS_OUT_OF_INDEXES) {
        return FALSE;
      }
    }
    /* FALLTHROUGH */

    /**
     * New thread is created.
     */
    case DLL_THREAD_ATTACH: {
      P32InitTls (P32TlsIndex ());
      break;
    }

    /**
     * Thread terminates normally.
     */
    case DLL_THREAD_DETACH: {
      P32DestroyTls ();
      break;
    }

    /**
     * DLL is being unloaded or current thread (which can be the main thread)
     * caused process to exit normally (e.g. called `ExitProcess`).
     */
    case DLL_PROCESS_DETACH: {
      P32DestroyTls ();

      if (!TlsFree (P32Tls)) {
        p32_terminate (L"TLS: failed to free TLS index.");
      }

      P32Tls = TLS_OUT_OF_INDEXES;

      p32_destroy_global_locale ();
      break;
    }
  }

  return TRUE;
  UNREFERENCED_PARAMETER (Module);
  UNREFERENCED_PARAMETER (Reserved);
}
#else
void p32_tls_check (void) {
  uint32_t       tlsIndex = P32TlsIndex ();
  ThreadStorage *tls      = TlsGetValue (tlsIndex);

  if (tls == NULL && GetLastError () != ERROR_SUCCESS) {
    p32_terminate (L"TLS: failed to access.");
  }

  /**
   * Current thread's TLS was not initialized.
   */
  if (tls == NULL) {
    return;
  }

  /**
   * TLS is not empty.
   */
  if (tls->ThreadLocale != NULL) {
    return;
  }

  /**
   * Free TLS.
   */
  P32FreeTls (tls, tlsIndex);
}
#endif

ThreadStorage *p32_tls (bool allowNullTls) {
  uint32_t       tlsIndex = P32TlsIndex ();
  ThreadStorage *tls      = TlsGetValue (tlsIndex);

  if (tls == NULL && GetLastError () != ERROR_SUCCESS) {
    p32_terminate (L"TLS: failed to access.");
  }

  /**
   * If current thread's TLS is not initialized and `allowNullTls` is `false`,
   * initialize it now.
   */
  if (tls == NULL && !allowNullTls) {
    tls = P32InitTls (tlsIndex);

    if (tls == NULL) {
      p32_terminate (L"TLS: initialization has failed.");
    }
  }

  return tls;
  UNREFERENCED_PARAMETER (allowNullTls);
}
