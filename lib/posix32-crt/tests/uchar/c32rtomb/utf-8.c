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
 * Test `c32rtomb` function with UTF-8 (code page 65001).
 */

#define LOCALE "en_US.UTF-8"

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
   * All Code Points in range [0,127] are valid ASCII characters.
   */
  for (char32_t c32 = 0; c32 < 0x80; ++c32) {
    memset (buffer, EOF, _countof (buffer));

    assert (c32rtomb (buffer, c32, &state) == 1);
    assert (buffer[0] == (char) c32 && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
    assert (mbsinit (&state));
    assert (errno == 0);
  }

  /**
   * Convert Code Points which will produce two UTF-8 Code Units.
   */
  for (char32_t c32 = 0x80; c32 < 0x800; ++c32) {
    memset (buffer, EOF, _countof (buffer));

    assert (c32rtomb (buffer, c32, &state) == 2);
    assert (buffer[0] != EOF && buffer[1] != EOF && buffer[2] == EOF && buffer[3] == EOF);
    assert (mbsinit (&state));
    assert (errno == 0);
  }

  /**
   * Convert Code Points which will produce three UTF-8 Code Units.
   */
  for (char32_t c32 = 0x800; c32 < 0x10000; ++c32) {
    memset (buffer, EOF, _countof (buffer));

    /**
     * Attempts to convert UTF-16 surrogates must fail.
     */
    if (IS_LOW_SURROGATE (c32) || IS_HIGH_SURROGATE (c32)) {
      assert (c32rtomb (buffer, c32, &state) == (size_t) -1);
      assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
      assert (mbsinit (&state));
      assert (errno == EILSEQ);

      // reset errno
      _set_errno (0);

      assert (c32rtomb (NULL, (char32_t) -1, &state) == 1);
      assert (mbsinit (&state));
      assert (errno == 0);
    } else {
      assert (c32rtomb (buffer, c32, &state) == 3);
      assert (buffer[0] != EOF && buffer[1] != EOF && buffer[2] != EOF && buffer[3] == EOF);
      assert (mbsinit (&state));
      assert (errno == 0);
    }
  }

  /**
   * Convert Code Points which will produce four UTF-8 Code Units.
   */
  for (char32_t c32 = 0x10000; c32 <= 0x0010FFFF; ++c32) {
    memset (buffer, EOF, _countof (buffer));

    assert (c32rtomb (buffer, c32, &state) == 4);
    assert (buffer[0] != EOF && buffer[1] != EOF && buffer[2] != EOF && buffer[3] != EOF);
    assert (mbsinit (&state));
    assert (errno == 0);
  }

  /**
   * Attempt to convert out-of-range Code Points.
   */
  for (char32_t c32 = 0x00110000; c32 <= 0x00220000; ++c32) {
    memset (buffer, EOF, _countof (buffer));

    assert (c32rtomb (buffer, c32, &state) == (size_t) -1);
    assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
    assert (mbsinit (&state));
    assert (errno == EILSEQ);

    // reset errno
    _set_errno (0);
  }
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
