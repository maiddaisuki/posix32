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

#ifndef LIBPOSIX32_LOCALE_FUNCTIONS_H_INCLUDED
#define LIBPOSIX32_LOCALE_FUNCTIONS_H_INCLUDED

#include <stdint.h>
#include <wchar.h>

#include "p32_locale.h"

#include "locale_types/charset.h"

/**
 * Pointers to locale-specific implementations of locale-dependant functions.
 */
typedef struct {
  /**
   * string.h
   */
  size_t (*F_strlen) (const char *, locale_t);
  size_t (*F_strnlen) (const char *, size_t, locale_t);
  char  *(*F_strcpy) (char *, const char *, locale_t);
  char  *(*F_strncpy) (char *, const char *, size_t, locale_t);
  char  *(*F_strcat) (char *, const char *, locale_t);
  char  *(*F_strncat) (char *, const char *, size_t, locale_t);
  int    (*F_strcmp) (const char *, const char *, locale_t);
  int    (*F_strncmp) (const char *, const char *, size_t, locale_t);
  char  *(*F_strchr) (const char *, int, locale_t);
  char  *(*F_strrchr) (const char *, int, locale_t);
  char  *(*F_strstr) (const char *, const char *, locale_t);
  char  *(*F_strpbrk) (const char *, const char *, locale_t);
  size_t (*F_strspn) (const char *, const char *, locale_t);
  size_t (*F_strcspn) (const char *, const char *, locale_t);
  char  *(*F_strtok) (char *, const char *, locale_t);
  char  *(*F_strtok_r) (char *, const char *, char **, locale_t);
  int    (*F_strcoll) (const char *, const char *, locale_t);
  size_t (*F_strxfrm) (char *, const char *, size_t, locale_t);
  /**
   * strings.h
   */
  int (*F_strcasecmp) (const char *, const char *, locale_t);
  int (*F_strncasecmp) (const char *, const char *, size_t, locale_t);
  /**
   * stdlib.h: C89 Conversion Functions
   */
  int    (*F_mblen) (const char *, size_t, locale_t);
  int    (*F_mbtowc) (wchar_t *, const char *, size_t, locale_t);
  size_t (*F_mbstowcs) (wchar_t *, const char *, size_t, locale_t);
  int    (*F_wctomb) (char *, wchar_t, locale_t);
  size_t (*F_wcstombs) (char *, const wchar_t *, size_t, locale_t);
  /**
   * wchar.h: C95 Conversion Functions
   */
  wint_t (*F_btowc) (int, Charset *);
  int    (*F_wctob) (wint_t, Charset *);
  size_t (*F_mbrlen) (const char *P32_RESTRICT, size_t, mbstate_t *P32_RESTRICT, locale_t);
  size_t (*F_mbrtowc) (wchar_t *P32_RESTRICT, const char *P32_RESTRICT, size_t, mbstate_t *P32_RESTRICT, locale_t);
  size_t (*F_mbsrtowcs) (wchar_t *P32_RESTRICT, const char **P32_RESTRICT, size_t, mbstate_t *P32_RESTRICT, locale_t);
  size_t (*F_mbsnrtowcs) (
    wchar_t *P32_RESTRICT,
    const char **P32_RESTRICT,
    size_t,
    size_t,
    mbstate_t *P32_RESTRICT,
    locale_t
  );
  size_t (*F_wcrtomb) (char *P32_RESTRICT, wchar_t, mbstate_t *P32_RESTRICT, locale_t);
  size_t (*F_wcsrtombs) (char *P32_RESTRICT, const wchar_t **P32_RESTRICT, size_t, mbstate_t *P32_RESTRICT, locale_t);
  size_t (*F_wcsnrtombs) (
    char *P32_RESTRICT,
    const wchar_t **P32_RESTRICT,
    size_t,
    size_t,
    mbstate_t *P32_RESTRICT,
    locale_t
  );
  /**
   * wchar.h: Operations on Wide Character Strings
   */
  int    (*F_wcscoll) (const wchar_t *, const wchar_t *, locale_t);
  size_t (*F_wcsxfrm) (wchar_t *, const wchar_t *, size_t, locale_t);
  int    (*F_wcscasecmp) (const wchar_t *, const wchar_t *, locale_t);
  int    (*F_wcsncasecmp) (const wchar_t *, const wchar_t *, size_t, locale_t);
  /**
   * uchar.h
   */
  size_t (*F_mbrtoc8) (uint_least8_t *, const char *, size_t, mbstate_t *, Charset *);
  size_t (*F_c8rtomb) (char *, uint_least8_t, mbstate_t *, Charset *);
  size_t (*F_mbrtoc16) (uint_least16_t *, const char *, size_t, mbstate_t *, Charset *);
  size_t (*F_c16rtomb) (char *, uint_least16_t, mbstate_t *, Charset *);
  size_t (*F_mbrtoc32) (uint_least32_t *, const char *, size_t, mbstate_t *, Charset *);
  size_t (*F_c32rtomb) (char *, uint_least32_t, mbstate_t *, Charset *);
} LocaleFunctions;

#endif /* LIBPOSIX32_LOCALE_FUNCTIONS_H_INCLUDED */
