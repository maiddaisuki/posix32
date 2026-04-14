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
 * However, it possible that CRT's thread locale was set prior to call to
 * `uselocale`. In this case `uselocale` will attempt to initialize
 * Thread Locale from CRT's thread locale.
 *
 * This test verifies that `uselocale` is capable of handling this scenario.
 */

typedef struct TestStrings {
  /**
   * Locale to set.
   */
  char *Locale;
  /**
   * Locale string expected to be returned by CRT's `[_w]setlocale`.
   */
  wchar_t *LocaleString;
  /**
   * Locale string expected to be returned by `getlocalename_l`.
   */
  char *LocaleName;
} TestStrings;

#define GLOBAL_LOCALE_STRING     "en-US.ACP"
#define GLOBAL_LOCALE_NAME       "en-US.1252"
#define GLOBAL_LOCALE_CRT_STRING L"English_United States.1252"
#define GLOBAL_LOCALE_CRT_NAME   L"en-US"

#define THREAD_LOCALE_STRING     "de-DE.ACP"
#define THREAD_LOCALE_NAME       "de-DE.1252"
#define THREAD_LOCALE_CRT_STRING L"German_Germany.1252"
#define THREAD_LOCALE_CRT_NAME   L"de-DE"

#if P32_CRT >= P32_MSVCR80
/**
 * Locale strings for Global Locale.
 */
static TestStrings TestGlobalLocale;

/**
 * Locale strings for Thread Locale.
 */
static TestStrings TestThreadLocale;

static DWORD CALLBACK Thread (LPVOID Param) {
  const wchar_t *localeString = (const wchar_t *) Param;

  /**
   * Set CRT's thread locale to `localeString`.
   */
  assert (_configthreadlocale (_ENABLE_PER_THREAD_LOCALE) != -1);
  assert (_wsetlocale (LC_ALL, localeString) != NULL);
  _RPTW1 (_CRT_WARN, L"Thread Locale (CRT): %s\n", _wsetlocale (LC_ALL, NULL));
  assert (wcscmp (_wsetlocale (LC_ALL, NULL), TestThreadLocale.LocaleString) == 0);

  /**
   * Set Thread Locale to Global Locale.
   *
   * This will initialize Thread Locale from CRT's thread locale and then
   * set both to Global Locale.
   */
  assert (uselocale (LC_GLOBAL_LOCALE) == LC_GLOBAL_LOCALE);

  assert (_configthreadlocale (0) == _DISABLE_PER_THREAD_LOCALE);
  _RPTW1 (_CRT_WARN, L"Thread Locale (CRT, global): %s\n", _wsetlocale (LC_ALL, NULL));
  assert (wcscmp (_wsetlocale (LC_ALL, NULL), TestGlobalLocale.LocaleString) == 0);

  return 0;
}
#endif

int main (void) {
#if P32_CRT >= P32_MSVCR80
  p32_test_init ();

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
  _RPTW1 (_CRT_WARN, L"Global Locale: %s\n", _wsetlocale (LC_ALL, NULL));
  assert (wcscmp (_wsetlocale (LC_ALL, NULL), TestGlobalLocale.LocaleString) == 0);

  HANDLE thread   = NULL;
  DWORD  exitCode = 0;

  assert ((thread = CreateThread (NULL, 0, Thread, TestThreadLocale.LocaleString, 0, NULL)) != NULL);

  WaitForSingleObject (thread, INFINITE);
  GetExitCodeThread (thread, &exitCode);
  CloseHandle (thread);

  assert (strcmp (getlocalename_l (LC_ALL, LC_GLOBAL_LOCALE), TestGlobalLocale.LocaleName) == 0);
  _RPTW1 (_CRT_WARN, L"Global Locale: %s\n", _wsetlocale (LC_ALL, NULL));
  assert (wcscmp (_wsetlocale (LC_ALL, NULL), TestGlobalLocale.LocaleString) == 0);

  return exitCode;
#else
  return 77;
#endif
}
