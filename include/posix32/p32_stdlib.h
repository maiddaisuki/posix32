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

#ifndef LIBPOSIX32_STDLIB_H_INCLUDED
#define LIBPOSIX32_STDLIB_H_INCLUDED

#include <stdlib.h>

#include "posix32.h"

#ifdef __cplusplus
extern "C" {
#endif

#if P32_REDIRECT
#define mblen    p32_mblen
#define mbtowc   p32_mbtowc
#define wctomb   p32_wctomb
#define mbstowcs p32_mbstowcs
#define wcstombs p32_wcstombs

#if P32_WANT_POSIX2008
#define mblen_l    p32_mblen_l
#define mbtowc_l   p32_mbtowc_l
#define wctomb_l   p32_wctomb_l
#define mbstowcs_l p32_mbstowcs_l
#define wcstombs_l p32_wcstombs_l
#endif
#endif

#if P32_WANT_POSIX2008
typedef struct p32_locale *locale_t;
#endif

#if P32_CRT < P32_UCRT
#ifndef P32_MB_CUR_MAX
P32_CRT_DECL (P32_DECL) int __cdecl p32_mb_cur_max (void);
#define P32_MB_CUR_MAX p32_mb_cur_max ()
#undef MB_CUR_MAX
#define MB_CUR_MAX P32_MB_CUR_MAX
#endif /* !P32_MB_CUR_MAX */
#endif

P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (mblen) (const char *, size_t);
P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (mbtowc) (wchar_t *P32_RESTRICT, const char *P32_RESTRICT, size_t);
P32_CRT_DECL (_CRTIMP) size_t __cdecl P32_FUNCNAME_STD (mbstowcs) (
  wchar_t *P32_RESTRICT,
  const char *P32_RESTRICT,
  size_t
);
P32_CRT_DECL (_CRTIMP) int __cdecl    P32_FUNCNAME_STD (wctomb) (char *, wchar_t);
P32_CRT_DECL (_CRTIMP) size_t __cdecl P32_FUNCNAME_STD (wcstombs) (
  char *P32_RESTRICT,
  const wchar_t *P32_RESTRICT,
  size_t
);

#if P32_WANT_POSIX2008
#ifndef P32_MB_CUR_MAX_L
P32_CRT_DECL (P32_DECL) int __cdecl p32_mb_cur_max_l (locale_t);
#define P32_MB_CUR_MAX_L(locale) p32_mb_cur_max_l (locale)
#undef MB_CUR_MAX_L
#define MB_CUR_MAX_L P32_MB_CUR_MAX_L
#endif /* !P32_MB_CUR_MAX_L */

P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (mblen_l) (const char *, size_t, locale_t);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (mbtowc_l) (
  wchar_t *P32_RESTRICT,
  const char *P32_RESTRICT,
  size_t,
  locale_t
);
P32_CRT_DECL (P32_DECL) size_t __cdecl P32_FUNCNAME (mbstowcs_l) (
  wchar_t *P32_RESTRICT,
  const char *P32_RESTRICT,
  size_t,
  locale_t
);
P32_CRT_DECL (P32_DECL) int __cdecl    P32_FUNCNAME (wctomb_l) (char *, wchar_t, locale_t);
P32_CRT_DECL (P32_DECL) size_t __cdecl P32_FUNCNAME (wcstombs_l) (
  char *P32_RESTRICT,
  const wchar_t *P32_RESTRICT,
  size_t,
  locale_t
);
#endif /* POSIX.1-2008 */

#ifdef __cplusplus
}
#endif

#endif /* LIBPOSIX32_STDLIB_H_INCLUDED */
