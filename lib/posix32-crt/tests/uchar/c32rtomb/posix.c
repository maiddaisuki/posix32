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
 * Test `c32rtomb` function with ISO-8859-1 (code page 28591).
 *
 * This code page is used with "POSIX" locale.
 */

#undef c32rtomb

/**
 * `Charset` structure with information about code page 28591 (ISO-8859-1).
 */
static Charset iso_8859_1;

#undef MB_CUR_MAX
#define MB_CUR_MAX (iso_8859_1.MaxLength)

/**
 * Convenience macro to call `p32_private_c32rtomb_cp28591`.
 */
#define c32rtomb(mb, c32, state) p32_private_c32rtomb_cp28591 (mb, c32, state, &iso_8859_1)

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
   * All Code Points in range [0,255] are valid characters.
   *
   * POSIX requires that in "POSIX" locale all bytes are valid characters.
   *
   * POSIX does not mention whether `c32rtomb` must support reverse conversion
   * for bytes converted by `mbrtoc32` which are outside of ASCII range.
   */
  for (char32_t c32 = 0; c32 < 0x100; ++c32) {
    memset (buffer, EOF, _countof (buffer));

    assert (c32rtomb (buffer, c32, &state) == 1);
    assert (buffer[0] == (char) c32 && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
    assert (mbsinit (&state));
    assert (errno == 0);
  }

  /**
   * All Code Point outside of range [0,255] are invalid.
   */
  for (char32_t c32 = 0x100; c32 <= 0x0010FFFF; ++c32) {
    memset (buffer, EOF, _countof (buffer));

    assert (c32rtomb (buffer, c32, &state) == (size_t) -1);
    assert (buffer[0] == EOF);
    assert (mbsinit (&state));
    assert (errno == EILSEQ);

    // reset errno
    _set_errno (0);
  }
}

int main (void) {
  p32_test_init ();
  srand (0xBADF);

  iso_8859_1.CodePage = P32_CODEPAGE_POSIX;
  assert (p32_charset_info (&iso_8859_1));
  assert (MB_CUR_MAX == 1);

  DoTest ();

  return EXIT_SUCCESS;
}
