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
 * Test `wcrtomb` function with UTF-8 (code page 65001).
 */

#undef wcrtomb

/**
 * `Charset` structure with information about code page 65001 (UTF-8).
 */
static Charset utf8;

#undef MB_CUR_MAX
#define MB_CUR_MAX (utf8.MaxLength)

/**
 * Convenience macro to call `p32_private_wcrtomb_utf8`.
 */
#define wcrtomb(mb, wc, state) p32_private_wcrtomb_utf8 (mb, wc, state, &utf8)

static void DoTest (void) {
  char      buffer[MB_LEN_MAX];
  mbstate_t state = {0};

  /**
   * Detect invalid conversion state
   */
  TaintConversionState (&state);
  memset (buffer, EOF, _countof (buffer));

  assert (wcrtomb (buffer, L'\0', &state) == (size_t) -1);
  assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
  assert (!mbsinit (&state));
  assert (errno == EINVAL);

  // reset errno
  _set_errno (0);

  /**
   * Reset conversion state.
   */
  assert (wcrtomb (NULL, WEOF, &state) == 1);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * All wide characters in range [0,127] are valid ASCII characters.
   */
  for (wchar_t wc = 0; wc < 0x80; ++wc) {
    memset (buffer, EOF, _countof (buffer));

    assert (wcrtomb (buffer, wc, &state) == 1);
    assert (buffer[0] == wc && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
    assert (mbsinit (&state));
    assert (errno == 0);
  }

  /**
   * Convert wide characters which will produce two UTF-8 Code Units.
   */
  for (wchar_t wc = 0x80; wc < 0x800; ++wc) {
    memset (buffer, EOF, _countof (buffer));

    assert (wcrtomb (buffer, wc, &state) == 2);
    assert (buffer[0] != EOF && buffer[1] != EOF && buffer[2] == EOF && buffer[3] == EOF);
    assert (mbsinit (&state));
    assert (errno == 0);
  }

  /**
   * Convert wide characters which will produce three UTF-8 Code Units.
   */
  for (wchar_t wc = 0x800;; ++wc) {
    memset (buffer, EOF, _countof (buffer));

    if (IS_LOW_SURROGATE (wc)) {
      assert (wcrtomb (buffer, wc, &state) == (size_t) -1);
      assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
      assert (mbsinit (&state));
      assert (errno = EILSEQ);

      // reset errno
      _set_errno (0);
    } else if (!IS_HIGH_SURROGATE (wc)) {
      assert (wcrtomb (buffer, wc, &state) == 3);
      assert (buffer[0] != EOF && buffer[1] != EOF && buffer[2] != EOF && buffer[3] == EOF);
      assert (mbsinit (&state));
      assert (errno == 0);
    }

    if (wc == WEOF) {
      break;
    }
  }

  /**
   * Convert UTF-16 Surrogate Pairs. They must produce four UTF-8 Code Units.
   */
  for (wchar_t hs = HIGH_SURROGATE_MIN; hs <= HIGH_SURROGATE_MAX; ++hs) {
    for (wchar_t ls = LOW_SURROGATE_MIN; ls <= LOW_SURROGATE_MAX; ++ls) {
      memset (buffer, EOF, _countof (buffer));

      /**
       * Consume high surrogate.
       *
       * This will produce UTF-8 Code Unit Sequence for U+FFFD.
       */
      assert (wcrtomb (buffer, hs, &state) == 3);
      assert (buffer[0] == C (0xEF) && buffer[1] == C (0xBF) && buffer[2] == C (0xBD) && buffer[3] == EOF);
      assert (!mbsinit (&state));
      assert (errno == 0);

      /**
       * Consume low surrogate to complete conversion.
       */
      assert (wcrtomb (buffer, ls, &state) == 4);
      assert (buffer[0] != EOF && buffer[1] != EOF && buffer[2] != EOF && buffer[3] != EOF);
      assert (mbsinit (&state));
      assert (errno == 0);
    }
  }
}

int main (void) {
  p32_test_init ();
  srand (0xBADF);

  utf8.CodePage = CP_UTF8;
  assert (p32_charset_info (&utf8));
  assert (MB_CUR_MAX == 4);

  DoTest ();

  return EXIT_SUCCESS;
}
