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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "tests-internal.h"

/**
 * Test Summary:
 *
 * Test "qps-ploca" locale.
 */

int main (void) {
#if P32_CRT != P32_MSVCRT10
  p32_test_init ();

  assert (setlocale (LC_ALL, "qps-ploca") != NULL);
  assert (strcmp (setlocale (LC_ALL, NULL), "qps-ploca") == 0);

  /**
   * LC_CTYPE
   */
  wprintf (L"CODESET = %hs\n", nl_langinfo (CODESET));
  /**
   * LC_MONETARY
   */
  wprintf (L"CRNCYSTR = %hs\n", nl_langinfo (CRNCYSTR));
  /**
   * LC_NUMERIC
   */
  wprintf (L"RADIXCHAR = %hs\n", nl_langinfo (RADIXCHAR));
  wprintf (L"THOUSEP = %hs\n", nl_langinfo (THOUSEP));
  /**
   * LC_TIME
   */
  wprintf (L"ERA = %hs\n", nl_langinfo (ERA));
  wprintf (L"T_FMT = %hs\n", nl_langinfo (T_FMT));
  wprintf (L"T_FMT_AMPM = %hs\n", nl_langinfo (T_FMT_AMPM));
  wprintf (L"D_FMT = %hs\n", nl_langinfo (D_FMT));
  wprintf (L"D_T_FMT = %hs\n", nl_langinfo (D_T_FMT));
  wprintf (L"ERA_T_FMT = %hs\n", nl_langinfo (ERA_T_FMT));
  wprintf (L"ERA_D_FMT = %hs\n", nl_langinfo (ERA_D_FMT));
  wprintf (L"ERA_D_T_FMT = %hs\n", nl_langinfo (ERA_D_T_FMT));
  wprintf (L"ALT_DIGITS = %hs\n", nl_langinfo (ALT_DIGITS));
  wprintf (L"DAY_1 = %hs\n", nl_langinfo (DAY_1));
  wprintf (L"DAY_2 = %hs\n", nl_langinfo (DAY_2));
  wprintf (L"DAY_3 = %hs\n", nl_langinfo (DAY_3));
  wprintf (L"DAY_4 = %hs\n", nl_langinfo (DAY_4));
  wprintf (L"DAY_5 = %hs\n", nl_langinfo (DAY_5));
  wprintf (L"DAY_6 = %hs\n", nl_langinfo (DAY_6));
  wprintf (L"DAY_7 = %hs\n", nl_langinfo (DAY_7));
  wprintf (L"MON_1 = %hs\n", nl_langinfo (MON_1));
  wprintf (L"MON_2 = %hs\n", nl_langinfo (MON_2));
  wprintf (L"MON_3 = %hs\n", nl_langinfo (MON_3));
  wprintf (L"MON_4 = %hs\n", nl_langinfo (MON_4));
  wprintf (L"MON_5 = %hs\n", nl_langinfo (MON_5));
  wprintf (L"MON_6 = %hs\n", nl_langinfo (MON_6));
  wprintf (L"MON_7 = %hs\n", nl_langinfo (MON_7));
  wprintf (L"MON_8 = %hs\n", nl_langinfo (MON_8));
  wprintf (L"MON_9 = %hs\n", nl_langinfo (MON_9));
  wprintf (L"MON_10 = %hs\n", nl_langinfo (MON_10));
  wprintf (L"MON_11 = %hs\n", nl_langinfo (MON_11));
  wprintf (L"MON_12 = %hs\n", nl_langinfo (MON_12));
  wprintf (L"AM_STR = %hs\n", nl_langinfo (AM_STR));
  wprintf (L"PM_STR = %hs\n", nl_langinfo (PM_STR));

  return 0;
#else
  return 77;
#endif
}
