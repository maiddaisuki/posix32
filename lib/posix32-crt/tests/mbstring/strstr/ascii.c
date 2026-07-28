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
 * Test `p32_private_strstr_l` function with ASCII (code page 20127).
 */

#define C(c)        (char) (unsigned) (c)
#define C8(c)       C (0x80 | (c))
#define STRING(...) (const char[]){__VA_ARGS__ __VA_OPT__ (, ) 0x00}

#define LOCALE "en_US.ASCII"
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
   * Test input which contains non-ASCII Code Points.
   */
  const char *Test2String = STRING ('A', 'B', C8 ('C'), C8 ('D'), 'E', 'F');

  assert (strstr (Test2String, "") == Test2String);
  assert (strstr (Test2String, STRING ('A')) == Test2String);
  assert (strstr (Test2String, STRING ('A', 'A')) == NULL);
  assert (strstr (Test2String, STRING ('A', 'B')) == Test2String);
  assert (strstr (Test2String, STRING ('B', 'B')) == NULL);
  assert (strstr (Test2String, STRING ('B', C8 ('C'))) == NULL);
  assert (strstr (Test2String, STRING (C8 ('C'))) == NULL);
  assert (strstr (Test2String, STRING (C8 ('C'), C8 ('D'))) == NULL);
  assert (strstr (Test2String, STRING (C8 ('D'))) == NULL);
  assert (strstr (Test2String, STRING (C8 ('D'), 'E')) == NULL);
  assert (strstr (Test2String, STRING ('E', 'E')) == NULL);
  assert (strstr (Test2String, STRING ('E', 'F')) == NULL);
  assert (strstr (Test2String, STRING ('F', 'F')) == NULL);
  assert (strstr (Test2String, STRING ('F')) == NULL);
  assert (strstr (Test2String, Test2String) == NULL);
}

int main (void) {
  p32_test_init ();

  locale = newlocale (LC_ALL_MASK, LOCALE, NULL);
  assert (locale != NULL);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
