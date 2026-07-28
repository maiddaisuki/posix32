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
 * Test `p32_private_strnlen_l` function with DBCS code page 932.
 */

#define C(c)        (char) (unsigned) (c)
#define STRING(...) (const char[]){__VA_ARGS__ __VA_OPT__ (, ) 0x00}

#define LOCALE "ja_JP.932"
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
   * Test input which contains valid DBCS characters.
   */
  const char *Test3String = STRING (C (0x82), C (0xA1), C (0xA1), C (0x83), C (0xA1));

  assert (strnlen (Test3String, 0) == 0);
  assert (strnlen (Test3String, 1) == 0);
  assert (strnlen (Test3String, 2) == 2);
  assert (strnlen (Test3String, 3) == 3);
  assert (strnlen (Test3String, 4) == 3);
  assert (strnlen (Test3String, SIZE_MAX) == 5);

  /**
   * Test input which contains incomplete DBCS characters.
   */
  const char *Test4String = STRING (C (0x82), C (0xA1), C (0x82));

  assert (strnlen (Test4String, 2) == 2);
  assert (strnlen (Test4String, 3) == 2);
  assert (strnlen (Test4String, SIZE_MAX) == (size_t) -1);

  /**
   * Test input which contains invalid DBCS characters.
   */
  const char *Test5String = STRING (C (0x82), C (0xA1), C (0xFF));
  const char *Test6String = STRING (C (0x82), C (0xA1), C (0x82), C (0xFF));
  const char *Test7String = STRING (C (0x82), C (0xA1), C (0x82), C (0x9B), C (0x40));

  assert (strnlen (Test5String, 2) == 2);
  assert (strnlen (Test5String, 3) == (size_t) -1);
  assert (strnlen (Test5String, SIZE_MAX) == (size_t) -1);

  assert (strnlen (Test6String, 2) == 2);
  assert (strnlen (Test6String, 3) == 2);
  assert (strnlen (Test6String, 4) == (size_t) -1);
  assert (strnlen (Test6String, SIZE_MAX) == (size_t) -1);

  assert (strnlen (Test7String, 2) == 2);
  assert (strnlen (Test7String, 3) == 2);
  assert (strnlen (Test7String, 4) == (size_t) -1);
  assert (strnlen (Test7String, SIZE_MAX) == (size_t) -1);
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
