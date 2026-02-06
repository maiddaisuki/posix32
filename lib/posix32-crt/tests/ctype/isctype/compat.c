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
 * This test is used to demonstrate difference between CRT's and posix32's
 * implementation of ctype.h character classification functions.
 */

static int exit_code = EXIT_FAILURE;

static bool __cdecl Test (locale_t locale, const wchar_t *localeName) {
  for (int i = 0; i < 0x100; ++i) {
    uint32_t crtMask = 0;
    uint32_t p32Mask = 0;

    if (iscntrl (i)) {
      crtMask |= P32_CHARTYPE_CONTROL;
    }

    if (isprint (i)) {
      crtMask |= P32_CHARTYPE_PRINT;
    }

    if (isgraph (i)) {
      crtMask |= P32_CHARTYPE_GRAPH;
    }

    if (isalnum (i)) {
      crtMask |= P32_CHARTYPE_ALNUM;
    }

    if (isalpha (i)) {
      crtMask |= P32_CHARTYPE_ALPHA;
    }

    if (isupper (i)) {
      crtMask |= P32_CHARTYPE_UPPER;
    }

    if (islower (i)) {
      crtMask |= P32_CHARTYPE_LOWER;
    }

    if (isdigit (i)) {
      crtMask |= P32_CHARTYPE_DIGIT;
    }

    if (ispunct (i)) {
      crtMask |= P32_CHARTYPE_PUNCT;
    }

    if (isspace (i)) {
      crtMask |= P32_CHARTYPE_SPACE;
    }

    if (isblank (i)) {
      crtMask |= P32_CHARTYPE_BLANK;
    }

    p32Mask |= iscntrl_l (i, locale);
    p32Mask |= isprint_l (i, locale);
    p32Mask |= isgraph_l (i, locale);
    p32Mask |= isalnum_l (i, locale);
    p32Mask |= isalpha_l (i, locale);
    p32Mask |= isupper_l (i, locale);
    p32Mask |= islower_l (i, locale);
    p32Mask |= isdigit_l (i, locale);
    p32Mask |= ispunct_l (i, locale);
    p32Mask |= isspace_l (i, locale);
    p32Mask |= isblank_l (i, locale);

    if (crtMask != p32Mask) {
      exit_code = EXIT_SUCCESS;

      wchar_t wc = btowc_l (i, locale);

      if (wc == WEOF || !iswgraph_l (wc, locale)) {
        wc = 0xFFFD;
      }

      fwprintf (
        stderr,
        L"%-24s: %0.2X %c | (%d|%d) (%d|%d) (%d|%d) (%d|%d) (%d|%d) (%d|%d) (%d|%d) (%d|%d) (%d|%d) (%d|%d) (%d|%d)\n",
        localeName, i, wc,
        /* clang-format off */
        !!(crtMask & P32_CHARTYPE_ALNUM), !!(p32Mask & P32_CHARTYPE_ALNUM),
        !!(crtMask & P32_CHARTYPE_ALPHA), !!(p32Mask & P32_CHARTYPE_ALPHA),
        !!(crtMask & P32_CHARTYPE_BLANK), !!(p32Mask & P32_CHARTYPE_BLANK),
        !!(crtMask & P32_CHARTYPE_CONTROL), !!(p32Mask & P32_CHARTYPE_CONTROL),
        !!(crtMask & P32_CHARTYPE_DIGIT), !!(p32Mask & P32_CHARTYPE_DIGIT),
        !!(crtMask & P32_CHARTYPE_GRAPH), !!(p32Mask & P32_CHARTYPE_GRAPH),
        !!(crtMask & P32_CHARTYPE_LOWER), !!(p32Mask & P32_CHARTYPE_LOWER),
        !!(crtMask & P32_CHARTYPE_PRINT), !!(p32Mask & P32_CHARTYPE_PRINT),
        !!(crtMask & P32_CHARTYPE_PUNCT), !!(p32Mask & P32_CHARTYPE_PUNCT),
        !!(crtMask & P32_CHARTYPE_SPACE), !!(p32Mask & P32_CHARTYPE_SPACE),
        !!(crtMask & P32_CHARTYPE_UPPER), !!(p32Mask & P32_CHARTYPE_UPPER)
        /* clang-format on */
      );
    }
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
