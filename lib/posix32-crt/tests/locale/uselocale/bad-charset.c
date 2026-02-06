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
 * Attempt to set Thread Locale with a code page which can be used to create
 * `locale_t` object, but is not supported by CRT.
 *
 * An example of such code page is 10001 (MAC Japanese). This code page is
 * supported by Windows and we can create `locale_t` using this code page,
 * but it cannot be set for CRT's LC_CTYPE locale category.
 *
 * Since we emulate Thread Locale for CRTs which do not support them natively,
 * such code pages can be set for Thread Locale for those CRTs.
 */

#if P32_CRT >= P32_MSVCRT20

#define SETLOCALE(c, l) _wsetlocale (c, l)
#define CMP(s1, s2)     wcscmp (s1, s2)
#define STR(s)          TEXT (s)

#define GLOBAL_LOCALE      "en-US.ACP"
#define GLOBAL_LOCALE_NAME "en-US.1252"
#if P32_LOCALE_NAMES
#define GLOBAL_LOCALE_STRING L"en-US"
#else
#define GLOBAL_LOCALE_STRING L"English_United States.1252"
#endif

#define THREAD_LOCALE      "de-DE.ACP"
#define THREAD_LOCALE_NAME "de-DE.1252"
#if P32_LOCALE_NAMES
#define THREAD_LOCALE_STRING L"de-DE"
#else
#define THREAD_LOCALE_STRING L"German_Germany.1252"
#endif

#else /* crtdll.dll or msvcrt10.dll */
typedef const char *(*__cdecl setlocale_t) (int, const char *);

#define SETLOCALE(c, l) crt_setlocale (c, l)
#define CMP(s1, s2)     strcmp (s1, s2)
#define STR(s)          s

#define GLOBAL_LOCALE        "en-US.OCP"
#define GLOBAL_LOCALE_NAME   "en-US.437"
#define GLOBAL_LOCALE_STRING "English_United States.437"

#define THREAD_LOCALE        "de-DE.OCP"
#define THREAD_LOCALE_NAME   "de-DE.850"
#define THREAD_LOCALE_STRING L"German_Germany.850"

#endif

static DWORD CALLBACK Thread (LPVOID Param) {
  const char *localeName = (char *) Param;
  locale_t    locale     = NULL;

  /**
   * Set Thread Locale.
   */
  assert ((locale = newlocale (LC_ALL_MASK, THREAD_LOCALE, NULL)) != NULL);
  assert (uselocale (locale) == LC_GLOBAL_LOCALE);
  assert (strcmp (getlocalename_l (LC_ALL, uselocale (NULL)), THREAD_LOCALE_NAME) == 0);
#if P32_CRT >= P32_MSVCR80
  assert (_configthreadlocale (0) == _ENABLE_PER_THREAD_LOCALE);
  assert (wcscmp (_wsetlocale (LC_ALL, NULL), THREAD_LOCALE_STRING) == 0);
#endif

  locale_t bad_locale = NULL;
  assert ((bad_locale = newlocale (LC_ALL_MASK, localeName, NULL)) != NULL);

#if P32_CRT >= P32_MSVCR80
  /**
   * Attempt to set Thread Locale using code page unsupported by CRT.
   */
  assert (uselocale (bad_locale) == NULL);

  /**
   * Verify that Thread Locale has not changed.
   */
  assert (strcmp (getlocalename_l (LC_ALL, uselocale (NULL)), THREAD_LOCALE_NAME) == 0);
  assert (_configthreadlocale (0) == _ENABLE_PER_THREAD_LOCALE);
  assert (wcscmp (_wsetlocale (LC_ALL, NULL), THREAD_LOCALE_STRING) == 0);

  /**
   * Set Thread Locale back to Global Locale.
   */
  assert (uselocale (LC_GLOBAL_LOCALE) == locale);
  assert (_configthreadlocale (0) == _DISABLE_PER_THREAD_LOCALE);
  assert (wcscmp (_wsetlocale (LC_ALL, NULL), GLOBAL_LOCALE_STRING) == 0);
#else
  /**
   * Attempt to set Thread Locale using code page unsupported by CRT.
   */
  assert (uselocale (bad_locale) == locale);
  assert (strcmp (getlocalename_l (LC_ALL, uselocale (NULL)), localeName) == 0);

  /**
   * Set Thread Locale back to Global Locale.
   */
  assert (uselocale (LC_GLOBAL_LOCALE) == bad_locale);
#endif

  freelocale (bad_locale);
  freelocale (locale);

  return EXIT_SUCCESS;
}

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

  /**
   * Set Global Locale.
   */
  assert (setlocale (LC_ALL, GLOBAL_LOCALE) != NULL);
  assert (strcmp (getlocalename_l (LC_ALL, LC_GLOBAL_LOCALE), GLOBAL_LOCALE_NAME) == 0);
  assert (CMP (SETLOCALE (LC_ALL, NULL), GLOBAL_LOCALE_STRING) == 0);

  HANDLE thread   = NULL;
  DWORD  exitCode = 0;

  assert ((thread = CreateThread (NULL, 0, Thread, "ja-JP.10001", 0, NULL)) != NULL);

  WaitForSingleObject (thread, INFINITE);
  GetExitCodeThread (thread, &exitCode);
  CloseHandle (thread);

  return exitCode;
}
