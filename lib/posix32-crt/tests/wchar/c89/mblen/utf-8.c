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
 * Test `mblen` function with UTF-8 (code page 65001).
 */

#define LOCALE "en_US.UTF-8"

static void DoTest (void) {
  /**
   * When first argument to `mblen` is `NULL`, it must return non-zero for
   * state-dependant encodings. Otherwise it must return 0.
   */
  assert (mblen (NULL, 0) == 0);
  assert (errno == 0);

  /**
   * When second argument to `mblen` is zero, it must not examine its first
   * argument.
   */
  assert (mblen ("", 0) == -1);
  assert (errno == 0);

  /**
   * All bytes in range [0,127] are valid ASCII characters.
   */
  for (uint8_t c = 0;; ++c) {
    assert (mblen ((char *) &c, MB_CUR_MAX) == !!c);
    assert (errno == 0);

    if (c == 0x7F) {
      break;
    }
  }

  for (size_t i = 0; i < _countof (CJK); ++i) {
    /**
     * Attempt to get length of incomplete UTF-8 character.
     */
    assert (mblen (&CJK[i].UTF8[0], 1) == -1);
    assert (errno == 0);

    /**
     * Verify that internal conversion state is sane.
     */
    assert (mblen ("", 1) == 0);
    assert (errno == 0);

    /**
     * Get length of UTF-8 character.
     */
    assert (mblen (CJK[i].UTF8, MB_CUR_MAX) == 3);
    assert (errno == 0);
  }

  /**
   * Get length of UTF-8 character with length 2.
   */
  assert (mblen (UTF8Length2, 0) == -1);
  assert (errno == 0);

  assert (mblen (UTF8Length2, 1) == -1);
  assert (errno == 0);

  assert (mblen (UTF8Length2, MB_CUR_MAX) == 2);
  assert (errno == 0);

  /**
   * Get length of UTF-8 character with length 3.
   */
  assert (mblen (UTF8Length3, 0) == -1);
  assert (errno == 0);

  assert (mblen (UTF8Length3, 1) == -1);
  assert (errno == 0);

  assert (mblen (UTF8Length3, 2) == -1);
  assert (errno == 0);

  assert (mblen (UTF8Length3, MB_CUR_MAX) == 3);
  assert (errno == 0);

  /**
   * Get length of UTF-8 character with length 4.
   */
  assert (mblen (UTF8Length4, 0) == -1);
  assert (errno == 0);

  assert (mblen (UTF8Length4, 1) == -1);
  assert (errno == 0);

  assert (mblen (UTF8Length4, 2) == -1);
  assert (errno == 0);

  assert (mblen (UTF8Length4, 3) == -1);
  assert (errno == 0);

  assert (mblen (UTF8Length4, MB_CUR_MAX) == 4);
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
