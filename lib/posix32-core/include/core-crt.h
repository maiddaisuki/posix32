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

#ifndef LIBPOSIX32_CORE_CRT_H_INCLUDED
#define LIBPOSIX32_CORE_CRT_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>

#include "posix32.h"

/**
 * Get CRT handle; this handle can be passed to `GetProcAddress`.
 *
 * This function returns `0` if library was built against static CRT.
 */
P32_CORE_DECL uintptr_t p32_crt_handle (void);

#endif /* LIBPOSIX32_CORE_CRT_H_INCLUDED */
