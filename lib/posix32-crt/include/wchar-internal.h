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

#ifndef LIBPOSIX32_WCHAR_INTERNAL_H_INCLUDED
#define LIBPOSIX32_WCHAR_INTERNAL_H_INCLUDED

#include "p32_stdlib.h"
#include "p32_wchar.h"

size_t p32_private_mbrlen_l (const char *P32_RESTRICT, size_t, mbstate_t *P32_RESTRICT, locale_t);
size_t p32_private_mbrtowc_l (
  wchar_t *P32_RESTRICT,
  const char *P32_RESTRICT,
  size_t,
  mbstate_t *P32_RESTRICT,
  locale_t
);
size_t p32_private_mbsrtowcs_l (
  wchar_t *P32_RESTRICT,
  const char **P32_RESTRICT,
  size_t,
  mbstate_t *P32_RESTRICT,
  locale_t
);
size_t p32_private_mbsnrtowcs_l (
  wchar_t *P32_RESTRICT,
  const char **P32_RESTRICT,
  size_t,
  size_t,
  mbstate_t *P32_RESTRICT,
  locale_t
);
size_t p32_private_wcrtomb_l (char *P32_RESTRICT, wchar_t, mbstate_t *P32_RESTRICT, locale_t);
size_t p32_private_wcsrtombs_l (
  char *P32_RESTRICT,
  const wchar_t **P32_RESTRICT,
  size_t,
  mbstate_t *P32_RESTRICT,
  locale_t
);
size_t p32_private_wcsnrtombs_l (
  char *P32_RESTRICT,
  const wchar_t **P32_RESTRICT,
  size_t,
  size_t,
  mbstate_t *P32_RESTRICT,
  locale_t
);

/**
 * Store locale-specific versions of wide character string functions declared
 * in wchar.h in `locale->Functions`.
 */
void p32_wchar_functions (locale_t locale);

/**
 * Store locale-specific versions of C89 conversion functions declared in
 * stdlib.h in `locale->Functions`.
 */
void p32_wchar_c89_functions (locale_t locale);

/**
 * Store locale-specific versions of C95 conversion functions declared in
 * wchar.h in `locale->Functions`.
 */
void p32_wchar_c95_functions (locale_t locale);

#endif /* LIBPOSIX32_WCHAR_INTERNAL_H_INCLUDED */
