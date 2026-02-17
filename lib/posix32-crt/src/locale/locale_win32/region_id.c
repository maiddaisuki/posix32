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

P32_STATIC_ASSERT (sizeof ((Locale) {0}.GeoId), "Size of `Locale.GeoId` must be 4 bytes.");

static bool P32Geo (Locale *locale, uintptr_t heap) {
  LocaleInfoRequest infoRequest = {0};

  infoRequest.Flags  = (P32_LOCALE_INFO_REQUEST_NUMERIC);
  infoRequest.Info   = LOCALE_IGEOID;
  infoRequest.Output = (uint32_t *) &locale->GeoId;

  return p32_winlocale_get_locale_info (&infoRequest, heap, locale);
}

static bool P32GeoDuplicate (Locale *destLocale, uintptr_t heap, Locale *srcLocale) {
  destLocale->GeoId = srcLocale->GeoId;
  return true;
  UNREFERENCED_PARAMETER (heap);
}

static void P32GeoDestroy (Locale *locale, uintptr_t heap) {
  locale->GeoId = 0;
  return;
  UNREFERENCED_PARAMETER (heap);
}
