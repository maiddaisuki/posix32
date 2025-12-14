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
 * This test is used to demonstrate difference between CRT's and posix32's
 * implementation of wctype.h character classification functions.
 */

static int exit_code = EXIT_FAILURE;

static bool __cdecl Test (locale_t locale, const wchar_t *localeName) {
  for (wint_t wc = 0;; ++wc) {
    uint32_t crtMask = 0;
    uint32_t p32Mask = 0;

    if (iswcntrl (wc)) {
      crtMask |= P32_CHARTYPE_CONTROL;
    }

    if (iswprint (wc)) {
      crtMask |= P32_CHARTYPE_PRINT;
    }

    if (iswgraph (wc)) {
      crtMask |= P32_CHARTYPE_GRAPH;
    }

    if (iswalnum (wc)) {
      crtMask |= P32_CHARTYPE_ALNUM;
    }

    if (iswalpha (wc)) {
      crtMask |= P32_CHARTYPE_ALPHA;
    }

    if (iswupper (wc)) {
      crtMask |= P32_CHARTYPE_UPPER;
    }

    if (iswlower (wc)) {
      crtMask |= P32_CHARTYPE_LOWER;
    }

    if (iswdigit (wc)) {
      crtMask |= P32_CHARTYPE_DIGIT;
    }

    if (iswpunct (wc)) {
      crtMask |= P32_CHARTYPE_PUNCT;
    }

    if (iswspace (wc)) {
      crtMask |= P32_CHARTYPE_SPACE;
    }

    if (iswblank (wc)) {
      crtMask |= P32_CHARTYPE_BLANK;
    }

    p32Mask |= iswcntrl_l (wc, locale);
    p32Mask |= iswprint_l (wc, locale);
    p32Mask |= iswgraph_l (wc, locale);
    p32Mask |= iswalnum_l (wc, locale);
    p32Mask |= iswalpha_l (wc, locale);
    p32Mask |= iswupper_l (wc, locale);
    p32Mask |= iswlower_l (wc, locale);
    p32Mask |= iswdigit_l (wc, locale);
    p32Mask |= iswpunct_l (wc, locale);
    p32Mask |= iswspace_l (wc, locale);
    p32Mask |= iswblank_l (wc, locale);

    if (crtMask != p32Mask) {
      exit_code = EXIT_SUCCESS;

      wchar_t wcPrint = wc;

      if ((p32Mask & P32_CHARTYPE_GRAPH) == 0) {
        wcPrint = 0xFFFD;
      }

#if P32_CRT >= P32_MSVCR80
      fwprintf (
        stderr,
        L"%-24s: %0.4X %c | (%d|%d) (%d|%d) (%d|%d) (%d|%d) (%d|%d) (%d|%d) (%d|%d) (%d|%d) (%d|%d) (%d|%d) (%d|%d)\n",
        localeName, wc, wcPrint,
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
#else
      HANDLE    heapHandle = GetProcessHeap ();
      uintptr_t heap       = (uintptr_t) heapHandle;

      wchar_t format[] =
        L"%-24s: %0.4X %c | (%d|%d) (%d|%d) (%d|%d) (%d|%d) (%d|%d) (%d|%d) (%d|%d) (%d|%d) (%d|%d) (%d|%d) (%d|%d)";

      char    *mbs = NULL;
      wchar_t *wcs = NULL;

      int mbsLength = p32_private_aswprintf (
        &wcs, heap, format, localeName, wc, wcPrint,
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

      assert (mbsLength != -1);
      assert (p32_private_wcstombs (&mbs, wcs, heap, CP_UTF8, false) != -1);

      fprintf (stderr, "%s\n", mbs);

      assert (HeapFree (heapHandle, 0, mbs));
      assert (HeapFree (heapHandle, 0, wcs));
#endif
    }

    if (wc == WEOF) {
      break;
    }
  }

  return true;
}

int main (void) {
  p32_test_init ();

  /**
   * Test all supported locales.
   */
  p32_locale_test_func2 (Test, P32_LOCALE_TEST_SUPPORTED | P32_LOCALE_TEST_ALL | P32_LOCALE_TEST_ONCE);

  return exit_code;
}
