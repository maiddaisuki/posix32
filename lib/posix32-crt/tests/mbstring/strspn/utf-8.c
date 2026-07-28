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
 * Test `p32_private_strspn_l` function with UTF-8 (code page 65001).
 */

#define C(c)        (char) (unsigned) (c)
#define STRING(...) (const char[]){__VA_ARGS__ __VA_OPT__ (, ) 0x00}

#define LOCALE "en_US.UTF-8"
static locale_t locale;

/**
 * Convenience macro to call `p32_private_strspn_l`.
 */
#define strspn(s, l) p32_private_strspn_l (s, l, locale)

static void DoTest (void) {
  /**
   * Sanity checks.
   */
  const char *Test1String = STRING ('\0', 'A', 'B', 'C', '1', '2', '3');

  assert (strspn (Test1String, "") == 0);
  assert (strspn (Test1String, "ABC") == 0);
  assert (strspn (Test1String, "123") == 0);

  /**
   * Basic `strspn` usage.
   */
  const char *Test2String = "AABBCCDDEEFFGGHHIIJJKKLLMMNNOOPPQQRRSSTTUUVVWWXXYYZZ";

  assert (strspn (Test2String, "") == 0);
  assert (strspn (Test2String, "A") == 2);
  assert (strspn (Test2String, "AB") == 4);
  assert (strspn (Test2String, "ABC") == 6);
  assert (strspn (Test2String, "ABCD") == 8);
  assert (strspn (Test2String, "ABCDE") == 10);
  assert (strspn (Test2String, "ABCDEF") == 12);
  assert (strspn (Test2String, "ABCDEFG") == 14);
  assert (strspn (Test2String, "ABCDEFGH") == 16);
  assert (strspn (Test2String, "ABCDEFGHI") == 18);
  assert (strspn (Test2String, "ABCDEFGHIJ") == 20);
  assert (strspn (Test2String, "ABCDEFGHIJK") == 22);
  assert (strspn (Test2String, "ABCDEFGHIJKL") == 24);
  assert (strspn (Test2String, "ABCDEFGHIJKLM") == 26);
  assert (strspn (Test2String, "ABCDEFGHIJKLMN") == 28);
  assert (strspn (Test2String, "ABCDEFGHIJKLMNO") == 30);
  assert (strspn (Test2String, "ABCDEFGHIJKLMNOP") == 32);
  assert (strspn (Test2String, "ABCDEFGHIJKLMNOPQ") == 34);
  assert (strspn (Test2String, "ABCDEFGHIJKLMNOPQR") == 36);
  assert (strspn (Test2String, "ABCDEFGHIJKLMNOPQRS") == 38);
  assert (strspn (Test2String, "ABCDEFGHIJKLMNOPQRST") == 40);
  assert (strspn (Test2String, "ABCDEFGHIJKLMNOPQRSTU") == 42);
  assert (strspn (Test2String, "ABCDEFGHIJKLMNOPQRSTUV") == 44);
  assert (strspn (Test2String, "ABCDEFGHIJKLMNOPQRSTUVW") == 46);
  assert (strspn (Test2String, "ABCDEFGHIJKLMNOPQRSTUVWX") == 48);
  assert (strspn (Test2String, "ABCDEFGHIJKLMNOPQRSTUVWXY") == 50);
  assert (strspn (Test2String, "ABCDEFGHIJKLMNOPQRSTUVWXYZ") == 52);
  assert (strspn (Test2String, Test2String) == strlen (Test2String));

  /**
   * Test input which contains multibyte characters.
   */
  const char *Test3String = STRING (
    // clang-format off
    C (0xC2), C (0x80),
    C (0xE0), C (0xA0), C (0x80),
    C (0xF0), C (0x90), C (0x80), C (0x80),
    C (0xF4), C (0x8F), C (0xBF), C (0xBF),
    C (0xEF), C (0xBF), C (0xBF),
    C (0xDF), C (0xBF)
    // clang-format on
  );

  const char *Test3Set1 = STRING (
    // clang-format off
    C (0xC2), C (0x80)
    // clang-format on
  );
  const char *Test3Set2 = STRING (
    // clang-format off
    C (0xE0), C (0xA0), C (0x80),
    C (0xC2), C (0x80)
    // clang-format on
  );
  const char *Test3Set3 = STRING (
    // clang-format off
    C (0xF0), C (0x90), C (0x80), C (0x80),
    C (0xE0), C (0xA0), C (0x80),
    C (0xC2), C (0x80)
    // clang-format on
  );
  const char *Test3Set4 = STRING (
    // clang-format off
    C (0xF4), C (0x8F), C (0xBF), C (0xBF),
    C (0xF0), C (0x90), C (0x80), C (0x80),
    C (0xE0), C (0xA0), C (0x80),
    C (0xC2), C (0x80)
    // clang-format on
  );
  const char *Test3Set5 = STRING (
    // clang-format off
    C (0xEF), C (0xBF), C (0xBF),
    C (0xF4), C (0x8F), C (0xBF), C (0xBF),
    C (0xF0), C (0x90), C (0x80), C (0x80),
    C (0xE0), C (0xA0), C (0x80),
    C (0xC2), C (0x80)
    // clang-format on
  );
  const char *Test3Set6 = STRING (
    // clang-format off
    C (0xDF), C (0xBF),
    C (0xEF), C (0xBF), C (0xBF),
    C (0xF4), C (0x8F), C (0xBF), C (0xBF),
    C (0xF0), C (0x90), C (0x80), C (0x80),
    C (0xE0), C (0xA0), C (0x80),
    C (0xC2), C (0x80)
    // clang-format on
  );

  assert (strspn (Test3String, "") == 0);
  assert (strspn (Test3String, Test3String) == strlen (Test3String));
  assert (strspn (Test3String, Test3Set1) == 2);
  assert (strspn (Test3String, Test3Set2) == 5);
  assert (strspn (Test3String, Test3Set3) == 9);
  assert (strspn (Test3String, Test3Set4) == 13);
  assert (strspn (Test3String, Test3Set5) == 16);
  assert (strspn (Test3String, Test3Set6) == 18);
}

int main (void) {
  p32_test_init ();

  locale = newlocale (LC_ALL_MASK, LOCALE, NULL);
  assert (locale != NULL);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
