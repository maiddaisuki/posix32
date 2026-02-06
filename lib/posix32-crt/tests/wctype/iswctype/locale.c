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
#include <stdio.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "tests-internal.h"

/**
 * Test Summary:
 *
 * Verify that `iswctype` is consistent with class-specific wctype.h functions.
 *
 * Example:
 *
 * `iswctype (wc, wctype ("print"))` must be equivalent to `iswprint (wc)`.
 *
 * This requirement applies to all character classes defined by POSIX.
 */

static int exit_code = EXIT_SUCCESS;

static bool __cdecl Test (locale_t locale, const wchar_t *localeName) {
  wctype_t alnum  = (wctype_t) 0;
  wctype_t alpha  = (wctype_t) 0;
  wctype_t blank  = (wctype_t) 0;
  wctype_t cntrl  = (wctype_t) 0;
  wctype_t digit  = (wctype_t) 0;
  wctype_t graph  = (wctype_t) 0;
  wctype_t lower  = (wctype_t) 0;
  wctype_t print  = (wctype_t) 0;
  wctype_t punct  = (wctype_t) 0;
  wctype_t space  = (wctype_t) 0;
  wctype_t upper  = (wctype_t) 0;
  wctype_t xdigit = (wctype_t) 0;

  assert ((alnum = wctype_l ("alnum", locale)) != (wctype_t) 0);
  assert ((alpha = wctype_l ("alpha", locale)) != (wctype_t) 0);
  assert ((blank = wctype_l ("blank", locale)) != (wctype_t) 0);
  assert ((cntrl = wctype_l ("cntrl", locale)) != (wctype_t) 0);
  assert ((digit = wctype_l ("digit", locale)) != (wctype_t) 0);
  assert ((graph = wctype_l ("graph", locale)) != (wctype_t) 0);
  assert ((lower = wctype_l ("lower", locale)) != (wctype_t) 0);
  assert ((print = wctype_l ("print", locale)) != (wctype_t) 0);
  assert ((punct = wctype_l ("punct", locale)) != (wctype_t) 0);
  assert ((space = wctype_l ("space", locale)) != (wctype_t) 0);
  assert ((upper = wctype_l ("upper", locale)) != (wctype_t) 0);
  assert ((xdigit = wctype_l ("xdigit", locale)) != (wctype_t) 0);

  for (wchar_t wc = 0;; ++wc) {
    assert (iswalnum_l (wc, locale) == iswctype_l (wc, alnum, locale));
    assert (iswalpha_l (wc, locale) == iswctype_l (wc, alpha, locale));
    assert (iswblank_l (wc, locale) == iswctype_l (wc, blank, locale));
    assert (iswcntrl_l (wc, locale) == iswctype_l (wc, cntrl, locale));
    assert (iswdigit_l (wc, locale) == iswctype_l (wc, digit, locale));
    assert (iswgraph_l (wc, locale) == iswctype_l (wc, graph, locale));
    assert (iswlower_l (wc, locale) == iswctype_l (wc, lower, locale));
    assert (iswprint_l (wc, locale) == iswctype_l (wc, print, locale));
    assert (iswpunct_l (wc, locale) == iswctype_l (wc, punct, locale));
    assert (iswspace_l (wc, locale) == iswctype_l (wc, space, locale));
    assert (iswupper_l (wc, locale) == iswctype_l (wc, upper, locale));
    assert (iswxdigit_l (wc, locale) == iswctype_l (wc, xdigit, locale));

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
