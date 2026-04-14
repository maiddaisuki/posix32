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
#define STR(s)          TEXT (s)

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
#define STR(s)          s

#define GLOBAL_LOCALE_STRING     "en-US.OCP"
#define GLOBAL_LOCALE_NAME       "en-US.437"
#define GLOBAL_LOCALE_CRT_STRING "English_United States.437"
#define GLOBAL_LOCALE_CRT_NAME   GLOBAL_LOCALE_CRT_STRING

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

static DWORD CALLBACK Thread (LPVOID Param) {
  const char *localeName = (char *) Param;
  locale_t    locale     = NULL;

  /**
   * Set Thread Locale.
   */
  assert ((locale = newlocale (LC_ALL_MASK, TestThreadLocale.Locale, NULL)) != NULL);
  assert (uselocale (locale) == LC_GLOBAL_LOCALE);
  assert (strcmp (getlocalename_l (LC_ALL, uselocale (NULL)), TestThreadLocale.LocaleName) == 0);
#if P32_CRT >= P32_MSVCR80
  assert (_configthreadlocale (0) == _ENABLE_PER_THREAD_LOCALE);
  assert (wcscmp (_wsetlocale (LC_ALL, NULL), TestThreadLocale.LocaleString) == 0);
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
  assert (strcmp (getlocalename_l (LC_ALL, uselocale (NULL)), TestThreadLocale.LocaleName) == 0);
  assert (_configthreadlocale (0) == _ENABLE_PER_THREAD_LOCALE);
  assert (wcscmp (_wsetlocale (LC_ALL, NULL), TestThreadLocale.LocaleString) == 0);

  /**
   * Set Thread Locale back to Global Locale.
   */
  assert (uselocale (LC_GLOBAL_LOCALE) == locale);
  assert (_configthreadlocale (0) == _DISABLE_PER_THREAD_LOCALE);
  assert (wcscmp (_wsetlocale (LC_ALL, NULL), TestGlobalLocale.LocaleString) == 0);
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

  /**
   * Code page 10001 may not be supported/installed on old Windows versions.
   */
  if (!IsValidCodePage (10001)) {
    return 77;
  }

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

  if (P32_LOCALE_API & P32_LOCALE_API_LN) {
    TestGlobalLocale.LocaleString = GLOBAL_LOCALE_CRT_NAME;
    TestThreadLocale.LocaleString = THREAD_LOCALE_CRT_NAME;
  } else {
    TestGlobalLocale.LocaleString = GLOBAL_LOCALE_CRT_STRING;
    TestThreadLocale.LocaleString = THREAD_LOCALE_CRT_STRING;
  }

  /**
   * Set Global Locale.
   */
  assert (setlocale (LC_ALL, TestGlobalLocale.Locale) != NULL);
  assert (strcmp (getlocalename_l (LC_ALL, LC_GLOBAL_LOCALE), TestGlobalLocale.LocaleName) == 0);
  assert (CMP (SETLOCALE (LC_ALL, NULL), TestGlobalLocale.LocaleString) == 0);

  HANDLE thread   = NULL;
  DWORD  exitCode = 0;

  assert ((thread = CreateThread (NULL, 0, Thread, "ja-JP.10001", 0, NULL)) != NULL);

  WaitForSingleObject (thread, INFINITE);
  GetExitCodeThread (thread, &exitCode);
  CloseHandle (thread);

  return exitCode;
}
