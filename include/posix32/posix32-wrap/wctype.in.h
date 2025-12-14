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

#ifndef LIBPOSIX32_WCTYPE_WRAP_INCLUDED
#define LIBPOSIX32_WCTYPE_WRAP_INCLUDED

#define _CTYPE_DISABLE_MACROS
#ifdef __has_include_next
#if __has_include_next("wctype.h")
#include_next <wctype.h>
#endif
#else
/* clang-format off */
@include@
/* clang-format on */
#endif
#undef _CTYPE_DISABLE_MACROS

#include "p32_wctype.h"

#ifdef __cplusplus

namespace posix32 {
#ifdef LIBPOSIX32_STATIC
#if P32_WANT_POSIX2008
P32_ALWAYS_INLINE wctype_t wctype_l (const char *_S, locale_t _L) {
  return p32_wctype_l (_S, _L);
}
P32_ALWAYS_INLINE int iswctype_l (wint_t _W, wctype_t _C, locale_t _L) {
  return p32_iswctype_l (_W, _C, _L);
}

P32_ALWAYS_INLINE int iswalnum_l (wint_t _C, locale_t _L) {
  return p32_iswalnum_l (_C, _L);
}
P32_ALWAYS_INLINE int iswalpha_l (wint_t _C, locale_t _L) {
  return p32_iswalpha_l (_C, _L);
}
P32_ALWAYS_INLINE int iswblank_l (wint_t _C, locale_t _L) {
  return p32_iswblank_l (_C, _L);
}
P32_ALWAYS_INLINE int iswcntrl_l (wint_t _C, locale_t _L) {
  return p32_iswcntrl_l (_C, _L);
}
P32_ALWAYS_INLINE int iswdigit_l (wint_t _C, locale_t _L) {
  return p32_iswdigit_l (_C, _L);
}
P32_ALWAYS_INLINE int iswgraph_l (wint_t _C, locale_t _L) {
  return p32_iswgraph_l (_C, _L);
}
P32_ALWAYS_INLINE int iswlower_l (wint_t _C, locale_t _L) {
  return p32_iswlower_l (_C, _L);
}
P32_ALWAYS_INLINE int iswprint_l (wint_t _C, locale_t _L) {
  return p32_iswprint_l (_C, _L);
}
P32_ALWAYS_INLINE int iswpunct_l (wint_t _C, locale_t _L) {
  return p32_iswpunct_l (_C, _L);
}
P32_ALWAYS_INLINE int iswspace_l (wint_t _C, locale_t _L) {
  return p32_iswspace_l (_C, _L);
}
P32_ALWAYS_INLINE int iswupper_l (wint_t _C, locale_t _L) {
  return p32_iswupper_l (_C, _L);
}
P32_ALWAYS_INLINE int iswxdigit_l (wint_t _C, locale_t _L) {
  return p32_iswxdigit_l (_C, _L);
}

P32_ALWAYS_INLINE wctrans_t wctrans_l (const char *_S, locale_t _L) {
  return p32_wctype_l (_S, _L);
}
P32_ALWAYS_INLINE wint_t towctrans_l (wint_t _W, wctrans_t _C, locale_t _L) {
  return p32_towctrans_l (_W, _C, _L);
}

P32_ALWAYS_INLINE wint_t towlower_l (wint_t _C, locale_t _L) {
  return p32_towlower_l (_C, _L);
}
P32_ALWAYS_INLINE wint_t towupper_l (wint_t _C, locale_t _L) {
  return p32_towupper_l (_C, _L);
}
#endif
#endif /* LIBPOSIX32_STATIC */
}; // namespace posix32

#ifdef LIBPOSIX32_STATIC
#if P32_WANT_POSIX2008
using posix32::iswalnum_l;
using posix32::iswalpha_l;
using posix32::iswblank_l;
using posix32::iswcntrl_l;
using posix32::iswctype_l;
using posix32::iswdigit_l;
using posix32::iswgraph_l;
using posix32::iswlower_l;
using posix32::iswprint_l;
using posix32::iswpunct_l;
using posix32::iswspace_l;
using posix32::iswupper_l;
using posix32::iswxdigit_l;
using posix32::towctrans_l;
using posix32::towlower_l;
using posix32::towupper_l;
using posix32::wctrans_l;
using posix32::wctype_l;
#endif
#endif /* LIBPOSIX32_STATIC */

#endif /* C++ */

#endif /* LIBPOSIX32_WCTYPE_WRAP_INCLUDED */
