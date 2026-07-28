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
 * Test `p32_private_strspn_l` function with ASCII (code page 20127).
 */

#define C(c)        (char) (unsigned) (c)
#define C8(c)       C (0x80 | (c))
#define STRING(...) (const char[]){__VA_ARGS__ __VA_OPT__ (, ) 0x00}

#define LOCALE "en_US.ASCII"
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
   * Test input which contains non-ASCII Code Points.
   */
  const char *Test3String = STRING ('A', 'B', 'C', 'D', C8 ('A'), C8 ('B'), C8 ('C'), C8 ('D'));

  const char *Test3Set1 = STRING ('A');
  const char *Test3Set2 = STRING (C8 ('A'), 'A');
  const char *Test3Set3 = STRING ('B', 'A');
  const char *Test3Set4 = STRING (C8 ('B'), 'B', C8 ('A'), 'A');
  const char *Test3Set5 = STRING ('C', 'B', 'A');
  const char *Test3Set6 = STRING (C8 ('C'), 'C', C8 ('B'), 'B', C8 ('A'), 'A');
  const char *Test3Set7 = STRING ('D', 'C', 'B', 'A');
  const char *Test3Set8 = STRING (C8 ('D'), 'D', C8 ('C'), 'C', C8 ('B'), 'B', C8 ('A'), 'A');

  assert (strspn (Test3String, "") == 0);
  assert (strspn (Test3String, Test3Set1) == 1);
  assert (strspn (Test3String, Test3Set2) == 0);
  assert (strspn (Test3String, Test3Set3) == 2);
  assert (strspn (Test3String, Test3Set4) == 0);
  assert (strspn (Test3String, Test3Set5) == 3);
  assert (strspn (Test3String, Test3Set6) == 0);
  assert (strspn (Test3String, Test3Set7) == 4);
  assert (strspn (Test3String, Test3Set8) == 0);
  assert (strspn (Test3String, Test3String) == 4);
}

int main (void) {
  p32_test_init ();

  locale = newlocale (LC_ALL_MASK, LOCALE, NULL);
  assert (locale != NULL);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
