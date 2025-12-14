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
 * Test `wcrtomb` function with a double-byte code page.
 */

#if P32_CRT >= P32_MSVCRT20
#define LOCALE "ja_JP.ACP"
#else
#define LOCALE "ja_JP.OCP"
#endif

static void DoTest (void) {
  mbstate_t state = {0};
  char      buffer[MB_LEN_MAX];

  /**
   * Detect invalid conversion state
   */
  TaintConversionState (&state);
  memset (buffer, EOF, _countof (buffer));

  assert (wcrtomb (buffer, L'\0', &state) == (size_t) -1);
  assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
  assert (!mbsinit (&state));
  assert (errno == EINVAL);

  // reset errno
  _set_errno (0);

  /**
   * Reset conversion state.
   */
  assert (wcrtomb (NULL, WEOF, &state) == 1);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * All wide characters in range [0,127] are valid ASCII characters.
   */
  for (wchar_t wc = 0; wc < 0x80; ++wc) {
    memset (buffer, EOF, _countof (buffer));

    assert (wcrtomb (buffer, wc, &state) == 1);
    assert (buffer[0] == (char) wc && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
    assert (mbsinit (&state));
    assert (errno == 0);
  }

  for (size_t i = 0; i < _countof (CJK); ++i) {
    memset (buffer, EOF, _countof (buffer));

    assert (wcrtomb (buffer, CJK[i].Wc, &state) == 2);
    assert (memcmp (CJK[i].DBCS, buffer, 2) == 0);
    assert (mbsinit (&state));
    assert (errno == 0);
  }

  /**
   * Attempt to convert UTF-16 surrogates.
   */
  for (wchar_t wc = 0;; ++wc) {
    memset (buffer, EOF, _countof (buffer));

    if (IS_HIGH_SURROGATE (wc)) {
      /**
       * Consume high surrogate.
       */
      assert (wcrtomb (buffer, wc, &state) == 1);
      assert (buffer[0] == '?' && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
      assert (!mbsinit (&state));
      assert (errno == 0);

      for (wchar_t ls = LOW_SURROGATE_MIN; ls <= LOW_SURROGATE_MAX; ++ls) {
        assert (wcrtomb (&buffer[1], wc, &state) == (size_t) -1);
        assert (buffer[0] == '?' && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
        assert (!mbsinit (&state));
        assert (errno == EILSEQ);

        // reset errno
        _set_errno (0);
      }

      /**
       * Reset conversion state.
       */
      assert (wcrtomb (NULL, WEOF, &state) == 1);
      assert (mbsinit (&state));
      assert (errno == 0);
    } else if (IS_LOW_SURROGATE (wc)) {
      assert (wcrtomb (buffer, wc, &state) == (size_t) -1);
      assert (buffer[0] == EOF && buffer[1] == EOF && buffer[2] == EOF && buffer[3] == EOF);
      assert (mbsinit (&state));
      assert (errno == EILSEQ);
    }

    // reset errno
    _set_errno (0);

    if (wc == WEOF) {
      break;
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
