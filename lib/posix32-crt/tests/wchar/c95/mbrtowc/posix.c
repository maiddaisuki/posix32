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
 * Test `mbrtowc` function with "POSIX" locale.
 */

static void DoTest (void) {
  wchar_t   wc    = WEOF;
  mbstate_t state = {0};

  /**
   * Calling `mbrtowc (..., NULL, ..., state)` is equivalent to
   * `mbrtoc16 (NULL, "", 1, state)`.
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
   * POSIX requires that all bytes are valid characters.
   */
  for (uint8_t c = 0;; ++c) {
    wc = WEOF;

    assert (mbrtowc (&wc, (char *) &c, MB_CUR_MAX, &state) == !!c);
    assert (wc == c);
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

  assert (setlocale (LC_ALL, "POSIX") != NULL);
  assert (MB_CUR_MAX == 1);

  DoTest ();

  return EXIT_SUCCESS;
}
