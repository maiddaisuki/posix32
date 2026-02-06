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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "tests-internal.h"

/**
 * Test Summary:
 *
 * In addition to setting Thread Locale, `uselocale` also manages
 * CRT's thread locale state.
 *
 * When it is called to set Thread Locale, it changes thread locale state
 * to `_ENABLE_PER_THREAD_LOCALE`.
 *
 * When it is calles to set Thread Locale to Global Locale, it changes
 * thread locale state to `_DISABLE_PER_THREAD_LOCALE`.
 *
 * When `setlocale` is called from a thread which is using Thread Locale,
 * it must not affect calling thread's locale in any way.
 *
 * This tests verifies that:
 *
 * 1. `uselocale` is capable to correctly manage CRT's thread locale state
 * 2. interaction between `setlocale` and `uselocale` behaves as expected
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

#else /* crtdll.dll or msvcrt10.dll */
typedef const char *(*__cdecl setlocale_t) (int, const char *);

#define SETLOCALE(c, l) crt_setlocale (c, l)
#define CMP(s1, s2)     strcmp (s1, s2)
#define STR(s)          s

#define GLOBAL_LOCALE        "en-US.OCP"
#define GLOBAL_LOCALE_NAME   "en-US.437"
#define GLOBAL_LOCALE_STRING "English_United States.437"

#endif

#define THREAD_LOCALE      "ja-JP.ACP"
#define THREAD_LOCALE_NAME "ja-JP.932"
#if P32_LOCALE_NAMES
#define THREAD_LOCALE_STRING L"ja-JP"
#else
#define THREAD_LOCALE_STRING L"Japanese_Japan.932"
#endif

