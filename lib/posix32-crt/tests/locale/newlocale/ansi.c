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
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "tests-internal.h"

/**
 * Test Summary:
 *
 * Attempt to create `locale_t` object for each locale supported by the
 * operating system with their default ANSI code page.
 *
 * This test determines which locales can be used with their default
 * ANSI code page.
 *
 * We maintain list of locales which are known to not work with their default
 * ANSI code pages. Any "FAIL" or "XPASS" must be investigated.
 */

#define P32_NEWLOCALE_TEST P32_NEWLOCALE_TEST_ANSI
#include "common/newlocale.c"
