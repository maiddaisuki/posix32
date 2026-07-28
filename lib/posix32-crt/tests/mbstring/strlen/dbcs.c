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
 * Test `p32_private_strlen_l` function with DBCS code page 932.
 */

#define C(c)        (char) (unsigned) (c)
#define STRING(...) (const char[]){__VA_ARGS__ __VA_OPT__ (, ) 0x00}

#define LOCALE "ja_JP.932"
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
   * Test input which contains valid DBCS characters.
   */
  const char *Test1String = STRING (C (0x82), C (0xA1));
  const char *Test2String = STRING (C (0x82), C (0xA1), C (0xA1));
  const char *Test3String = STRING (C (0x82), C (0xA1), C (0xA1), C (0x83), C (0xA1));

  assert (strlen (Test1String) == 2);
  assert (strlen (Test2String) == 3);
  assert (strlen (Test3String) == 5);

  /**
   * Test input which contains incomplete DBCS characters.
   */
  const char *Test4String = STRING (C (0x82));
  const char *Test5String = STRING (C (0x82), C (0xA1), C (0x82));

  assert (strlen (Test4String) == (size_t) -1);
  assert (strlen (Test5String) == (size_t) -1);

  /**
   * Test input which contains invalid DBCS characters.
   */
  const char *Test6String = STRING (C (0x82), C (0xA1), C (0xFF));
  const char *Test7String = STRING (C (0x82), C (0xA1), C (0x82), C (0xFF));
  const char *Test8String = STRING (C (0x82), C (0xA1), C (0x82), C (0x9B), C (0x40));

  assert (strlen (Test6String) == (size_t) -1);
  assert (strlen (Test7String) == (size_t) -1);
  assert (strlen (Test8String) == (size_t) -1);
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
