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

#ifndef LIBPOSIX32_CORE_HEAP_H_INCLUDED
#define LIBPOSIX32_CORE_HEAP_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

#include "posix32.h"

/**
 * Create low fragmentation heap.
 */
#define P32_HEAP_CREATE_LFH (1)

/**
 * Create heap with terminate on corruption feature enabled.
 */
#define P32_HEAP_CREATE_TERMINATE_ON_CORRUPTION (1 << 1)

/**
 * Wrapper for `HeapCreate`.
 *
 * The first argument may contain any valid combination of `P32_HEAP_CREATE_*`
 * flags.
 *
 * The remaining arguments are equivalent to those of `HeapCreate`.
 */
P32_CORE_DECL uintptr_t p32_heap_create (uint32_t createFlags, uint32_t flags, size_t initialSize, size_t maxSize);

/**
 * Wrapper for `HeapDestroy`.
 *
 * In the test version of the library, it print summary for `heap` before
 * destroying it.
 */
P32_CORE_DECL bool p32_heap_destroy (uintptr_t heap);

/**
 * Wrapper for `HeapAlloc`.
 *
 * In the test version of the library, `heap` can be zero;
 * in this case the process heap (`GetProcessHeap`) is used.
 */
P32_CORE_DECL void *p32_heap_alloc (uintptr_t heap, uint32_t flags, size_t size);

/**
 * Wrapper for `HeapReAlloc`.
 *
 * In the test version of the library, `heap` can be zero;
 * in this case the process heap (`GetProcessHeap`) is used.
 *
 * If `memory` is `NULL` and `flags` does not contain
 * `HEAP_REALLOC_IN_PLACE_ONLY`, this function calls `HeapAlloc` to allocate
 * `size` bytes of memory from `heap`.
 */
P32_CORE_DECL void *p32_heap_realloc (uintptr_t heap, uint32_t flags, void *memory, size_t size);

/**
 * Wrapper for `HeapSize`.
 *
 * In the test version of the library, `heap` can be zero;
 * in this case the process heap (`GetProcessHeap`) is used.
 */
P32_CORE_DECL size_t p32_heap_size (uintptr_t heap, uint32_t flags, void *memory);

/**
 * Wrapper for `HeapFree`.
 *
 * In the test version of the library, `heap` can be zero;
 * in this case the process heap (`GetProcessHeap`) is used.
 */
P32_CORE_DECL bool p32_heap_free (uintptr_t heap, uint32_t flags, void *memory);

/**
 * A version of `strdup` which allocates memory from `heap`.
 *
 * On success, stores copied string in `*address` and returns length of
 * the copied string.
 *
 * On failure, returns `-1`.
 */
P32_CORE_INLINE_DECL int p32_heap_strdup (char **address, uintptr_t heap, const char *str) {
  size_t bufferSize = strlen (str);

  bufferSize += 1;
  *address    = p32_heap_alloc (heap, 0, bufferSize * sizeof (char));

  if (*address == NULL) {
    return -1;
  }

  memcpy (*address, str, bufferSize);
  return (int) (bufferSize - 1);
}

/**
 * A version of `wcsdup` which allocates memory from `heap`.
 *
 * On success, stores copied string in `*address` and returns length of
 * the copied string.
 *
 * On failure, returns `-1`.
 */
P32_CORE_INLINE_DECL int p32_heap_wcsdup (wchar_t **address, uintptr_t heap, const wchar_t *wcs) {
  size_t bufferSize = wcslen (wcs);

  bufferSize += 1;
  *address    = p32_heap_alloc (heap, 0, bufferSize * sizeof (wchar_t));

  if (*address == NULL) {
    return -1;
  }

  wmemcpy (*address, wcs, bufferSize);
  return (int) (bufferSize - 1);
}

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

#endif /* LIBPOSIX32_CORE_HEAP_H_INCLUDED */
