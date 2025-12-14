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
 * This test is used to demonstrate difference between CRT's function
 * `_wcsicmp` and posix32's function `wcscasecmp`.
 *
 * CRT's `_wcsicmp` uses filesystem rules for casing, while our `wcscasecmp`
 * uses linguistic rules for casing.
 */

static int exit_code = EXIT_FAILURE;

static bool __cdecl Test (locale_t locale, const wchar_t *localeName) {
  /**
   * Test ASCII input.
   */
  int asciiDiffCrt = 0;
  int asciiDiffP32 = 0;

  assert ((asciiDiffCrt = wcscasecmp (ASCIILower.W, ASCIIUpper.W)) != _NLSCMPERROR);
  assert ((asciiDiffP32 = wcscasecmp_l (ASCIILower.W, ASCIIUpper.W, locale)) != _NLSCMPERROR);

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

  int localeDiffCrt = 0;
  int localeDiffP32 = 0;

  assert ((localeDiffCrt = wcscasecmp (localeLower, localeUpper)) != _NLSCMPERROR);
  assert ((localeDiffP32 = wcscasecmp_l (localeLower, localeUpper, locale)) != _NLSCMPERROR);

  free (localeLower);
  free (localeUpper);

  /**
   * Test Unicode input.
   */
  int unicodeDiffCrt = 0;
  int unicodeDiffP32 = 0;

  assert ((unicodeDiffCrt = wcscasecmp (Unicode1Lower.W, Unicode1Upper.W)) != _NLSCMPERROR);
  assert ((unicodeDiffP32 = wcscasecmp_l (Unicode1Lower.W, Unicode1Upper.W, locale)) != _NLSCMPERROR);

  if (asciiDiffCrt != asciiDiffP32 || localeDiffCrt != localeDiffP32 || unicodeDiffCrt != unicodeDiffP32) {
    exit_code = EXIT_SUCCESS;
    fwprintf (
      stderr, L"%-24s: (%d|%d) (%d|%d) (%d|%d)\n", localeName, asciiDiffCrt, asciiDiffP32, localeDiffCrt, localeDiffP32,
      unicodeDiffCrt, unicodeDiffP32
    );
  }

  return true;
}

int main (void) {
  p32_test_init ();

  /**
   * Test all supported locales.
   */
  p32_locale_test_func2 (Test, P32_LOCALE_TEST_SUPPORTED | P32_LOCALE_TEST_ALL);

  return exit_code;
}
