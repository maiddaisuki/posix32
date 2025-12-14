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

#ifndef LIBPOSIX32_LOCALE_WRAP_INCLUDED
#define LIBPOSIX32_LOCALE_WRAP_INCLUDED

#ifdef __has_include_next
#if __has_include_next("locale.h")
#include_next <locale.h>
#endif
#else
/* clang-format off */
@include@
/* clang-format on */
#endif

#include "p32_locale.h"

#ifdef __cplusplus

namespace posix32 {
#ifdef LIBPOSIX32_STATIC
#if P32_WANT_POSIX2008
P32_ALWAYS_INLINE locale_t newlocale (int _C, const char *_S, locale_t _L) {
  return p32_newlocale (_C, _S, _L);
}
P32_ALWAYS_INLINE void freelocale (locale_t _L) {
  p32_freelocale (_L);
}
P32_ALWAYS_INLINE locale_t duplocale (locale_t _L) {
  return p32_duplocale (_L);
}
P32_ALWAYS_INLINE locale_t uselocale (locale_t _L) {
  return p32_uselocale (_L);
}
#endif

#if P32_WANT_POSIX2024
P32_ALWAYS_INLINE const char *getlocalename_l (int _C, locale_t _L) {
  return p32_getlocalename_l (_C, _L);
}
#endif
#endif /* LIBPOSIX32_STATIC */
}; // namespace posix32

#ifdef LIBPOSIX32_STATIC
#if P32_WANT_POSIX2008
using posix32::duplocale;
using posix32::freelocale;
using posix32::newlocale;
using posix32::uselocale;
#endif

#if P32_WANT_POSIX2024
using posix32::getlocalename_l;
#endif
#endif /* LIBPOSIX32_STATIC */

#endif /* C++ */

#endif /* LIBPOSIX32_LOCALE_WRAP_INCLUDED */
