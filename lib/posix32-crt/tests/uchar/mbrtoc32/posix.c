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
 * Test `mbrtoc32` function with "POSIX" locale.
 */

static void DoTest (void) {
  char32_t  u32   = 0xFFFFFFFF;
  mbstate_t state = {0};

  /**
   * Calling `mbrtoc32 (..., NULL, ..., state)` is equivalent to
   * `mbrtoc32 (NULL, "", 1, state)`.
   */
  assert (mbrtoc32 (&u32, NULL, 0, &state) == 0);
  assert (u32 == 0xFFFFFFFF);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * When third argument to `mbrtoc32` is zero, it must not examine passed
   * string, update conversion state or write to output buffer.
   */
  assert (mbrtoc32 (&u32, "", 0, &state) == (size_t) -2);
  assert (u32 == 0xFFFFFFFF);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Detect invalid conversion state.
   */
  TaintConversionState (&state);

  assert (mbrtoc32 (&u32, "", 0, &state) == (size_t) -1);
  assert (u32 == 0xFFFFFFFF);
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
    u32 = 0xFFFFFFFF;

    assert (mbrtoc32 (&u32, (char *) &c, 1, &state) == !!c);
    assert (u32 == c);
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
