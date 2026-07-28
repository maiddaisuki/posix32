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
 * Test `p32_private_strcmp_l` function with ASCII (code page 20127).
 */

#define LOCALE "en_US.ASCII"
static locale_t locale;

/**
 * Convenience macro to call `p32_private_strcmp_l`.
 */
#define strcmp(s1, s2) p32_private_strcmp_l (s1, s2, locale)

static void DoTest (void) {
  /**
   * Previous converted Code Point.
   */
  char Prev[MB_LEN_MAX];
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

    assert ((diff = strcmp ("", Char)) != _NLSCMPERROR);
    assert (diff < 0);
    assert ((diff = strcmp (Char, "")) != _NLSCMPERROR);
    assert (diff > 0);

    assert ((diff = strcmp (Prev, Char)) != _NLSCMPERROR);
    assert (diff < 0);
    assert ((diff = strcmp (Char, Prev)) != _NLSCMPERROR);
    assert (diff > 0);

    memcpy (Prev, Char, MB_LEN_MAX);
  }
}

int main (void) {
  p32_test_init ();

  locale = newlocale (LC_ALL_MASK, LOCALE, NULL);
  assert (locale != NULL);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
