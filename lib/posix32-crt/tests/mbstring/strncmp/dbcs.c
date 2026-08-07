/**
 * Copyright 2026 Kirill Makurin
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
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "tests-internal.h"

/**
 * Test Summary:
 *
 * Test `p32_private_strncmp_l` function with DBCS code page 932.
 */

#define C(c) (char) (unsigned) (c)

#define LOCALE "ja_JP.932"
static locale_t locale;

/**
 * Convenience macro to call `p32_private_strncmp_l`.
 */
#define strncmp(s1, s2, n) p32_private_strncmp_l (s1, s2, n, locale)

static void DoTest (void) {
  /**
   * Previous converted Code Point.
   */
  size_t PrevLength = 0;
  char   Prev[MB_LEN_MAX];
  memset (Prev, 0, MB_LEN_MAX);

  for (char32_t c32 = 0x01; c32 <= 0x0010FFFF; ++c32) {
    /**
     * Converted Code Point.
     */
    char Char[MB_LEN_MAX];
    memset (Char, 0, MB_LEN_MAX);

    size_t length = c32rtomb_l (Char, c32, NULL, locale);

    if (length == (size_t) -1) {
      continue;
    }

    int diff;

    assert ((diff = strncmp (Prev, Char, 0)) != _NLSCMPERROR);
    assert (diff == 0);
    assert ((diff = strncmp (Char, Prev, 0)) != _NLSCMPERROR);
    assert (diff == 0);

#if !1 // Functions `c{8,16,32}rtomb` are no longer affected by this issue
    /**
     * `WideCharToMultiByte`, which is used to implement `c{n}rtomb` functions,
     * converts some Code Points from Private Use Area (U+E000-U+F8FF) to
     * code page's unassigned Code Points.
     *
     * `MultiByteToWideChar`, which is used to implement `mbrtoc{n}` functions,
     * does not support round-trip conversion of those unassigned Code Points.
     */
    if (Char[0] == C (0xA0) || Char[0] == C (0xFD) || Char[0] == C (0xFE) || Char[0] == C (0xFF)) {
      assert ((diff = strncmp (Prev, Char, 1)) == _NLSCMPERROR);
      assert ((diff = strncmp (Char, Prev, 1)) == _NLSCMPERROR);
      continue;
    }
#endif

    /**
     * Compare multibyte sequence to an empty string; valid, non-zero-length
     * multibyte sequence must always sort after an empty string.
     */
    if (length == 2) {
      assert ((diff = strncmp ("", Char, 1)) != _NLSCMPERROR);
      assert (diff < 0);
      assert ((diff = strncmp (Char, "", 1)) != _NLSCMPERROR);
      assert (diff > 0);
    }

    /**
     * Compare incomplete multibyte sequence to a complete multibyte sequence;
     * complete multibyte sequence must always sort after incomplete one.
     */
    if (PrevLength != length) {
      assert ((diff = strncmp (Prev, Char, 1)) != _NLSCMPERROR);
      assert (diff != 0);
      assert ((diff < 0) == (length == 1));
      assert ((diff > 0) == (PrevLength == 1));

      assert ((diff = strncmp (Char, Prev, 1)) != _NLSCMPERROR);
      assert (diff != 0);
      assert ((diff < 0) == (PrevLength == 1));
      assert ((diff > 0) == (length == 1));
    }

    /**
     * Compare one incomplete multibyte sequence to another;
     * they must be sorted using binary order.
     *
     * FIXME: currently, two incomplete multibyte sequences compare equal.
     */
    if (PrevLength == 2 && length == 2) {
      assert ((diff = strncmp (Prev, Char, 1)) != _NLSCMPERROR);
      assert (diff == 0);
      assert ((diff = strncmp (Char, Prev, 1)) != _NLSCMPERROR);
      assert (diff == 0);
    }

    assert ((diff = strncmp (Prev, Char, 2)) != _NLSCMPERROR);
    assert (diff < 0);
    assert ((diff = strncmp (Char, Prev, 2)) != _NLSCMPERROR);
    assert (diff > 0);

    memcpy (Prev, Char, MB_LEN_MAX);
    PrevLength = length;
  }
}

int main (void) {
  p32_test_init ();

  /**
   * Skip test if code page 932 is not installed.
   */
  if (!IsValidCodePage (932)) {
    return 77;
  }

  locale = newlocale (LC_ALL_MASK, LOCALE, NULL);
  assert (locale != NULL);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
