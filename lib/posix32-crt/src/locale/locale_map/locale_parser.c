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

/**
 * File Summary:
 *
 * This file contains generic implementation for locale string parsers used
 * by `p32_locale_map` function.
 */

/**
 * We distinguish three formats of locale string:
 *
 * - ISO Format
 * - Windows Format
 * - CRT Format
 *
 * Complete locale string has the following syntax:
 *
 *  locale[.CHARSET][@MODIFIER]
 *
 * where `locale` is string in one of formats listed above.
 *
 * CHARSET:
 *
 * Name of character set to use (e.g. UTF-8) or code page (e.g. 65001).
 *
 * MODIFIER:
 *
 * Modifier may specify one of the following:
 *
 * 1. Script name. This may be a 4-letter string as used by Windows Format
 *  (e.g. `Latn`), or string following language name in CRT Format,
 *  (e.g. `Latin`).
 *
 * 2. Sorting order. This may be a 6-letter string as used by Windows Format,
 *  (e.g. `stroke`).
 *
 * 3. One of the following strings:
 *
 *  - valencia (use with ca[_ES][.CHARSET]@valencia)
 *
 * If `MODIFIER` does not match any of these, it is ignored.
 *
 * If `MODIFIER` specifies information explicitly provided by `locale` part of
 * the string, the `MODIFIER` is ignored.
 *
 * Modifiers are generally for use with the ISO format. They are the only way
 * to supply information such as script with ISO format.
 *
 * Both Windows and CRT formats have a way to supply this information.
 * Using modifiers with these formats is redundant and non-portable.
 */

#ifndef P32_LOCALE_PARSER
#error P32_LOCALE_PARSER must be defined before including locale_parser.c
#endif

#define P32_LOCALE_PARSER_ISO     0
#define P32_LOCALE_PARSER_WINDOWS 1
#define P32_LOCALE_PARSER_CRT     2

#if P32_LOCALE_PARSER == P32_LOCALE_PARSER_ISO
/**
 * ISO Format:
 *
 *  ll[_CC]
 *
 * This format is supported by UCRT when used with UTF-8 (e.g. "en_US.UTF-8").
 * This format cannot be used with non-UTF-8 locales and older CRTs.
 *
 * `ll` is an ISO 639[-2] language code.
 * `CC` is an ISO 3166[-2] country code.
 */
#define FuncName     P32ParseIsoLocaleString
#define SingleLookup true
#elif P32_LOCALE_PARSER == P32_LOCALE_PARSER_WINDOWS
/**
 * Windows Format:
 *
 * ll[-SSSS][-CC][_xxxxxx]
 *
 * This is the locale names used by Windows APIs which were introduced in
 * Windows Vista.
 *
 * This format is supported by CRT's `setlocale` since msvcr110.dll.
 *
 * `ll` and `CC` have the same meaning as for ISO format.
 *
 * `SSSS` is a 4-letter name of script (e.g. "Latn").
 * `xxxxxx` is 6-letter name of sorting order (e.g. "stroke").
 */
#define FuncName     P32ParseWindowsLocaleString
#define SingleLookup true
#elif P32_LOCALE_PARSER == P32_LOCALE_PARSER_CRT
/**
 * CRT Format:
 *
 * language[_country]
 *
 * This format is supported by all CRTs.
 * This is the only format supported by CRTs before msvcr110.dll.
 *
 * `language` is the language name
 * `country` is the country name
 *
 * Both `language` and `country` may be aliases supported by `setlocale`,
 * for example:
 *
 *  `american english` is equivalent to `en-US`
 *
 * and
 *
 *  `english_britain` is equivalent to `en-GB`
 *
 * In addition something like this is also supported:
 *
 *  `american-english_United Kingdom` is equivalent to `en-GB`
 */
#define FuncName     P32ParseCrtLocaleString
#define SingleLookup false
#else
#error P32_LOCALE_PARSER is set to invalid value
#endif

