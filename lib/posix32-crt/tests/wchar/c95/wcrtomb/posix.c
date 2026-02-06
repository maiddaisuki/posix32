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
 * Test `wcrtomb` function with "POSIX" locale.
 */

static void DoTest (void) {
  mbstate_t state = {0};
  char      buffer[MB_LEN_MAX];

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
   * POSIX requires that all bytes are valid characters.
   *
   * POSIX does not mention whether `wcrtomb` must support reverse conversion
   * for bytes converted by `mbrtowc` which are outside of ASCII range.
   */
  for (wchar_t wc = 0; wc < 0x100; ++wc) {
    memset (buffer, EOF, _countof (buffer));

    assert (wcrtomb (buffer, wc, &state) == 1);
    assert (buffer[0] == (char) wc && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
    assert (mbsinit (&state));
    assert (errno == 0);
  }

  /**
   * All wide characters in range [255,WEOF] are invalid.
   */
  for (wchar_t wc = 0x100;; ++wc) {
    memset (buffer, EOF, _countof (buffer));

    if (IS_HIGH_SURROGATE (wc)) {
      /**
       * Consume high surrogate.
       */
      assert (wcrtomb (buffer, wc, &state) == 1);
      assert (buffer[0] == '?' && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
      assert (!mbsinit (&state));
      assert (errno == 0);

      for (wchar_t ls = LOW_SURROGATE_MIN; ls <= LOW_SURROGATE_MAX; ++ls) {
        assert (wcrtomb (&buffer[1], wc, &state) == (size_t) -1);
        assert (buffer[0] == '?' && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
        assert (!mbsinit (&state));
        assert (errno == EILSEQ);

        // reset errno
        _set_errno (0);
      }

      /**
       * Reset conversion state.
       */
      assert (wcrtomb (NULL, WEOF, &state) == 1);
      assert (mbsinit (&state));
      assert (errno == 0);
    } else {
      assert (wcrtomb (buffer, wc, &state) == (size_t) -1);
      assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
      assert (mbsinit (&state));
      assert (errno == EILSEQ);
    }

    // reset errno
    _set_errno (0);

    if (wc == WEOF) {
      break;
    }
  }
}

int main (void) {
  p32_test_init ();
  srand (0xBADF);

  assert (setlocale (LC_ALL, "POSIX") != NULL);
  assert (MB_CUR_MAX == 1);

  DoTest ();

  return EXIT_SUCCESS;
}
