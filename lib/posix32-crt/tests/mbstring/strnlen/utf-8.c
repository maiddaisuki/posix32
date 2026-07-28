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
 * Test `p32_private_strnlen_l` function with UTF-8 (code page 65001).
 */

#define C(c)        (char) (unsigned) (c)
#define STRING(...) (const char[]){__VA_ARGS__ __VA_OPT__ (, ) 0x00}

#define LOCALE "en_US.UTF-8"
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
   * Test input which contains well-formed UTF-8 Code Unit Sequence.
   */
  const char *Test3String = STRING (
    // clang-format off
    C (0xC2), C (0x80),
    C (0xE0), C (0xA0), C (0x80),
    C (0xF0), C (0x90), C (0x80), C (0x80)
    // clang-format on
  );

  assert (strnlen (Test3String, 0) == 0);
  assert (strnlen (Test3String, 1) == 0);
  assert (strnlen (Test3String, 2) == 2);
  assert (strnlen (Test3String, 3) == 2);
  assert (strnlen (Test3String, 4) == 2);
  assert (strnlen (Test3String, 5) == 5);
  assert (strnlen (Test3String, 6) == 5);
  assert (strnlen (Test3String, 7) == 5);
  assert (strnlen (Test3String, 8) == 5);
  assert (strnlen (Test3String, SIZE_MAX) == 9);

  /**
   * Test input which contains incomplete UTF-8 Code Unit Sequence.
   */
  const char *Test4String = STRING (C (0xCA));
  const char *Test5String = STRING (C (0xDF), C (0xBF), C (0xE0), C (0xA0));
  const char *Test6String = STRING (C (0xDF), C (0xBF), C (0xEF), C (0xBF), C (0xBF), C (0xF0), C (0x90), C (0x80));

  assert (strnlen (Test4String, 0) == 0);
  assert (strnlen (Test4String, 1) == 0);
  assert (strnlen (Test4String, SIZE_MAX) == (size_t) -1);

  assert (strnlen (Test5String, 2) == 2);
  assert (strnlen (Test5String, 3) == 2);
  assert (strnlen (Test5String, 4) == 2);
  assert (strnlen (Test5String, SIZE_MAX) == (size_t) -1);

  assert (strnlen (Test6String, 5) == 5);
  assert (strnlen (Test6String, 6) == 5);
  assert (strnlen (Test6String, 7) == 5);
  assert (strnlen (Test6String, 8) == 5);
  assert (strnlen (Test6String, SIZE_MAX) == (size_t) -1);

  /**
   * Test input which contains ill-formed UTF-8 Code Unit Sequence.
   */
  const char *Test7String  = STRING (C (0xFF));
  const char *Test8String  = STRING (C (0xE0), C (0x80), C (0x80));
  const char *Test9String  = STRING (C (0xED), C (0xA0), C (0x80));
  const char *Test10String = STRING (C (0xF0), C (0x80), C (0x80), C (0x80));

  assert (strnlen (Test7String, 0) == 0);
  assert (strnlen (Test7String, SIZE_MAX) == (size_t) -1);

  assert (strnlen (Test8String, 0) == 0);
  assert (strnlen (Test8String, 1) == 0);
  assert (strnlen (Test8String, 2) == (size_t) -1);
  assert (strnlen (Test8String, SIZE_MAX) == (size_t) -1);

  assert (strnlen (Test9String, 0) == 0);
  assert (strnlen (Test9String, 1) == 0);
  /**
   * FIXME: according to Unicode Standard, 0xED 0xA0-0xBF ... is an ill-formed
   *  UTF-8 Code Unit Sequence as it corresponds to a Surrogate Code Point.
   */
  assert (strnlen (Test9String, 2) == 0);
  assert (strnlen (Test9String, SIZE_MAX) == (size_t) -1);

  assert (strnlen (Test10String, 0) == 0);
  assert (strnlen (Test10String, 1) == 0);
  assert (strnlen (Test10String, 2) == (size_t) -1);
  assert (strnlen (Test10String, SIZE_MAX) == (size_t) -1);
}

int main (void) {
  p32_test_init ();

  locale = newlocale (LC_ALL_MASK, LOCALE, NULL);
  assert (locale != NULL);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
