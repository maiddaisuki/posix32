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
 * `Locale` objects as `GEOID` objects.
 *
 * Functions defined in this file must not be called directly outside of this
 * file; use `WinlocaleGeo*` macros defined in `locale_win32.c` instead.
 */

P32_STATIC_ASSERT (sizeof ((Locale){0}.GeoId), "Size of `Locale.GeoId` must be 4 bytes.");

static bool P32WinlocaleGeo (Locale *locale, uintptr_t heap) {
  LocaleInfoRequest infoRequest = {0};

  infoRequest.Info   = LOCALE_IGEOID;
  infoRequest.Output = (uint32_t *) &locale->GeoId;

  return WinlocaleGetNumericLocaleInfo (&infoRequest, heap, locale);
}

static bool P32WinlocaleGeoCopy (Locale *destLocale, uintptr_t heap, Locale *srcLocale) {
  destLocale->GeoId = srcLocale->GeoId;
  return true;
  UNREFERENCED_PARAMETER (heap);
}

static void P32WinlocaleGeoDestroy (Locale *locale, uintptr_t heap) {
  locale->GeoId = 0;
  return;
  UNREFERENCED_PARAMETER (heap);
}
