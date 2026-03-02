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
 * Test `wctomb` function with UTF-8 (code page 65001).
 */

#define LOCALE "en_US.UTF-8"

static locale_t locale;

static void DoTest (void) {
  char buffer[MB_LEN_MAX];

  /**
   * When first argument to `wctomb` is `NULL`, it must return non-zero for
   * state-dependant encodings. Otherwise it must return 0.
   */
  assert (wctomb_l (NULL, WEOF, locale) == 0);
  assert (errno == 0);

  /**
   * All wide characters in range [0,127] are valid ASCII characters.
   */
  for (wchar_t wc = 0; wc < 0x80; ++wc) {
    memset (buffer, EOF, _countof (buffer));

    assert (wctomb_l (buffer, wc, locale) == 1);
    assert (buffer[0] == wc && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
    assert (errno == 0);
  }

  /**
   * Convert wide characters which will produce two UTF-8 Code Units.
   */
  for (wchar_t wc = 0x80; wc < 0x800; ++wc) {
    memset (buffer, EOF, _countof (buffer));

    assert (wctomb_l (buffer, wc, locale) == 2);
    assert (buffer[0] != EOF && buffer[1] != EOF && buffer[2] == EOF && buffer[3] == EOF);
    assert (errno == 0);
  }

  /**
   * Convert wide characters which will produce three UTF-8 Code Units.
   */
  for (wchar_t wc = 0x800;; ++wc) {
    memset (buffer, EOF, _countof (buffer));

    if (IS_HIGH_SURROGATE (wc)) {
      assert (wctomb_l (buffer, wc, locale) == 3);
      assert (buffer[0] == C (0xEF) && buffer[1] == C (0xBF) && buffer[2] == C (0xBD) && buffer[3] == EOF);
      assert (errno == 0);

      assert (wctomb_l (buffer, LOW_SURROGATE_MIN, locale) == 4);
      assert (buffer[0] != EOF && buffer[1] != EOF && buffer[2] != EOF && buffer[3] != EOF);
      assert (errno == 0);
    } else if (IS_LOW_SURROGATE (wc)) {
      assert (wctomb_l (buffer, wc, locale) == -1);
      assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
      assert (errno = EILSEQ);

      // reset errno
      _set_errno (0);
    } else {
      assert (wctomb_l (buffer, wc, locale) == 3);
      assert (buffer[0] != EOF && buffer[1] != EOF && buffer[2] != EOF && buffer[3] == EOF);
      assert (errno == 0);
    }

    /**
     * Probe internal state.
     */
    assert (wctomb_l (buffer, L'\0', locale) == 1);
    assert (buffer[0] == 0);
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
