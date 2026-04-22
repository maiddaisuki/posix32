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

#include "core-memory.h"
#include "core-runtime.h"

#include "crt-internal.h"

/**
 * File Summary:
 *
 * This file defines functions to manipulate TLS.
 */

/**
 * Suppress warnings about conversion between data and function pointers with
 * picky compilers.
 */
#define F(ptr) (PVOID) (UINT_PTR) ptr

/**
 * Convenience wrapper for `InterlockedExchangePointer`.
 */
#define P32AtomicExchange(target, source) InterlockedExchangePointer ((void *volatile *) target, F (source))

#ifndef LIBPOSIX32_DLL
/**
 * Function type corresponding to `TlsIndex`;
 * returns TLS index used by the library.
 */
typedef uint32_t (*FuncTlsIndex) (void);

/**
 * Initialization thunk for `TlsIndex`.
 */
static uint32_t P32InitTlsIndex (void);

/**
 * Get TLS index used by the library.
 */
static uint32_t P32TlsIndex (void);
#endif /* Static build */

/**
 * Structure to store TLS Index used by the library.
 *
 * For static builds, we use `pthread_once` to allocate TLS index.
 */
typedef struct Tls {
#ifndef LIBPOSIX32_DLL
  pthread_once_t Init;
  /**
   * Pointer to `TlsIndex` implementation.
   */
  FuncTlsIndex PtrTlsIndex;
#endif /* Static build */
  /**
   * TLS index used by the library.
   */
  uint32_t Index;
} Tls;

/**
 * TLS Index used by the library.
 */
static Tls P32Tls = {
#ifndef LIBPOSIX32_DLL
  .Init        = PTHREAD_ONCE_INIT,
  .PtrTlsIndex = P32InitTlsIndex,
#endif /* Static build */
  .Index = TLS_OUT_OF_INDEXES,
};

#ifndef LIBPOSIX32_DLL
/**
 * Initialize `P32Tls`.
 */
static void P32TlsInit (void) {
  P32Tls.Index = TlsAlloc ();

  if (P32Tls.Index == TLS_OUT_OF_INDEXES) {
    p32_terminate (L"TLS: failed to allocate TLS index.");
  }

  P32AtomicExchange (&P32Tls.PtrTlsIndex, P32TlsIndex);
}
#endif /* Static build */

#ifdef LIBPOSIX32_DLL
#define TlsIndex P32TlsIndex
#else /* Static build */
#define TlsIndex P32Tls.PtrTlsIndex

static uint32_t P32InitTlsIndex (void) {
  pthread_once (&P32Tls.Init, P32TlsInit);
  return TlsIndex ();
}
#endif /* Static build */

static uint32_t P32TlsIndex (void) {
  return P32Tls.Index;
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

  uintptr_t heap = (uintptr_t) heapHandle;

  /**
   * Request Low-Fragmentation Heap.
   */
  p32_heap_low_fragmentation (heap);

  /**
   * Request termination if heap corruption has occured.
   */
  p32_heap_terminate_on_corruption (heap);

  /**
   * TODO: is there any performance gain from calling `HeapLock`?
   */
  p32_heap_lock (heap);

  /**
   * Allocate TLS structure.
   */
  ThreadStorage *tls = HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, sizeof (ThreadStorage));

  if (tls == NULL) {
    p32_terminate (L"TLS: failed to allocate storage.");
  }

  tls->Heap = heap;

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
  uintptr_t heap       = tls->Heap;
  HANDLE    heapHandle = (HANDLE) heap;

  tls->Heap = 0;

  if (!HeapFree (heapHandle, 0, tls)) {
    p32_terminate (L"TLS: failed to deallocate storage.");
  }

#if defined(LIBPOSIX32_TEST)
  p32_heap_print_summary (heap);
#endif

  /**
   * TODO: is there any performance gain from calling `HeapLock`?
   */
  p32_heap_unlock (heap);

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
  uint32_t       tlsIndex = TlsIndex ();
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
      P32Tls.Index = TlsAlloc ();

      if (P32Tls.Index == TLS_OUT_OF_INDEXES) {
        return FALSE;
      }
    }
    /* FALLTHROUGH */

    /**
     * New thread is created.
     */
    case DLL_THREAD_ATTACH: {
      P32InitTls (TlsIndex ());
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

      if (!TlsFree (P32Tls.Index)) {
        p32_terminate (L"TLS: failed to free TLS index.");
      }

      P32Tls.Index = TLS_OUT_OF_INDEXES;

      p32_destroy_global_locale_state ();
      break;
    }
  }

  return TRUE;
  UNREFERENCED_PARAMETER (Module);
  UNREFERENCED_PARAMETER (Reserved);
}
#else  /* Static build */
void p32_tls_check (void) {
  uint32_t       tlsIndex = TlsIndex ();
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
#endif /* Static build */

ThreadStorage *p32_tls (bool allowNullTls) {
  uint32_t       tlsIndex = TlsIndex ();
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
