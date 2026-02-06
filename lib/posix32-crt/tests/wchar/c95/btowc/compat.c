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
 * implementation of `btowc` function.
 */

typedef wint_t (__cdecl *btowc_t) (int);

/**
 * CRT's btowc function.
 */
static btowc_t crt_btowc = NULL;

static int exit_code = EXIT_FAILURE;

static bool __cdecl Test (locale_t locale, const wchar_t *localeName) {
  for (int c = 0; c < 0x100; ++c) {
    wint_t crt = crt_btowc (c);
    wint_t p32 = btowc_l (c, locale);

    if (crt != p32) {
      exit_code = EXIT_SUCCESS;
      fwprintf (stderr, L"%-24s: %0.2X -> %0.4X | %0.4X\n", localeName, c, crt, p32);
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

  crt_btowc = (btowc_t) (uintptr_t) GetProcAddress ((HMODULE) crtHandle, "btowc");

  if (crt_btowc == NULL) {
    return 77;
  }

  /**
   * Test all supported code pages.
   */
  p32_locale_test_func4 (Test, P32_LOCALE_TEST_SUPPORTED);

  return exit_code;
}
