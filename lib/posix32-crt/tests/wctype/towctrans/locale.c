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
#include <stdio.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "tests-internal.h"

/**
 * Test Summary:
 *
 * Verify that `towctrans` is consistent with mapping-specific wctype.h
 * functions.
 *
 * `towctrans (wc, wctrans ("tolower"))` must be equivalent to `towlower (wc)`.
 * `towctrans (wc, wctrans ("toupper"))` must be equivalent to `towupper (wc)`.
 *
 * POSIX requires that if second argument to `towctrans` is `(wctrans_t)0`,
 * then `wc` is returned unchanged.
 */

static int exit_code = EXIT_SUCCESS;

static bool __cdecl Test (locale_t locale, const wchar_t *localeName) {
  wctrans_t upper = (wctrans_t) 0;
  wctrans_t lower = (wctrans_t) 0;

  assert ((upper = wctrans_l ("toupper", locale)) != (wctrans_t) 0);
  assert ((lower = wctrans_l ("tolower", locale)) != (wctrans_t) 0);

  for (wchar_t wc = 0;; ++wc) {
    assert (towctrans_l (wc, (wctrans_t) 0, locale) == wc);
    assert (towctrans_l (wc, lower, locale) == towlower_l (wc, locale));
    assert (towctrans_l (wc, upper, locale) == towupper_l (wc, locale));

    if (wc == WEOF) {
      break;
    }
  }

  return true;
  UNREFERENCED_PARAMETER (localeName);
}

int main (void) {
  p32_test_init ();

  /**
   * Test all supported locales.
   */
  p32_locale_test_func2 (Test, P32_LOCALE_TEST_UTF8);

  return exit_code;
}
