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
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "tests-internal.h"

/**
 * Test Summary:
 *
 * When `setlocale` is called for the first time, if Global Locale was not yet
 * initialized by another function, it will attempt to initialize it from
 * CRT's global locale.
 *
 * In most cases CRT's global locale will be "C" and this will succeed.
 *
 * However, it is possible that posix32's `setlocale` will be called after
 * CRT's global locale was set to another locale.
 *
 * This test verifies that `setlocale` is capable of handling this scenario.
 */

#if P32_CRT >= P32_MSVCRT20
#define SETLOCALE(c, l) _wsetlocale (c, l)
#define CMP(s1, s2)     wcscmp (s1, s2)
#else
typedef const char *(*__cdecl setlocale_t) (int, const char *);
#define SETLOCALE(c, l) crt_setlocale (c, l)
#define CMP(s1, s2)     strcmp (s1, s2)
#endif

/**
 * `locale_t` object for "ro-RO" locale cannot be created with its default
 * ANSI code page.
 */
#if LOCALE_NAMES
#define LOCALE_STRING L"ro-RO"
#elif P32_CRT >= P32_MSVCRT20
#define LOCALE_STRING L"Romanian_Romania"
#else
#define LOCALE_STRING "norwegian-bokmal"
#endif

/**
 * Handler to run from `p32_terminate`.
 */
static void Handler (void) {
  _exit (EXIT_SUCCESS);
}

int main (void) {
  p32_test_init ();

#if P32_CRT >= P32_MSVCR70 || (P32_MSVCRT && _WIN32_WINNT >= _WIN32_WINNT_WIN2K)
  /**
   * Set CRT's global locale.
   */
  assert (SETLOCALE (LC_ALL, LOCALE_STRING) != NULL);
#elif P32_CRT >= P32_MSVCRT20 || P32_MSVCRT
  /**
   * Set CRT's global locale.
   */
  if (SETLOCALE (LC_ALL, LOCALE_STRING) == NULL) {
    return 77;
  }
#else
  setlocale_t crt_setlocale = NULL;

  if (GetACP () != CP_UTF8) {
    return 77;
  }

  HMODULE crt = (HMODULE) p32_crt_handle ();

  if (crt == NULL) {
    return 77;
  }

  assert ((crt_setlocale = (setlocale_t) (UINT_PTR) GetProcAddress (crt, "setlocale")) != NULL);

  /**
   * Set CRT's global locale.
   */
  assert (SETLOCALE (LC_ALL, LOCALE_STRING) != NULL);
#endif

  p32_terminate_handler (Handler);

  /**
   * `setlocale` must terminate process after failed attempt to initialize
   * Global Locale from CRT's global locale.
   */
  setlocale (LC_ALL, NULL);

  return EXIT_FAILURE;
}
