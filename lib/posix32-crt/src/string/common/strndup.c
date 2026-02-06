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

char *p32_strndup (const char *str, size_t count) {
  size_t length = strnlen (str, count);
  char  *buffer = malloc (length + 1);

  if (buffer == NULL) {
    _set_errno (ENOMEM);
    return NULL;
  }

  buffer[length] = '\0';
  return memcpy (buffer, str, length);
}

char *p32_private_strndup_l (const char *str, size_t count, locale_t locale) {
  size_t length = p32_private_strnlen_l (str, count, locale);

  if (length == (size_t) -1) {
    return NULL;
  }

  char *buffer = malloc (length + 1);

  if (buffer == NULL) {
    _set_errno (ENOMEM);
    return NULL;
  }

  buffer[length] = '\0';
  return memcpy (buffer, str, length);
}
