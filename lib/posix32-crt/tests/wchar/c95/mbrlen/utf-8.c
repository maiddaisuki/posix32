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
 * Test `mbrlen` function with UTF-8 (code page 65001).
 */

#undef mbrlen

/**
 * `Charset` structure with information about code page 65001 (UTF-8).
 */
static Charset utf8;

#undef MB_CUR_MAX
#define MB_CUR_MAX (utf8.MaxLength)

/**
 * Convenience macro to call `p32_private_mbrlen_utf8`.
 */
#define mbrlen(mb, count, state) p32_private_mbrlen_utf8 (mb, count, state, &utf8)

static void DoTest (void) {
  mbstate_t state = {0};

  /**
   * Calling `mbrlen (NULL, ..., state)` is equivalent to
   * `mbrlen ("", 1, state)`.
   */
  assert (mbrlen (NULL, 0, &state) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * When third argument to `mbrlen` is zero, it must not examine passed
   * string, update conversion state or write to output buffer.
   */
  assert (mbrlen ("", 0, &state) == (size_t) -2);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Detect invalid conversion state.
   */
  TaintConversionState (&state);

  assert (mbrlen ("", 0, &state) == (size_t) -1);
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
    assert (mbrlen ((char *) &c, MB_CUR_MAX, &state) == !!c);
    assert (mbsinit (&state));
    assert (errno == 0);

    if (c == 0x7F) {
      break;
    }
  }

  for (size_t i = 0; i < _countof (CJK); ++i) {
    /**
     * Get length of CJK character with single call.
     */
    assert (mbrlen (CJK[i].UTF8, MB_CUR_MAX, &state) == 3);
    assert (mbsinit (&state));
    assert (errno == 0);

    /**
     * Get length of CJK character with multiple calls.
     */
    assert (mbrlen (&CJK[i].UTF8[0], 1, &state) == (size_t) -2);
    assert (!mbsinit (&state));
    assert (errno == 0);

    assert (mbrlen (&CJK[i].UTF8[1], 1, &state) == (size_t) -2);
    assert (!mbsinit (&state));
    assert (errno == 0);

    assert (mbrlen (&CJK[i].UTF8[2], 1, &state) == 1);
    assert (mbsinit (&state));
    assert (errno == 0);
  }

  /**
   * Get length of UTF-8 character with length 2 (single call).
   */
  assert (mbrlen (UTF8Length2, MB_CUR_MAX, &state) == 2);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Get length of UTF-8 character with length 2 (multiple calls).
   */
  assert (mbrlen (&UTF8Length2[0], 1, &state) == (size_t) -2);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (mbrlen (&UTF8Length2[1], 1, &state) == 1);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Get length of UTF-8 character with length 3 (single call).
   */
  assert (mbrlen (UTF8Length3, MB_CUR_MAX, &state) == 3);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Get length of UTF-8 character with length 3 (multiple calls).
   */
  assert (mbrlen (&UTF8Length3[0], 1, &state) == (size_t) -2);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (mbrlen (&UTF8Length3[1], 1, &state) == (size_t) -2);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (mbrlen (&UTF8Length3[2], 1, &state) == 1);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Get length of UTF-8 character with length 4 (single call).
   */
  assert (mbrlen (UTF8Length4, MB_CUR_MAX, &state) == 4);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Get length of UTF-8 character with length 4 (multiple calls).
   */
  assert (mbrlen (&UTF8Length4[0], 1, &state) == (size_t) -2);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (mbrlen (&UTF8Length4[1], 1, &state) == (size_t) -2);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (mbrlen (&UTF8Length4[2], 1, &state) == (size_t) -2);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (mbrlen (&UTF8Length4[3], 1, &state) == 1);
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
