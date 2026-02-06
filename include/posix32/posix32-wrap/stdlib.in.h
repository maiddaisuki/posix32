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

#ifndef LIBPOSIX32_STDLIB_WRAP_INCLUDED
#define LIBPOSIX32_STDLIB_WRAP_INCLUDED

#ifdef __has_include_next
#if __has_include_next("stdlib.h")
#include_next <stdlib.h>
#endif
#else
/* clang-format off */
@include@
/* clang-format on */
#endif

#include "p32_stdlib.h"

#ifdef __cplusplus

namespace posix32 {
#ifdef LIBPOSIX32_STATIC
#if P32_WANT_POSIX2008
P32_ALWAYS_INLINE int mblen_l (const char *_S, size_t _N, locale_t _L) {
  return p32_mblen_l (_S, _N, _L);
}
P32_ALWAYS_INLINE int mbtowc_l (wchar_t *_W, const char *_S, size_t _N, locale_t _L) {
  return p32_mbtowc_l (_W, _S, _N, _L);
}
P32_ALWAYS_INLINE size_t mbstowcs_l (wchar_t *_W, const char *_S, size_t _N, locale_t _L) {
  return p32_mbstowcs_l (_W, _S, _N, _L);
}
P32_ALWAYS_INLINE int wctomb_l (char *_S, wchar_t _W, locale_t _L) {
  return p32_wctomb_l (_S, _W, _L);
}
P32_ALWAYS_INLINE size_t wcstombs_l (char *_S, const wchar_t *_W, size_t _N, locale_t _L) {
  return p32_wcstombs_l (_S, _W, _N, _L);
}
#endif
#endif /* LIBPOSIX32_STATIC */
}; // namespace posix32

#ifdef LIBPOSIX32_STATIC
#if P32_WANT_POSIX2008
using posix32::mblen_l;
using posix32::mbstowcs_l;
using posix32::mbtowc_l;
using posix32::wcstombs_l;
using posix32::wctomb_l;
#endif
#endif /* LIBPOSIX32_STATIC */

#endif /* C++ */

#endif /* LIBPOSIX32_STDLIB_WRAP_INCLUDED */
