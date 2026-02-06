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
#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "locale-internal.h"

/**
 * File Summary:
 *
 * This file defines functions to gather locale information dependant on
 * LC_COLLATE locale category.
 */

bool p32_localeinfo_collate (locale_t locale, uintptr_t heap) {
  Locale        *lcCollate     = &locale->WinLocale.LcCollate;
  LcCollateInfo *lcCollateInfo = &locale->LocaleInfo.LcCollate;

  /**
   * This information is not used for "POSIX" ("C") locale.
   *
   * For all other locales `LcColalteInfo` contains extra flags to pass to
   * functions `CompareString[Ex]` and `LCMapString[Ex]`.
   */
  if (lcCollate->Type == LOCALE_TYPE_POSIX) {
    lcCollateInfo->StringCompareFlags        = 0;
    lcCollateInfo->StringCaseCompareFlags    = 0;
    lcCollateInfo->StringTransformationFlags = 0;
  } else {
    lcCollateInfo->StringCompareFlags        = NORM_LINGUISTIC_CASING;
    lcCollateInfo->StringCaseCompareFlags    = NORM_LINGUISTIC_CASING;
    lcCollateInfo->StringTransformationFlags = NORM_LINGUISTIC_CASING;
  }

  return true;
  UNREFERENCED_PARAMETER (heap);
}

bool p32_localeinfo_collate_copy (LcCollateInfo *dest, uintptr_t heap, LcCollateInfo *src) {
  dest->StringCompareFlags        = src->StringCompareFlags;
  dest->StringCaseCompareFlags    = src->StringCaseCompareFlags;
  dest->StringTransformationFlags = src->StringTransformationFlags;

  return true;
  UNREFERENCED_PARAMETER (heap);
}

void p32_localeinfo_collate_free (LcCollateInfo *lcCollateInfo, uintptr_t heap) {
  lcCollateInfo->StringCompareFlags        = 0;
  lcCollateInfo->StringCaseCompareFlags    = 0;
  lcCollateInfo->StringTransformationFlags = 0;

  return;
  UNREFERENCED_PARAMETER (heap);
}
