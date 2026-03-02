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
 * Test `mbrtoc16` function with some DBCS code page.
 *
 * We test code page 932; this is the ANSI/OEM code page for `ja-JP` locale.
 */

#undef mbrtoc16
#undef mbrtoc16_l

/**
 * `Charset` structure with information about code page 932.
 */
static Charset cp932;

#undef MB_CUR_MAX
#define MB_CUR_MAX (cp932.MaxLength)

/**
 * `Charset` structure with information about DBCS code page other than 932.
 */
static Charset dbcs;

/**
 * Convenience macros to call `p32_private_mbrtoc16_dbcs`.
 */
#define mbrtoc16(c16, mb, count, state)            p32_private_mbrtoc16_dbcs (c16, mb, count, state, &cp932)
#define mbrtoc16_l(c16, mb, count, state, charset) p32_private_mbrtoc16_dbcs (c16, mb, count, state, &charset)

static void DoTest (void) {
  char16_t  u16   = 0xFFFF;
  mbstate_t state = {0};

  /**
   * Calling `mbrtoc16 (..., NULL, ..., state)` is equivalent to
   * `mbrtoc16 (NULL, "", 1, state)`.
   */
  assert (mbrtoc16 (&u16, NULL, 0, &state) == 0);
  assert (u16 == 0xFFFF);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * When third argument to `mbrtoc16` is zero, it must not examine passed
   * string, update conversion state or write to output buffer.
   */
  assert (mbrtoc16 (&u16, "", 0, &state) == (size_t) -2);
  assert (u16 == 0xFFFF);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Detect invalid conversion state.
   */
  TaintConversionState (&state);

  assert (mbrtoc16 (&u16, "", 0, &state) == (size_t) -1);
  assert (u16 == 0xFFFF);
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
    u16 = 0xFFFF;

    assert (mbrtoc16 (&u16, (char *) &c, MB_CUR_MAX, &state) == !!c);
    assert (u16 == (uint8_t) c);
    assert (mbsinit (&state));
    assert (errno == 0);

    if (c == 0x7F) {
      break;
    }
  }

  for (size_t i = 0; i < _countof (CJK); ++i) {
    /**
     * Convert DBCS character with single call.
     */
    u16 = 0xFFFF;

    assert (mbrtoc16 (&u16, CJK[i].DBCS, MB_CUR_MAX, &state) == 2);
    assert (u16 == CJK[i].UTF16);
    assert (mbsinit (&state));
    assert (errno == 0);

    /**
     * Convert DBCS character with multiple calls.
     */
    u16 = 0xFFFF;

    assert (mbrtoc16 (&u16, &CJK[i].DBCS[0], 1, &state) == (size_t) -2);
    assert (u16 == 0xFFFF);
    assert (!mbsinit (&state));
    assert (errno == 0);

    assert (mbrtoc16 (&u16, &CJK[i].DBCS[1], 1, &state) == 1);
    assert (u16 == CJK[i].UTF16);
    assert (mbsinit (&state));
    assert (errno == 0);

    /**
     * Attempt to convert invalid DBCS character.
     */
    char InvalidDBCS[2] = {CJK[i].DBCS[0], '\0'};

    u16 = 0xFFFF;

    assert (mbrtoc16 (&u16, InvalidDBCS, MB_CUR_MAX, &state) == (size_t) -1);
    assert (u16 == 0xFFFF);
    assert (mbsinit (&state));
    assert (errno == EILSEQ);

    // reset errno
    _set_errno (0);
  }

  /**
   * Consume single byte of DBCS character.
   */
  assert (mbrtoc16 (&u16, &CJK[0].DBCS[0], 1, &state) == (size_t) -2);
  assert (u16 == 0xFFFF);
  assert (!mbsinit (&state));
  assert (errno == 0);

  /**
   * Call with `0` for third argument.
   */
  assert (mbrtoc16 (&u16, &CJK[0].DBCS[1], 0, &state) == (size_t) -2);
  assert (u16 == 0xFFFF);
  assert (!mbsinit (&state));
  assert (errno == 0);

  /**
   * Call with `NULL` for second argument.
   */
  assert (mbrtoc16 (&u16, NULL, 0, &state) == (size_t) -1);
  assert (u16 == 0xFFFF);
  assert (!mbsinit (&state));
  assert (errno == EILSEQ);

  // reset errno
  _set_errno (0);

  /**
   * Attempt to complete conversion using different code page.
   */
  assert (mbrtoc16_l (&u16, "", 0, &state, dbcs) == (size_t) -1);
  assert (u16 == 0xFFFF);
  assert (!mbsinit (&state));
  assert (errno == EINVAL);

  // reset errno
  _set_errno (0);

  /**
   * Complete conversion.
   */
  assert (mbrtoc16 (&u16, &CJK[0].DBCS[1], 1, &state) == 1);
  assert (u16 == CJK[0].UTF16);
  assert (mbsinit (&state));
  assert (errno == 0);
}

int main (void) {
  p32_test_init ();
  srand (0xBADF);

  if (!IsValidCodePage (932)) {
    return 77;
  }

  cp932.CodePage = 932;
  assert (p32_charset_info (&cp932));
  assert (MB_CUR_MAX == 2);

  if (IsValidCodePage (936)) {
    dbcs.CodePage = 936;
  } else if (IsValidCodePage (949)) {
    dbcs.CodePage = 949;
  } else if (IsValidCodePage (950)) {
    dbcs.CodePage = 950;
  } else {
    return 77;
  }

  assert (p32_charset_info (&dbcs));
  assert (dbcs.MaxLength == 2);

  DoTest ();

  return EXIT_SUCCESS;
}
