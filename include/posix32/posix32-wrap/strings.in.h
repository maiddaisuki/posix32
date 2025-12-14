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

#ifndef LIBPOSIX32_STRINGS_WRAP_INCLUDED
#define LIBPOSIX32_STRINGS_WRAP_INCLUDED

#ifdef __has_include_next
#if __has_include_next("strings.h")
#include_next <strings.h>
#endif
#else
/* clang-format off */
@include@
/* clang-format on */
#endif

#include "p32_strings.h"

#ifdef __cplusplus

namespace posix32 {
#ifdef LIBPOSIX32_STATIC
#if P32_WANT_POSIX2001
P32_ALWAYS_INLINE int strcasecmp (const char *_S1, const char *_S2) {
  return p32_strcasecmp (_S1, _S2);
}
P32_ALWAYS_INLINE int strncasecmp (const char *_S1, const char *_S2, size_t _N) {
  return p32_strncasecmp (_S1, _S2, _N);
}
#endif

#if P32_WANT_POSIX2008
P32_ALWAYS_INLINE int strcasecmp_l (const char *_S1, const char *_S2, locale_t _L) {
  return p32_strcasecmp_l (_S1, _S2, _L);
}
P32_ALWAYS_INLINE int strncasecmp_l (const char *_S1, const char *_S2, size_t _N, locale_t _L) {
  return p32_strncasecmp_l (_S1, _S2, _N, _L);
}
#endif
#endif /* LIBPOSIX32_STATIC */
}; // namespace posix32

#ifdef LIBPOSIX32_STATIC
#if P32_WANT_POSIX2001
using posix32::strcasecmp;
using posix32::strncasecmp;
#endif

#if P32_WANT_POSIX2008
using posix32::strcasecmp_l;
using posix32::strncasecmp_l;
#endif
#endif /* LIBPOSIX32_STATIC */

#endif /* C++ */

#endif /* LIBPOSIX32_STRINGS_WRAP_INCLUDED */
