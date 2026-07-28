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
 * Test `p32_private_strcspn_l` function with UTF-8 (code page 65001).
 */

#define C(c)        (char) (unsigned) (c)
#define STRING(...) (const char[]){__VA_ARGS__ __VA_OPT__ (, ) 0x00}

#define LOCALE "en_US.UTF-8"
static locale_t locale;

/**
 * Convenience macro to call `p32_private_strcspn_l`.
 */
#define strcspn(s, l) p32_private_strcspn_l (s, l, locale)

static void DoTest (void) {
  /**
   * Sanity checks.
   */
  const char *Test1String = STRING ('\0', 'A', 'B', 'C', '1', '2', '3');

  assert (strcspn (Test1String, "") == 0);
  assert (strcspn (Test1String, "ABC") == 0);
  assert (strcspn (Test1String, "123") == 0);

  /**
   * Basic `strcspn` usage.
   */
  const char *Test2String = "AABBCCDDEEFFGGHHIIJJKKLLMMNNOOPPQQRRSSTTUUVVWWXXYYZZ";

  assert (strcspn (Test2String, "") == strlen (Test2String));
  assert (strcspn (Test2String, "Z") == 50);
  assert (strcspn (Test2String, "ZY") == 48);
  assert (strcspn (Test2String, "ZYX") == 46);
  assert (strcspn (Test2String, "ZYXW") == 44);
  assert (strcspn (Test2String, "ZYXWV") == 42);
  assert (strcspn (Test2String, "ZYXWVU") == 40);
  assert (strcspn (Test2String, "ZYXWVUT") == 38);
  assert (strcspn (Test2String, "ZYXWVUTS") == 36);
  assert (strcspn (Test2String, "ZYXWVUTSR") == 34);
  assert (strcspn (Test2String, "ZYXWVUTSRQ") == 32);
  assert (strcspn (Test2String, "ZYXWVUTSRQP") == 30);
  assert (strcspn (Test2String, "ZYXWVUTSRQPO") == 28);
  assert (strcspn (Test2String, "ZYXWVUTSRQPON") == 26);
  assert (strcspn (Test2String, "ZYXWVUTSRQPONM") == 24);
  assert (strcspn (Test2String, "ZYXWVUTSRQPONML") == 22);
  assert (strcspn (Test2String, "ZYXWVUTSRQPONMLK") == 20);
  assert (strcspn (Test2String, "ZYXWVUTSRQPONMLKJ") == 18);
  assert (strcspn (Test2String, "ZYXWVUTSRQPONMLKJI") == 16);
  assert (strcspn (Test2String, "ZYXWVUTSRQPONMLKJIH") == 14);
  assert (strcspn (Test2String, "ZYXWVUTSRQPONMLKJIHG") == 12);
  assert (strcspn (Test2String, "ZYXWVUTSRQPONMLKJIHGF") == 10);
  assert (strcspn (Test2String, "ZYXWVUTSRQPONMLKJIHGFE") == 8);
  assert (strcspn (Test2String, "ZYXWVUTSRQPONMLKJIHGFED") == 6);
  assert (strcspn (Test2String, "ZYXWVUTSRQPONMLKJIHGFEDC") == 4);
  assert (strcspn (Test2String, "ZYXWVUTSRQPONMLKJIHGFEDCB") == 2);
  assert (strcspn (Test2String, "ZYXWVUTSRQPONMLKJIHGFEDCBA") == 0);
  assert (strcspn (Test2String, Test2String) == 0);

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
    C (0xDF), C (0xBF)
    // clang-format on
  );
  const char *Test3Set2 = STRING (
    // clang-format off
    C (0xDF), C (0xBF),
    C (0xEF), C (0xBF), C (0xBF)
    // clang-format on
  );
  const char *Test3Set3 = STRING (
    // clang-format off
    C (0xDF), C (0xBF),
    C (0xEF), C (0xBF), C (0xBF),
    C (0xF4), C (0x8F), C (0xBF), C (0xBF)
    // clang-format on
  );
  const char *Test3Set4 = STRING (
    // clang-format off
    C (0xDF), C (0xBF),
    C (0xEF), C (0xBF), C (0xBF),
    C (0xF4), C (0x8F), C (0xBF), C (0xBF),
    C (0xF0), C (0x90), C (0x80), C (0x80)
    // clang-format on
  );
  const char *Test3Set5 = STRING (
    // clang-format off
    C (0xDF), C (0xBF),
    C (0xEF), C (0xBF), C (0xBF),
    C (0xF4), C (0x8F), C (0xBF), C (0xBF),
    C (0xF0), C (0x90), C (0x80), C (0x80),
    C (0xE0), C (0xA0), C (0x80)
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

  assert (strcspn (Test3String, "") == strlen (Test3String));
  assert (strcspn (Test3String, Test3String) == 0);
  assert (strcspn (Test3String, Test3Set1) == 16);
  assert (strcspn (Test3String, Test3Set2) == 13);
  assert (strcspn (Test3String, Test3Set3) == 9);
  assert (strcspn (Test3String, Test3Set4) == 5);
  assert (strcspn (Test3String, Test3Set5) == 2);
  assert (strcspn (Test3String, Test3Set6) == 0);
}

int main (void) {
  p32_test_init ();

  locale = newlocale (LC_ALL_MASK, LOCALE, NULL);
  assert (locale != NULL);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
