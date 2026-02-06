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
 * implementation of `toupper` function.
 */

static int exit_code = EXIT_FAILURE;

static bool __cdecl Test (locale_t locale, const wchar_t *localeName) {
  for (int c = 0; c < 0x100; ++c) {
    int crt = toupper (c);
    int p32 = toupper_l (c, locale);

    if (crt != p32) {
      exit_code = EXIT_SUCCESS;

      wchar_t wc = btowc_l (c, locale);

      if (wc == WEOF || !iswgraph_l (wc, locale)) {
        wc = 0xFFFD;
      }

      wchar_t wcCrt = btowc_l (crt, locale);

      if (wcCrt == WEOF || !iswgraph_l (wcCrt, locale)) {
        wcCrt = 0xFFFD;
      }

      wchar_t wcP32 = btowc_l (p32, locale);

      if (wcP32 == WEOF || !iswgraph_l (wcP32, locale)) {
        wcP32 = 0xFFFD;
      }

      fwprintf (stderr, L"%-24s: %0.2X (%c) -> %0.2X (%c) | %0.2X (%c)\n", localeName, c, wc, crt, wcCrt, p32, wcP32);
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
