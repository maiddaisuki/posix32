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
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "tests-internal.h"

/**
 * Test Summary:
 *
 * Test `p32_private_strndup_l` function with UTF-8 (code page 65001).
 */

#define C(c)        (char) (unsigned) (c)
#define STRING(...) (const char[]){__VA_ARGS__ __VA_OPT__ (, ) 0x00}

#define LOCALE "en_US.UTF-8"
static locale_t locale;

/**
 * Convenience macro to call `p32_private_strndup_l`.
 */
#define strndup(s, n) p32_private_strndup_l (s, n, locale)

static void DoTest (void) {
  char *str = NULL;

  /**
   * Basic `strndup` usage.
   */
  const char *Test1String = "";
  const char *Test2String = "ABC";

  _set_errno (0);
  assert ((str = strndup (Test1String, 0)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, Test1String) == 0);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test1String, SIZE_MAX)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, Test1String) == 0);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test2String, 0)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, "") == 0);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test2String, 1)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, "A") == 0);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test2String, 2)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, "AB") == 0);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test2String, SIZE_MAX)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, Test2String) == 0);
  free (str);

  /**
   * Test input which contains well-formed UTF-8 Code Unit Sequence.
   */
  const char *Test3String = STRING (
    // clang-format off
    C (0xDF), C (0xBF),
    C (0xEF), C (0xBF), C (0xBF),
    C (0xF4), C (0x8F), C (0xBF), C (0xBF)
    // clang-format on
  );

  _set_errno (0);
  assert ((str = strndup (Test3String, 1)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, "") == 0);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test3String, 2)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, STRING (C (0xDF), C (0xBF))) == 0);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test3String, 3)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, STRING (C (0xDF), C (0xBF))) == 0);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test3String, 4)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, STRING (C (0xDF), C (0xBF))) == 0);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test3String, 5)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, STRING (C (0xDF), C (0xBF), C (0xEF), C (0xBF), C (0xBF))) == 0);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test3String, 6)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, STRING (C (0xDF), C (0xBF), C (0xEF), C (0xBF), C (0xBF))) == 0);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test3String, 7)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, STRING (C (0xDF), C (0xBF), C (0xEF), C (0xBF), C (0xBF))) == 0);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test3String, 8)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, STRING (C (0xDF), C (0xBF), C (0xEF), C (0xBF), C (0xBF))) == 0);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test3String, SIZE_MAX)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, Test3String) == 0);
  free (str);

  /**
   * Test input which contains incomplete UTF-8 Code Unit Sequence.
   */
  const char *Test4String = STRING (C (0xCA));
  const char *Test5String = STRING (C (0xDF), C (0xBF), C (0xE0), C (0xA0));
  const char *Test6String = STRING (C (0xDF), C (0xBF), C (0xEF), C (0xBF), C (0xBF), C (0xF0), C (0x90), C (0x80));

  _set_errno (0);
  assert ((str = strndup (Test4String, 1)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, "") == 0);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test4String, SIZE_MAX)) == NULL);
  assert (errno != 0 && errno != ENOMEM);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test5String, 2)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, STRING (C (0xDF), C (0xBF))) == 0);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test5String, 3)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, STRING (C (0xDF), C (0xBF))) == 0);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test5String, 4)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, STRING (C (0xDF), C (0xBF))) == 0);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test5String, SIZE_MAX)) == NULL);
  assert (errno != 0 && errno != ENOMEM);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test6String, 5)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, STRING (C (0xDF), C (0xBF), C (0xEF), C (0xBF), C (0xBF))) == 0);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test6String, 6)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, STRING (C (0xDF), C (0xBF), C (0xEF), C (0xBF), C (0xBF))) == 0);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test6String, 7)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, STRING (C (0xDF), C (0xBF), C (0xEF), C (0xBF), C (0xBF))) == 0);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test6String, 8)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, STRING (C (0xDF), C (0xBF), C (0xEF), C (0xBF), C (0xBF))) == 0);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test6String, SIZE_MAX)) == NULL);
  assert (errno != 0 && errno != ENOMEM);
  free (str);

  /**
   * Test input which contains ill-formed UTF-8 Code Unit Sequence.
   */
  const char *Test7String  = STRING (C (0xFF));
  const char *Test8String  = STRING (C (0xE0), C (0x80), C (0x80));
  const char *Test9String  = STRING (C (0xED), C (0xA0), C (0x80));
  const char *Test10String = STRING (C (0xF0), C (0x80), C (0x80), C (0x80));

  _set_errno (0);
  assert ((str = strndup (Test7String, 1)) == NULL);
  assert (errno != 0 && errno != ENOMEM);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test7String, SIZE_MAX)) == NULL);
  assert (errno != 0 && errno != ENOMEM);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test8String, 2)) == NULL);
  assert (errno != 0 && errno != ENOMEM);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test8String, SIZE_MAX)) == NULL);
  assert (errno != 0 && errno != ENOMEM);
  free (str);

  /**
   * FIXME: according to Unicode Standard, 0xED 0xA0-0xBF ... is an ill-formed
   *  UTF-8 Code Unit Sequence as it corresponds to a Surrogate Code Point.
   */
  _set_errno (0);
  assert ((str = strndup (Test9String, 2)) != NULL || errno == ENOMEM);
  assert (str == NULL || strcmp (str, "") == 0);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test9String, SIZE_MAX)) == NULL);
  assert (errno != 0 && errno != ENOMEM);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test10String, 2)) == NULL);
  assert (errno != 0 && errno != ENOMEM);
  free (str);

  _set_errno (0);
  assert ((str = strndup (Test10String, SIZE_MAX)) == NULL);
  assert (errno != 0 && errno != ENOMEM);
  free (str);
}

int main (void) {
  p32_test_init ();

  locale = newlocale (LC_ALL_MASK, LOCALE, NULL);
  assert (locale != NULL);

  DoTest ();

  freelocale (locale);

  return EXIT_SUCCESS;
}
