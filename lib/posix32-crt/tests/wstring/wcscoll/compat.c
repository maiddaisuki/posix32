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
 * This test is used to demonstrate difference between CRT's and posix32's
 * implementation of `wcscoll` function.
 */

static int exit_code = EXIT_FAILURE;

static bool __cdecl Test (locale_t locale, const wchar_t *localeName) {
  /**
   * Test ASCII input.
   */
  int asciiDiffCrt = 0;
  int asciiDiffP32 = 0;

  assert ((asciiDiffCrt = wcscoll (ASCIILower.W, ASCIIUpper.W)) != _NLSCMPERROR);
  assert ((asciiDiffP32 = wcscoll_l (ASCIILower.W, ASCIIUpper.W, locale)) != _NLSCMPERROR);

  /**
   * Test locale-specific input.
   */
  wchar_t *wcs1 = (wchar_t *) nl_langinfo_l (p32_WMON_1, locale);
  wchar_t *wcs2 = (wchar_t *) nl_langinfo_l (p32_WMON_2, locale);

  int localeDiffCrt = 0;
  int localeDiffP32 = 0;

  assert ((localeDiffCrt = wcscoll (wcs1, wcs2)) != _NLSCMPERROR);
  assert ((localeDiffP32 = wcscoll_l (wcs1, wcs2, locale)) != _NLSCMPERROR);

  /**
   * Test Unicode input.
   */
  int unicodeDiffCrt = 0;
  int unicodeDiffP32 = 0;

  assert ((unicodeDiffCrt = wcscoll (Unicode1Lower.W, Unicode1Upper.W)) != _NLSCMPERROR);
  assert ((unicodeDiffP32 = wcscoll_l (Unicode1Lower.W, Unicode1Upper.W, locale)) != _NLSCMPERROR);

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
