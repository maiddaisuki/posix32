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
 * Call to `duplocale (LC_GLOBAL_LOCALE)` must create `locale_t` equivalent to
 * active Global Locale.
 *
 * If the call is made from a thread which uses Thread Locale, it must not
 * affect current Thread Locale in any way. Similarly, it must not affect
 * CRT's thread locale state, as well as CRT's thread locale.
 *
 * This test verifies that `duplocale` conforms to this behavior.
 */

typedef struct TestStrings {
  /**
   * Locale to set.
   */
  char *Locale;
  /**
   * Locale string expected to be returned by CRT's `[_w]setlocale`.
   */
#if P32_CRT >= P32_MSVCRT20
  wchar_t *LocaleString;
#else
  char *LocaleString;
#endif
  /**
   * Locale string expected to be returned by `getlocalename_l`.
   */
  char *LocaleName;
} TestStrings;

#if P32_CRT >= P32_MSVCRT20

#define SETLOCALE(c, l) _wsetlocale (c, l)
#define CMP(s1, s2)     wcscmp (s1, s2)

#define GLOBAL_LOCALE_STRING     "en-US.ACP"
#define GLOBAL_LOCALE_NAME       "en-US.1252"
#define GLOBAL_LOCALE_CRT_STRING L"English_United States.1252"
#define GLOBAL_LOCALE_CRT_NAME   L"en-US"

#define THREAD_LOCALE_STRING     "de-DE.ACP"
#define THREAD_LOCALE_NAME       "de-DE.1252"
#define THREAD_LOCALE_CRT_STRING L"German_Germany.1252"
#define THREAD_LOCALE_CRT_NAME   L"de-DE"

#else /* crtdll.dll or msvcrt10.dll */
typedef const char *(*__cdecl setlocale_t) (int, const char *);

#define SETLOCALE(c, l) crt_setlocale (c, l)
#define CMP(s1, s2)     strcmp (s1, s2)

#define GLOBAL_LOCALE_STRING     "en-US.OCP"
#define GLOBAL_LOCALE_NAME       "en-US.437"
#define GLOBAL_LOCALE_CRT_STRING "English_United States.437"
#define GLOBAL_LOCALE_CRT_NAME   THREAD_LOCALE_CRT_STRING

#define THREAD_LOCALE_STRING     "de-DE.OCP"
#define THREAD_LOCALE_NAME       "de-DE.850"
#define THREAD_LOCALE_CRT_STRING "German_Germany.850"
#define THREAD_LOCALE_CRT_NAME   THREAD_LOCALE_CRT_STRING

#endif

/**
 * Locale strings for Global Locale.
 */
static TestStrings TestGlobalLocale;

/**
 * Locale strings for Thread Locale.
 */
static TestStrings TestThreadLocale;

