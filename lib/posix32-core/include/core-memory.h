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

#ifndef LIBPOSIX32_CORE_MEMORY_H_INCLUDED
#define LIBPOSIX32_CORE_MEMORY_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>

#include "posix32.h"

/**
 * Portability wrapper for `HeapLock`.
 */
P32_CORE_DECL bool p32_heap_lock (uintptr_t heap);

/**
 * Portability wrapper for `HeapUnlock`.
 */
P32_CORE_DECL bool p32_heap_unlock (uintptr_t heap);

/**
 * Portability wrapper for `HeapSummary`.
 *
 * The `data` argument must point to properly initialized `HEAP_SUMMARY`
 * structure; `cb` field must be set to `sizeof (HEAP_SUMMARY)`.
 */
P32_CORE_DECL bool p32_heap_summary (uintptr_t heap, uint32_t flags, void *data);

/**
 * Portability wrapper for `HeapSetInformation`.
 */
P32_CORE_DECL bool p32_heap_set_information (uintptr_t heap, uint32_t infoClass, void *data, size_t dataSize);

/**
 * Enable Low Fragmentation Heap on `heap`.
 */
P32_CORE_DECL bool p32_heap_low_fragmentation (uintptr_t heap);

/**
 * Enable termination if heap corruption occured on `heap`.
 */
P32_CORE_DECL bool p32_heap_terminate_on_corruption (uintptr_t heap);

#ifdef LIBPOSIX32_TEST
/**
 * Print heap summary for `heap`.
 */
P32_CORE_DECL void p32_heap_print_summary (uintptr_t heap);
#endif

#endif /* LIBPOSIX32_CORE_MEMORY_H_INCLUDED */
