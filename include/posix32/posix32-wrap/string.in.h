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

#ifndef LIBPOSIX32_STRING_WRAP_INCLUDED
#define LIBPOSIX32_STRING_WRAP_INCLUDED

#ifdef __has_include_next
#if __has_include_next("string.h")
#include_next <string.h>
#endif
#else
/* clang-format off */
@include@
/* clang-format on */
#endif

#include "p32_string.h"

#ifdef __cplusplus

namespace posix32 {
#ifdef LIBPOSIX32_STATIC
#if P32_WANT_POSIX
P32_ALWAYS_INLINE char *strtok_r (char *_S, const char *_D, char **_C) {
  return p32_strtok_r (_S, _D, _C);
}
#endif

#if P32_WANT_SUSv1 || P32_WANT_POSIX2001
P32_ALWAYS_INLINE char *strndup (const char *_S, size_t _N) {
  return p32_strndup (_S, _N);
}
#endif

#if P32_WANT_POSIX2008
P32_ALWAYS_INLINE int strcoll_l (const char *_S1, const char *_S2, locale_t _L) {
  return p32_strcoll_l (_S1, _S2, _L);
}
P32_ALWAYS_INLINE size_t strxfrm_l (char *_D, const char *_S, size_t _N, locale_t _L) {
  return p32_strxfrm_l (_D, _S, _N, _L);
}
#endif
#endif /* LIBPOSIX32_STATIC */
}; // namespace posix32

#ifdef LIBPOSIX32_STATIC
#if P32_WANT_POSIX
using posix32::strtok_r;
#endif

#if P32_WANT_SUSv1 || P32_WANT_POSIX2001
using posix32::strndup;
#endif

#if P32_WANT_POSIX2008
using posix32::strcoll_l;
using posix32::strxfrm_l;
#endif
#endif /* LIBPOSIX32_STATIC */

#endif /* C++ */

#endif /* LIBPOSIX32_STRING_WRAP_INCLUDED */
