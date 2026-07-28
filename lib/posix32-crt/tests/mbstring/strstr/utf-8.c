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
 * Test `p32_private_strstr_l` function with UTF-8 (code page 65001).
 */

#define C(c)        (char) (unsigned) (c)
#define STRING(...) (const char[]){__VA_ARGS__ __VA_OPT__ (, ) 0x00}

#define LOCALE "en_US.UTF-8"
static locale_t locale;

/**
 * Convenience macro to call `p32_private_strstr_l`.
 */
#define strstr(t, s) p32_private_strstr_l (t, s, locale)

static void DoTest (void) {
  /**
   * Basic `strstr` usage.
   */
  const char *Test1String = "AAABBBCCC";

  assert (strstr (Test1String, "") == Test1String);
  assert (strstr (Test1String, "A") == Test1String);
  assert (strstr (Test1String, "AA") == Test1String);
  assert (strstr (Test1String, "AAA") == Test1String);
  assert (strstr (Test1String, "AAAA") == NULL);
  assert (strstr (Test1String, "AAB") == Test1String + 1);
  assert (strstr (Test1String, "AB") == Test1String + 2);
  assert (strstr (Test1String, "B") == Test1String + 3);
  assert (strstr (Test1String, "BB") == Test1String + 3);
  assert (strstr (Test1String, "BBB") == Test1String + 3);
  assert (strstr (Test1String, "BBBB") == NULL);
  assert (strstr (Test1String, "BBC") == Test1String + 4);
  assert (strstr (Test1String, "BC") == Test1String + 5);
  assert (strstr (Test1String, "C") == Test1String + 6);
  assert (strstr (Test1String, "CC") == Test1String + 6);
  assert (strstr (Test1String, "CCC") == Test1String + 6);
  assert (strstr (Test1String, "CCCC") == NULL);
  assert (strstr (Test1String, Test1String) == Test1String);

  /**
   * Test input which contains multibyte characters.
   */
  const char *Test2String = STRING (
    // clang-format off
    C (0xF0), C (0x90), C (0x80), C (0x80),
    C (0xC2), C (0x80),
    C (0xE0), C (0xA0), C (0x80),
    C (0xEF), C (0xBF), C (0xBF),
    C (0xDF), C (0xBF),
    C (0xF4), C (0x8F), C (0xBF), C (0xBF)
    // clang-format on
  );

  const char *Test2Set1 = STRING (C (0xF0), C (0x90), C (0x80), C (0x80), C (0xC2), C (0x80));
  const char *Test2Set2 = STRING (C (0xC2), C (0x80), C (0xE0), C (0xA0), C (0x80));
  const char *Test2Set3 = STRING (C (0xE0), C (0xA0), C (0x80), C (0xEF), C (0xBF), C (0xBF));
  const char *Test2Set4 = STRING (C (0xEF), C (0xBF), C (0xBF), C (0xDF), C (0xBF));
  const char *Test2Set5 = STRING (C (0xDF), C (0xBF), C (0xF4), C (0x8F), C (0xBF), C (0xBF));
  const char *Test2Set6 = STRING (C (0xF4), C (0x8F), C (0xBF), C (0xBF));

  assert (strstr (Test2String, "") == Test2String);
  assert (strstr (Test2String, Test2Set1) == Test2String);
  assert (strstr (Test2String, Test2Set2) == Test2String + 4);
  assert (strstr (Test2String, Test2Set3) == Test2String + 6);
  assert (strstr (Test2String, Test2Set4) == Test2String + 9);
  assert (strstr (Test2String, Test2Set5) == Test2String + 12);
  assert (strstr (Test2String, Test2Set6) == Test2String + 14);
  assert (strstr (Test2String, Test2String) == Test2String);
}

int main (void) {
  p32_test_init ();

  locale = newlocale (LC_ALL_MASK, LOCALE, NULL);
  assert (locale != NULL);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
