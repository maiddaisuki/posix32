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
 * This test is used to verify correctness of posix32's implementation of
 * Unicode conversion functions:
 *
 * - c16rtomb
 * - mbrtoc16
 * - c32rtomb
 * - mbrtoc32
 *
 * For each valid UTF-16 Code Unit Sequence:
 *
 * 1. Convert UTF-16 Code Unit Sequence to UTF-8 Code Unit Sequence.
 *
 *  This step will also call `WideCharToMultiByte` to perform the conversion,
 *  and compare its output to that of `c16rtomb`.
 *
 *  Both functions must produce the same result.
 *
 * 2. Convert UTF-8 Code Unit Sequence from step 1 to UTF-32 Code Unit.
 *
 * 3. Convert UTF-32 Code Unit from step 2 back to UTF-8.
 *
 *  Produced UTF-8 Code Unit Sequence must be the same as input
 *  used in step 2.
 *
 * 4. Convert UTF-8 Code Unit Sequence from step 1 back to UTF-16.
 *
 *  Produced UTF-16 Code Unit Sequence must be the same as input
 *  used in step 1.
 *
 *  This step will also call `MultiByteToWideChar` to perform the conversion,
 *  and compare its output to that of `mbrtoc16`.
 *
 *  Both functions must produce the same result.
 */

static locale_t locale = NULL;

/**
 * Convert UTF-8 Code Unit Sequence to UTF-32 and back.
 */
static void DoTest (char *u8, size_t length) {
  /**
   * Convert `u8` to UTF-32.
   */
  char32_t UTF32[1];

  assert (mbrtoc32_l (UTF32, u8, length, NULL, locale) == length);

  /**
   * Convert `c32` back to UTF-8.
   */
  char UTF8[4];

  assert (c32rtomb_l (UTF8, UTF32[0], NULL, locale) == length);

  assert (memcmp (u8, UTF8, length) == 0);
}

/**
 * Convert single UTF-16 Code Unit `wc` to UTF-8, which must produce `length`
 * UTF-8 Code Units.
 */
static void DoTest1 (wchar_t wc, int length) {
  /**
   * `wc` converted to UTF-8.
   */
  struct {
    char P32[MB_LEN_MAX];
    char W32[MB_LEN_MAX];
  } Utf8FromUtf16;

  assert (c16rtomb_l (Utf8FromUtf16.P32, wc, NULL, locale) == (size_t) length);
  assert (WideCharToMultiByte (CP_UTF8, 0, &wc, 1, Utf8FromUtf16.W32, length, NULL, NULL) == length);
  assert (memcmp (Utf8FromUtf16.P32, Utf8FromUtf16.W32, length) == 0);

  DoTest (Utf8FromUtf16.P32, length);

  /**
   * UTF-8 converted back to UTF-16.
   */
  struct {
    wchar_t P32[2];
    wchar_t W32[2];
  } Utf16FromUtf8;

  assert (mbrtoc16_l (Utf16FromUtf8.P32, Utf8FromUtf16.P32, length, NULL, locale) == (size_t) length);
  assert (MultiByteToWideChar (CP_UTF8, 0, Utf8FromUtf16.W32, length, Utf16FromUtf8.W32, 2) == 1);
  assert (wmemcmp (Utf16FromUtf8.P32, Utf16FromUtf8.W32, 1) == 0);
}

/**
 * Convert each UTF-16 Surrogate Pair with `hs` being the high surrogate to
 * UTF-8 Code Unit Sequence.
 */
static void DoTest2 (wchar_t hs) {
  for (wchar_t ls = LOW_SURROGATE_MIN; ls <= LOW_SURROGATE_MAX; ++ls) {
    /**
     * UTF-16 Code Unit Sequence.
     */
    wchar_t UTF16[2] = {hs, ls};

    /**
     * `UTF16` converted to UTF-8.
     */
    struct {
      char P32[MB_LEN_MAX];
      char W32[MB_LEN_MAX];
    } Utf8FromUtf16;

    assert (c16rtomb_l (Utf8FromUtf16.P32, UTF16[0], NULL, locale) == 0);
    assert (c16rtomb_l (Utf8FromUtf16.P32, UTF16[1], NULL, locale) == 4);
    assert (WideCharToMultiByte (CP_UTF8, 0, UTF16, 2, Utf8FromUtf16.W32, 4, NULL, NULL) == 4);
    assert (memcmp (Utf8FromUtf16.P32, Utf8FromUtf16.W32, 4) == 0);

    DoTest (Utf8FromUtf16.P32, 4);

    /**
     * UTF-8 converted back to UTF-16.
     */
    struct {
      wchar_t P32[2];
      wchar_t W32[2];
    } Utf16FromUtf8;

    assert (mbrtoc16_l (&Utf16FromUtf8.P32[0], Utf8FromUtf16.P32, 4, NULL, locale) == 4);
    assert (mbrtoc16_l (&Utf16FromUtf8.P32[1], "", 0, NULL, locale) == (size_t) -3);
    assert (MultiByteToWideChar (CP_UTF8, 0, Utf8FromUtf16.W32, 4, Utf16FromUtf8.W32, 2) == 2);
    assert (wmemcmp (Utf16FromUtf8.P32, Utf16FromUtf8.W32, 2) == 0);
  }
}

int main (void) {
  p32_test_init ();

  /**
   * TODO: is there Windows NT version which does not support UTF-8?
   */
  if (!IsValidCodePage (CP_UTF8)) {
    return 77;
  }

  assert ((locale = newlocale (LC_ALL_MASK, "en_US.UTF-8", NULL)) != NULL);

  for (wchar_t wc = 0x01; wc < 0x80; ++wc) {
    DoTest1 (wc, 1);
  }

  for (wchar_t wc = 0x80; wc < 0x800; ++wc) {
    DoTest1 (wc, 2);
  }

  for (wchar_t wc = 0x800;; ++wc) {
    if (IS_HIGH_SURROGATE (wc) || IS_LOW_SURROGATE (wc)) {
      continue;
    }

    DoTest1 (wc, 3);

    if (wc == WEOF) {
      break;
    }
  }

  for (wchar_t hs = HIGH_SURROGATE_MIN; hs <= HIGH_SURROGATE_MAX; ++hs) {
    DoTest2 (hs);
  }

  freelocale (locale);

  return EXIT_SUCCESS;
}
