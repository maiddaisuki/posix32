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

#include "string-test.h"

/**
 * Test Summary:
 *
 * Test `wcscasecmp` function.
 *
 * Note that our implementation of `wcscasecmp` uses linguistic casing rules,
 * unlike CRT function `_wcsicmp`, which uses filesystem casing rules.
 */

static int exit_code = EXIT_SUCCESS;

static bool __cdecl Test (locale_t locale, const wchar_t *localeName) {
  /**
   * Test ASCII input.
   */
  const wchar_t *ascii       = ASCIIString.W;
  size_t         asciiLength = wcslen (ascii);

  int      asciiDiff  = 0;
  wchar_t *asciiLower = NULL;
  wchar_t *asciiUpper = NULL;

  assert ((asciiLower = wcsdup (ascii)) != NULL);
  assert ((asciiUpper = wcsdup (ascii)) != NULL);

  for (size_t i = 0; i < asciiLength; ++i) {
    asciiLower[i] = towlower_l (ascii[i], locale);
    asciiUpper[i] = towupper_l (ascii[i], locale);
  }

  assert ((asciiDiff = wcscasecmp_l (asciiLower, asciiUpper, locale)) != _NLSCMPERROR);

  free (asciiLower);
  free (asciiUpper);

  /**
   * Test locale-specific input.
   */
  const wchar_t *localeStr    = (wchar_t *) nl_langinfo_l (p32_WERA, locale);
  const size_t   localeLength = wcslen (localeStr);

  int      localeDiff  = 0;
  wchar_t *localeLower = NULL;
  wchar_t *localeUpper = NULL;

  assert ((localeLower = wcsdup (localeStr)) != NULL);
  assert ((localeUpper = wcsdup (localeStr)) != NULL);

  for (size_t i = 0; i < localeLength; ++i) {
    localeLower[i] = towlower_l (localeStr[i], locale);
    localeUpper[i] = towupper_l (localeStr[i], locale);
  }

  assert ((localeDiff = wcscasecmp_l (localeLower, localeUpper, locale)) != _NLSCMPERROR);

  free (localeLower);
  free (localeUpper);

  /**
   * Test Unicode input.
   */
  int unicode1Diff = 0;
  int unicode2Diff = 0;

  assert ((unicode1Diff = wcscasecmp_l (Unicode1Lower.W, Unicode1Upper.W, locale)) != _NLSCMPERROR);
  assert ((unicode2Diff = wcscasecmp_l (Unicode2Lower.W, Unicode2Upper.W, locale)) != _NLSCMPERROR);

  if (asciiDiff || localeDiff || unicode1Diff || unicode2Diff) {
    exit_code = EXIT_FAILURE;
    fwprintf (stderr, L"%-24s: %d | %d | %d | %d\n", localeName, asciiDiff, localeDiff, unicode1Diff, unicode2Diff);
  }

  return true;
}

int main (void) {
  p32_test_init ();

  /**
   * Test all supported locales.
   */
  p32_locale_test_func2 (Test, P32_LOCALE_TEST_ALL | P32_LOCALE_TEST_ONCE);

  return exit_code;
}
