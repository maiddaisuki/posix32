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

#include "p32_wctype.h"

wint_t p32_towupper_l (wint_t wc, locale_t locale) {
  return p32_towctrans_l (wc, P32_WCTRANS_POSIX_UPPER, locale);
}

wint_t p32_towupper (wint_t wc) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    return towupper (wc);
  }
#endif

  return p32_towupper_l (wc, activeLocale);
}
