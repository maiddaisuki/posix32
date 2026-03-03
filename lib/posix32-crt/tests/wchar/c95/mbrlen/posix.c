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
 * Test `mbrlen` functions with ISO-8859-1 (code page 28591).
 *
 * This code page is used with "POSIX" locale.
 */

#undef mbrlen

/**
 * `Charset` structure with information about code page 28591 (ISO-8859-1).
 */
static Charset iso_8859_1;

#undef MB_CUR_MAX
#define MB_CUR_MAX (iso_8859_1.MaxLength)

/**
 * Convenience macro to call `p32_private_mbrlen_cp28591`.
 */
#define mbrlen(mb, count, state) p32_private_mbrlen_cp28591 (mb, count, state, &iso_8859_1)

static void DoTest (void) {
  mbstate_t state = {0};

  /**
   * Calling `mbrlen (NULL, ..., state)` is equivalent to
   * `mbrtoc16 ("", 1, state)`.
   */
  assert (mbrlen (NULL, 0, &state) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * When second argument to `mbrlen` is zero, it must not examine passed
   * string or update conversion state.
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
   * All bytes are valid characters.
   *
   * POSIX requires that in "POSIX" locale all bytes are valid characters.
   */
  for (uint8_t c = 0;; ++c) {
    assert (mbrlen ((char *) &c, MB_CUR_MAX, &state) == !!c);
    assert (mbsinit (&state));
    assert (errno == 0);

    if (c == 0xFF) {
      break;
    }
  }
}

int main (void) {
  p32_test_init ();
  srand (0xBADF);

  iso_8859_1.CodePage = P32_CODEPAGE_ISO_8859_1;
  assert (p32_charset_info (&iso_8859_1));
  assert (MB_CUR_MAX == 1);

  DoTest ();

  return EXIT_SUCCESS;
}
