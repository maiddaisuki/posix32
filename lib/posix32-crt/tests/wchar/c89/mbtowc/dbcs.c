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
 * Test `mbtowc` function with a double-byte code page.
 */

#if P32_CRT >= P32_MSVCRT20
#define LOCALE "ja_JP.ACP"
#else
#define LOCALE "ja_JP.OCP"
#endif

static void DoTest (void) {
  wchar_t wc = WEOF;

  /**
   * When second argument to `mbtowc` is `NULL`, it must return non-zero for
   * state-dependant encodings. Otherwise it must return 0.
   */
  assert (mbtowc (&wc, NULL, 0) == 0);
  assert (errno == 0);

  /**
   * When third argument to `mbtowc` is zero, it must not examine its second
   * argument.
   */
  assert (mbtowc (&wc, "", 0) == -1);
  assert (errno == 0);

  /**
   * All bytes in range [0,127] are valid ASCII characters.
   */
  for (uint8_t c = 0;; ++c) {
    wc = WEOF;

    assert (mbtowc (&wc, (char *) &c, 1) == !!c);
    assert (wc == c);
    assert (errno == 0);

    if (c == 0x7F) {
      break;
    }
  }

  for (size_t i = 0; i < _countof (CJK); ++i) {
    /**
     * Convert DBCS character with single call.
     */
    wc = WEOF;

    assert (mbtowc (&wc, CJK[i].DBCS, MB_CUR_MAX) == 2);
    assert (wc == CJK[i].Wc);
    assert (errno == 0);

    /**
     * Probe internal conversion state.
     */
    assert (mbtowc (&wc, "", 1) == 0);
    assert (wc == '\0');
    assert (errno == 0);

    /**
     * Convert DBCS character with multiple call.
     */
    wc = WEOF;

    assert (mbtowc (&wc, CJK[i].DBCS, 1) == -1);
    assert (wc == WEOF);
    assert (errno == 0);

    /**
     * Probe internal conversion state.
     *
     * Unlike `mblen`, `mbtowc` stores incomplete multibyte sequence in its
     * private conversion state.
     */
    assert (mbtowc (&wc, "", 1) == -1);
    assert (wc == WEOF);
    assert (errno == EILSEQ);

    // reset errno
    _set_errno (0);

    /**
     * Complete conversion of DBCS character.
     */
    assert (mbtowc (&wc, &CJK[i].DBCS[1], 1) == 1);
    assert (wc == CJK[i].Wc);
    assert (errno == 0);

    /**
     * Probe internal conversion state.
     */
    assert (mbtowc (&wc, "", 1) == 0);
    assert (wc == '\0');
    assert (errno == 0);

    /**
     * Attempt to convert invalid DBCS character.
     */
    char InvalidDBCS[2] = {CJK[i].DBCS[0], '\0'};

    wc = WEOF;

    assert (mbtowc (&wc, InvalidDBCS, MB_CUR_MAX) == -1);
    assert (wc == WEOF);
    assert (errno == EILSEQ);

    // reset errno
    _set_errno (0);
  }

  /**
   * Probe internal conversion state.
   */
  wc = WEOF;

  assert (mbtowc (&wc, "", 1) == 0);
  assert (wc == '\0');
  assert (errno == 0);
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
