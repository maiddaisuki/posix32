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

#ifndef LIBPOSIX32_CORE_RUNTIME_H_INCLUDED
#define LIBPOSIX32_CORE_RUNTIME_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

#include "posix32.h"

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
 * The `context` must be point to `CONTEXT` structure.
 */
P32_CORE_DECL P32_NORETURN void p32_terminate (const wchar_t *message, void *context);

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

#endif /* LIBPOSIX32_CORE_RUNTIME_H_INCLUDED */
