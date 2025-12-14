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

#ifndef LIBPOSIX32_LC_CTYPE_H_INCLUDED
#define LIBPOSIX32_LC_CTYPE_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>
#include <wctype.h>

/**
 * Character class name (multibyte) and corresponding `wctype_t` value.
 */
typedef struct CharTypeA {
  char    *Name;
  wctype_t CharType;
} CharTypeA;

/**
 * Character class name (wide) and corresponding `wctype_t` value.
 */
typedef struct CharTypeW {
  const wchar_t *Name;
  wctype_t       CharType;
} CharTypeW;

/**
 * Character mapping name (multibyte) and corresponding `wctrans_t` value.
 */
typedef struct CharMappingA {
  char     *Name;
  wctrans_t CharMapping;
} CharMappingA;

/**
 * Character mapping name (wide) and corresponding `wctrans_t` value.
 */
typedef struct CharMappingW {
  const wchar_t *Name;
  wctrans_t      CharMapping;
} CharMappingW;

/**
 * Mapping of ASCII characters for a specific code page.
 *
 * An element `Map[N]` contains value used to represent ASCII character with
 * decimal value `N`.
 *
 * This is used to support EBCDIC code pages.
 */
typedef struct AsciiMap {
  char Map[128];
} AsciiMap;

/**
 * Character class bits for each single-byte character in specific code page.
 *
 * NOTE: `P32_CHARTYPE_*` bits are defined in `ctype-internal.h`.
 */
typedef struct CharTypeMap {
  wctype_t Map[256];
} CharTypeMap;

/**
 * Lowercase mapping for each single-byte character in specific code page.
 */
typedef struct LowerMap {
  char Map[256];
} LowerMap;

/**
 * Uppercase mapping for each single-byte character in specific code page.
 */
typedef struct UpperMap {
  char Map[256];
} UpperMap;

/**
 * Locale information dependant on `LC_CTYPE` locale category.
 */
typedef struct LcCtypeInfo {
  /**
   * Character set name.
   */
  struct {
    char    *A;
    wchar_t *W;
  } CharsetName;
  /**
   * Character classes supported by locale.
   */
  CharTypeA CharTypes[16];
  /**
   * Character mappings supported by locale.
   */
  CharMappingA CharMappings[16];
  /**
   * Locale-specific flags for `LCMapString[Ex]`.
   */
  uint32_t WcTransFlags;
  /**
   * Locale-specific flags for `CompareString[Ex]`.
   */
  uint32_t    CaseCmpFlags;
  AsciiMap    AsciiMap;
  CharTypeMap CharType;
  LowerMap    ToLower;
  UpperMap    ToUpper;
} LcCtypeInfo;

/**
 * Gather locale information dependant on `LC_CTYPE` locale category.
 *
 * Returns `true` on success, and `false` otherwise.
 */
bool p32_localeinfo_ctype (locale_t locale, uintptr_t heap);

/**
 * Copy locale information from `src` to `dest`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
bool p32_localeinfo_ctype_copy (LcCtypeInfo *dest, uintptr_t heap, LcCtypeInfo *src);

/**
 * Free locale information stored in `lcCtypeInfo`.
 */
void p32_localeinfo_ctype_free (LcCtypeInfo *lcCtypeInfo, uintptr_t heap);

#endif /* LIBPOSIX32_LC_CTYPE_H_INCLUDED */