static DWORD CALLBACK Thread (LPVOID arg) {
  const char *localeString = arg;

  locale_t locale           = NULL;
  locale_t lc_global_locale = NULL;

#if P32_CRT >= P32_MSVCR80
  wchar_t *localestr_global_pre  = NULL;
  wchar_t *localestr_thread_pre  = NULL;
  wchar_t *localestr_thread_post = NULL;
  wchar_t *localestr_global_post = NULL;
#endif

#if P32_CRT >= P32_MSVCR80
  assert (_configthreadlocale (0) == _DISABLE_PER_THREAD_LOCALE);
  assert ((localestr_global_pre = (wcsdup (_wsetlocale (LC_ALL, NULL)))) != NULL);
  _RPTW1 (_CRT_WARN, L"Thread Locale (CRT, global): %s\n", localestr_global_pre);
  assert (wcscmp (localestr_global_pre, TestGlobalLocale.LocaleString) == 0);
#endif

  assert ((locale = newlocale (LC_ALL_MASK, localeString, NULL)) != NULL);
  assert (uselocale (locale) == LC_GLOBAL_LOCALE);
  _RPTW1 (_CRT_WARN, L"Thread Locale: %hs\n", getlocalename_l (LC_ALL, locale));
  assert (strcmp (getlocalename_l (LC_ALL, locale), TestThreadLocale.LocaleName) == 0);

#if P32_CRT >= P32_MSVCR80
  assert (_configthreadlocale (0) == _ENABLE_PER_THREAD_LOCALE);
  assert ((localestr_thread_pre = (wcsdup (_wsetlocale (LC_ALL, NULL)))) != NULL);
  _RPTW1 (_CRT_WARN, L"Thread Locale (CRT): %s\n", localestr_thread_pre);
  assert (wcscmp (localestr_thread_pre, TestThreadLocale.LocaleString) == 0);
#endif

  assert ((lc_global_locale = duplocale (LC_GLOBAL_LOCALE)) != NULL);
  _RPTW1 (_CRT_WARN, L"LC_GLOBAL_LOCALE: %hs\n", getlocalename_l (LC_ALL, lc_global_locale));
  assert (strcmp (getlocalename_l (LC_ALL, lc_global_locale), TestGlobalLocale.LocaleName) == 0);

#if P32_CRT >= P32_MSVCR80
  assert (_configthreadlocale (0) == _ENABLE_PER_THREAD_LOCALE);
  assert ((localestr_thread_post = (wcsdup (_wsetlocale (LC_ALL, NULL)))) != NULL);
  _RPTW1 (_CRT_WARN, L"Thread Locale (CRT): %s\n", localestr_thread_post);
  assert (wcscmp (localestr_thread_pre, localestr_thread_post) == 0);
#endif

  assert (uselocale (LC_GLOBAL_LOCALE) == locale);

#if P32_CRT >= P32_MSVCR80
  assert (_configthreadlocale (0) == _DISABLE_PER_THREAD_LOCALE);
  assert ((localestr_global_post = (wcsdup (_wsetlocale (LC_ALL, NULL)))) != NULL);
  _RPTW1 (_CRT_WARN, L"Thread Locale (CRT, global): %s\n", localestr_global_post);
  assert (wcscmp (localestr_global_pre, localestr_global_post) == 0);
#endif

#if P32_CRT >= P32_MSVCR80
  free (localestr_global_post);
  free (localestr_thread_post);
  free (localestr_thread_pre);
  free (localestr_global_pre);
#endif

  freelocale (lc_global_locale);
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

  TestGlobalLocale.Locale     = GLOBAL_LOCALE_STRING;
  TestGlobalLocale.LocaleName = GLOBAL_LOCALE_NAME;

  TestThreadLocale.Locale     = THREAD_LOCALE_STRING;
  TestThreadLocale.LocaleName = THREAD_LOCALE_NAME;

  if (P32_CRT >= P32_MSVCR110 && P32_WINNT_CHECK (P32_WINNT_VISTA, WindowsNtVista)) {
    TestGlobalLocale.LocaleString = GLOBAL_LOCALE_CRT_NAME;
    TestThreadLocale.LocaleString = THREAD_LOCALE_CRT_NAME;
  } else {
    TestGlobalLocale.LocaleString = GLOBAL_LOCALE_CRT_STRING;
    TestThreadLocale.LocaleString = THREAD_LOCALE_CRT_STRING;
  }

  assert (setlocale (LC_ALL, TestGlobalLocale.Locale) != NULL);
  assert (strcmp (getlocalename_l (LC_ALL, LC_GLOBAL_LOCALE), TestGlobalLocale.LocaleName) == 0);
  _RPTW1 (_CRT_WARN, L"Global Locale: %s\n", SETLOCALE (LC_ALL, NULL));
  assert (CMP (SETLOCALE (LC_ALL, NULL), TestGlobalLocale.LocaleString) == 0);

  DWORD  exitCode = EXIT_FAILURE;
  HANDLE thread   = NULL;

  assert ((thread = CreateThread (NULL, 0, Thread, TestThreadLocale.Locale, 0, NULL)) != NULL);

  WaitForSingleObject (thread, INFINITE);
  GetExitCodeThread (thread, &exitCode);
  CloseHandle (thread);

  assert (strcmp (getlocalename_l (LC_ALL, LC_GLOBAL_LOCALE), TestGlobalLocale.LocaleName) == 0);
  _RPTW1 (_CRT_WARN, L"Global Locale: %s\n", SETLOCALE (LC_ALL, NULL));
  assert (CMP (SETLOCALE (LC_ALL, NULL), TestGlobalLocale.LocaleString) == 0);

  return exitCode;
}
