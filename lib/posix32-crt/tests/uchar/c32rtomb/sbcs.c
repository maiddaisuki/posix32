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
 * Test `c32rtomb` function with some SBCS code page.
 *
 * We test code page 1252; this is the ANSI code page for `en-US` locale.
 * All 256 bytes in this code page are assigned code points.
 */

#undef c32rtomb

/**
 * `Charset` structure with information about code page 1252.
 */
static Charset cp1252;

#undef MB_CUR_MAX
#define MB_CUR_MAX (cp1252.MaxLength)

/**
 * Convenience macro to call `p32_private_c32rtomb_sbcs`.
 */
#define c32rtomb(mb, c32, state) p32_private_c32rtomb_sbcs (mb, c32, state, &cp1252)

static void DoTest (void) {
  char      buffer[MB_LEN_MAX];
  mbstate_t state = {0};

  /**
   * Detect invalid conversion state
   */
  TaintConversionState (&state);
  memset (buffer, EOF, _countof (buffer));

  assert (c32rtomb (buffer, U'\0', &state) == (size_t) -1);
  assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
  assert (!mbsinit (&state));
  assert (errno == EINVAL);

  // reset errno
  _set_errno (0);

  /**
   * Reset conversion state.
   */
  assert (c32rtomb (NULL, 0xFFFFFFFF, &state) == 1);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * All Code Points in range [0,127] are valid ASCII characters.
   */
  for (char32_t c32 = 0; c32 < 0x80; ++c32) {
    memset (buffer, EOF, _countof (buffer));

    assert (c32rtomb (buffer, c32, &state) == 1);
    assert (buffer[0] == (char) c32 && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
    assert (mbsinit (&state));
    assert (errno == 0);
  }

  uint32_t codePointsConverted = 0;

  /**
   * Attempt to convert Code Point outside of range [0,127].
   *
   * We should be able to convert exactly 128 code ponts.
   */
  for (char32_t c32 = 0x80; c32 <= 0x0010FFFF; ++c32) {
    memset (buffer, EOF, _countof (buffer));

    size_t length = c32rtomb (buffer, c32, &state);

    if (length == (size_t) -1) {
      assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
      assert (mbsinit (&state));
      assert (errno == EILSEQ);

      // reset errno
      _set_errno (0);
    } else {
      assert (length == 1);
      assert (buffer[0] < 0 && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
      assert (mbsinit (&state));
      assert (errno == 0);

      codePointsConverted += 1;
    }
  }

  assert (codePointsConverted == 128);
}

int main (void) {
  p32_test_init ();
  srand (0xBADF);

  if (!IsValidCodePage (1252)) {
    return 77;
  }

  cp1252.CodePage = 1252;
  assert (p32_charset_info (&cp1252));
  assert (MB_CUR_MAX == 1);

  DoTest ();

  return EXIT_SUCCESS;
}
