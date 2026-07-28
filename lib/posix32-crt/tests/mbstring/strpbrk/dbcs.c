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
 * Test `p32_private_strpbrk_l` function with DBCS code page 932.
 */

#define C(c)        (char) (unsigned) (c)
#define STRING(...) (const char[]){__VA_ARGS__ __VA_OPT__ (, ) 0x00}

#define LOCALE "ja_JP.932"
static locale_t locale;

/**
 * Convenience macro to call `p32_private_strpbrk_l`.
 */
#define strpbrk(s, l) p32_private_strpbrk_l (s, l, locale)

static void DoTest (void) {
  /**
   * Sanity checks.
   */
  const char *Test1String = STRING ('\0', 'A', 'B', 'C', '1', '2', '3');

  assert (strpbrk (Test1String, "") == NULL);
  assert (strpbrk (Test1String, "ABC") == NULL);
  assert (strpbrk (Test1String, "123") == NULL);

  /**
   * Basic `strpbrk` usage.
   */
  const char *Test2String = "AABBCCDDEEFFGGHHIIJJKKLLMMNNOOPPQQRRSSTTUUVVWWXXYYZZ";

  assert (strpbrk (Test2String, "") == NULL);
  assert (strpbrk (Test2String, "Z") == Test2String + 50);
  assert (strpbrk (Test2String, "ZY") == Test2String + 48);
  assert (strpbrk (Test2String, "ZYX") == Test2String + 46);
  assert (strpbrk (Test2String, "ZYXW") == Test2String + 44);
  assert (strpbrk (Test2String, "ZYXWV") == Test2String + 42);
  assert (strpbrk (Test2String, "ZYXWVU") == Test2String + 40);
  assert (strpbrk (Test2String, "ZYXWVUT") == Test2String + 38);
  assert (strpbrk (Test2String, "ZYXWVUTS") == Test2String + 36);
  assert (strpbrk (Test2String, "ZYXWVUTSR") == Test2String + 34);
  assert (strpbrk (Test2String, "ZYXWVUTSRQ") == Test2String + 32);
  assert (strpbrk (Test2String, "ZYXWVUTSRQP") == Test2String + 30);
  assert (strpbrk (Test2String, "ZYXWVUTSRQPO") == Test2String + 28);
  assert (strpbrk (Test2String, "ZYXWVUTSRQPON") == Test2String + 26);
  assert (strpbrk (Test2String, "ZYXWVUTSRQPONM") == Test2String + 24);
  assert (strpbrk (Test2String, "ZYXWVUTSRQPONML") == Test2String + 22);
  assert (strpbrk (Test2String, "ZYXWVUTSRQPONMLK") == Test2String + 20);
  assert (strpbrk (Test2String, "ZYXWVUTSRQPONMLKJ") == Test2String + 18);
  assert (strpbrk (Test2String, "ZYXWVUTSRQPONMLKJI") == Test2String + 16);
  assert (strpbrk (Test2String, "ZYXWVUTSRQPONMLKJIH") == Test2String + 14);
  assert (strpbrk (Test2String, "ZYXWVUTSRQPONMLKJIHG") == Test2String + 12);
  assert (strpbrk (Test2String, "ZYXWVUTSRQPONMLKJIHGF") == Test2String + 10);
  assert (strpbrk (Test2String, "ZYXWVUTSRQPONMLKJIHGFE") == Test2String + 8);
  assert (strpbrk (Test2String, "ZYXWVUTSRQPONMLKJIHGFED") == Test2String + 6);
  assert (strpbrk (Test2String, "ZYXWVUTSRQPONMLKJIHGFEDC") == Test2String + 4);
  assert (strpbrk (Test2String, "ZYXWVUTSRQPONMLKJIHGFEDCB") == Test2String + 2);
  assert (strpbrk (Test2String, "ZYXWVUTSRQPONMLKJIHGFEDCBA") == Test2String + 0);
  assert (strpbrk (Test2String, Test2String) == Test2String + 0);

  /**
   * Test input which contains DBCS characters.
   */
  const char *Test3String = STRING (
    // clang-format off
    C (0xA1), C (0x82), C (0xA1),
    C (0xA2), C (0x82), C (0xA2),
    C (0xA3), C (0x82), C (0xA3)
    // clang-format off
  );

  const char *Test3Set1 = STRING (
    // clang-format off
    C (0x82), C (0xA3)
    // clang-format on
  );

  const char *Test3Set2 = STRING (
    // clang-format off
    C (0x82), C (0xA3), C (0xA3)
    // clang-format on
  );

  const char *Test3Set3 = STRING (
    // clang-format off
    C (0x82), C (0xA3), C (0xA3),
    C (0x82), C (0xA2)
    // clang-format on
  );

  const char *Test3Set4 = STRING (
    // clang-format off
    C (0x82), C (0xA3), C (0xA3),
    C (0x82), C (0xA2), C (0xA2)
    // clang-format on
  );

  const char *Test3Set5 = STRING (
    // clang-format off
    C (0x82), C (0xA3), C (0xA3),
    C (0x82), C (0xA2), C (0xA2),
    C (0x82), C (0xA1)
    // clang-format on
  );

  const char *Test3Set6 = STRING (
    // clang-format off
    C (0x82), C (0xA3), C (0xA3),
    C (0x82), C (0xA2), C (0xA2),
    C (0x82), C (0xA1), C (0xA1)
    // clang-format on
  );

  assert (strpbrk (Test3String, "") == NULL);
  assert (strpbrk (Test3String, Test3Set1) == Test3String + 7);
  assert (strpbrk (Test3String, Test3Set2) == Test3String + 6);
  assert (strpbrk (Test3String, Test3Set3) == Test3String + 4);
  assert (strpbrk (Test3String, Test3Set4) == Test3String + 3);
  assert (strpbrk (Test3String, Test3Set5) == Test3String + 1);
  assert (strpbrk (Test3String, Test3Set6) == Test3String);
  assert (strpbrk (Test3String, Test3String) == Test3String);
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
