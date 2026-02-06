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
 * implementation of `wctob` function.
 */

typedef int (__cdecl *wctob_t) (wint_t);

/**
 * CRT's wctob function.
 */
static wctob_t crt_wctob = NULL;

static int exit_code = EXIT_FAILURE;

static bool __cdecl Test (locale_t locale, const wchar_t *localeName) {
#if P32_CRT < P32_MSVCR80
  HANDLE    heapHandle = GetProcessHeap ();
  uintptr_t heap       = (uintptr_t) heapHandle;
#endif

  for (wchar_t wc = 0;; ++wc) {
    int crt = crt_wctob (wc);
    int p32 = wctob_l (wc, locale);

    /**
     * CRT's `wctob` failed to convert `wc` while our `wctob` succeeded.
     *
     * CRT's `wctob` sign-extends its return value, which means that returning
     * (char)255 yelds EOF. If our `wctob` returned `255`, treat it the same.
     */
    if (crt == EOF && p32 != EOF && p32 != 255) {
      exit_code = EXIT_SUCCESS;

      wchar_t wcToPrint = wc;

      if (!iswprint_l (wcToPrint, locale)) {
        wcToPrint = 0xFFFD;
      }

#if P32_CRT >= P32_MSVCR80
      fwprintf (stderr, L"%-24s: %0.4X (%c) -> EOF | %0.2X\n", localeName, wc, wcToPrint, p32);
#else
      wchar_t formatString[] = L"%-24s: %0.4X (%c) -> EOF | %0.2X";

      char    *u8Str = NULL;
      wchar_t *wStr  = NULL;

      assert (p32_private_aswprintf (&wStr, heap, formatString, localeName, wc, wcToPrint, p32) != -1);
      assert (p32_private_wcstombs (&u8Str, wStr, heap, CP_UTF8, false) != -1);

      fprintf (stderr, "%s\n", u8Str);

      assert (HeapFree (heapHandle, 0, wStr));
      assert (HeapFree (heapHandle, 0, u8Str));
#endif

      /**
       * CRT's `wctob` succeeded to convert `wc` while our `wctob` failed.
       */
    } else if (crt != EOF && p32 == EOF) {
      exit_code = EXIT_SUCCESS;

      wchar_t wcToPrint = wc;
      wchar_t crtAsWc   = btowc_l ((uint8_t) crt, locale);

      if (!iswprint_l (wcToPrint, locale)) {
        wcToPrint = 0xFFFD;
      }

      if (crtAsWc == WEOF || !iswprint_l (crtAsWc, locale)) {
        crtAsWc = 0xFFFD;
      }

      crt = (uint8_t) crt;

#if P32_CRT >= P32_MSVCR80
      fwprintf (stderr, L"%-24s: %0.4X (%c) -> %0.2X (%c) | EOF\n", localeName, wc, wcToPrint, crt, crtAsWc);
#else
      wchar_t formatString[] = L"%-24s: %0.4X (%c) -> %0.2X (%c) | EOF";

      char    *u8Str = NULL;
      wchar_t *wStr  = NULL;

      assert (p32_private_aswprintf (&wStr, heap, formatString, localeName, wc, wcToPrint, crt, crtAsWc) != -1);
      assert (p32_private_wcstombs (&u8Str, wStr, heap, CP_UTF8, false) != -1);

      fprintf (stderr, "%s\n", u8Str);

      assert (HeapFree (heapHandle, 0, wStr));
      assert (HeapFree (heapHandle, 0, u8Str));
#endif

      /**
       * CRT's `wctob` and our `wctob` converted `wc` to a different value.
       */
    } else if ((uint8_t) crt != (uint8_t) p32) {
      exit_code = EXIT_SUCCESS;
      fwprintf (stderr, L"%-24s: %0.4X -> %0.2X | %0.2X\n", localeName, wc, (uint8_t) crt, p32);
    }

    if (wc == WEOF) {
      break;
    }
  }

  return true;
}

int main (void) {
  p32_test_init ();

  uintptr_t crtHandle = p32_crt_handle ();

  if (crtHandle == 0) {
    return 77;
  }

  crt_wctob = (wctob_t) (uintptr_t) GetProcAddress ((HMODULE) crtHandle, "wctob");

  if (crt_wctob == NULL) {
    return 77;
  }

  /**
   * Test all supported code pages.
   */
  p32_locale_test_func4 (Test, P32_LOCALE_TEST_SUPPORTED);

  return exit_code;
}
