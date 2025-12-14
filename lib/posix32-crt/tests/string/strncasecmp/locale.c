/**
 * Copyright 2025 Kirill Makurin
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

#include "string-test.h"

/**
 * Test Summary:
 *
 * Test `strncasecmp` function.
 *
 * Note that our implementation of `strncasecmp` uses linguistic casing rules,
 * unlike CRT function `_strnicmp`, which uses filesystem casing rules.
 */

static int exit_code = EXIT_SUCCESS;

static void CompareStringIncremental (
  const char    *lower,
  const char    *upper,
  locale_t       locale,
  const wchar_t *localeName
) {
  mbstate_t state       = {0};
  size_t    totalLength = 0;

  while (1) {
    size_t length = mbrlen_l (lower + totalLength, MB_CUR_MAX_L (locale), &state, locale);
    assert (length != (size_t) -1);

    if (length == 0) {
      break;
    }

    totalLength += length;

    int diff = 0;

    assert ((diff = strncasecmp_l (lower, upper, totalLength, locale)) != _NLSCMPERROR);

    if (diff) {
      exit_code = EXIT_FAILURE;
      fwprintf (stderr, L"%-24s: %d\n", localeName, diff);
    }
  }

  assert (mbsinit (&state));
}

static bool __cdecl Test (locale_t locale, const wchar_t *localeName) {
  mbstate_t state = {0};

  /**
   * Test ASCII input.
   */
  const char *ascii       = ASCIIString.A;
  size_t      asciiLength = p32_private_strlen_l (ascii, locale);

  char *asciiLower = NULL;
  char *asciiUpper = NULL;

  assert ((asciiLower = p32_private_strdup_l (ascii, locale)) != NULL);
  assert ((asciiUpper = p32_private_strdup_l (ascii, locale)) != NULL);

  for (size_t i = 0; i < asciiLength; ++i) {
    int isinit = mbsinit (&state);

    if (mbrlen_l (ascii + i, 1, &state, locale) <= 1 && isinit) {
      asciiLower[i] = (uint8_t) tolower_l ((uint8_t) ascii[i], locale);
      asciiUpper[i] = (uint8_t) toupper_l ((uint8_t) ascii[i], locale);
    }
  }

  assert (mbsinit (&state));
  CompareStringIncremental (asciiLower, asciiUpper, locale, localeName);

  free (asciiLower);
  free (asciiUpper);

  /**
   * Test locale-specific input.
   */
  char  *str       = nl_langinfo_l (MON_1, locale);
  size_t strLength = p32_private_strlen_l (str, locale);

  char *Lower = NULL;
  char *Upper = NULL;

  assert ((Lower = p32_private_strdup_l (str, locale)) != NULL);
  assert ((Upper = p32_private_strdup_l (str, locale)) != NULL);

  for (size_t i = 0; i < strLength; ++i) {
    int isinit = mbsinit (&state);

    if (mbrlen_l (str + i, 1, &state, locale) <= 1 && isinit) {
      Lower[i] = (uint8_t) tolower_l ((uint8_t) str[i], locale);
      Upper[i] = (uint8_t) toupper_l ((uint8_t) str[i], locale);
    }
  }

  assert (mbsinit (&state));
  CompareStringIncremental (Lower, Upper, locale, localeName);

  free (Lower);
  free (Upper);

  /**
   * Test Unicode input.
   */
  if (p32_private_strcmp_l (nl_langinfo_l (CODESET, locale), "UTF-8", locale) == 0) {
    CompareStringIncremental (Unicode1Lower.A, Unicode1Upper.A, locale, localeName);
    CompareStringIncremental (Unicode2Lower.A, Unicode2Upper.A, locale, localeName);
  }

  return true;
}

int main (void) {
  p32_test_init ();

  /**
   * Test all supported locales.
   */
  p32_locale_test_func2 (Test, P32_LOCALE_TEST_ALL);

  return exit_code;
}
