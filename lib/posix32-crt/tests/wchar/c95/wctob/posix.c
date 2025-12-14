/**
 * Copyright 2025 Kirill Makurin
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
 * Test `wctob` function with "POSIX" locale.
 */

static void DoTest (void) {
  /**
   * POSIX requires that all bytes are valid characters.
   *
   * POSIX does not mention whether `wctob` must support reverse conversion
   * for bytes converted by `btowc` which are outside of ASCII range.
   */
  for (wchar_t wc = 0; wc < 0x100; ++wc) {
    assert (wctob (wc) == wc);
    assert (errno == 0);
  }

  /**
   * All wide characters in range [255,WEOF] are invalid.
   */
  for (wchar_t wc = 0x100;; ++wc) {
    assert (wctob (wc) == EOF);
    assert (errno == 0);

    if (wc == WEOF) {
      break;
    }
  }
}

int main (void) {
  p32_test_init ();

  assert (setlocale (LC_ALL, "POSIX") != NULL);
  assert (MB_CUR_MAX == 1);

  DoTest ();

  return EXIT_SUCCESS;
}
