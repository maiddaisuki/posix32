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

/**
 * Map single wide character using `flags`.
 *
 * This function is wrapper around `LCMapStringW` or `LCMapStringEx`.
 *
 * Returns `true` on success and `false` otherwise.
 */
static bool P32WcTrans (wchar_t *out, wchar_t in, uint32_t flags, locale_t locale) {
#if P32_LOCALE_NAMES
  LPCWSTR localeName = locale->WinLocale.LcCtype.LocaleName;
  return LCMapStringEx (localeName, flags, &in, 1, out, 1, NULL, NULL, 0) == 1;
#else
  LCID localeId = locale->WinLocale.LcCtype.LocaleId;
  return LCMapStringW (localeId, flags, &in, 1, out, 1) == 1;
#endif
}

/**
 * Get flags for LCMapString[Ex] for character mapping `wctrans`.
 *
 * This function returns `0` if `wctrans` describes invalid character mapping.
 */
static uint32_t P32WcTransFlags (wctrans_t wctrans, locale_t locale) {
  if ((wctrans & P32_WCTRANS_POSIX_ALL) == P32_WCTRANS_POSIX_ALL) {
    return 0;
  }

  if ((wctrans & P32_WCTRANS_JA_ALL) == P32_WCTRANS_JA_ALL) {
    return 0;
  }

  if ((wctrans & P32_WCTRANS_ZH_ALL) == P32_WCTRANS_ZH_ALL) {
    return 0;
  }

  uint32_t flags = 0;

  if (wctrans & P32_WCTRANS_POSIX_LOWER) {
    flags |= LCMAP_LOWERCASE;
    flags |= locale->LocaleInfo.LcCtype.WcTransFlags;
  }

  if (wctrans & P32_WCTRANS_POSIX_UPPER) {
    flags |= LCMAP_UPPERCASE;
    flags |= locale->LocaleInfo.LcCtype.WcTransFlags;
  }

  if (wctrans & P32_WCTRANS_JA_HIRAGANA) {
    flags |= LCMAP_HIRAGANA;
  }

  if (wctrans & P32_WCTRANS_JA_KATAKANA) {
    flags |= LCMAP_KATAKANA;
  }

  if (wctrans & P32_WCTRANS_ZH_SIMPLIFIED) {
    flags |= LCMAP_SIMPLIFIED_CHINESE;
  }

  if (wctrans & P32_WCTRANS_ZH_TRADITIONAL) {
    flags |= LCMAP_TRADITIONAL_CHINESE;
  }

  assert (flags != 0);

  return flags;
}

wint_t p32_towctrans_l (wint_t wc, wctrans_t wctrans, locale_t locale) {
  /**
   * POSIX requires that if `wctrans` is zero, `wc` is returned unchanged.
   */
  if (wctrans == 0) {
    return wc;
  }

  uint32_t flags = P32WcTransFlags (wctrans, locale);

  /**
   * `wctrans` describes invalid character mapping.
   */
  if (flags == 0) {
    _set_errno (EINVAL);
    return wc;
  }

  wchar_t wcTranslated = WEOF;

  /**
   * Return `wc` unchanged if failed to apply mapping described by `wctrans`.
   */
  if (!P32WcTrans (&wcTranslated, wc, flags, locale)) {
    return wc;
  }

  return wcTranslated;
}

wint_t p32_towctrans (wint_t wc, wctrans_t trans) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    activeLocale = p32_default_locale ();
  }
#endif

  return p32_towctrans_l (wc, trans, activeLocale);
}
