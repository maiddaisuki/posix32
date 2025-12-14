/**
 * Copyright 2025 Kirill Makurin
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

#ifndef LIBPOSIX32_CRT_INTERNAL_H_INCLUDED
#define LIBPOSIX32_CRT_INTERNAL_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>

#include "posix32.h"

/**
 * Forward declaration.
 */
typedef struct ThreadLocale ThreadLocale;

/**
 * TLS structure.
 */
typedef struct {
  uintptr_t     Heap;
  ThreadLocale *ThreadLocale;
} ThreadStorage;

/**
 * Return current thread's TLS structure.
 *
 * If `allowNullTls` is `true`, this function may return `NULL` if
 * current thread's TLS was not initialized.
 */
ThreadStorage *p32_tls (bool allowNullTls);

/**
 * Deallocate Thread Locale from TLS structure `tls`.
 *
 * This function is called from `DllMain` when current thread terminates.
 */
void p32_destroy_thread_locale (ThreadStorage *tls);

#ifdef LIBPOSIX32_DLL
/**
 * Destroy Global Locale and free related resources.
 *
 * This function is called from `DllMain` when DLL is unloaded.
 */
void p32_destroy_global_locale (void);
#else
/**
 * Check if current thread's TLS structure contains any allocted data.
 *
 * If there is no more allocated data, current thread's TLS structure will be
 * freed.
 */
void p32_tls_check (void);
#endif

#ifdef LIBPOSIX32_TEST
typedef void (*TerminateHandler) (void);

/**
 * Set terminate handler to be invoked from `p32_terminate`.
 *
 * Some tests may need to test scenarios when process is expected to be
 * terminated by `p32_terminate`.
 *
 * In order to avoid marking such tests as `XFAIL`, those tests should
 * set terminate handler which will be called by `p32_terminate`.
 */
P32_TEST_DECL void p32_terminate_handler (TerminateHandler handler);
#endif

/**
 * Terminate current process.
 *
 * The `message` is sent to debugger (if present) before terminating
 * the process.
 *
 * The `context` must be point to `CONTEXT` strcture.
 */
P32_NORETURN void p32_terminate (const wchar_t *message, void *context);

/**
 * Convenience macro to call `p32_terminate` function and supply `CONTEXT`
 * structure for it.
 */
#define p32_terminate(msg)                            \
  do {                                                \
    CONTEXT context = {0};                            \
    GetThreadContext (GetCurrentThread (), &context); \
    p32_terminate (msg, &context);                    \
  } while (0)

#endif /* LIBPOSIX32_CRT_INTERNAL_H_INCLUDED */
