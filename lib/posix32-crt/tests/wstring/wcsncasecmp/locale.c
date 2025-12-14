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
 * Note that our implementation of `wcsncasecmp` uses linguistic casing rules,
 * unlike CRT function `_wcsnicmp`, which uses filesystem casing rules.
 */

static int exit_code = EXIT_SUCCESS;

static void CompareStringIncremental (
  const wchar_t *lower,
  const wchar_t *upper,
  locale_t       locale,
  const wchar_t *localeName
) {
  size_t totalLength = 0;

  while (lower[totalLength] != L'\0') {
    totalLength += 1;

    int diff = wcsncasecmp_l (lower, upper, totalLength, locale);

    if (diff) {
      exit_code = EXIT_FAILURE;
      fwprintf (stderr, L"%-24s: %d (%s | %s)\n", localeName, diff, lower, upper);
    }

    assert (diff != _NLSCMPERROR);
  }
}

static bool __cdecl Test (locale_t locale, const wchar_t *localeName) {
  /**
   * Test ASCII input.
   */
  const wchar_t *ascii       = ASCIIString.W;
  const size_t   asciiLength = wcslen (ascii);

  wchar_t *asciiLower = NULL;
  wchar_t *asciiUpper = NULL;

  assert ((asciiLower = wcsdup (ascii)) != NULL);
  assert ((asciiUpper = wcsdup (ascii)) != NULL);

  for (size_t i = 0; i < asciiLength; ++i) {
    asciiLower[i] = towlower_l (asciiLower[i], locale);
    asciiUpper[i] = towlower_l (asciiUpper[i], locale);
  }

  CompareStringIncremental (asciiLower, asciiUpper, locale, localeName);

  free (asciiLower);
  free (asciiUpper);

  /**
   * Test locale-specific input.
   */
  const wchar_t *localeStr    = (wchar_t *) nl_langinfo_l (p32_WERA, locale);
  const size_t   localeLength = wcslen (localeStr);

  wchar_t *localeLower = NULL;
  wchar_t *localeUpper = NULL;

  assert ((localeLower = wcsdup (localeStr)) != NULL);
  assert ((localeUpper = wcsdup (localeStr)) != NULL);

  for (size_t i = 0; i < localeLength; ++i) {
    localeLower[i] = towlower_l (localeStr[i], locale);
    localeUpper[i] = towupper_l (localeStr[i], locale);
  }

  CompareStringIncremental (localeLower, localeUpper, locale, localeName);

  free (localeLower);
  free (localeUpper);

  /**
   * Test Unicode input.
   */
  const wchar_t *unicode       = Unicode1String.W;
  const size_t   unicodeLength = wcslen (unicode);

  wchar_t *unicodeLower = NULL;
  wchar_t *unicodeUpper = NULL;

  assert ((unicodeLower = wcsdup (unicode)) != NULL);
  assert ((unicodeUpper = wcsdup (unicode)) != NULL);

  for (size_t i = 0; i < unicodeLength; ++i) {
    unicodeLower[i] = towlower_l (unicodeLower[i], locale);
    unicodeUpper[i] = towlower_l (unicodeUpper[i], locale);
  }

  CompareStringIncremental (unicodeLower, unicodeUpper, locale, localeName);
  CompareStringIncremental (Unicode2Lower.W, Unicode2Upper.W, locale, localeName);

  free (unicodeLower);
  free (unicodeUpper);

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
