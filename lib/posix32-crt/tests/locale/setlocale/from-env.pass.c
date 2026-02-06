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
 * When `setlocale` is called with empty string for the second argument,
 * it must set locale from LC_* and LANG environment variables.
 *
 * This test verifies that `setlocale` is capable of handling this scenario.
 */

/**
 * TODO: use `setenv` once implemented in posix32-crt.
 */

#if P32_CRT >= P32_MSVCRT20

#define SETLOCALE(c, l) _wsetlocale (c, l)
#define CMP(s1, s2)     wcscmp (s1, s2)

/**
 * "en-US" with its default ANSI code page the safest locale we can test.
 */
#define LOCALE      L"en-US.ACP"
#define LOCALE_NAME "en-US.1252"
#if P32_LOCALE_NAMES
#define LOCALE_STRING L"en-US"
#else
#define LOCALE_STRING L"English_United States.1252"
#endif

#else /* crtdll.dll or msvcrt10.dll */
typedef const char *(*__cdecl setlocale_t) (int, const char *);

#define SETLOCALE(c, l) crt_setlocale (c, l)
#define CMP(s1, s2)     strcmp (s1, s2)

/**
 * "en-US" with its default OEM code page the safest locale we can test.
 */
#define LOCALE          L"en-US.OCP"
#define LOCALE_NAME     "en-US.437"
#define LOCALE_STRING   "English_United States.437"

#endif

int main (void) {
  p32_test_init ();

#if P32_CRT <= P32_MSVCRT10
  setlocale_t crt_setlocale = NULL;

  HMODULE crt = (HMODULE) p32_crt_handle ();

  if (crt == NULL) {
    return 77;
  }

  assert ((crt_setlocale = (setlocale_t) (UINT_PTR) GetProcAddress (crt, "setlocale")) != NULL);
#endif

  assert (SetEnvironmentVariableW (L"LC_ALL", LOCALE));

  /**
   * Set locale from environment variable `LC_ALL`.
   */
  assert (setlocale (LC_ALL, "") != NULL);
  assert (strcmp (getlocalename_l (LC_ALL, LC_GLOBAL_LOCALE), LOCALE_NAME) == 0);
  _RPTW1 (_CRT_WARN, L"Global Locale (CRT): %s\n", SETLOCALE (LC_ALL, NULL));
  assert (CMP (SETLOCALE (LC_ALL, NULL), LOCALE_STRING) == 0);

  return 0;
}
