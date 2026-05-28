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

static int p32_iswcntrl_unicode (wint_t wc, locale_t locale) {
  return p32_iswctype_unicode (wc, P32_CHARTYPE_CONTROL, locale);
}

static void P32LocaleFunction_iswcntrl (LocaleFunctions *functions) {
  functions->F_iswcntrl = p32_iswcntrl_unicode;
}

int p32_private_iswcntrl_l (wint_t wc, locale_t locale) {
  return locale->Functions.F_iswcntrl (wc, locale);
}

int p32_iswcntrl_l (wint_t wc, locale_t locale) {
  return p32_private_iswcntrl_l (wc, locale);
}

int p32_iswcntrl (wint_t wc) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
#if 1
    return iswcntrl (wc);
#else
    return iswctype (wc, _CONTROL);
#endif
  }
#endif

  return p32_private_iswcntrl_l (wc, activeLocale);
}
