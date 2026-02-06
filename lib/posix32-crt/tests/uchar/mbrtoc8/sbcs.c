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
 * Test `mbrtoc8` function with single-byte character set.
 */

#define LOCALE "en_US.ISO-8859-1"

static void DoTest (void) {
  char8_t   u8[MB_LEN_MAX];
  mbstate_t state = {0};

  memset (u8, EOF, _countof (u8));

  /**
   * Calling `mbrtoc8 (..., NULL, ..., state)` is equivalent to
   * `mbrtoc8 (NULL, "", 1, state)`.
   */
  assert (mbrtoc8 (u8, NULL, 0, &state) == 0);
  assert (u8[0] == 0xFF && u8[1] == 0xFF && u8[2] == 0xFF && u8[3] == 0xFF);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * When third argument to `mbrtoc8` is zero, it must not examine passed
   * string, update conversion state or write to output buffer.
   */
  assert (mbrtoc8 (u8, "", 0, &state) == (size_t) -2);
  assert (u8[0] == 0xFF && u8[1] == 0xFF && u8[2] == 0xFF && u8[3] == 0xFF);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Detect invalid conversion state.
   */
  TaintConversionState (&state);

  assert (mbrtoc8 (u8, "", 0, &state) == (size_t) -1);
  assert (u8[0] == 0xFF && u8[1] == 0xFF && u8[2] == 0xFF && u8[3] == 0xFF);
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
    memset (u8, EOF, _countof (u8));

    assert (mbrtoc8 (u8, (char *) &c, 1, &state) == !!c);
    assert (u8[0] == c && u8[1] == 0xFF && u8[2] == 0xFF && u8[3] == 0xFF);
    assert (mbsinit (&state));
    assert (errno == 0);

    if (c == 0x7F) {
      break;
    }
  }

  /**
   * All bytes in range [128,255] are valid characters.
   */
  for (uint8_t c = 0x80;; ++c) {
    memset (u8, EOF, _countof (u8));

    assert (mbrtoc8 (&u8[0], (char *) &c, 1, &state) == 1);
    assert (u8[0] != 0xFF && u8[1] == 0xFF && u8[2] == 0xFF && u8[3] == 0xFF);
    assert (!mbsinit (&state));
    assert (errno == 0);

    assert (mbrtoc8 (&u8[1], "", 0, &state) == (size_t) -3);
    assert (u8[0] != 0xFF && u8[1] != 0xFF && u8[2] == 0xFF && u8[3] == 0xFF);
    assert (mbsinit (&state));
    assert (errno == 0);

    if (c == 0xFF) {
      break;
    }
  }
}

static DWORD CALLBACK Thread (LPVOID arg) {
  const char *localeString = arg;

  locale_t locale = newlocale (LC_ALL_MASK, localeString, NULL);
  assert (locale != NULL && uselocale (locale) != NULL);
  assert (MB_CUR_MAX == 1);

  DoTest ();

  assert (uselocale (LC_GLOBAL_LOCALE) == locale);
  freelocale (locale);

  return EXIT_SUCCESS;
}

int main (void) {
  p32_test_init ();
  srand (0xBADF);

  assert (setlocale (LC_ALL, LOCALE) != NULL);
  assert (MB_CUR_MAX == 1);

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
