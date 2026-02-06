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
 * Test `c8rtomb` function with UTF-8 (code page 65001).
 */

#define LOCALE "en_US.UTF-8"

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

  for (size_t i = 0; i < _countof (CJK); ++i) {
    memset (buffer, EOF, _countof (buffer));

    assert (c8rtomb (buffer, (char8_t) CJK[i].UTF8[0], &state) == 0);
    assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
    assert (!mbsinit (&state));
    assert (errno == 0);

    assert (c8rtomb (buffer, (char8_t) CJK[i].UTF8[1], &state) == 0);
    assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
    assert (!mbsinit (&state));
    assert (errno == 0);

    assert (c8rtomb (buffer, (char8_t) CJK[i].UTF8[2], &state) == 3);
    assert (memcmp (buffer, CJK[i].UTF8, 3) == 0);
    assert (mbsinit (&state));
    assert (errno == 0);
  }

  /**
   * Convert UTF-8 Character with length 2.
   */
  memset (buffer, EOF, _countof (buffer));

  assert (c8rtomb (buffer, (char8_t) UTF8Length2[0], &state) == 0);
  assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (c8rtomb (buffer, (char8_t) UTF8Length2[1], &state) == 2);
  assert (memcmp (buffer, UTF8Length2, 2) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert UTF-8 Character with length 3.
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

  assert (c8rtomb (buffer, (char8_t) UTF8Length3[2], &state) == 3);
  assert (memcmp (buffer, UTF8Length3, 3) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert UTF-8 Character with length 4.
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

  assert (c8rtomb (buffer, (char8_t) UTF8Length4[3], &state) == 4);
  assert (memcmp (buffer, UTF8Length4, 4) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);
}

static DWORD CALLBACK Thread (LPVOID arg) {
  const char *localeString = arg;

  locale_t locale = newlocale (LC_ALL_MASK, localeString, NULL);
  assert (locale != NULL && uselocale (locale) != NULL);
  assert (MB_CUR_MAX == 4);

  DoTest ();

  assert (uselocale (LC_GLOBAL_LOCALE) == locale);
  freelocale (locale);

  return EXIT_SUCCESS;
}

int main (void) {
  p32_test_init ();
  srand (0xBADF);

  assert (setlocale (LC_ALL, LOCALE) != NULL);
  assert (MB_CUR_MAX == 4);

  DoTest ();

  assert (setlocale (LC_ALL, "C") != NULL);
  assert (MB_CUR_MAX == 1);

  HANDLE thread   = NULL;
  DWORD  exitCode = EXIT_FAILURE;

  assert ((thread = CreateThread (NULL, 0, Thread, LOCALE, 0, NULL)) != NULL);

  WaitForSingleObject (thread, INFINITE);
  GetExitCodeThread (thread, &exitCode);
  CloseHandle (thread);

  return EXIT_SUCCESS;
}
