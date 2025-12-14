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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "uchar-test.h"

/**
 * Test Summary:
 *
 * Test `mbrtoc32` function with UTF-8 (code page 65001).
 */

#define LOCALE "en_US.UTF-8"

static void DoTest (void) {
  char32_t  u32   = 0xFFFFFFF;
  mbstate_t state = {0};

  /**
   * Calling `mbrtoc32 (..., NULL, ..., state)` is equivalent to
   * `mbrtoc32 (NULL, "", 1, state)`.
   */
  assert (mbrtoc32 (&u32, NULL, 0, &state) == 0);
  assert (u32 == 0xFFFFFFF);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * When third argument to `mbrtoc32` is zero, it must not examine passed
   * string, update conversion state or write to output buffer.
   */
  assert (mbrtoc32 (&u32, "", 0, &state) == (size_t) -2);
  assert (u32 == 0xFFFFFFF);
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Detect invalid conversion state.
   */
  TaintConversionState (&state);

  assert (mbrtoc32 (&u32, "", 0, &state) == (size_t) -1);
  assert (u32 == 0xFFFFFFF);
  assert (!mbsinit (&state));
  assert (errno == EINVAL);

  // reset errno
  _set_errno (0);

  // reset `state`
  ResetConversionState (&state);

  /**
   * All bytes in range [0,127] are valid ASCII characters.
   */
  for (uint8_t u8 = 0; u8 < 0x80; ++u8) {
    const char u8str[4] = {u8, 0, 0, 0};
    u32                 = 0xFFFFFFF;

    assert (mbrtoc32 (&u32, u8str, 4, &state) == !!u8);
    assert (u32 == u8);
    assert (mbsinit (&state));
    assert (errno == 0);
  }

  /**
   * Attempt to convert bytes in range [128,255] followed by NUL bytes.
   */
  for (uint8_t u8 = 0x80;; ++u8) {
    const char u8str[4] = {u8, 0, 0, 0};
    u32                 = 0xFFFFFFFF;

    assert (mbrtoc32 (&u32, u8str, 4, &state) == (size_t) -1);
    assert (u32 == 0xFFFFFFFF);
    assert (mbsinit (&state));
    assert (errno == EILSEQ);

    // reset errno
    _set_errno (0);

    if (u8 == 0xFF) {
      break;
    }
  }

  for (size_t i = 0; i < _countof (CJK); ++i) {
    /**
     * Convert CJK character with single call.
     */
    u32 = 0xFFFFFFFF;

    assert (mbrtoc32 (&u32, CJK[i].UTF8, MB_LEN_MAX, &state) == 3);
    assert (u32 == CJK[i].UTF32);
    assert (mbsinit (&state));
    assert (errno == 0);

    /**
     * Convert CJK character with multiple calls.
     */
    u32 = 0xFFFFFFFF;

    assert (mbrtoc32 (&u32, &CJK[i].UTF8[0], 1, &state) == (size_t) -2);
    assert (u32 == 0xFFFFFFFF);
    assert (!mbsinit (&state));
    assert (errno == 0);

    assert (mbrtoc32 (&u32, &CJK[i].UTF8[1], 1, &state) == (size_t) -2);
    assert (u32 == 0xFFFFFFFF);
    assert (!mbsinit (&state));
    assert (errno == 0);

    assert (mbrtoc32 (&u32, &CJK[i].UTF8[2], 1, &state) == 1);
    assert (u32 == CJK[i].UTF32);
    assert (mbsinit (&state));
    assert (errno == 0);
  }

  /**
   * Convert UTF-8 character with length 2 (single call).
   */
  u32 = 0xFFFFFFFF;

  assert (mbrtoc32 (&u32, UTF8Length2, 4, &state) == 2);
  assert (u32 == U'¥');
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert UTF-8 character with length 2 (multiple calls).
   */
  u32 = 0xFFFFFFFF;

  assert (mbrtoc32 (&u32, &UTF8Length2[0], 1, &state) == (size_t) -2);
  assert (u32 == 0xFFFFFFFF);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (mbrtoc32 (&u32, &UTF8Length2[1], 1, &state) == 1);
  assert (u32 == U'¥');
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert UTF-8 character with length 3 (single call).
   */
  u32 = 0xFFFFFFFF;

  assert (mbrtoc32 (&u32, UTF8Length3, 4, &state) == 3);
  assert (u32 == U'語');
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert UTF-8 character with length 3 (multiple calls).
   */
  u32 = 0xFFFFFFFF;

  assert (mbrtoc32 (&u32, UTF8Length3, 1, &state) == (size_t) -2);
  assert (u32 == 0xFFFFFFFF);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (mbrtoc32 (&u32, UTF8Length3 + 1, 1, &state) == (size_t) -2);
  assert (u32 == 0xFFFFFFFF);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (mbrtoc32 (&u32, UTF8Length3 + 2, 1, &state) == 1);
  assert (u32 == U'語');
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert UTF-8 character with length 4 (single call).
   */
  u32 = 0xFFFFFFFF;

  assert (mbrtoc32 (&u32, UTF8Length4, 4, &state) == 4);
  assert (u32 == U'🧡');
  assert (mbsinit (&state));
  assert (errno == 0);

  /**
   * Convert UTF-8 character with length 4 (multiple calls).
   */
  u32 = 0xFFFFFFFF;

  assert (mbrtoc32 (&u32, &UTF8Length4[0], 1, &state) == (size_t) -2);
  assert (u32 == 0xFFFFFFFF);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (mbrtoc32 (&u32, &UTF8Length4[1], 1, &state) == (size_t) -2);
  assert (u32 == 0xFFFFFFFF);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (mbrtoc32 (&u32, &UTF8Length4[2], 1, &state) == (size_t) -2);
  assert (u32 == 0xFFFFFFFF);
  assert (!mbsinit (&state));
  assert (errno == 0);

  assert (mbrtoc32 (&u32, &UTF8Length4[3], 1, &state) == 1);
  assert (u32 == U'🧡');
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
