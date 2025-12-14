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

#ifndef LIBPOSIX32_CTYPE_WRAP_INCLUDED
#define LIBPOSIX32_CTYPE_WRAP_INCLUDED

#define _CTYPE_DISABLE_MACROS
#ifdef __has_include_next
#if __has_include_next("ctype.h")
#include_next <ctype.h>
#endif
#else
/* clang-format off */
@include@
/* clang-format on */
#endif
#undef _CTYPE_DISABLE_MACROS

#include "p32_ctype.h"

#ifdef __cplusplus

namespace posix32 {
#ifdef LIBPOSIX32_STATIC
P32_ALWAYS_INLINE int isascii (int _C) {
  return p32_isascii (_C);
}
P32_ALWAYS_INLINE int toascii (int _C) {
  return p32_toascii (_C);
}

#if P32_WANT_POSIX2008
P32_ALWAYS_INLINE int isalnum_l (int _C, locale_t _L) {
  return p32_isalnum_l (_C, _L);
}
P32_ALWAYS_INLINE int isalpha_l (int _C, locale_t _L) {
  return p32_isalpha_l (_C, _L);
}
P32_ALWAYS_INLINE int isblank_l (int _C, locale_t _L) {
  return p32_isblank_l (_C, _L);
}
P32_ALWAYS_INLINE int iscntrl_l (int _C, locale_t _L) {
  return p32_iscntrl_l (_C, _L);
}
P32_ALWAYS_INLINE int isdigit_l (int _C, locale_t _L) {
  return p32_isdigit_l (_C, _L);
}
P32_ALWAYS_INLINE int isgraph_l (int _C, locale_t _L) {
  return p32_isgraph_l (_C, _L);
}
P32_ALWAYS_INLINE int islower_l (int _C, locale_t _L) {
  return p32_islower_l (_C, _L);
}
P32_ALWAYS_INLINE int isprint_l (int _C, locale_t _L) {
  return p32_isprint_l (_C, _L);
}
P32_ALWAYS_INLINE int ispunct_l (int _C, locale_t _L) {
  return p32_ispunct_l (_C, _L);
}
P32_ALWAYS_INLINE int isspace_l (int _C, locale_t _L) {
  return p32_isspace_l (_C, _L);
}
P32_ALWAYS_INLINE int isupper_l (int _C, locale_t _L) {
  return p32_isupper_l (_C, _L);
}
P32_ALWAYS_INLINE int isxdigit_l (int _C, locale_t _L) {
  return p32_isxdigit_l (_C, _L);
}

P32_ALWAYS_INLINE int tolower_l (int _C, locale_t _L) {
  return p32_tolower_l (_C, _L);
}
P32_ALWAYS_INLINE int toupper_l (int _C, locale_t _L) {
  return p32_toupper_l (_C, _L);
}
#endif
#endif /* LIBPOSIX32_STATIC */
}; // namespace posix32

#ifdef LIBPOSIX32_STATIC
using posix32::isascii;
using posix32::toascii;

#if P32_WANT_POSIX2008
using posix32::isalnum_l;
using posix32::isalpha_l;
using posix32::isblank_l;
using posix32::iscntrl_l;
using posix32::isdigit_l;
using posix32::isgraph_l;
using posix32::islower_l;
using posix32::isprint_l;
using posix32::ispunct_l;
using posix32::isspace_l;
using posix32::isupper_l;
using posix32::isxdigit_l;
using posix32::tolower_l;
using posix32::toupper_l;
#endif
#endif /* LIBPOSIX32_STATIC */

#endif /* C++ */

#endif /* LIBPOSIX32_CTYPE_WRAP_INCLUDED */
