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
 * Flags passed to `p32_locale_test_func2` or `p32_locale_test_func4`.
 */
static int P32LocaleTestFlags = 0;

#include "common/crt_handle.c"
#include "common/locale_test_func1.c"
#include "common/locale_test_func2.c"
#include "common/locale_test_func3.c"
#include "common/locale_test_func4.c"
#include "common/test_init.c"
