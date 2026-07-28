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

#include "string-internal.h"

static char *p32_strncpy_posix (char *dest, const char *src, size_t count, locale_t locale) {
  return strncpy (dest, src, count);
  UNREFERENCED_PARAMETER (locale);
}

static char *p32_strncpy_common (char *dest, const char *src, size_t count, locale_t locale) {
  size_t length = p32_private_strnlen_l (src, count, locale);

  if (length == (size_t) -1) {
    memset (dest, 0x00, count);
    return NULL;
  }

  /**
   * Copy `length` bytes from `src` to `dest`.
   */
  memcpy (dest, src, length);

  /**
   * Fill the rest of `dest` with NUL bytes.
   * The copied string is NUL-terminated only when `length < count`.
   */
  memset (dest + length, 0x00, count - length);

  return dest;
}

static void P32LocaleFunction_strncpy (LocaleFunctions *functions, Charset *charset) {
  if (P32_IS_SBCS (charset)) {
    functions->F_strncpy = p32_strncpy_posix;
  } else {
    functions->F_strncpy = p32_strncpy_common;
  }
}

char *p32_private_strncpy_l (char *dest, const char *src, size_t count, locale_t locale) {
  return locale->Functions.F_strncpy (dest, src, count, locale);
}
