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

#include "ctype-internal.h"

int p32_tolower_l (int c, locale_t locale) {
  if (c == EOF) {
    return EOF;
  }

  return (uint8_t) locale->LocaleInfo.LcCtype.ToLower.Map[(uint8_t) c];
}

int p32_tolower (int c) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    return tolower (c);
  }
#endif

  return p32_tolower_l (c, activeLocale);
}
