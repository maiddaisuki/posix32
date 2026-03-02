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
#include <errno.h>
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "uchar-test.h"

/**
 * Test Summary:
 *
 * Test `wctob` function with UTF-8 (code page 65001).
 */

#define LOCALE "en_US.UTF-8"

locale_t locale;

static void DoTest (void) {
  /**
   * All wide characters in range [0,127] are valid ASCII characters.
   */
  for (wchar_t wc = 0; wc <= 0x7F; ++wc) {
    assert (wctob_l (wc, locale) == wc);
    assert (errno == 0);
  }

  /**
   * All wide characters in range [128,WEOF] are invalid.
   */
  for (wchar_t wc = 0x80;; ++wc) {
    assert (wctob_l (wc, locale) == EOF);
    assert (errno == 0);

    if (wc == WEOF) {
      break;
    }
  }
}

int main (void) {
  p32_test_init ();

  assert ((locale = newlocale (LC_ALL_MASK, LOCALE, NULL)) != NULL);
  assert (MB_CUR_MAX_L (locale) == 4);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