static DWORD CALLBACK Thread (LPVOID Param) {
  const char *localeName = (char *) Param;
  locale_t    locale     = NULL;

#if P32_CRT >= P32_MSVCR80
  wchar_t *global_locale_pre  = NULL;
  wchar_t *thread_locale_pre  = NULL;
  wchar_t *thread_locale_post = NULL;
  wchar_t *global_locale_post = NULL;
#endif

#if P32_CRT >= P32_MSVCR80
  assert (_configthreadlocale (0) == _DISABLE_PER_THREAD_LOCALE);
  assert ((global_locale_pre = _wcsdup (_wsetlocale (LC_ALL, NULL))) != NULL);
  _RPTW1 (_CRT_WARN, L"Thread Locale (CRT, global): %s\n", global_locale_pre);
  assert (wcscmp (global_locale_pre, L"C") == 0);
#endif

  /**
   * Set Thread Locale.
   */
  assert ((locale = newlocale (LC_ALL_MASK, localeName, NULL)) != NULL);
  assert (uselocale (locale) == LC_GLOBAL_LOCALE);
  _RPTW1 (_CRT_WARN, L"Thread Locale: %hs\n", getlocalename_l (LC_ALL, uselocale (NULL)));
  assert (strcmp (getlocalename_l (LC_ALL, uselocale (NULL)), THREAD_LOCALE_NAME) == 0);

#if P32_CRT >= P32_MSVCR80
  assert (_configthreadlocale (0) == _ENABLE_PER_THREAD_LOCALE);
  assert ((thread_locale_pre = _wcsdup (_wsetlocale (LC_ALL, NULL))) != NULL);
  _RPTW1 (_CRT_WARN, L"Thread Locale (CRT): %s\n", thread_locale_pre);
  assert (wcscmp (thread_locale_pre, THREAD_LOCALE_STRING) == 0);
#endif

  /**
   * Query Global Locale.
   */
  assert (strcmp (setlocale (LC_ALL, NULL), "C") == 0);
  assert (strcmp (getlocalename_l (LC_ALL, LC_GLOBAL_LOCALE), "C") == 0);

#if P32_CRT >= P32_MSVCR80
  assert (_configthreadlocale (0) == _ENABLE_PER_THREAD_LOCALE);
  assert (wcscmp (thread_locale_pre, _wsetlocale (LC_ALL, NULL)) == 0);
#endif

  /**
   * Set Global Locale.
   */
  assert (setlocale (LC_ALL, GLOBAL_LOCALE) != NULL);
  assert (strcmp (setlocale (LC_ALL, NULL), GLOBAL_LOCALE_NAME) == 0);
  assert (strcmp (getlocalename_l (LC_ALL, LC_GLOBAL_LOCALE), GLOBAL_LOCALE_NAME) == 0);

#if P32_CRT >= P32_MSVCR80
  assert (_configthreadlocale (0) == _ENABLE_PER_THREAD_LOCALE);
  assert ((thread_locale_post = _wcsdup (_wsetlocale (LC_ALL, NULL))) != NULL);
  _RPTW1 (_CRT_WARN, L"Thread Locale (CRT): %s\n", thread_locale_post);
  assert (wcscmp (thread_locale_pre, thread_locale_post) == 0);
#endif

  /**
   * Set Thread Locale to Global Locale.
   */
  assert (uselocale (LC_GLOBAL_LOCALE) == locale);
  _RPTW1 (_CRT_WARN, L"Thread Locale (global): %hs\n", getlocalename_l (LC_ALL, uselocale (NULL)));
  assert (strcmp (getlocalename_l (LC_ALL, uselocale (NULL)), GLOBAL_LOCALE_NAME) == 0);

#if P32_CRT >= P32_MSVCR80
  assert (_configthreadlocale (0) == _DISABLE_PER_THREAD_LOCALE);
  assert ((global_locale_post = _wcsdup (_wsetlocale (LC_ALL, NULL))) != NULL);
  _RPTW1 (_CRT_WARN, L"Thread Locale (CRT, global): %s\n", global_locale_post);
  assert (wcscmp (global_locale_post, GLOBAL_LOCALE_STRING) == 0);
#endif

  /**
   * `uselocale (LC_GLOBAL_LOCALE)` is a no-op if Thread Locale is set to
   * Global Locale.
   */
  assert (uselocale (LC_GLOBAL_LOCALE) == LC_GLOBAL_LOCALE);

#if P32_CRT >= P32_MSVCR80
  assert (_configthreadlocale (0) == _DISABLE_PER_THREAD_LOCALE);
  _RPTW1 (_CRT_WARN, L"Thread Locale (CRT, global): %s\n", _wsetlocale (LC_ALL, NULL));
  assert (wcscmp (_wsetlocale (LC_ALL, NULL), GLOBAL_LOCALE_STRING) == 0);
#endif

#if P32_CRT >= P32_MSVCR80
  free (global_locale_post);
  free (thread_locale_post);
  free (thread_locale_pre);
  free (global_locale_pre);
#endif

  freelocale (locale);

  return 0;
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
   * Set Global Locale to "POSIX" locale.
   */
  assert (setlocale (LC_ALL, "POSIX") != NULL);
  _RPTW1 (_CRT_WARN, L"Global Locale: %s\n", SETLOCALE (LC_ALL, NULL));
  assert (CMP (SETLOCALE (LC_ALL, NULL), STR ("C")) == 0);

  HANDLE thread   = NULL;
  DWORD  exitCode = 0;

  assert ((thread = CreateThread (NULL, 0, Thread, THREAD_LOCALE, 0, NULL)) != NULL);

  WaitForSingleObject (thread, INFINITE);
  GetExitCodeThread (thread, &exitCode);
  CloseHandle (thread);

  /**
   * Global Locale has been changed to `en-US`.
   */
  assert (strcmp (setlocale (LC_ALL, NULL), GLOBAL_LOCALE_NAME) == 0);
  _RPTW1 (_CRT_WARN, L"Global Locale: %s\n", SETLOCALE (LC_ALL, NULL));
  assert (CMP (SETLOCALE (LC_ALL, NULL), GLOBAL_LOCALE_STRING) == 0);

  return exitCode;
}
