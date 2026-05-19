/**
 * Copyright 2026 Kirill Makurin
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

#ifndef LIBPOSIX32_CORE_ATOMIC_H_INCLUDED
#define LIBPOSIX32_CORE_ATOMIC_H_INCLUDED

#include <stdint.h>

#include "posix32.h"

/* clang-format off */
#define P32_ATOMIC_COMPOUND_LITERAL(type, initializer) (type) {initializer}
/* clang-format on */

#define p32_atomic_exchange_8(t, s)                                                                         \
  InterlockedExchange8 ((char *) t, (char) s);                                                              \
  P32_STATIC_ASSERT (sizeof (*(t)) == sizeof (char), "t must be a ponter to an object with sizeof (char)"); \
  P32_STATIC_ASSERT (sizeof ((s)) == sizeof (char), "s must be an object with sizeof (char)")

#define p32_atomic_exchange_16(t, s)                                                                          \
  InterlockedExchange16 ((short *) t, (short) s);                                                             \
  P32_STATIC_ASSERT (sizeof (*(t)) == sizeof (short), "t must be a ponter to an object with sizeof (short)"); \
  P32_STATIC_ASSERT (sizeof ((s)) == sizeof (short), "s must be an object with sizeof (short)")

#define p32_atomic_exchange_32(t, s)                                                                        \
  InterlockedExchange ((long *) t, (long) s);                                                               \
  P32_STATIC_ASSERT (sizeof (*(t)) == sizeof (long), "t must be a ponter to an object with sizeof (long)"); \
  P32_STATIC_ASSERT (sizeof ((s)) == sizeof (long), "s must be an object with sizeof (long)")

#define p32_atomic_exchange_64(t, s)                                                                                \
  InterlockedExchange64 ((__int64 *) t, (__int64) s);                                                               \
  P32_STATIC_ASSERT (sizeof (*(t)) == sizeof (__int64), "t must be a ponter to an object with sizeof (long long)"); \
  P32_STATIC_ASSERT (sizeof ((s)) == sizeof (__int64), "s must be an object with sizeof (long long)")

#define p32_atomic_exchange_pointer(t, s)                                                                       \
  InterlockedExchangePointer ((void *volatile *) t, (void *) s);                                                \
  P32_STATIC_ASSERT (sizeof (*(t)) == sizeof (void *), "t must be a ponter to an object with sizeof (void *)"); \
  P32_STATIC_ASSERT (sizeof ((s)) == sizeof (void *), "s must be an object with sizeof (void *)")

#define p32_atomic_exchange_fpointer(t, s)                                                                      \
  InterlockedExchangePointer ((void *volatile *) t, (void *) (uintptr_t) s);                                    \
  P32_STATIC_ASSERT (sizeof (*(t)) == sizeof (void *), "t must be a ponter to an object with sizeof (void *)"); \
  (void) P32_ATOMIC_COMPOUND_LITERAL (__typeof__ (*(t)), s)

#endif /* LIBPOSIX32_CORE_ATOMIC_H_INCLUDED */
