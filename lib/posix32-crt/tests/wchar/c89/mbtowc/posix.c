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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "uchar-test.h"

/**
 * Test Summary:
 *
 * Test `mbtowc` function with ISO-8859-1 (code page 28591).
 *
 * This code page is used with "POSIX" locale.
 */

#define LOCALE "POSIX"

static locale_t locale;

static void DoTest (void) {
  wchar_t wc = WEOF;

  /**
   * When second argument to `mbtowc` is `NULL`, it must return non-zero for
   * state-dependant encodings. Otherwise it must return 0.
   */
  assert (mbtowc_l (&wc, NULL, MB_CUR_MAX_L (locale), locale) == 0);
  assert (wc == WEOF);
  assert (errno == 0);

  /**
   * When third argument to `mbtowc` is zero, it must not examine its second
   * argument.
   */
  assert (mbtowc_l (&wc, "", 0, locale) == -1);
  assert (wc == WEOF);
  assert (errno == 0);

  /**
   * All bytes are valid characters.
   *
   * POSIX requires that in "POSIX" locale all bytes are valid characters.
   */
  for (uint8_t c = 0;; ++c) {
    wc = WEOF;

    assert (mbtowc_l (&wc, (char *) &c, MB_CUR_MAX_L (locale), locale) == !!c);
    assert (wc == c);
    assert (errno == 0);

    if (c == 0x7F) {
      break;
    }
  }

  /**
   * Probe internal conversion state.
   */
  wc = WEOF;

  assert (mbtowc_l (&wc, "", 1, locale) == 0);
  assert (wc == '\0');
  assert (errno == 0);
}

int main (void) {
  p32_test_init ();

  assert ((locale = newlocale (LC_ALL_MASK, LOCALE, NULL)) != NULL);
  assert (MB_CUR_MAX_L (locale) == 1);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
