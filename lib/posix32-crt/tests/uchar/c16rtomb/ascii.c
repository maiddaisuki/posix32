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
 * Test `c16rtomb` function with ASCII (code page 20127).
 */

#undef c16rtomb

/**
 * `Charset` structure with information about code page 20127 (ASCII).
 */
static Charset ascii;

#undef MB_CUR_MAX
#define MB_CUR_MAX (ascii.MaxLength)

/**
 * Convenience macro to call `p32_private_c16rtomb_cp20127`.
 */
#define c16rtomb(mb, c16, state) p32_private_c16rtomb_cp20127 (mb, c16, state, &ascii)

static void DoTest (void) {
  char      buffer[MB_LEN_MAX];
  mbstate_t state = {0};

  /**
   * Detect invalid conversion state
   */
  TaintConversionState (&state);
  memset (buffer, EOF, _countof (buffer));

  assert (c16rtomb (buffer, u'\0', &state) == (size_t) -1);
  assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
  assert (!mbsinit (&state));
  assert (errno == EINVAL);

  // reset errno
  _set_errno (0);

  /**
   * Reset conversion state.
   */
  assert (c16rtomb (NULL, 0xFFFF, &state) == 1);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * All Code Points in range [0,127] are valid ASCII characters.
   */
  for (char16_t c16 = 0; c16 < 0x80; ++c16) {
    memset (buffer, EOF, _countof (buffer));

    assert (c16rtomb (buffer, c16, &state) == 1);
    assert (buffer[0] == (char) c16 && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
    assert (mbsinit (&state));
    assert (errno == 0);
  }

  /**
   * All Code Point outside of range [0,127] are invalid.
   */
  for (char16_t c16 = 0x80;; ++c16) {
    memset (buffer, EOF, _countof (buffer));

    if (IS_HIGH_SURROGATE (c16)) {
      /**
       * Consume high surrogate.
       */
      assert (c16rtomb (buffer, c16, &state) == 0);
      assert (buffer[0] == EOF);
      assert (!mbsinit (&state));
      assert (errno == 0);

      /**
       * Attempt to convert surrogate pair.
       */
      assert (c16rtomb (buffer, LOW_SURROGATE_MIN, &state) == (size_t) -1);
      assert (buffer[0] == EOF);
      assert (!mbsinit (&state));
      assert (errno == EILSEQ);

      // reset errno
      _set_errno (0);

      /**
       * Reset conversion state.
       */
      assert (c16rtomb (NULL, 0xFFFF, &state) == 1);
      assert (mbsinit (&state));
      assert (errno == 0);
    } else {
      assert (c16rtomb (buffer, c16, &state) == (size_t) -1);
      assert (buffer[0] == EOF);
      assert (mbsinit (&state));
      assert (errno == EILSEQ);

      // reset errno
      _set_errno (0);
    }

    if (c16 == 0xFFFF) {
      break;
    }
  }
}

int main (void) {
  p32_test_init ();
  srand (0xBADF);

  ascii.CodePage = P32_CODEPAGE_ASCII;
  assert (p32_charset_info (&ascii));
  assert (MB_CUR_MAX == 1);

  DoTest ();

  return EXIT_SUCCESS;
}
