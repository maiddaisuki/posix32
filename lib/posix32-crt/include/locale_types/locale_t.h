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

#ifndef LIBPOSIX32_LOCALE_T_H_INCLUDED
#define LIBPOSIX32_LOCALE_T_H_INCLUDED

/**
 * Structure pointed to by `locale_t` handle.
 */
typedef struct p32_locale {
  /**
   * Must always be `P32_LOCALE_MAGIC` for a valid `locale_t` object.
   */
  int Magic;
  /**
   * Must always be `LC_ALL_MASK` for a valid `locale_t` object.
   */
  int Mask;
  /**
   * `Locale` object for each locale category.
   */
  WindowsLocale WinLocale;
  /**
   * Character set information.
   */
  Charset Charset;
  /**
   * Locale strings in Windows Format.
   */
  LocaleStrings WindowsLocaleStrings;
  /**
   * Locale strings in CRT Format.
   */
  LocaleStrings CrtLocaleStrings;
  /**
   * Locale strings in ISO Format.
   */
  LocaleStrings IsoLocaleStrings;
  /**
   * Locale information.
   */
  LocaleInfo LocaleInfo;
  /**
   * Locale-specific functions.
   */
  LocaleFunctions Functions;
#if P32_CRT >= P32_MSVCR80 && defined(LIBPOSIX32_TEST)
  /**
   * `_locale_t` objects for each locale category supported by CRT.
   */
  CrtLocale CrtLocale;
#endif
} p32_locale_t;

#endif /* LIBPOSIX32_LOCALE_T_H_INCLUDED */
