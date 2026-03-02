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
 * Test `mbrtowc` function with UTF-8 (code page 65001).
 */

#undef mbrtowc

/**
 * `Charset` structure with information about code page 65001 (UTF-8).
 */
static Charset utf8;

#undef MB_CUR_MAX
#define MB_CUR_MAX (utf8.MaxLength)

/**
 * Convenience macro to call `p32_private_mbrtowc_utf8`.
 */
#define mbrtowc(wc, mb, count, state) p32_private_mbrtowc_utf8 (wc, mb, count, state, &utf8)

static void DoTest (void) {
  mbstate_t state = {0};
  wchar_t   wc    = WEOF;

  /**
   * Calling `mbrtowc (..., NULL, ..., state)` is equivalent to
   * `mbrtowc (NULL, "", 1, state)`.
   */
  assert (mbrtowc (&wc, NULL, 0, &state) == 0);
  assert (wc == WEOF);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * When third argument to `mbrtowc` is zero, it must not examine passed
   * string, update conversion state or write to output buffer.
   */
  assert (mbrtowc (&wc, "", 0, &state) == (size_t) -2);
  assert (wc == WEOF);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Detect invalid conversion state.
   */
  TaintConversionState (&state);

  assert (mbrtowc (&wc, "", 0, &state) == (size_t) -1);
  assert (wc == WEOF);
  assert (!mbsinit (&state));
  assert (errno == EINVAL);

  // reset errno
  _set_errno (0);

  // reset `state`
  ResetConversionState (&state);

  /**
   * All bytes in range [0,127] are valid ASCII characters.
   */
  for (uint8_t c = 0;; ++c) {
    assert (mbrtowc (&wc, (char *) &c, MB_CUR_MAX, &state) == !!c);
    assert (wc == c);
    assert (mbsinit (&state));
    assert (errno == 0);

    if (c == 0x7F) {
      break;
    }
  }

  for (size_t i = 0; i < _countof (CJK); ++i) {
    /**
     * Convert CJK character with single call.
     */
    wc = WEOF;

    assert (mbrtowc (&wc, CJK[i].UTF8, MB_CUR_MAX, &state) == 3);
    assert (wc == CJK[i].UTF16);
    assert (mbsinit (&state));
    assert (errno == 0);

    /**
     * Convert CJK character with multiple calls.
     */
    wc = WEOF;

    assert (mbrtowc (&wc, &CJK[i].UTF8[0], 1, &state) == (size_t) -2);
    assert (wc == WEOF);
    assert (!mbsinit (&state));
    assert (errno == 0);

    assert (mbrtowc (&wc, &CJK[i].UTF8[1], 1, &state) == (size_t) -2);
    assert (wc == WEOF);
    assert (!mbsinit (&state));
    assert (errno == 0);

    assert (mbrtowc (&wc, &CJK[i].UTF8[2], 1, &state) == 1);
    assert (wc == CJK[i].Wc);
    assert (mbsinit (&state));
    assert (errno == 0);
  }

  /**
   * Convert UTF-8 character with length 2 (single call).
   */
  wc = WEOF;

  assert (mbrtowc (&wc, UTF8Length2, MB_CUR_MAX, &state) == 2);
  assert (wc == L'¥');
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert UTF-8 character with length 2 (multiple calls).
   */
  wc = WEOF;

  assert (mbrtowc (&wc, &UTF8Length2[0], 1, &state) == (size_t) -2);
  assert (wc == WEOF);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (mbrtowc (&wc, &UTF8Length2[1], 1, &state) == 1);
  assert (wc == L'¥');
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert UTF-8 character with length 3 (single call).
   */
  wc = WEOF;

  assert (mbrtowc (&wc, UTF8Length3, MB_CUR_MAX, &state) == 3);
  assert (wc == L'語');
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert UTF-8 character with length 3 (multiple calls).
   */
  wc = WEOF;

  assert (mbrtowc (&wc, &UTF8Length3[0], 1, &state) == (size_t) -2);
  assert (wc == WEOF);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (mbrtowc (&wc, &UTF8Length3[1], 1, &state) == (size_t) -2);
  assert (wc == WEOF);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (mbrtowc (&wc, &UTF8Length3[2], 1, &state) == 1);
  assert (wc == L'語');
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert UTF-8 character with length 4.
   */

  /**
   * Convert UTF-8 character with length 4 (single call).
   */
  wc = WEOF;

  assert (mbrtowc (&wc, UTF8Length4, MB_CUR_MAX, &state) == 4);
  assert (wc == 0xFFFD);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert UTF-8 character with length 4 (multiple calls).
   */
  wc = WEOF;

  assert (mbrtowc (&wc, &UTF8Length4[0], 1, &state) == (size_t) -2);
  assert (wc == WEOF);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (mbrtowc (&wc, &UTF8Length4[1], 1, &state) == (size_t) -2);
  assert (wc == WEOF);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (mbrtowc (&wc, &UTF8Length4[2], 1, &state) == (size_t) -2);
  assert (wc == WEOF);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (mbrtowc (&wc, &UTF8Length4[3], 1, &state) == 1);
  assert (wc == 0xFFFD);
  assert (mbsinit (&state));
  assert (errno == 0);
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
