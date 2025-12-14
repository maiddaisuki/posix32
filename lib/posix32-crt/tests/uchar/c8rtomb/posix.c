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
 * Test `c8rtomb` function with "POSIX" locale.
 */

static void DoTest (void) {
  char      buffer[4];
  mbstate_t state = {0};

  memset (buffer, EOF, _countof (buffer));

  /**
   * Detect invalid conversion state.
   */
  TaintConversionState (&state);

  assert (c8rtomb (buffer, 0xFF, &state) == (size_t) -1);
  assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
  assert (!mbsinit (&state));
  assert (errno == EINVAL);

  // reset errno
  _set_errno (0);

  /**
   * Reset conversion state.
   */
  assert (c8rtomb (NULL, 0xFF, &state) == 1);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * All Code Points in range [0,127] are valid ASCII characters.
   */
  for (char8_t c = 0;; ++c) {
    memset (buffer, EOF, _countof (buffer));

    assert (c8rtomb (buffer, c, &state) == 1);
    assert (buffer[0] == (char) c && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
    assert (mbsinit (&state));
    assert (errno == 0);

    if (c == 0x7F) {
      break;
    }
  }

  /**
   * Convert UTF-8 Character with length 2.
   *
   * "¥" is 0xA5 in ISO-8859-1.
   */
  memset (buffer, EOF, _countof (buffer));

  assert (c8rtomb (buffer, (char8_t) UTF8Length2[0], &state) == 0);
  assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (c8rtomb (buffer, (char8_t) UTF8Length2[1], &state) == 1);
  assert (buffer[0] == C (0xA5) && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Attempt to convert UTF-8 Character with length 3.
   */
  memset (buffer, EOF, _countof (buffer));

  assert (c8rtomb (buffer, (char8_t) UTF8Length3[0], &state) == 0);
  assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (c8rtomb (buffer, (char8_t) UTF8Length3[1], &state) == 0);
  assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (c8rtomb (buffer, (char8_t) UTF8Length3[2], &state) == (size_t) -1);
  assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
  assert (!mbsinit (&state));
  assert (errno == EILSEQ);

  // reset errno
  _set_errno (0);

  // reset conversion state
  assert (c8rtomb (NULL, 0xFF, &state) == 1);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Attempt to convert UTF-8 Character with length 4.
   */
  memset (buffer, EOF, _countof (buffer));

  assert (c8rtomb (buffer, (char8_t) UTF8Length4[0], &state) == 0);
  assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (c8rtomb (buffer, (char8_t) UTF8Length4[1], &state) == 0);
  assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (c8rtomb (buffer, (char8_t) UTF8Length4[2], &state) == 0);
  assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (c8rtomb (buffer, (char8_t) UTF8Length4[3], &state) == (size_t) -1);
  assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
  assert (!mbsinit (&state));
  assert (errno == EILSEQ);

  // reset errno
  _set_errno (0);

  // reset conversion state
  assert (c8rtomb (NULL, 0xFF, &state) == 1);
  assert (mbsinit (&state));
  assert (errno == 0);
}

int main (void) {
  p32_test_init ();
  srand (0xBADF);

  assert (setlocale (LC_ALL, "POSIX") != NULL);
  assert (MB_CUR_MAX == 1);

  DoTest ();

  return EXIT_SUCCESS;
}
