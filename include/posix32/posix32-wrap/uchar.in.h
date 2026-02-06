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

#ifndef LIBPOSIX32_UCHAR_WRAP_INCLUDED
#define LIBPOSIX32_UCHAR_WRAP_INCLUDED

#ifdef __has_include_next
#if __has_include_next("uchar.h")
#include_next <uchar.h>
#endif
#else
/* clang-format off */
@include@
/* clang-format on */
#endif

#include "p32_uchar.h"

#ifdef __cplusplus

namespace posix32 {
#ifdef LIBPOSIX32_STATIC
P32_ALWAYS_INLINE size_t c8rtomb (char *_S, char8_t _C, mbstate_t *_M) {
  return p32_c8rtomb (_S, _C, _M);
}
P32_ALWAYS_INLINE size_t mbrtoc8 (char8_t *_C, const char *_S, size_t _N, mbstate_t *_M) {
  return p32_mbrtoc8 (_C, _S, _N, _M);
}

#if P32_WANT_POSIX2008
P32_ALWAYS_INLINE size_t c8rtomb_l (char *_S, char8_t _C, mbstate_t *_M, locale_t _L) {
  return p32_c8rtomb_l (_S, _C, _M, _L);
}
P32_ALWAYS_INLINE size_t mbrtoc8_l (char8_t *_C, const char *_S, size_t _N, mbstate_t *_M, locale_t _L) {
  return p32_mbrtoc8_l (_C, _S, _N, _M, _L);
}
P32_ALWAYS_INLINE size_t c16rtomb_l (char *_S, char16_t _C, mbstate_t *_M, locale_t _L) {
  return p32_c16rtomb_l (_S, _C, _M, _L);
}
P32_ALWAYS_INLINE size_t mbrtoc16_l (char16_t *_C, const char *_S, size_t _N, mbstate_t *_M, locale_t _L) {
  return p32_mbrtoc16_l (_C, _S, _N, _M, _L);
}
P32_ALWAYS_INLINE size_t c32rtomb_l (char *_S, char32_t _C, mbstate_t *_M, locale_t _L) {
  return p32_c32rtomb_l (_S, _C, _M, _L);
}
P32_ALWAYS_INLINE size_t mbrtoc32_l (char32_t *_C, const char *_S, size_t _N, mbstate_t *_M, locale_t _L) {
  return p32_mbrtoc32_l (_C, _S, _N, _M, _L);
}
#endif
#endif /* LIBPOSIX32_STATIC */
}; // namespace posix32

#ifdef LIBPOSIX32_STATIC
using posix32::c8rtomb;
using posix32::mbrtoc8;

#if P32_WANT_POSIX2008
using posix32::c16rtomb_l;
using posix32::c32rtomb_l;
using posix32::c8rtomb_l;
using posix32::mbrtoc16_l;
using posix32::mbrtoc32_l;
using posix32::mbrtoc8_l;
#endif
#endif /* LIBPOSIX32_STATIC */

#endif /* C++ */

#endif /* LIBPOSIX32_UCHAR_WRAP_INCLUDED */
