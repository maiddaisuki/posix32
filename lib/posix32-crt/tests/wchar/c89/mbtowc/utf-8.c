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
 * Test `mbtowc` function with UTF-8 (code page 65001).
 */

#define LOCALE "en_US.UTF-8"

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
   * All bytes in range [0,127] are valid ASCII characters.
   */
  for (uint8_t c = 0;; ++c) {
    assert (mbtowc_l (&wc, (char *) &c, MB_CUR_MAX_L (locale), locale) == !!c);
    assert (wc == c);
    assert (errno == 0);

    if (c == 0x7F) {
      break;
    }
  }

  for (size_t i = 0; i < _countof (CJK); ++i) {
    /**
     * Convert UTF-8 character with single call.
     */
    wc = WEOF;

    assert (mbtowc_l (&wc, CJK[i].UTF8, MB_CUR_MAX_L (locale), locale) == 3);
    assert (wc == CJK[i].Wc);
    assert (errno == 0);

    /**
     * Probe internal conversion state.
     */
    assert (mbtowc_l (&wc, "", 1, locale) == 0);
    assert (wc == '\0');
    assert (errno == 0);

    /**
     * Convert UTF-8 character with multiple call.
     */
    wc = WEOF;

    assert (mbtowc_l (&wc, CJK[i].UTF8, 2, locale) == -1);
    assert (wc == WEOF);
    assert (errno == 0);

    /**
     * Probe internal conversion state.
     *
     * Unlike `mblen`, `mbtowc` stores incomplete multibyte sequence in its
     * private conversion state.
     */
    assert (mbtowc_l (&wc, "", 1, locale) == -1);
    assert (wc == WEOF);
    assert (errno == EILSEQ);

    // reset errno
    _set_errno (0);

    /**
     * Complete conversion.
     */
    assert (mbtowc_l (&wc, &CJK[i].UTF8[2], MB_CUR_MAX_L (locale), locale) == 1);
    assert (wc == CJK[i].Wc);
    assert (errno == 0);

    /**
     * Probe internal conversion state.
     */
    assert (mbtowc_l (&wc, "", 1, locale) == 0);
    assert (wc == '\0');
    assert (errno == 0);
  }

  /**
   * Convert UTF-8 character with length 2.
   */
  wc = WEOF;

  assert (mbtowc_l (&wc, UTF8Length2, MB_CUR_MAX_L (locale), locale) == 2);
  assert (wc == L'¥');
  assert (errno == 0);

  /**
   * Convert UTF-8 character with length 3.
   */
  wc = WEOF;

  assert (mbtowc_l (&wc, UTF8Length3, MB_CUR_MAX_L (locale), locale) == 3);
  assert (wc == L'語');
  assert (errno == 0);

  /**
   * Convert UTF-8 character with length 4.
   */
  wc = WEOF;

  assert (mbtowc_l (&wc, UTF8Length4, MB_CUR_MAX_L (locale), locale) == 4);
  assert (wc == 0xFFFD);
  assert (errno == 0);
}

int main (void) {
  p32_test_init ();

  assert ((locale = newlocale (LC_ALL_MASK, LOCALE, NULL)) != NULL);
  assert (MB_CUR_MAX_L (locale) == 4);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
