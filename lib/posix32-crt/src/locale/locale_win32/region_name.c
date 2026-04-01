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

#include "locale-internal.h"

/**
 * File Summary:
 *
 * This file defines functions to manipulate geological information stored in
 * `Locale` objects as Region Names.
 *
 * Functions defined in this file must not be called directly outside of this
 * file; use `WinlocaleGeo*` macros defined in `locale_win32.c` instead.
 */

static bool P32WinlocaleRegionName (Locale *locale, uintptr_t heap) {
  return WinlocaleGetCountryCode (&locale->GeoName, heap, locale);
}

static bool P32WinlocaleRegionNameCopy (Locale *destLocale, uintptr_t heap, Locale *srcLocale) {
  return p32_private_wcsdup (&destLocale->GeoName, srcLocale->GeoName, heap) != -1;
}

static void P32WinlocaleRegionNameDestroy (Locale *locale, uintptr_t heap) {
  if (locale->GeoName != NULL) {
    HeapFree ((HANDLE) heap, 0, locale->GeoName);
    locale->GeoName = NULL;
  }
}
