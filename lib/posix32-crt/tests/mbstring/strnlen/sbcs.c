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
 * Test `p32_private_strnlen_l` function with SBCS code page 1253.
 *
 * This code page has three unassigned Code Points:
 *
 * - 0xAA
 * - 0xD2
 * - 0xFF
 */

#define C(c)        (char) (unsigned) (c)
#define C8(c)       C (0x80 | (c))
#define STRING(...) (const char[]){__VA_ARGS__ __VA_OPT__ (, ) 0x00}

#define LOCALE "el_GR.1253"
static locale_t locale;

/**
 * Convenience macro to call `p32_private_strnlen_l`.
 */
#define strnlen(s, n) p32_private_strnlen_l (s, n, locale)

static void DoTest (void) {
  /**
   * Basic `strnlen` usage.
   */
  const char *Test1String = "";
  const char *Test2String = "ABC";

  assert (strnlen (Test1String, 0) == 0);
  assert (strnlen (Test1String, SIZE_MAX) == 0);

  assert (strnlen (Test2String, 0) == 0);
  assert (strnlen (Test2String, 1) == 1);
  assert (strnlen (Test2String, 2) == 2);
  assert (strnlen (Test2String, SIZE_MAX) == 3);

  /**
   * Test input which contains non-ASCII Code Points.
   */
  const char *Test3String = STRING (C8 ('A'), C8 ('B'), C8 ('C'));

  assert (strnlen (Test3String, 0) == 0);
  assert (strnlen (Test3String, 1) == 1);
  assert (strnlen (Test3String, 2) == 2);
  assert (strnlen (Test3String, SIZE_MAX) == 3);

  /**
   * Test input which contains unassigned Code Points.
   */
  const char *Test4String = STRING (C8 ('A'), C (0xAA), C8 ('B'), C (0xFF), C8 ('C'));

  assert (strnlen (Test4String, 0) == 0);
  assert (strnlen (Test4String, 1) == 1);
  assert (strnlen (Test4String, 2) == (size_t) -1);
  assert (strnlen (Test4String, SIZE_MAX) == (size_t) -1);
}

int main (void) {
  p32_test_init ();

  /**
   * Skip test if code page 1253 is not installed.
   */
  if (!IsValidCodePage (1253)) {
    return 77;
  }

  locale = newlocale (LC_ALL_MASK, LOCALE, NULL);
  assert (locale != NULL);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
