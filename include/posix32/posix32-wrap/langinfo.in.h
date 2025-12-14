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

#ifndef LIBPOSIX32_LANGINFO_WRAP_INCLUDED
#define LIBPOSIX32_LANGINFO_WRAP_INCLUDED

#ifdef __has_include_next
#if __has_include_next("langinfo.h")
#include_next <langinfo.h>
#endif
#else
/* clang-format off */
@include@
/* clang-format on */
#endif

#include "p32_langinfo.h"

#ifdef __cplusplus

namespace posix32 {
#ifdef LIBPOSIX32_STATIC
P32_ALWAYS_INLINE char *nl_langinfo (nl_item _N) {
  return p32_nl_langinfo (_N);
}

#if P32_WANT_POSIX2008
P32_ALWAYS_INLINE char *nl_langinfo_l (nl_item _N, locale_t _L) {
  return p32_nl_langinfo_l (_N, _L);
}
#endif
#endif /* LIBPOSIX32_STATIC */
}; // namespace posix32

#ifdef LIBPOSIX32_STATIC
using posix32::nl_langinfo;

#if P32_WANT_POSIX2008
using posix32::nl_langinfo_l;
#endif
#endif /* LIBPOSIX32_STATIC */

#endif /* C++ */

#endif /* LIBPOSIX32_LANGINFO_WRAP_INCLUDED */
