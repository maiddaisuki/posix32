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

#include "locale-internal.h"

static bool P32Geo (Locale *locale, uintptr_t heap) {
  return p32_winlocale_getinfo (&locale->GeoName, heap, locale, LOCALE_SISO3166CTRYNAME);
}

static bool P32GeoDuplicate (Locale *destLocale, uintptr_t heap, Locale *srcLocale) {
  return p32_private_wcsdup (&destLocale->GeoName, srcLocale->GeoName, heap) != -1;
}

static void P32GeoDestroy (Locale *locale, uintptr_t heap) {
  if (locale->GeoName != NULL) {
    HeapFree ((HANDLE) heap, 0, locale->GeoName);
    locale->GeoName = NULL;
  }
}
