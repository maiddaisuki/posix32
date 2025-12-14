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
 * This test is used to verify correctness of posix32's implementation of
 * Unicode conversion functions:
 *
 * - c8rtomb
 * - mbrtoc8
 * - c16rtomb
 * - mbrtoc16
 * - c32rtomb
 * - mbrtoc32
 *
 * For each valid Unicode code point (a valid UTF-32 Code Unit):
 *
 * 1. Convert UTF-32 Code Unit to UTF-8 Code Unit Sequence.
 *
 * 2. Convert UTF-8 Code Unit Sequence from step 1 to UTF-16 Code Unit Sequence.
 *
 *  This step will also call `MultiByteToWideChar` to perform the conversion,
 *  and compare its output to that of `mbrtoc16`.
 *
 *  Both functions must produce the same result.
 *
 * 3. Convert UTF-16 Code Unit Sequence from step 2 back to UTF-8.
 *
 *  It must produce the same UTF-8 Code Unit Sequence as input
 *  used in step 2.
 *
 *  This step will also call `WideCharToMultiByte` to perform the conversion,
 *  and compare its output to that of `c16rtomb`.
 *
 *  Both functions must produce the same result.
 *
 * 4. Pass UTF-8 Code Unit Sequence from step 3 through `mbrtoc8`.
 *
 *  Produced UTF-8 Code Unit Sequence must be equivalent to the input.
 *
 * 5. Convert UTF-8 Code Unit Sequence from step 4 to UTF-32 Code Unit.
 *
 *  Produced UTF-32 Code Unit must be the same as input used in step 1.
 *
 * UCRT implemets the following uchar.h functions:
 *
 *  - c16rtomb
 *  - mbrtoc16
 *  - c32rtomb
 *  - mbrtoc32
 *
 * However, they ignore active locale for LC_CTYPE category and always operate
 * on UTF-8.
 *
 * If they are available, they will be called from steps 1, 2, 3 and 5,
 * in addition to `MultiByteToWideChar` and `WideCharToMultiByte` functions.
 */

static locale_t locale = NULL;

#if P32_CRT >= P32_UCRT
typedef size_t (__cdecl *c32rtomb_t) (char *, char32_t, mbstate_t *);
typedef size_t (__cdecl *mbrtoc32_t) (char32_t *, const char *, size_t, mbstate_t *);
typedef size_t (__cdecl *c16rtomb_t) (char *, char16_t, mbstate_t *);
typedef size_t (__cdecl *mbrtoc16_t) (char16_t *, const char *, size_t, mbstate_t *);

static c32rtomb_t crt_c32rtomb = NULL;
static mbrtoc32_t crt_mbrtoc32 = NULL;
static c16rtomb_t crt_c16rtomb = NULL;
static mbrtoc16_t crt_mbrtoc16 = NULL;
#endif

/**
 * Convert `codePoint` to UTF-8 and UTF-16.
 */
