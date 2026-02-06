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
 * Test `mbrlen` function with UTF-8 (code page 65001).
 */

#define LOCALE "en_US.UTF-8"

static void DoTest (void) {
  mbstate_t state = {0};

  /**
   * Calling `mbrlen (NULL, ..., state)` is equivalent to
   * `mbrlen ("", 1, state)`.
   */
  assert (mbrlen (NULL, 0, &state) == 0);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * When third argument to `mbrlen` is zero, it must not examine passed
   * string, update conversion state or write to output buffer.
   */
  assert (mbrlen ("", 0, &state) == (size_t) -2);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Detect invalid conversion state.
   */
  TaintConversionState (&state);

  assert (mbrlen ("", 0, &state) == (size_t) -1);
  assert (!mbsinit (&state));
  assert (errno == EINVAL);

  // reset errno
  _set_errno (0);

  // reset `state`
  ResetConversionState (&state);

  /**
   * All bytes in range [0,127] are valid ASCII characters.
   */
  for (uint8_t c = 0;; ++c) {
    assert (mbrlen ((char *) &c, 1, &state) == !!c);
    assert (mbsinit (&state));
    assert (errno == 0);

    if (c == 0x7F) {
      break;
    }
  }

  for (size_t i = 0; i < _countof (CJK); ++i) {
    /**
     * Get length of CJK character with single call.
     */
    assert (mbrlen (CJK[i].UTF8, MB_LEN_MAX, &state) == 3);
    assert (mbsinit (&state));
    assert (errno == 0);

    /**
     * Get length of CJK character with multiple calls.
     */
    assert (mbrlen (&CJK[i].UTF8[0], 1, &state) == (size_t) -2);
    assert (!mbsinit (&state));
    assert (errno == 0);

    assert (mbrlen (&CJK[i].UTF8[1], 1, &state) == (size_t) -2);
    assert (!mbsinit (&state));
    assert (errno == 0);

    assert (mbrlen (&CJK[i].UTF8[2], 1, &state) == 1);
    assert (mbsinit (&state));
    assert (errno == 0);
  }

  /**
   * Get length of UTF-8 character with length 2 (single call).
   */
  assert (mbrlen (UTF8Length2, MB_CUR_MAX, &state) == 2);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Get length of UTF-8 character with length 2 (multiple calls).
   */
  assert (mbrlen (&UTF8Length2[0], 1, &state) == (size_t) -2);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (mbrlen (&UTF8Length2[1], 1, &state) == 1);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Get length of UTF-8 character with length 3 (single call).
   */
  assert (mbrlen (UTF8Length3, MB_CUR_MAX, &state) == 3);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Get length of UTF-8 character with length 3 (multiple calls).
   */
  assert (mbrlen (&UTF8Length3[0], 1, &state) == (size_t) -2);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (mbrlen (&UTF8Length3[1], 1, &state) == (size_t) -2);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (mbrlen (&UTF8Length3[2], 1, &state) == 1);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Get length of UTF-8 character with length 4 (single call).
   */
  assert (mbrlen (UTF8Length4, MB_CUR_MAX, &state) == 4);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Get length of UTF-8 character with length 4 (multiple calls).
   */
  assert (mbrlen (&UTF8Length4[0], 1, &state) == (size_t) -2);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (mbrlen (&UTF8Length4[1], 1, &state) == (size_t) -2);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (mbrlen (&UTF8Length4[2], 1, &state) == (size_t) -2);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (mbrlen (&UTF8Length4[3], 1, &state) == 1);
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
