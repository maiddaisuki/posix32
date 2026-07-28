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
 * Test `p32_private_strlen_l` function with UTF-8 (code page 65001).
 */

#define C(c)        (char) (unsigned) (c)
#define STRING(...) (const char[]){__VA_ARGS__ __VA_OPT__ (, ) 0x00}

#define LOCALE "en_US.UTF-8"
static locale_t locale;

/**
 * Convenience macro to call `p32_private_strlen_l`.
 */
#define strlen(s) p32_private_strlen_l (s, locale)

static void DoTest (void) {
  /**
   * Basic `strlen` usage.
   */
  assert (strlen ("") == 0);
  assert (strlen ("A") == 1);
  assert (strlen ("AB") == 2);
  assert (strlen ("ABC") == 3);

  /**
   * Test input which contains well-formed UTF-8 Code Unit Sequence.
   */
  const char *Test1String = STRING (
    // clang-format off
    C (0xC2), C (0x80)
    // clang-format on
  );

  const char *Test2String = STRING (
    // clang-format off
    C (0xDF), C (0xBF),
    C (0xE0), C (0xA0), C (0x80)
    // clang-format on
  );

  const char *Test3String = STRING (
    // clang-format off
    C (0xDF), C (0xBF),
    C (0xEF), C (0xBF), C (0xBF),
    C (0xF0), C (0x90), C (0x80), C (0x80)
    // clang-format on
  );

  assert (strlen (Test1String) == 2);
  assert (strlen (Test2String) == 5);
  assert (strlen (Test3String) == 9);

  /**
   * Test input which contains incomplete UTF-8 Code Unit Sequence.
   */
  const char *Test4String = STRING (C (0xCA));
  const char *Test5String = STRING (C (0xDF), C (0xBF), C (0xE0), C (0xA0));
  const char *Test6String = STRING (C (0xDF), C (0xBF), C (0xEF), C (0xBF), C (0xBF), C (0xF0), C (0x90), C (0x80));

  assert (strlen (Test4String) == (size_t) -1);
  assert (strlen (Test5String) == (size_t) -1);
  assert (strlen (Test6String) == (size_t) -1);

  /**
   * Test input which contains ill-formed UTF-8 Code Unit Sequence.
   */
  const char *Test7String  = STRING (C (0xFF));
  const char *Test8String  = STRING (C (0xE0), C (0x80), C (0x80));
  const char *Test9String  = STRING (C (0xED), C (0xA0), C (0x80));
  const char *Test10String = STRING (C (0xF0), C (0x80), C (0x80), C (0x80));

  assert (strlen (Test7String) == (size_t) -1);
  assert (strlen (Test8String) == (size_t) -1);
  assert (strlen (Test9String) == (size_t) -1);
  assert (strlen (Test10String) == (size_t) -1);
}

int main (void) {
  p32_test_init ();

  locale = newlocale (LC_ALL_MASK, LOCALE, NULL);
  assert (locale != NULL);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
