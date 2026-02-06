/**
 * Copyright 2025-2026 Kirill Makurin
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
 * This test is used to demonstrate difference between CRT's function
 * `_stricmp` and posix32's function `strcasecmp`.
 *
 * CRT's `_stricmp` uses filesystem rules for casing, while our `strcasecmp`
 * uses linguistic rules for casing.
 */

static int exit_code = EXIT_FAILURE;

static bool __cdecl Test (locale_t locale, const wchar_t *localeName) {
  mbstate_t state = {0};

  /**
   * Test ASCII input.
   */
  int asciiDiffCrt = 0;
  int asciiDiffP32 = 0;

  assert ((asciiDiffCrt = strcasecmp (ASCIILower.A, ASCIIUpper.A)) != _NLSCMPERROR);
  assert ((asciiDiffP32 = strcasecmp_l (ASCIILower.A, ASCIIUpper.A, locale)) != _NLSCMPERROR);

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

  int localeDiffCrt = 0;
  int localeDiffP32 = 0;

  assert ((localeDiffCrt = strcasecmp (Lower, Upper)) != _NLSCMPERROR);
  assert ((localeDiffP32 = strcasecmp_l (Lower, Upper, locale)) != _NLSCMPERROR);

  free (Lower);
  free (Upper);

  /**
   * Test Unicode input.
   */
  int unicodeDiffCrt = _NLSCMPERROR;
  int unicodeDiffP32 = _NLSCMPERROR;

  if (p32_private_strcmp_l (nl_langinfo_l (CODESET, locale), "UTF-8", locale) == 0) {
    assert ((unicodeDiffCrt = strcasecmp (Unicode1Lower.A, Unicode1Upper.A)) != _NLSCMPERROR);
    assert ((unicodeDiffP32 = strcasecmp_l (Unicode1Lower.A, Unicode1Upper.A, locale)) != _NLSCMPERROR);
  }

  if (asciiDiffCrt != asciiDiffP32 || localeDiffCrt != localeDiffP32 || unicodeDiffCrt != unicodeDiffP32) {
    exit_code = EXIT_SUCCESS;
    fwprintf (
      stderr, L"%-24s: (%d|%d) (%d|%d) (%d|%d)\n", localeName, asciiDiffCrt, asciiDiffP32, localeDiffCrt, localeDiffP32,
      unicodeDiffCrt, unicodeDiffP32
    );
  }

  return true;
}

int main (void) {
  p32_test_init ();

  int testFlags = P32_LOCALE_TEST_SETLOCALE | P32_LOCALE_TEST_ALL;

#if P32_CRT < P32_UCRT
  testFlags |= P32_LOCALE_TEST_USABLE_CP;
#endif

  /**
   * Test all working locales
   */
  p32_locale_test_func2 (Test, testFlags);

  return exit_code;
}
