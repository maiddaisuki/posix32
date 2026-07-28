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
 * Test `p32_private_strrchr_l` function with ASCII (code page 20127).
 */

#define C(c)        (char) (unsigned) (c)
#define C8(c)       C (0x80 | (c))
#define STRING(...) (const char[]){__VA_ARGS__ __VA_OPT__ (, ) 0x00}

#define LOCALE "en_US.ASCII"
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
  assert (strrchr (Test1String, C8 ('1')) == NULL);
  assert (strrchr (Test1String, '2') == Test1String + 4);
  assert (strrchr (Test1String, C8 ('2')) == NULL);
  assert (strrchr (Test1String, '3') == Test1String + 3);
  assert (strrchr (Test1String, C8 ('3')) == NULL);
  assert (strrchr (Test1String, '\0') == Test1String + 6);
  assert (strrchr (Test1String, 0x7F) == NULL);

  /**
   * Test input which contains non-ASCII Code Points.
   */
  const char *Test2String = STRING ('A', C8 ('A'), 'A', 'B', C8 ('B'), 'B', 'C', C8 ('C'), 'C');

  assert (strrchr (Test2String, 'A') == Test2String);
  assert (strrchr (Test2String, C8 ('A')) == NULL);
  assert (strrchr (Test2String, 'B') == NULL);
  assert (strrchr (Test2String, C8 ('B')) == NULL);
  assert (strrchr (Test2String, 'C') == NULL);
  assert (strrchr (Test2String, C8 ('C')) == NULL);
  assert (strrchr (Test2String, '\0') == NULL);
}

int main (void) {
  p32_test_init ();

  locale = newlocale (LC_ALL_MASK, LOCALE, NULL);
  assert (locale != NULL);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
