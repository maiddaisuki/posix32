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
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "tests-internal.h"

/**
 * Test Summary:
 *
 * Test `p32_private_strrchr_l` function with UTF-8 (code page 65001).
 */

#define C(c)        (char) (unsigned) (c)
#define STRING(...) (const char[]){__VA_ARGS__ __VA_OPT__ (, ) 0x00}

#define LOCALE "en_US.UTF-8"
static locale_t locale;

/**
 * Convenience macro to call `p32_private_strrchr_l`.
 */
#define strrchr(s, c) p32_private_strrchr_l (s, c, locale)

static void DoTest (void) {
  /**
   * Basic `strrchr` usage.
   */
  const char *Test1String = STRING ('1', '2', '3', '3', '2', '1', '\0', 0x7F);

  assert (strrchr (Test1String, '1') == Test1String + 5);
  assert (strrchr (Test1String, '2') == Test1String + 4);
  assert (strrchr (Test1String, '3') == Test1String + 3);
  assert (strrchr (Test1String, '\0') == Test1String + 6);
  assert (strrchr (Test1String, 0x7F) == NULL);

  /**
   * Test input which contains multibyte characters.
   */
  const char *Test2String = STRING (
    // clang-format off
    'A',      C (0xC2), C (0x80), 'B',      C (0xE0), C (0xA0), C (0x80), 'C',
    C (0xF0), C (0x90), C (0x80), C (0x80), C (0xF4), C (0x8F), C (0xBF), C (0xBF),
    'C',      C (0xEF), C (0xBF), C (0xBF), 'B',      C (0xDF), C (0xBF), 'A'
    // clang-format on
  );

  assert (strrchr (Test2String, 'A') == Test2String + 23);
  assert (strrchr (Test2String, 'B') == Test2String + 20);
  assert (strrchr (Test2String, 'C') == Test2String + 16);
  assert (strrchr (Test2String, '\0') == Test2String + 24);

  for (int c = 0x80; c <= 0xFF; ++c) {
    assert (strrchr (Test2String, c) == NULL);
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