static void DoTest (char32_t codePoint, int u8Length, int u16Length) {
  /**
   * Convert `codePoint` to UTF-8.
   */
  struct {
    char P32[4];
#if P32_CRT >= P32_UCRT
    char CRT[4];
#endif
  } Utf8FromUtf32;

  assert (c32rtomb_l (Utf8FromUtf32.P32, codePoint, NULL, locale) == (size_t) u8Length);

#if P32_CRT >= P32_UCRT
  if (crt_c32rtomb != NULL) {
    mbstate_t state = {0};
    assert (crt_c32rtomb (Utf8FromUtf32.CRT, codePoint, &state) == (size_t) u8Length);
    assert (memcmp (Utf8FromUtf32.P32, Utf8FromUtf32.CRT, u8Length) == 0);
  }
#endif

  /**
   * Convert UTF-8 (from `codePoint`) to UTF-16.
   */
  struct {
    char16_t P32[2];
    char16_t W32[2];
#if P32_CRT >= P32_UCRT
    char16_t CRT[2];
#endif
  } Utf16FromUtf8;

  assert (mbrtoc16_l (&Utf16FromUtf8.P32[0], Utf8FromUtf32.P32, u8Length, NULL, locale) == (size_t) u8Length);
  if (u8Length == 4) {
    assert (mbrtoc16_l (&Utf16FromUtf8.P32[1], "", 0, NULL, locale) == (size_t) -3);
  }
  assert (MultiByteToWideChar (CP_UTF8, 0, Utf8FromUtf32.P32, u8Length, Utf16FromUtf8.W32, 2) == u16Length);
  assert (wmemcmp (Utf16FromUtf8.P32, Utf16FromUtf8.W32, u16Length) == 0);

#if P32_CRT >= P32_UCRT
  if (crt_mbrtoc16 != NULL) {
    mbstate_t state = {0};
    assert (crt_mbrtoc16 (&Utf16FromUtf8.CRT[0], Utf8FromUtf32.P32, u8Length, &state) == (size_t) u8Length);
    if (u8Length == 4) {
      assert (crt_mbrtoc16 (&Utf16FromUtf8.CRT[1], "", 0, &state) == (size_t) -3);
    }
    assert (wmemcmp (Utf16FromUtf8.P32, Utf16FromUtf8.CRT, u16Length) == 0);
  }
#endif

  /**
   * Convert UTF-16 back to UTF-8.
   */
  struct {
    char P32[4];
    char W32[4];
#if P32_CRT >= P32_UCRT
    char CRT[4];
#endif
  } Utf8FromUtf16;

  if (u8Length == 4) {
    assert (c16rtomb_l (Utf8FromUtf16.P32, Utf16FromUtf8.P32[0], NULL, locale) == 0);
    assert (c16rtomb_l (Utf8FromUtf16.P32, Utf16FromUtf8.P32[1], NULL, locale) == (size_t) u8Length);
  } else {
    assert (c16rtomb_l (Utf8FromUtf16.P32, Utf16FromUtf8.P32[0], NULL, locale) == (size_t) u8Length);
  }
  assert (WideCharToMultiByte (CP_UTF8, 0, Utf16FromUtf8.P32, u16Length, Utf8FromUtf16.W32, 4, NULL, NULL) == u8Length);
  assert (memcmp (Utf8FromUtf16.P32, Utf8FromUtf16.W32, u8Length) == 0);

#if P32_CRT >= P32_UCRT
  if (crt_c16rtomb != NULL) {
    mbstate_t state = {0};
    if (u8Length == 4) {
      assert (crt_c16rtomb (Utf8FromUtf16.CRT, Utf16FromUtf8.P32[0], &state) == 0);
      assert (crt_c16rtomb (Utf8FromUtf16.CRT, Utf16FromUtf8.P32[1], &state) == (size_t) u8Length);
    } else {
      assert (crt_c16rtomb (Utf8FromUtf16.CRT, Utf16FromUtf8.P32[0], &state) == (size_t) u8Length);
    }
    assert (memcmp (Utf8FromUtf16.P32, Utf8FromUtf16.CRT, u8Length) == 0);
  }
#endif

  /**
   * Pass UTF-8 (from UTF-16) through `mbrtoc8`.
   */
  union {
    char    Mb[4];
    char8_t U8[4];
  } UTF8;

  if (u8Length == 1) {
    assert (mbrtoc8_l (&UTF8.U8[0], &Utf8FromUtf16.P32[0], 1, NULL, locale) == 1);
    assert (mbrtoc8_l (NULL, NULL, 0, NULL, locale) == 0);
  } else if (u8Length == 2) {
    assert (mbrtoc8_l (&UTF8.U8[0], &Utf8FromUtf16.P32[0], 1, NULL, locale) == (size_t) -2);
    assert (mbrtoc8_l (&UTF8.U8[0], &Utf8FromUtf16.P32[1], 1, NULL, locale) == 1);
    assert (mbrtoc8_l (&UTF8.U8[1], "", 0, NULL, locale) == (size_t) -3);
    assert (mbrtoc8_l (NULL, NULL, 0, NULL, locale) == 0);
  } else if (u8Length == 3) {
    assert (mbrtoc8_l (&UTF8.U8[0], &Utf8FromUtf16.P32[0], 1, NULL, locale) == (size_t) -2);
    assert (mbrtoc8_l (&UTF8.U8[0], &Utf8FromUtf16.P32[1], 1, NULL, locale) == (size_t) -2);
    assert (mbrtoc8_l (&UTF8.U8[0], &Utf8FromUtf16.P32[2], 1, NULL, locale) == 1);
    assert (mbrtoc8_l (&UTF8.U8[1], "", 0, NULL, locale) == (size_t) -3);
    assert (mbrtoc8_l (&UTF8.U8[2], "", 0, NULL, locale) == (size_t) -3);
    assert (mbrtoc8_l (NULL, NULL, 0, NULL, locale) == 0);
  } else if (u8Length == 4) {
    assert (mbrtoc8_l (&UTF8.U8[0], &Utf8FromUtf16.P32[0], 1, NULL, locale) == (size_t) -2);
    assert (mbrtoc8_l (&UTF8.U8[0], &Utf8FromUtf16.P32[1], 1, NULL, locale) == (size_t) -2);
    assert (mbrtoc8_l (&UTF8.U8[0], &Utf8FromUtf16.P32[2], 1, NULL, locale) == (size_t) -2);
    assert (mbrtoc8_l (&UTF8.U8[0], &Utf8FromUtf16.P32[3], 1, NULL, locale) == 1);
    assert (mbrtoc8_l (&UTF8.U8[1], "", 0, NULL, locale) == (size_t) -3);
    assert (mbrtoc8_l (&UTF8.U8[2], "", 0, NULL, locale) == (size_t) -3);
    assert (mbrtoc8_l (&UTF8.U8[3], "", 0, NULL, locale) == (size_t) -3);
    assert (mbrtoc8_l (NULL, NULL, 0, NULL, locale) == 0);
  }

  assert (memcmp (UTF8.Mb, Utf8FromUtf16.P32, u8Length) == 0);

  /**
   * Convert UTF-8 (from `mbrtoc8`) back to UTF-32.
   */
  struct {
    char32_t P32[1];
#if P32_CRT >= P32_UCRT
    char32_t CRT[1];
#endif
  } Utf32FromUtf8;

  assert (mbrtoc32_l (Utf32FromUtf8.P32, UTF8.Mb, u8Length, NULL, locale) == (size_t) u8Length);

#if P32_CRT >= P32_UCRT
  if (crt_mbrtoc32 != NULL) {
    mbstate_t state = {0};
    assert (crt_mbrtoc32 (Utf32FromUtf8.CRT, UTF8.Mb, u8Length, &state) == (size_t) u8Length);
    assert (Utf32FromUtf8.P32[0] == Utf32FromUtf8.CRT[0]);
  }
#endif

  assert (Utf32FromUtf8.P32[0] == codePoint);
}

