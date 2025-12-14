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

#ifndef LIBPOSIX32_WCHAR_H_INCLUDED
#define LIBPOSIX32_WCHAR_H_INCLUDED

#include <wchar.h>

#include "posix32.h"

#ifdef __cplusplus
extern "C" {
#endif

#if P32_REDIRECT
#define wcsxfrm p32_wcsxfrm
#define wcscoll p32_wcscoll

#define btowc p32_btowc
#define wctob p32_wctob

#define mbsinit   p32_mbsinit
#define mbrlen    p32_mbrlen
#define mbrtowc   p32_mbrtowc
#define mbsrtowcs p32_mbsrtowcs
#define wcrtomb   p32_wcrtomb
#define wcsrtombs p32_wcsrtombs

#if P32_WANT_POSIX2008
#define wcsdup  p32_wcsdup
#define wcsndup p32_wcsndup

#define wcscasecmp    p32_wcscasecmp
#define wcscasecmp_l  p32_wcscasecmp_l
#define wcsncasecmp   p32_wcsncasecmp
#define wcsncasecmp_l p32_wcsncasecmp_l

#define wcscoll_l p32_wcscoll_l
#define wcsxfrm_l p32_wcsxfrm_l

#define btowc_l p32_btowc_l
#define wctob_l p32_wctob_l

#define mbrlen_l    p32_mbrlen_l
#define mbrtowc_l   p32_mbrtowc_l
#define mbsrtowcs_l p32_mbsrtowcs_l
#define wcrtomb_l   p32_wcrtomb_l
#define wcsrtombs_l p32_wcsrtombs_l
#endif

#if P32_WANT_SUSv4 || P32_WANT_POSIX2024
#define mbsnrtowcs   p32_mbsnrtowcs
#define mbsnrtowcs_l p32_mbsnrtowcs_l
#define wcsnrtombs   p32_wcsnrtombs
#define wcsnrtombs_l p32_wcsnrtombs_l
#endif
#endif

#if P32_WANT_POSIX2008
typedef struct p32_locale *locale_t;
#endif

P32_CRT_DECL (_CRTIMP) size_t __cdecl P32_FUNCNAME_STD (wcsxfrm) (wchar_t *, const wchar_t *, size_t);
P32_CRT_DECL (_CRTIMP) int __cdecl    P32_FUNCNAME_STD (wcscoll) (const wchar_t *, const wchar_t *);

P32_CRT_DECL (_CRTIMP) wint_t __cdecl P32_FUNCNAME_STD (btowc) (int);
P32_CRT_DECL (_CRTIMP) int __cdecl    P32_FUNCNAME_STD (wctob) (wint_t);

#if P32_CRT < P32_MSVCR80
typedef int mbstate_t;
#endif

P32_INLINE_DECL int __cdecl p32_mbsinit (const mbstate_t *state) {
#if P32_CRT >= P32_UCRT
  return state == NULL || (state->_Wchar == 0 && state->_Byte == 0 && state->_State == 0);
#else
  return state == NULL || *state == 0;
#endif
}
P32_CRT_DECL (_CRTIMP) size_t __cdecl P32_FUNCNAME_STD (mbrlen) (
  const char *P32_RESTRICT,
  size_t,
  mbstate_t *P32_RESTRICT
);
P32_CRT_DECL (_CRTIMP) size_t __cdecl P32_FUNCNAME_STD (mbrtowc) (
  wchar_t *P32_RESTRICT,
  const char *P32_RESTRICT,
  size_t,
  mbstate_t *P32_RESTRICT
);
P32_CRT_DECL (_CRTIMP) size_t __cdecl P32_FUNCNAME_STD (mbsrtowcs) (
  wchar_t *P32_RESTRICT,
  const char **P32_RESTRICT,
  size_t,
  mbstate_t *P32_RESTRICT
);
P32_CRT_DECL (_CRTIMP) size_t __cdecl P32_FUNCNAME_STD (wcrtomb) (char *P32_RESTRICT, wchar_t, mbstate_t *P32_RESTRICT);
P32_CRT_DECL (_CRTIMP) size_t __cdecl P32_FUNCNAME_STD (wcsrtombs) (
  char *P32_RESTRICT,
  const wchar_t **P32_RESTRICT,
  size_t,
  mbstate_t *P32_RESTRICT
);

#if P32_WANT_POSIX2008
P32_CRT_DECL (_CRTIMP) wchar_t *__cdecl  P32_FUNCNAME_STD (wcsdup) (const wchar_t *);
P32_CRT_DECL (P32_DECL) wchar_t *__cdecl P32_FUNCNAME (wcsndup) (const wchar_t *, size_t);

P32_CRT_DECL (P32_DECL) int __cdecl    P32_FUNCNAME (wcscoll_l) (const wchar_t *, const wchar_t *, locale_t);
P32_CRT_DECL (P32_DECL) size_t __cdecl P32_FUNCNAME (wcsxfrm_l) (wchar_t *, const wchar_t *, size_t, locale_t);

P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (wcscasecmp) (const wchar_t *, const wchar_t *);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (wcscasecmp_l) (const wchar_t *, const wchar_t *, locale_t);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (wcsncasecmp) (const wchar_t *, const wchar_t *, size_t);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (wcsncasecmp_l) (const wchar_t *, const wchar_t *, size_t, locale_t);

P32_CRT_DECL (P32_DECL) wint_t __cdecl P32_FUNCNAME (btowc_l) (int, locale_t);
P32_CRT_DECL (P32_DECL) int __cdecl    P32_FUNCNAME (wctob_l) (wint_t, locale_t);

P32_CRT_DECL (P32_DECL) size_t __cdecl P32_FUNCNAME (mbrlen_l) (
  const char *P32_RESTRICT,
  size_t,
  mbstate_t *P32_RESTRICT,
  locale_t
);
P32_CRT_DECL (P32_DECL) size_t __cdecl P32_FUNCNAME (mbrtowc_l) (
  wchar_t *P32_RESTRICT,
  const char *P32_RESTRICT,
  size_t,
  mbstate_t *P32_RESTRICT,
  locale_t
);
P32_CRT_DECL (P32_DECL) size_t __cdecl P32_FUNCNAME (mbsrtowcs_l) (
  wchar_t *P32_RESTRICT,
  const char **P32_RESTRICT,
  size_t,
  mbstate_t *P32_RESTRICT,
  locale_t
);
P32_CRT_DECL (P32_DECL) size_t __cdecl P32_FUNCNAME (wcrtomb_l) (
  char *P32_RESTRICT,
  wchar_t,
  mbstate_t *P32_RESTRICT,
  locale_t
);
P32_CRT_DECL (P32_DECL) size_t __cdecl P32_FUNCNAME (wcsrtombs_l) (
  char *P32_RESTRICT,
  const wchar_t **P32_RESTRICT,
  size_t,
  mbstate_t *P32_RESTRICT,
  locale_t
);
#endif /* POSIX.1-2008 */

#if P32_WANT_SUSv4 || P32_WANT_POSIX2024
P32_CRT_DECL (P32_DECL) size_t __cdecl P32_FUNCNAME (mbsnrtowcs) (
  wchar_t *P32_RESTRICT,
  const char **P32_RESTRICT,
  size_t,
  size_t,
  mbstate_t *P32_RESTRICT
);
P32_CRT_DECL (P32_DECL) size_t __cdecl P32_FUNCNAME (wcsnrtombs) (
  char *P32_RESTRICT,
  const wchar_t **P32_RESTRICT,
  size_t,
  size_t,
  mbstate_t *P32_RESTRICT
);

P32_CRT_DECL (P32_DECL) size_t __cdecl P32_FUNCNAME (mbsnrtowcs_l) (
  wchar_t *P32_RESTRICT,
  const char **P32_RESTRICT,
  size_t,
  size_t,
  mbstate_t *P32_RESTRICT,
  locale_t
);
P32_CRT_DECL (P32_DECL) size_t __cdecl P32_FUNCNAME (wcsnrtombs_l) (
  char *P32_RESTRICT,
  const wchar_t **P32_RESTRICT,
  size_t,
  size_t,
  mbstate_t *P32_RESTRICT,
  locale_t
);
#endif /* SUSv4 || POSIX.1-2024 */

#ifdef __cplusplus
}
#endif

#endif /* LIBPOSIX32_WCHAR_H_INCLUDED */