static int FuncName (LocaleMap *localeMap, const wchar_t *localeString, uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

#if P32_LOCALE_PARSER == P32_LOCALE_PARSER_WINDOWS
  ptrdiff_t scriptSeparatorPosition = 0;
#endif
  ptrdiff_t countrySeparatorPosition = 0;
#if P32_LOCALE_PARSER == P32_LOCALE_PARSER_WINDOWS
  ptrdiff_t sortingSeparatorPosition = 0;
#endif
  ptrdiff_t charsetSeparatorPosition  = 0;
  ptrdiff_t modifierSeparatorPosition = 0;

  /**
   * Do simple checks on `localeString` so we can fail early without any
   * memory allocations.
   */
  wchar_t *separator = NULL;

  separator = wcschr (localeString, L'@');

  /**
   * Locale string cannot start with modifier.
   */
  if (separator == localeString) {
    return -1;
  } else if (separator != NULL) {
    modifierSeparatorPosition = separator - localeString;
  }

  separator = wcschr (localeString, L'.');

  /**
   * Locale string cannot start with charset.
   */
  if (separator == localeString) {
    return -1;
  } else if (separator != NULL) {
    charsetSeparatorPosition = separator - localeString;

    /**
     * Modifier cannot precede charset.
     */
    if (modifierSeparatorPosition > 0 && modifierSeparatorPosition < charsetSeparatorPosition) {
      return -1;
    }
  }

  separator = wcschr (localeString, L'_');

#if P32_LOCALE_PARSER == P32_LOCALE_PARSER_WINDOWS
#define xx sortingSeparatorPosition
#else
#define xx countrySeparatorPosition
#endif

  /**
   * Locale string cannot start with `xx`.
   */
  if (separator == localeString) {
    return -1;
  } else if (separator != NULL) {
    xx = separator - localeString;

    /**
     * `xx` must precede both charset and modifier.
     */
    if ((modifierSeparatorPosition > 0 && modifierSeparatorPosition < xx)
        || (charsetSeparatorPosition > 0 && charsetSeparatorPosition < xx)) {
      return -1;
    }
  }

#undef xx

  /**
   * Format-specific processing.
   */
  size_t stringLength = wcslen (localeString);

#if P32_LOCALE_PARSER == P32_LOCALE_PARSER_ISO
  size_t length = 0;

  if (charsetSeparatorPosition > 0) {
    length = charsetSeparatorPosition;
  } else if (modifierSeparatorPosition > 0) {
    length = modifierSeparatorPosition;
  } else {
    length = stringLength;
  }

  if (length == 2 || length == 3) {
    countrySeparatorPosition = 0;
  } else if (length == 5 && localeString[2] == L'_') {
    countrySeparatorPosition = 2;
  } else if (length == 6 && localeString[2] == L'_') {
    countrySeparatorPosition = 2;
  } else if (length == 6 && localeString[3] == L'_') {
    countrySeparatorPosition = 3;
  } else if (length == 7 && localeString[3] == L'_') {
    countrySeparatorPosition = 3;
  } else {
    return 1;
  }
#elif P32_LOCALE_PARSER == P32_LOCALE_PARSER_WINDOWS
  size_t length = 0;

  if (sortingSeparatorPosition > 0) {
    length = sortingSeparatorPosition;
  } else if (charsetSeparatorPosition > 0) {
    length = charsetSeparatorPosition;
  } else if (modifierSeparatorPosition > 0) {
    length = modifierSeparatorPosition;
  } else {
    length = stringLength;
  }

  /* ll or lll */
  if (length == 2 || length == 3) {
    countrySeparatorPosition = 0;
    /* ll-CC */
  } else if (length == 5 && localeString[2] == L'-') {
    countrySeparatorPosition = 2;
    /* ll-CCC */
  } else if (length == 6 && localeString[2] == L'-') {
    countrySeparatorPosition = 2;
    /* lll-CC */
  } else if (length == 6 && localeString[3] == L'-') {
    countrySeparatorPosition = 3;
    /* ll-Ssss */
  } else if (length == 7 && localeString[2] == L'-') {
    scriptSeparatorPosition = 2;
    /* lll-CCC */
  } else if (length == 7 && localeString[3] == L'-') {
    countrySeparatorPosition = 3;
    /* lll-Ssss */
  } else if (length == 8 && localeString[3] == L'-') {
    scriptSeparatorPosition = 3;
    /* ll-Ssss-CC */
  } else if (length == 10 && localeString[2] == L'-' && localeString[7] == L'-') {
    scriptSeparatorPosition  = 2;
    countrySeparatorPosition = 7;
    /* ll-Ssss-CCC */
  } else if (length == 11 && localeString[2] == L'-' && localeString[7] == L'-') {
    scriptSeparatorPosition  = 2;
    countrySeparatorPosition = 7;
    /* lll-Ssss-CC */
  } else if (length == 11 && localeString[3] == L'-' && localeString[8] == L'-') {
    scriptSeparatorPosition  = 3;
    countrySeparatorPosition = 8;
    /* lll-Ssss-CCC */
  } else if (length == 12 && localeString[3] == L'-' && localeString[8] == L'-') {
    scriptSeparatorPosition  = 3;
    countrySeparatorPosition = 8;
  } else {
    return 1;
  }
#endif

  /**
   * Convert and validate locale string.
   */
  int returnCode = 0;

  LocaleStringMap stringMap = {0};
  stringMap.Language        = HeapAlloc (heapHandle, 0, stringLength + 1);

  if (stringMap.Language == NULL) {
    return -1;
  }

  if (!P32WcsToAscii (stringMap.Language, localeString, stringLength)) {
    returnCode = -1;
    goto fail;
  }

#if P32_LOCALE_PARSER == P32_LOCALE_PARSER_WINDOWS
  if (scriptSeparatorPosition > 0) {
    stringMap.Language[scriptSeparatorPosition] = '\0';
    stringMap.Script                            = stringMap.Language + scriptSeparatorPosition + 1;
  }
#endif

  if (countrySeparatorPosition > 0) {
    stringMap.Language[countrySeparatorPosition] = '\0';
    stringMap.Country                            = stringMap.Language + countrySeparatorPosition + 1;
  }

#if P32_LOCALE_PARSER == P32_LOCALE_PARSER_WINDOWS
  if (sortingSeparatorPosition > 0) {
    stringMap.Language[sortingSeparatorPosition] = '\0';
    stringMap.Sorting                            = stringMap.Language + sortingSeparatorPosition + 1;
  }
#endif

  if (charsetSeparatorPosition > 0) {
    stringMap.Language[charsetSeparatorPosition] = '\0';
    stringMap.Charset                            = stringMap.Language + charsetSeparatorPosition + 1;
  }

  if (modifierSeparatorPosition > 0) {
    stringMap.Language[modifierSeparatorPosition] = '\0';
    stringMap.Modifier                            = stringMap.Language + modifierSeparatorPosition + 1;
  }

  if (!P32LocaleMap (localeMap, &stringMap, SingleLookup)) {
#if P32_LOCALE_PARSER == P32_LOCALE_PARSER_CRT
    returnCode = -1;
#else
    returnCode = 1;
#endif
  }

fail:
  HeapFree (heapHandle, 0, stringMap.Language);

  return returnCode;
}

#undef FuncName
#undef SingleLookup
