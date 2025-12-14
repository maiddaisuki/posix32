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

#ifndef LIBPOSIX32_WCTYPE_H_INCLUDED
#define LIBPOSIX32_WCTYPE_H_INCLUDED

#include <wctype.h>

#include "posix32.h"

#ifdef __cplusplus
extern "C" {
#endif

#undef iswalnum
#undef iswalpha
#undef iswblank
#undef iswcntrl
#undef iswdigit
#undef iswgraph
#undef iswlower
#undef iswprint
#undef iswpunct
#undef iswspace
#undef iswupper
#undef iswxdigit
#undef towlower
#undef towupper

#if P32_REDIRECT
#define wctype   p32_wctype
#define iswctype p32_iswctype

#define iswalnum  p32_iswalnum
#define iswalpha  p32_iswalpha
#define iswblank  p32_iswblank
#define iswcntrl  p32_iswcntrl
#define iswdigit  p32_iswdigit
#define iswgraph  p32_iswgraph
#define iswlower  p32_iswlower
#define iswprint  p32_iswprint
#define iswpunct  p32_iswpunct
#define iswspace  p32_iswspace
#define iswupper  p32_iswupper
#define iswxdigit p32_iswxdigit

#define wctrans   p32_wctrans
#define towctrans p32_towctrans

#define towlower p32_towlower
#define towupper p32_towupper

#if P32_WANT_POSIX2008
#define wctype_l   p32_wctype_l
#define iswctype_l p32_iswctype_l

#define iswalnum_l  p32_iswalnum_l
#define iswalpha_l  p32_iswalpha_l
#define iswblank_l  p32_iswblank_l
#define iswcntrl_l  p32_iswcntrl_l
#define iswdigit_l  p32_iswdigit_l
#define iswgraph_l  p32_iswgraph_l
#define iswlower_l  p32_iswlower_l
#define iswprint_l  p32_iswprint_l
#define iswpunct_l  p32_iswpunct_l
#define iswspace_l  p32_iswspace_l
#define iswupper_l  p32_iswupper_l
#define iswxdigit_l p32_iswxdigit_l

#define wctrans_l   p32_wctrans_l
#define towctrans_l p32_towctrans_l

#define towlower_l p32_towlower_l
#define towupper_l p32_towupper_l
#endif
#endif

#if P32_CRT <= P32_MSVCR110
typedef wchar_t wctype_t;
typedef wchar_t wctrans_t;
#endif

#if P32_WANT_POSIX2008
typedef struct p32_locale *locale_t;
#endif

P32_CRT_DECL (_CRTIMP) wctype_t __cdecl P32_FUNCNAME_STD (wctype) (const char *);
P32_CRT_DECL (_CRTIMP) int __cdecl      P32_FUNCNAME_STD (iswctype) (wint_t, wctype_t);

P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (iswalnum) (wint_t wc);
P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (iswalpha) (wint_t wc);
P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (iswblank) (wint_t wc);
P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (iswcntrl) (wint_t wc);
P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (iswdigit) (wint_t wc);
P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (iswgraph) (wint_t wc);
P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (iswlower) (wint_t wc);
P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (iswprint) (wint_t wc);
P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (iswpunct) (wint_t wc);
P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (iswspace) (wint_t wc);
P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (iswupper) (wint_t wc);
P32_CRT_DECL (_CRTIMP) int __cdecl P32_FUNCNAME_STD (iswxdigit) (wint_t wc);

P32_CRT_DECL (_CRTIMP) wctrans_t __cdecl P32_FUNCNAME_STD (wctrans) (const char *);
P32_CRT_DECL (_CRTIMP) wint_t __cdecl    P32_FUNCNAME_STD (towctrans) (wint_t, wctrans_t);

P32_CRT_DECL (_CRTIMP) wint_t __cdecl P32_FUNCNAME_STD (towlower) (wint_t wc);
P32_CRT_DECL (_CRTIMP) wint_t __cdecl P32_FUNCNAME_STD (towupper) (wint_t wc);

#if P32_WANT_POSIX2008
P32_CRT_DECL (P32_DECL) wctype_t __cdecl P32_FUNCNAME (wctype_l) (const char *, locale_t);
P32_CRT_DECL (P32_DECL) int __cdecl      P32_FUNCNAME (iswctype_l) (wint_t, wctype_t, locale_t);

P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (iswalnum_l) (wint_t wc, locale_t locale);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (iswalpha_l) (wint_t wc, locale_t locale);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (iswblank_l) (wint_t wc, locale_t locale);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (iswcntrl_l) (wint_t wc, locale_t locale);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (iswdigit_l) (wint_t wc, locale_t locale);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (iswgraph_l) (wint_t wc, locale_t locale);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (iswlower_l) (wint_t wc, locale_t locale);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (iswprint_l) (wint_t wc, locale_t locale);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (iswpunct_l) (wint_t wc, locale_t locale);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (iswspace_l) (wint_t wc, locale_t locale);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (iswupper_l) (wint_t wc, locale_t locale);
P32_CRT_DECL (P32_DECL) int __cdecl P32_FUNCNAME (iswxdigit_l) (wint_t wc, locale_t locale);

P32_CRT_DECL (P32_DECL) wctrans_t __cdecl P32_FUNCNAME (wctrans_l) (const char *, locale_t);
P32_CRT_DECL (P32_DECL) wint_t __cdecl    P32_FUNCNAME (towctrans_l) (wint_t, wctrans_t, locale_t);

P32_CRT_DECL (P32_DECL) wint_t __cdecl P32_FUNCNAME (towlower_l) (wint_t wc, locale_t locale);
P32_CRT_DECL (P32_DECL) wint_t __cdecl P32_FUNCNAME (towupper_l) (wint_t wc, locale_t locale);
#endif /* POSIX.1-2008 */

#ifdef __cplusplus
}
#endif

#endif /* LIBPOSIX32_WCTYPE_H_INCLUDED */
