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
#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "tests-internal.h"

/**
 * File Summary:
 *
 * This file defines helper functions used by unit tests.
 */

/**
 * Strucutre used with `p32_locale_test_func*` functions.
 *
 * It is used to pass data to underlying callbacks.
 */
typedef struct LocaleTestFuncData {
  /**
   * `flags` argument passed to `p32_locale_test_func*`.
   */
  int Flags;
  /**
   * Heap to use for memory allocations from callbacks.
   */
  union {
    uintptr_t Heap;
    void     *HeapHandle;
  };
  /**
   * `callback` argument passed to `p32_locale_test_func*`.
   */
  union {
    LocaleCallback1 Callback1;
    LocaleCallback2 Callback2;
    LocaleCallback3 Callback3;
    LocaleCallback4 Callback4;
  };
} LocaleTestFuncData;

#include "common/crt_handle.c"
#include "common/locale_test_func1.c"
#include "common/locale_test_func2.c"
#include "common/locale_test_func3.c"
#include "common/locale_test_func4.c"
#include "common/test_init.c"
