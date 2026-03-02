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
 * Test `wctomb` function with some DBCS code page.
 *
 * We test code page 932; this is the ANSI/OEM code page for `ja-JP` locale.
 */

#if P32_CRT >= P32_MSVCRT20
#define LOCALE "ja_JP.ACP"
#else
#define LOCALE "ja_JP.OCP"
#endif

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
    assert (buffer[0] == (char) wc && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
    assert (errno == 0);
  }

  for (size_t i = 0; i < _countof (CJK); ++i) {
    memset (buffer, EOF, _countof (buffer));

    assert (wctomb_l (buffer, CJK[i].Wc, locale) == 2);
    assert (memcmp (CJK[i].DBCS, buffer, 2) == 0);
    assert (errno == 0);
  }

  /**
   * Attempt to convert UTF-16 surrogates.
   */
  for (wchar_t wc = 0;; ++wc) {
    memset (buffer, EOF, _countof (buffer));

    if (IS_HIGH_SURROGATE (wc)) {
      assert (wctomb_l (buffer, wc, locale) == 1);
      assert (buffer[0] == '?' && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
      assert (errno == 0);

      assert (wctomb_l (&buffer[1], LOW_SURROGATE_MIN, locale) == -1);
      assert (buffer[0] == '?' && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
      assert (errno == EILSEQ);

      // reset errno
      _set_errno (0);

      /**
       * Reset internal state.
       */
      assert (wctomb_l (NULL, WEOF, locale) == 0);
      assert (errno == 0);
    } else if (IS_LOW_SURROGATE (wc)) {
      assert (wctomb_l (buffer, wc, locale) == -1);
      assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
      assert (errno == EILSEQ);

      // reset errno
      _set_errno (0);
    }

    if (wc == WEOF) {
      break;
    }
  }
}

int main (void) {
  p32_test_init ();

  if (!IsValidCodePage (932)) {
    return 77;
  }

  assert ((locale = newlocale (LC_ALL_MASK, LOCALE, NULL)) != NULL);
  assert (MB_CUR_MAX_L (locale) == 2);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
