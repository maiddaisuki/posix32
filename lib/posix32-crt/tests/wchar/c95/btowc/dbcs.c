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
 * Test `btowc` function with some DBCS code page.
 *
 * We test code page 932; this is the ANSI/OEM code page for `ja-JP` locale.
 */

#if P32_CRT >= P32_MSVCRT20
#define LOCALE "ja_JP.ACP"
#else
#define LOCALE "ja_JP.OCP"
#endif

static locale_t locale;

static void DoTest (void) {
  /**
   * `EOF` must convert to `WEOF`.
   */
  assert (btowc_l (EOF, locale) == WEOF);
  assert (errno == 0);

  /**
   * All bytes in range [0,127] are valid ASCII characters.
   */
  for (int c = 0; c <= 127; ++c) {
    assert (btowc_l (c, locale) == c);
    assert (errno == 0);
  }

  for (size_t i = 0; i < _countof (CJK); ++i) {
    assert (btowc_l ((uint8_t) CJK[i].DBCS[0], locale) == WEOF);
    assert (errno == 0);
  }
}

int main (void) {
  p32_test_init ();

  if (!IsValidCodePage (932)) {
    return 77;
  }

  assert ((locale = newlocale (LC_ALL_MASK, LOCALE, NULL)) != NULL);
  assert (MB_CUR_MAX_L (locale) == 2);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
