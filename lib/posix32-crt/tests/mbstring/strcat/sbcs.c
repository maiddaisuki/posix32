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
#include <stdio.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "tests-internal.h"

/**
 * Test Summary:
 *
 * Test `p32_private_strcat_l` function with SBCS code page 1253.
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
 * Convenience macro to call `p32_private_strcat_l`.
 */
#define strcat(d, s) p32_private_strcat_l (d, s, locale)

static void DoTest (void) {
  char buffer[BUFSIZ];

  /**
   * Basic `strcat` usage.
   */
  const char *Test1Base   = "123";
  const char *Test1String = "ABC";
  const char *Test1Result = "123ABC";

  memset (buffer, 0xFF, BUFSIZ);
  strcpy (buffer, Test1Base);
  assert (strcat (buffer, "") == buffer);
  assert (strcmp (buffer, Test1Base) == 0);
  assert (buffer[4] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  strcpy (buffer, Test1Base);
  assert (strcat (buffer, Test1String) == buffer);
  assert (strcmp (buffer, Test1Result) == 0);
  assert (buffer[7] == C (0xFF));

  /**
   * Test input which contains non-ASCII Code Points.
   */
  const char *Test2Base   = STRING (C8 ('1'), C8 ('2'), C8 ('3'));
  const char *Test2String = STRING (C8 ('A'), C8 ('B'), C8 ('C'));
  const char *Test2Result = STRING (C8 ('1'), C8 ('2'), C8 ('3'), C8 ('A'), C8 ('B'), C8 ('C'));

  memset (buffer, 0xFF, BUFSIZ);
  strcpy (buffer, Test2Base);
  assert (strcat (buffer, "") == buffer);
  assert (strcmp (buffer, Test2Base) == 0);
  assert (buffer[4] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  strcpy (buffer, Test2Base);
  assert (strcat (buffer, Test2String) == buffer);
  assert (strcmp (buffer, Test2Result) == 0);
  assert (buffer[7] == C (0xFF));

  /**
   * Test input which contains unassigned Code Points.
   */
  const char *Test3Base   = STRING (C8 ('1'), C8 ('2'), C (0xAA));
  const char *Test3String = STRING (C8 ('A'), C8 ('B'), C (0xFF));
  const char *Test3Result = STRING (C8 ('1'), C8 ('2'), C (0xAA), C8 ('A'), C8 ('B'), C (0xFF));
  UNREFERENCED_PARAMETER (Test3Result);

  memset (buffer, 0xFF, BUFSIZ);
  strcpy (buffer, Test3Base);
  assert (strcat (buffer, "") == NULL);
  assert (strcmp (buffer, Test3Base) == 0);
  assert (buffer[4] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  strcpy (buffer, Test3Base);
  assert (strcat (buffer, Test3String) == NULL);
  assert (strcmp (buffer, Test3Base) == 0);
  assert (buffer[4] == C (0xFF));
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
