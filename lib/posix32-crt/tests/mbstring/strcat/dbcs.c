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
 * Test `p32_private_strcat_l` function with DBCS code page 932.
 */

#define C(c)        (char) (unsigned) (c)
#define STRING(...) (const char[]){__VA_ARGS__ __VA_OPT__ (, ) 0x00}

#define LOCALE "ja_JP.932"
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
   * Test input which contains valid DBCS characters.
   */
  const char *Test3Base   = STRING (C (0x82), C (0xA1));
  const char *Test3String = STRING (C (0xA1), C (0x83), C (0xA1));
  const char *Test3Result = STRING (C (0x82), C (0xA1), C (0xA1), C (0x83), C (0xA1));

  memset (buffer, 0xFF, BUFSIZ);
  strcpy (buffer, Test3Base);
  assert (strcat (buffer, Test3String) == buffer);
  assert (strcmp (buffer, Test3Result) == 0);
  assert (buffer[6] == C (0xFF));

  /**
   * Test input which contains incomplete DBCS characters.
   */
  const char *Test4Base   = STRING (C (0x82), C (0xA0), C (0x82));
  const char *Test4String = STRING (C (0xA1), C (0x83));
  const char *Test4Result = STRING (C (0x82), C (0xA0), C (0xA1));
  UNREFERENCED_PARAMETER (Test4Result);

  memset (buffer, 0xFF, BUFSIZ);
  strcpy (buffer, Test4Base);
  assert (strcat (buffer, Test4String) == NULL);
  assert (strcmp (buffer, Test4Base) == 0);
  assert (buffer[4] == C (0xFF));

  /**
   * Test input which contains invalid DBCS characters.
   */
  const char *Test5Base   = STRING (C (0x82), C (0xA1), C (0x82));
  const char *Test5String = STRING (C (0x82), C (0x9B), C (0x40));
  const char *Test5Result = STRING (C (0x82), C (0xA1), C (0x82), C (0x9B), C (0x40));
  UNREFERENCED_PARAMETER (Test5Result);

  memset (buffer, 0xFF, BUFSIZ);
  strcpy (buffer, Test5Base);
  assert (strcat (buffer, Test5String) == NULL);
  assert (strcmp (buffer, Test5Base) == 0);
  assert (buffer[4] == C (0xFF));
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
