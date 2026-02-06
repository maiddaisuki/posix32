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

#include "p32_wctype.h"

wctype_t p32_wctrans_l (const char *trans, locale_t locale) {
  for (size_t i = 0; i < _countof (locale->LocaleInfo.LcCtype.CharMappings); ++i) {
    int diff = p32_private_strcmp_l (trans, locale->LocaleInfo.LcCtype.CharMappings[i].Name, locale);

    if (diff == _NLSCMPERROR) {
      return 0;
    }

    if (diff == 0) {
      return locale->LocaleInfo.LcCtype.CharMappings[i].CharMapping;
    }
  }

  return 0;
}

wctype_t p32_wctrans (const char *trans) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    activeLocale = p32_default_locale ();
  }
#endif

  return p32_wctrans_l (trans, activeLocale);
}