int main (void) {
  p32_test_init ();

  /**
   * TODO: is there Windows NT version which does not support UTF-8?
   */
  if (!IsValidCodePage (CP_UTF8)) {
    return 77;
  }

#if P32_CRT >= P32_UCRT
  HANDLE crt = (HANDLE) p32_crt_handle ();

  if (crt != NULL) {
    crt_c32rtomb = (c32rtomb_t) (UINT_PTR) GetProcAddress (crt, "c32rtomb");
    crt_mbrtoc32 = (mbrtoc32_t) (UINT_PTR) GetProcAddress (crt, "mbrtoc32");
    crt_c16rtomb = (c16rtomb_t) (UINT_PTR) GetProcAddress (crt, "c16rtomb");
    crt_mbrtoc16 = (mbrtoc16_t) (UINT_PTR) GetProcAddress (crt, "mbrtoc16");
  }

  assert ((crt_c32rtomb != NULL) == (crt_mbrtoc32 != NULL));
  assert ((crt_c32rtomb != NULL) == (crt_c16rtomb != NULL));
  assert ((crt_c32rtomb != NULL) == (crt_mbrtoc16 != NULL));
#endif

  assert ((locale = newlocale (LC_ALL_MASK, "en_US.UTF-8", NULL)) != NULL);

  for (char32_t codePoint = 0x01; codePoint < 0x80; ++codePoint) {
    DoTest (codePoint, 1, 1);
  }

  for (char32_t codePoint = 0x80; codePoint < 0x800; ++codePoint) {
    DoTest (codePoint, 2, 1);
  }

  for (char32_t codePoint = 0x800; codePoint < 0x10000; ++codePoint) {
    if (IS_HIGH_SURROGATE (codePoint) || IS_LOW_SURROGATE (codePoint)) {
      continue;
    }

    DoTest (codePoint, 3, 1);
  }

  for (char32_t codePoint = 0x10000; codePoint < 0x00110000; ++codePoint) {
    DoTest (codePoint, 4, 2);
  }

  freelocale (locale);

  return EXIT_SUCCESS;
}
