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

#ifndef LIBPOSIX32_CORE_LOADER_H_INCLUDED
#define LIBPOSIX32_CORE_LOADER_H_INCLUDED

#include <stdint.h>
#include <wchar.h>

#include "posix32.h"

/* clang-format off */
#define P32_LOADER_COMPOUND_LITERAL(type, initializer) (type) {initializer}
/* clang-format on */

#define P32_LOADER_PASTE(str)     #str
#define P32_LOADER_CONCAT(s1, s2) s1##s2

/**
 * Convenience wrapper for `GetProcAddress`.
 */
#define p32_get_proc_address(module, func)                                                                   \
  (P32_LOADER_CONCAT (Func, func)) (uintptr_t) GetProcAddress ((HMODULE) module, P32_LOADER_PASTE (func));   \
  P32_STATIC_ASSERT (sizeof ((module)) == sizeof (void *), "module must be an object with sizeof (void *)"); \
  (void) P32_LOADER_COMPOUND_LITERAL (P32_LOADER_CONCAT (Func, func), func)

#if P32_WIN9X
#define P32_MODULE_NAME(module) module
typedef const char *ModuleName;
#else /* !P32_WIN9X */
#define P32_MODULE_NAME(module) TEXT (module)
typedef const wchar_t *ModuleName;
#endif /* !P32_WIN9X */

/**
 * Portability wrapper for `LoadLibrary`.
 */
P32_CORE_DECL uintptr_t p32_load_library (ModuleName module);

/**
 * Portability wrapper for `GetModuleHandle`.
 */
P32_CORE_DECL uintptr_t p32_get_module_handle (ModuleName module);

/**
 * Portability wrapper for `FreeLibrary`.
 */
P32_CORE_DECL void p32_free_library (uintptr_t handle);

#endif /* LIBPOSIX32_CORE_LOADER_H_INCLUDED */
