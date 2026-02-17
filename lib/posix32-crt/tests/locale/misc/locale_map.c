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
 * This test verifies that our locale string parser (`p32_locale_map`) is
 * capable to parse all locale string which can be constructed from information
 * returned by `GetLocaleInfo[Ex]` functions for each locales supported by
 * the operating system.
 *
 * Example:
 *
 * Given `en-US` locale, the following strings can be constructed:
 *
 *  - English_United States
 *  - English_US
 *  - English_USA
 *  - en_United States
 *  - en_US
 *  - en_USA
 *  - eng_United States
 *  - eng_US
 *  - eng_USA
 *
 * Parser must be able to parse all of them.
 *
 * Note that there are string which are very unlikely to be used in practice.
 */

static int exit_code = EXIT_SUCCESS;

/**
 * Try to parse locale string constructed from `ll` and `cc`.
 */
static void DoTest (LPCWSTR ll, LPCWSTR cc) {
  HANDLE    heapHandle = GetProcessHeap ();
  uintptr_t heap       = (uintptr_t) heapHandle;

  LPWSTR    localeString = NULL;
  LocaleMap localeMap    = {0};

  assert (p32_private_aswprintf (&localeString, heap, L"%s_%s", ll, cc) != -1);

  if (!p32_locale_map (&localeMap, localeString, heap)) {
    exit_code = EXIT_FAILURE;
    fwprintf (stderr, L"FAIL: %s\n", localeString);
  }

  assert (HeapFree (heapHandle, 0, localeString));
}

/**
 * Try to parse locale name `locale->LocaleName`.
 */
static void DoTestLocaleName (Locale *locale) {
  HANDLE    heapHandle = GetProcessHeap ();
  uintptr_t heap       = (uintptr_t) heapHandle;

  LocaleMap localeMap = {0};

  if (!p32_locale_map (&localeMap, locale->LocaleName, heap)) {
    exit_code = EXIT_FAILURE;
    fwprintf (stderr, L"FAIL: %s\n", locale->LocaleName);
  }
}

static bool __cdecl Test (Locale *locale) {
  HANDLE    heapHandle = GetProcessHeap ();
  uintptr_t heap       = (uintptr_t) heapHandle;

  DoTestLocaleName (locale);

  /**
   * Language name
   */
  LPWSTR LanguageName = NULL;

  /**
   * Country name
   */
  LPWSTR CountryName = NULL;

  /**
   * ISO 639 language code
   */
  LPWSTR Ll = NULL;

  /**
   * ISO 3166 country code
   */
  LPWSTR Cc = NULL;

#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
  /**
   * ISO 639-2 language code
   */
  LPWSTR Lll = NULL;

  /**
   * ISO 3166-2 country code
   */
  LPWSTR Ccc = NULL;
#endif

  assert (p32_winlocale_get_language_name (&LanguageName, heap, locale));
  assert (p32_winlocale_get_country_name (&CountryName, heap, locale));
  assert (p32_winlocale_get_language_code (&Ll, heap, locale));
  assert (p32_winlocale_get_country_code (&Cc, heap, locale));
#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
  do {
    LocaleInfoRequest infoRequest = {0};

    infoRequest.Info    = LOCALE_SISO639LANGNAME2;
    infoRequest.OutputW = &Lll;

    assert (p32_winlocale_get_locale_info (&infoRequest, heap, locale));

    infoRequest.Info    = LOCALE_SISO3166CTRYNAME2;
    infoRequest.OutputW = &Ccc;

    assert (p32_winlocale_get_locale_info (&infoRequest, heap, locale));
  } while (0);
#endif

  DoTest (LanguageName, CountryName);
  DoTest (LanguageName, Cc);
  DoTest (Ll, Cc);
  DoTest (Ll, CountryName);

#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
  DoTest (LanguageName, Ccc);
  DoTest (Ll, Ccc);
  DoTest (Lll, Ccc);
  DoTest (Lll, CountryName);
  DoTest (Lll, Cc);
#endif

  assert (HeapFree (heapHandle, 0, LanguageName));
  assert (HeapFree (heapHandle, 0, CountryName));
  assert (HeapFree (heapHandle, 0, Ll));
  assert (HeapFree (heapHandle, 0, Cc));
#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
  assert (HeapFree (heapHandle, 0, Lll));
  assert (HeapFree (heapHandle, 0, Ccc));
#endif

  return true;
}

int main (void) {
  p32_test_init ();

  /**
   * Test all supported locales.
   */
  p32_locale_test_func1 (Test);

  return exit_code;
}
