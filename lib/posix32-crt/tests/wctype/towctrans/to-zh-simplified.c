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
#include <stdio.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "tests-internal.h"

/**
 * Test Summary:
 *
 * Test "to-zh-simplified" character mapping.
 *
 * This character mapping translates traditional Chinese logographics to
 * thier simplified counterparts.
 *
 * NOTE: this character mapping is posix32-specific extension.
 */

static int exit_code = EXIT_FAILURE;

static bool __cdecl Test (locale_t locale, const wchar_t *localeName) {
  wctrans_t to_zh_simplified = (wctrans_t) 0;

  assert ((to_zh_simplified = wctrans_l ("to-zh-simplified", locale)) != (wctrans_t) 0);

  for (wchar_t wc = 0;; ++wc) {
    wchar_t wcTranslated = towctrans_l (wc, to_zh_simplified, locale);

    if (wc != wcTranslated) {
      exit_code = EXIT_SUCCESS;
      _RPTW3 (_CRT_WARN, L"%s: %c -> %c\n", localeName, wc, wcTranslated);
    }

    if (wc == WEOF) {
      break;
    }
  }

  return true;
  UNREFERENCED_PARAMETER (localeName);
}

int main (void) {
  p32_test_init ();

  assert (setlocale (LC_ALL, "en_US") != NULL);

  /**
   * Test "POSIX" locale.
   */
  locale_t posix = duplocale (LC_GLOBAL_LOCALE);
  assert (posix != NULL);

  Test (posix, L"C");
  freelocale (posix);

  /**
   * Test zh-CN locale.
   */
  locale_t zh_CN = newlocale (LC_ALL_MASK, "zh-CN", NULL);
  assert (zh_CN != NULL);

  Test (zh_CN, L"zh-CN");
  freelocale (zh_CN);

  return exit_code;
}
