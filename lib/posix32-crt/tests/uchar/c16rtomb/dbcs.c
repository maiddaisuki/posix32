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
 * Test `c16rtomb` function with a double-byte code page.
 */

#if P32_CRT >= P32_MSVCRT20
#define LOCALE "ja_JP.ACP"
#else
#define LOCALE "ja_JP.OCP"
#endif

static void DoTest (void) {
  char      buffer[MB_LEN_MAX];
  mbstate_t state = {0};

  /**
   * Detect invalid conversion state
   */
  TaintConversionState (&state);
  memset (buffer, EOF, _countof (buffer));

  assert (c16rtomb (buffer, u'\0', &state) == (size_t) -1);
  assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
  assert (!mbsinit (&state));
  assert (errno == EINVAL);

  // reset errno
  _set_errno (0);

  /**
   * Reset conversion state.
   */
  assert (c16rtomb (NULL, 0xFFFF, &state) == 1);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * All Code Points in range [0,127] are valid ASCII characters.
   */
  for (char16_t c16 = 0; c16 < 0x80; ++c16) {
    memset (buffer, EOF, _countof (buffer));

    assert (c16rtomb (buffer, c16, &state) == 1);
    assert (buffer[0] == (char) c16 && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
    assert (mbsinit (&state));
    assert (errno == 0);
  }

  for (size_t i = 0; i < _countof (CJK); ++i) {
    memset (buffer, EOF, _countof (buffer));

    assert (c16rtomb (buffer, CJK[i].UTF16, &state) == 2);
    assert (memcmp (CJK[i].DBCS, buffer, 2) == 0);
    assert (mbsinit (&state));
    assert (errno == 0);
  }

  /**
   * Attempt to convert UTF-16 surrogate pairs.
   */
  for (char16_t hs = HIGH_SURROGATE_MIN; hs <= HIGH_SURROGATE_MAX; ++hs) {
    for (char16_t ls = LOW_SURROGATE_MIN; ls <= LOW_SURROGATE_MAX; ++ls) {
      memset (buffer, EOF, _countof (buffer));

      /**
       * Consume high surrogate.
       */
      assert (c16rtomb (buffer, hs, &state) == 0);
      assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
      assert (!mbsinit (&state));
      assert (errno == 0);

      /**
       * Attempt to convert surrogate pair.
       */
      assert (c16rtomb (buffer, ls, &state) == (size_t) -1);
      assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
      assert (!mbsinit (&state));
      assert (errno == EILSEQ);

      // reset errno
      _set_errno (0);

      /**
       * Reset conversion state.
       */
      assert (c16rtomb (NULL, 0xFFFF, &state) == 1);
      assert (mbsinit (&state));
      assert (errno == 0);
    }
  }
}

static DWORD CALLBACK Thread (LPVOID arg) {
  const char *localeString = arg;

  locale_t locale = newlocale (LC_ALL_MASK, localeString, NULL);
  assert (locale != NULL && uselocale (locale) != NULL);
  assert (MB_CUR_MAX == 2);

  DoTest ();

  assert (uselocale (LC_GLOBAL_LOCALE) == locale);
  freelocale (locale);

  return EXIT_SUCCESS;
}

int main (void) {
  p32_test_init ();
  srand (0xBADF);

#if P32_CRT == P32_MSVCRT10
  assert (setlocale (LC_ALL, LOCALE) == NULL);
#else
  assert (setlocale (LC_ALL, LOCALE) != NULL);
  assert (MB_CUR_MAX == 2);

  DoTest ();
#endif

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
