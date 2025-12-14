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
 * implementation of `towupper` function.
 */

static int exit_code = EXIT_FAILURE;

static bool __cdecl Test (locale_t locale, const wchar_t *localeName) {
  for (wchar_t wc = 0;; ++wc) {
    wchar_t crt = towupper (wc);
    wchar_t p32 = towupper_l (wc, locale);

    if (crt != p32) {
      exit_code = EXIT_SUCCESS;

#if P32_CRT >= P32_MSVCR80
      fwprintf (stdout, L"%-24s: %0.4X (%c) -> %.4X (%c) | %.4X (%c)\n", localeName, wc, wc, crt, crt, p32, p32);
#else
      HANDLE    heapHandle = GetProcessHeap ();
      uintptr_t heap       = (uintptr_t) heapHandle;

      wchar_t format[] = L"%-24s: %0.4X (%c) -> %.4X (%c) | %.4X (%c)";

      char    *mbs = NULL;
      wchar_t *wcs = NULL;

      assert (p32_private_aswprintf (&wcs, heap, format, localeName, wc, wc, crt, crt, p32, p32) != -1);
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
