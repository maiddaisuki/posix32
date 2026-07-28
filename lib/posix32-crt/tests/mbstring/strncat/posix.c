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
 * Test `p32_private_strncat_l` function with ISO-8859-1 (code page 28591).
 *
 * This code page is used with "POSIX" locale.
 */

#define C(c)        (char) (unsigned) (c)
#define C8(c)       C (0x80 | (c))
#define STRING(...) (const char[]){__VA_ARGS__ __VA_OPT__ (, ) 0x00}

#define LOCALE "POSIX"
static locale_t locale;

/**
 * Convenience macro to call `p32_private_strncat_l`.
 */
#define strncat(d, s, n) p32_private_strncat_l (d, s, n, locale)

static void DoTest (void) {
  char buffer[BUFSIZ];

  /**
   * Basic `strncat` usage.
   */
  const char *Test1Base   = "123";
  const char *Test1String = "ABC";
  const char *Test1Result = "123ABC";

  memset (buffer, 0xFF, BUFSIZ);
  strcpy (buffer, Test1Base);
  assert (strncat (buffer, "", 0) == buffer);
  assert (strncmp (buffer, Test1Result, 3) == 0);
  assert (buffer[3] == 0x00);
  assert (buffer[4] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  strcpy (buffer, Test1Base);
  assert (strncat (buffer, "", SIZE_MAX) == buffer);
  assert (strncmp (buffer, Test1Result, 3) == 0);
  assert (buffer[3] == 0x00);
  assert (buffer[4] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  strcpy (buffer, Test1Base);
  assert (strncat (buffer, Test1String, 0) == buffer);
  assert (strncmp (buffer, Test1Result, 3) == 0);
  assert (buffer[3] == 0x00);
  assert (buffer[4] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  strcpy (buffer, Test1Base);
  assert (strncat (buffer, Test1String, 1) == buffer);
  assert (strncmp (buffer, Test1Result, 4) == 0);
  assert (buffer[4] == 0x00);
  assert (buffer[5] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  strcpy (buffer, Test1Base);
  assert (strncat (buffer, Test1String, 2) == buffer);
  assert (strncmp (buffer, Test1Result, 5) == 0);
  assert (buffer[5] == 0x00);
  assert (buffer[6] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  strcpy (buffer, Test1Base);
  assert (strncat (buffer, Test1String, SIZE_MAX) == buffer);
  assert (strncmp (buffer, Test1Result, 6) == 0);
  assert (buffer[6] == 0x00);
  assert (buffer[7] == C (0xFF));

  /**
   * Test input which contains non-ASCII Code Points.
   *
   * In ISO-8859-1 all bytes are assigned Code Points.
   */
  const char *Test2Base   = STRING (C8 ('1'), C8 ('2'), C8 ('3'));
  const char *Test2String = STRING (C8 ('A'), C8 ('B'), C8 ('C'));
  const char *Test2Result = STRING (C8 ('1'), C8 ('2'), C8 ('3'), C8 ('A'), C8 ('B'), C8 ('C'));

  memset (buffer, 0xFF, BUFSIZ);
  strcpy (buffer, Test2Base);
  assert (strncat (buffer, Test2String, 0) == buffer);
  assert (strncmp (buffer, Test2Result, 3) == 0);
  assert (buffer[3] == 0x00);
  assert (buffer[4] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  strcpy (buffer, Test2Base);
  assert (strncat (buffer, Test2String, 1) == buffer);
  assert (strncmp (buffer, Test2Result, 4) == 0);
  assert (buffer[4] == 0x00);
  assert (buffer[5] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  strcpy (buffer, Test2Base);
  assert (strncat (buffer, Test2String, 2) == buffer);
  assert (strncmp (buffer, Test2Result, 5) == 0);
  assert (buffer[5] == 0x00);
  assert (buffer[6] == C (0xFF));

  memset (buffer, 0xFF, BUFSIZ);
  strcpy (buffer, Test2Base);
  assert (strncat (buffer, Test2String, SIZE_MAX) == buffer);
  assert (strncmp (buffer, Test2Result, 6) == 0);
  assert (buffer[6] == 0x00);
  assert (buffer[7] == C (0xFF));
}

int main (void) {
  p32_test_init ();

  locale = newlocale (LC_ALL_MASK, LOCALE, NULL);
  assert (locale != NULL);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
