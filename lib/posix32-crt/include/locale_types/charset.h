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

#ifndef LIBPOSIX32_CHARSET_H_INCLUDED
#define LIBPOSIX32_CHARSET_H_INCLUDED

#include "core-charset.h"

/**
 * Perform conversion according to `request`.
 *
 * The `request` argument must point to a properly initialized
 * `CharsetConversionRequest` structure; one and only one of
 * `P32_CHARSET_CONVERSION_MB_TO_WC` and `P32_CHARSET_CONVERSION_WC_TO_MB`
 * must be set in `request->Flags`.
 *
 * On success, returns length, excluding terminating NUL, of the converted
 * string the address of which is stored in variable pointed by
 * `request->Output`.
 *
 * On failure, returns `-1` and sets `request->Status` to provide additional
 * information about the cause.
 */
P32_TEST_DECL int p32_charset_convert (CharsetConversionRequest *request, uintptr_t heap);

#endif /* LIBPOSIX32_CHARSET_H_INCLUDED */
