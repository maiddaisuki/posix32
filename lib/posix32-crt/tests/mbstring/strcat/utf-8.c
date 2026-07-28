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
 * Test `p32_private_strcat_l` function with UTF-8 (code page 65001).
 */

#define C(c)        (char) (unsigned) (c)
#define STRING(...) (const char[]){__VA_ARGS__ __VA_OPT__ (, ) 0x00}

#define LOCALE "en_US.UTF-8"
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
   * Test input which contains well-formed UTF-8 Code Unit Sequence.
   */
  const char *Test2Base = STRING (
    // clang-format off
    C (0xDF), C (0xBF)
    // clang-format on
  );

  const char *Test2String = STRING (
    // clang-format off
    C (0xEF), C (0xBF), C (0xBF),
    C (0xF4), C (0x8F), C (0xBF), C (0xBF)
    // clang-format on
  );

  const char *Test2Result = STRING (
    // clang-format off
    C (0xDF), C (0xBF),
    C (0xEF), C (0xBF), C (0xBF),
    C (0xF4), C (0x8F), C (0xBF), C (0xBF)
    // clang-format on
  );

  memset (buffer, 0xFF, BUFSIZ);
  strcpy (buffer, Test2Base);
  assert (strcat (buffer, Test2String) == buffer);
  assert (strcmp (buffer, Test2Result) == 0);
  assert (buffer[10] == C (0xFF));

  /**
   * Test input which contains incomplete UTF-8 Code Unit Sequence.
   */
  const char *Test3Base   = STRING (C (0xC2), C (0x80), C (0xDF));
  const char *Test3String = STRING (C (0xE0), C (0xA0), C (0x80), C (0xEF), C (0xBF));
  const char *Test3Result = STRING (C (0xC2), C (0x80), C (0xE0), C (0xA0), C (0x80));
  UNREFERENCED_PARAMETER (Test3Result);

  memset (buffer, 0xFF, BUFSIZ);
  strcpy (buffer, Test3Base);
  assert (strcat (buffer, Test3String) == NULL);
  assert (strcmp (buffer, Test3Base) == 0);
  assert (buffer[4] == C (0xFF));

  /**
   * Test input which contains ill-formed UTF-8 Code Unit Sequence.
   */
  const char *Test4Base   = STRING (C (0xC2), C (0x80), C (0xFF));
  const char *Test4String = STRING (C (0xED), C (0xA0), C (0x80));
  const char *Test4Result = STRING (C (0xC2), C (0x80), C (0xFF), C (0xED), C (0xA0), C (0x80));
  UNREFERENCED_PARAMETER (Test4Result);

  memset (buffer, 0xFF, BUFSIZ);
  strcpy (buffer, Test4Base);
  assert (strcat (buffer, Test4String) == NULL);
  assert (strcmp (buffer, Test4Base) == 0);
  assert (buffer[4] == C (0xFF));
}

int main (void) {
  p32_test_init ();

  locale = newlocale (LC_ALL_MASK, LOCALE, NULL);
  assert (locale != NULL);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
